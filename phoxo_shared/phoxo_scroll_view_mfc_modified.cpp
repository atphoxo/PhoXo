#include "pch.h"
#include "phoxo_scroll_view.h"
#include <afxglobals.h>

#ifdef __BCGCBPRO_H
// 去掉ScrollWindow并加Invalidate刷新
BOOL PhoXoScrollViewBase::OnScrollBy(CSize sizeScroll, BOOL bDoScroll)
{
    int xOrig, x;
    int yOrig, y;

    // don't scroll if there is no valid scroll range (ie. no scroll bar)
    CScrollBar* pBar;
    DWORD dwStyle = GetStyle();
    pBar = GetScrollBarCtrl(SB_VERT);
    if ((pBar != NULL && !pBar->IsWindowEnabled()) ||
        (pBar == NULL && !(dwStyle & WS_VSCROLL)))
    {
        // vertical scroll bar not enabled
        sizeScroll.cy = 0;
    }
    pBar = GetScrollBarCtrl(SB_HORZ);
    if ((pBar != NULL && !pBar->IsWindowEnabled()) ||
        (pBar == NULL && !(dwStyle & WS_HSCROLL)))
    {
        // horizontal scroll bar not enabled
        sizeScroll.cx = 0;
    }

    // adjust current x position
    xOrig = x = GetScrollPos(SB_HORZ);
    int xMax = GetScrollLimit(SB_HORZ);
    x += sizeScroll.cx;
    if (x < 0)
        x = 0;
    else if (x > xMax)
        x = xMax;

    // adjust current y position
    yOrig = y = GetScrollPos(SB_VERT);
    int yMax = GetScrollLimit(SB_VERT);
    y += sizeScroll.cy;
    if (y < 0)
        y = 0;
    else if (y > yMax)
        y = yMax;

    // did anything change?
    if (x == xOrig && y == yOrig)
        return FALSE;

    if (bDoScroll)
    {
        // do scroll and update scroll positions
        // ScrollWindow(-(x - xOrig), -(y - yOrig));
        if (x != xOrig)
            SetScrollPos(SB_HORZ, x);
        if (y != yOrig)
            SetScrollPos(SB_VERT, y);
    }
    Invalidate();
    return TRUE;
}

// 没有修改，拷贝过来只是为了调用我们自己的UpdateBars
void PhoXoScrollViewBase::SetScrollSizes(int nMapMode, SIZE sizeTotal, const SIZE& sizePage, const SIZE& sizeLine)
{
    ASSERT(sizeTotal.cx >= 0 && sizeTotal.cy >= 0);
    ASSERT(nMapMode > 0);
    ASSERT(nMapMode != MM_ISOTROPIC && nMapMode != MM_ANISOTROPIC);

    int nOldMapMode = m_nMapMode;
    m_nMapMode = nMapMode;
    m_totalLog = sizeTotal;

    //BLOCK: convert logical coordinate space to device coordinates
    {
        CWindowDC dc(NULL);
        ASSERT(m_nMapMode > 0);
        dc.SetMapMode(m_nMapMode);

        // total size
        m_totalDev = m_totalLog;
        dc.LPtoDP((LPPOINT)&m_totalDev);
        m_pageDev = sizePage;
        dc.LPtoDP((LPPOINT)&m_pageDev);
        m_lineDev = sizeLine;
        dc.LPtoDP((LPPOINT)&m_lineDev);
        if (m_totalDev.cy < 0)
            m_totalDev.cy = -m_totalDev.cy;
        if (m_pageDev.cy < 0)
            m_pageDev.cy = -m_pageDev.cy;
        if (m_lineDev.cy < 0)
            m_lineDev.cy = -m_lineDev.cy;
    } // release DC here

    // now adjust device specific sizes
    ASSERT(m_totalDev.cx >= 0 && m_totalDev.cy >= 0);
    if (m_pageDev.cx == 0)
        m_pageDev.cx = m_totalDev.cx / 10;
    if (m_pageDev.cy == 0)
        m_pageDev.cy = m_totalDev.cy / 10;
    if (m_lineDev.cx == 0)
        m_lineDev.cx = m_pageDev.cx / 10;
    if (m_lineDev.cy == 0)
        m_lineDev.cy = m_pageDev.cy / 10;

    if (m_hWnd != NULL)
    {
        // window has been created, invalidate now
        UpdateBars();
        if (nOldMapMode != m_nMapMode)
            Invalidate(TRUE);
    }
}

/// 去掉ScrollWindow
void PhoXoScrollViewBase::ScrollToDevicePosition(POINT ptDev)
{
    ASSERT(ptDev.x >= 0);
    ASSERT(ptDev.y >= 0);

    // Note: ScrollToDevicePosition can and is used to scroll out-of-range
    //  areas as far as CScrollView is concerned -- specifically in
    //  the print-preview code.  Since OnScrollBy makes sure the range is
    //  valid, ScrollToDevicePosition does not vector through OnScrollBy.

    // int xOrig = GetScrollPos(SB_HORZ);
    SetScrollPos(SB_HORZ, ptDev.x);
    // int yOrig = GetScrollPos(SB_VERT);
    SetScrollPos(SB_VERT, ptDev.y);
    // ScrollWindow(xOrig - ptDev.x, yOrig - ptDev.y);
}

// 没有修改，拷贝过来只是为了调用我们自己的ScrollToDevicePosition
void PhoXoScrollViewBase::UpdateBars()
{
    // UpdateBars may cause window to be resized - ignore those resizings
    if (m_bInsideUpdate)
        return;         // Do not allow recursive calls

    // Lock out recursion
    m_bInsideUpdate = TRUE;

    // update the horizontal to reflect reality
    // NOTE: turning on/off the scrollbars will cause 'OnSize' callbacks
    ASSERT(m_totalDev.cx >= 0 && m_totalDev.cy >= 0);

    CRect rectClient;
    BOOL bCalcClient = TRUE;

    // allow parent to do inside-out layout first
    CWnd* pParentWnd = GetParent();
    if (pParentWnd != NULL)
    {
        // if parent window responds to this message, use just
        //  client area for scroll bar calc -- not "true" client area
        if ((BOOL)pParentWnd->SendMessage(WM_RECALCPARENT, 0,
            (LPARAM)(LPCRECT)&rectClient) != 0)
        {
            // use rectClient instead of GetTrueClientSize for
            //  client size calculation.
            bCalcClient = FALSE;
        }
    }

    CSize sizeClient;
    CSize sizeSb;

    if (bCalcClient)
    {
        // get client rect
        if (!GetTrueClientSize(sizeClient, sizeSb))
        {
            // no room for scroll bars (common for zero sized elements)
            CRect rect;
            GetClientRect(&rect);
            if (rect.right > 0 && rect.bottom > 0)
            {
                // if entire client area is not invisible, assume we have
                //  control over our scrollbars
                EnableScrollBarCtrl(SB_BOTH, FALSE);
            }
            m_bInsideUpdate = FALSE;
            return;
        }
    }
    else
    {
        // let parent window determine the "client" rect
        GetScrollBarSizes(sizeSb);
        sizeClient.cx = rectClient.right - rectClient.left;
        sizeClient.cy = rectClient.bottom - rectClient.top;
    }

    // enough room to add scrollbars
    CSize sizeRange;
    CPoint ptMove;
    CSize needSb;

    // get the current scroll bar state given the true client area
    GetScrollBarState(sizeClient, needSb, sizeRange, ptMove, bCalcClient);
    if (needSb.cx)
        sizeClient.cy -= sizeSb.cy;
    if (needSb.cy)
        sizeClient.cx -= sizeSb.cx;

    // first scroll the window as needed
    ScrollToDevicePosition(ptMove); // will set the scroll bar positions too

    // this structure needed to update the scrollbar page range
    SCROLLINFO info;
    info.fMask = SIF_PAGE | SIF_RANGE;
    info.nMin = 0;

    // now update the bars as appropriate
    EnableScrollBarCtrl(SB_HORZ, needSb.cx);
    if (needSb.cx)
    {
        info.nPage = sizeClient.cx;
        info.nMax = m_totalDev.cx - 1;
        if (!SetScrollInfo(SB_HORZ, &info, TRUE))
            SetScrollRange(SB_HORZ, 0, sizeRange.cx, TRUE);
    }
    EnableScrollBarCtrl(SB_VERT, needSb.cy);
    if (needSb.cy)
    {
        info.nPage = sizeClient.cy;
        info.nMax = m_totalDev.cy - 1;
        if (!SetScrollInfo(SB_VERT, &info, TRUE))
            SetScrollRange(SB_VERT, 0, sizeRange.cy, TRUE);
    }

    // SetScrollInfo设置垂直滚动条时会把水平滚动条也加上（原因不明），这里修正回来，只需要去掉属性就行，不需要重新排版，版式是对的
    if (!needSb.cx && (GetStyle() & WS_HSCROLL))
    {
        ModifyStyle(WS_HSCROLL, 0);
    }

    // remove recursion lockout
    m_bInsideUpdate = FALSE;
}

// 从 CScrollView::OnMouseWheel 拷贝过来
void PhoXoScrollViewBase::ScrollHorizByWheel(short zDelta)
{
    BOOL bHasHorzBar, bHasVertBar;
    CheckScrollBars(bHasHorzBar, bHasVertBar);
    if (!bHasHorzBar)
        return;

    BOOL bResult = FALSE;
    UINT uWheelScrollLines = 3;
    int nToScroll = ::MulDiv(-zDelta, uWheelScrollLines, WHEEL_DELTA);
    int nDisplacement;

    nDisplacement = nToScroll * m_lineDev.cx;
    nDisplacement = min(nDisplacement, m_pageDev.cx);

    bResult = OnScrollBy(CSize(nDisplacement, 0), TRUE);

    if (bResult)
        UpdateWindow();
}

// 拷贝过来，滚动条应该 -AFX_CX_BORDER
void PhoXoScrollViewBase::GetScrollBarSizes(CSize& sizeSb)
{
    __super::GetScrollBarSizes(sizeSb);

    if (sizeSb.cx) sizeSb.cx -= AFX_CX_BORDER;
    if (sizeSb.cy) sizeSb.cy -= AFX_CY_BORDER;
}

// 拷贝无修改，只是为了调用我们自己的GetScrollBarSizes
BOOL PhoXoScrollViewBase::GetTrueClientSize(CSize& size, CSize& sizeSb)
{
    CRect rect;
    GetClientRect(&rect);
    ASSERT(rect.top == 0 && rect.left == 0);
    size.cx = rect.right;
    size.cy = rect.bottom;
    DWORD dwStyle = GetStyle();

    // first get the size of the scrollbars for this window
    GetScrollBarSizes(sizeSb);

    // first calculate the size of a potential scrollbar
    // (scroll bar controls do not get turned on/off)
    if (sizeSb.cx != 0 && (dwStyle & WS_VSCROLL))
    {
        // vert scrollbars will impact client area of this window
        size.cx += sizeSb.cx;   // currently on - adjust now
    }
    if (sizeSb.cy != 0 && (dwStyle & WS_HSCROLL))
    {
        // horz scrollbars will impact client area of this window
        size.cy += sizeSb.cy;   // currently on - adjust now
    }

    // return TRUE if enough room
    return (size.cx > sizeSb.cx && size.cy > sizeSb.cy);
}

// 拷贝无修改，只是为了调用我们自己的GetScrollBarSizes
void PhoXoScrollViewBase::GetScrollBarState(CSize sizeClient, CSize& needSb,
    CSize& sizeRange, CPoint& ptMove, BOOL bInsideClient)
{
    // get scroll bar sizes (the part that is in the client area)
    CSize sizeSb;
    GetScrollBarSizes(sizeSb);

    // enough room to add scrollbars
    sizeRange = m_totalDev - sizeClient;
    // > 0 => need to scroll
    ptMove = GetDeviceScrollPosition();
    // point to move to (start at current scroll pos)

    BOOL bNeedH = sizeRange.cx > 0;
    if (!bNeedH)
        ptMove.x = 0;                       // jump back to origin
    else if (bInsideClient)
        sizeRange.cy += sizeSb.cy;          // need room for a scroll bar

    BOOL bNeedV = sizeRange.cy > 0;
    if (!bNeedV)
        ptMove.y = 0;                       // jump back to origin
    else if (bInsideClient)
        sizeRange.cx += sizeSb.cx;          // need room for a scroll bar

    if (bNeedV && !bNeedH && sizeRange.cx > 0)
    {
        ASSERT(bInsideClient);
        // need a horizontal scrollbar after all
        bNeedH = TRUE;
        sizeRange.cy += sizeSb.cy;
    }

    // if current scroll position will be past the limit, scroll to limit
    if (sizeRange.cx > 0 && ptMove.x >= sizeRange.cx)
        ptMove.x = sizeRange.cx;
    if (sizeRange.cy > 0 && ptMove.y >= sizeRange.cy)
        ptMove.y = sizeRange.cy;

    // now update the bars as appropriate
    needSb.cx = bNeedH;
    needSb.cy = bNeedV;

    // needSb, sizeRange, and ptMove area now all updated
}
#endif
