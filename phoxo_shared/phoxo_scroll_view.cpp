#include "pch.h"
#include "phoxo_scroll_view.h"

#ifdef __BCGCBPRO_H
IMPLEMENT_DYNAMIC(PhoXoScrollViewBase, CBCGPScrollView)
BEGIN_MESSAGE_MAP(PhoXoScrollViewBase, CBCGPScrollView)
    ON_WM_ERASEBKGND()
    ON_WM_CREATE()
    ON_WM_MOUSEMOVE()
    ON_WM_MOUSELEAVE()
END_MESSAGE_MAP()

int PhoXoScrollViewBase::OnCreate(LPCREATESTRUCT lcs)
{
    int   ret = __super::OnCreate(lcs);
    SetScrollSizes(MM_TEXT, CSize()); // 初始化空滚动条，否则全屏启动有assert
    ModifyStyleEx(WS_EX_CLIENTEDGE, 0); // 去除2px边框
    return ret;
}

void PhoXoScrollViewBase::OnMouseMove(UINT nFlags, CPoint point)
{
    if (!m_track)
    {
        TRACKMOUSEEVENT   tme = { sizeof(TRACKMOUSEEVENT), TME_LEAVE, m_hWnd };
        ::TrackMouseEvent(&tme);
        m_track = TRUE;
    }
    __super::OnMouseMove(nFlags, point);
}

void PhoXoScrollViewBase::OnMouseLeave()
{
    m_track = FALSE;
    __super::OnMouseLeave();
}

BOOL PhoXoScrollViewBase::OnScroll(UINT nScrollCode, UINT nPos, BOOL bDoScroll)
{
    // 处理超过 32K 的滚动范围，SB_THUMBTRACK 时 nPos 只支持到16bit
    if (LOBYTE(nScrollCode) == SB_THUMBTRACK)
    {
        nPos = GetScrollBarTrackPos(SB_HORZ);
    }
    else if (HIBYTE(nScrollCode) == SB_THUMBTRACK)
    {
        nPos = GetScrollBarTrackPos(SB_VERT);
    }
    return __super::OnScroll(nScrollCode, nPos, bDoScroll);
}
#endif
