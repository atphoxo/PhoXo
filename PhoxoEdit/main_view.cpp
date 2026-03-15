#include "pch.h"
#include "main_doc.h"
#include "main_view.h"
#include "tool_manager.h"

static_assert(ID_TOP_ZOOM_OUT + 1 == ID_TOP_ZOOM_IN); // range连续

IMPLEMENT_DYNCREATE(CMainView, PhoXoScrollViewBase)
BEGIN_MESSAGE_MAP(CMainView, PhoXoScrollViewBase)
    ON_WM_CONTEXTMENU()
    ON_WM_MOUSEWHEEL()
    ON_WM_MOUSEMOVE()
    ON_WM_LBUTTONDOWN()
    ON_WM_LBUTTONUP()
    ON_WM_SETCURSOR()
    ON_WM_MOUSELEAVE()
    ON_WM_KEYDOWN()
    ON_MESSAGE(WM_CAPTURECHANGED, OnCaptureChanged)
    // top toolbar zoom
    ON_COMMAND(ID_TOP_ZOOM_SLIDER, OnTopZoomSlider)
    ON_UPDATE_COMMAND_UI(ID_TOP_ZOOM_SLIDER, OnUpdateIfCanvasValid)
    ON_COMMAND(ID_TOP_ZOOM_COMBO_LIST, OnTopZoomRatioComboList)
    ON_UPDATE_COMMAND_UI(ID_TOP_ZOOM_COMBO_LIST, OnUpdateIfCanvasValid)
    ON_COMMAND(ID_TOP_ZOOM_ACTUAL, OnTopZoomActual)
    ON_UPDATE_COMMAND_UI(ID_TOP_ZOOM_ACTUAL, OnUpdateIfCanvasValid)
    ON_COMMAND(ID_TOP_ZOOM_FIT_WINDOW, OnTopZoomFitWindow)
    ON_UPDATE_COMMAND_UI(ID_TOP_ZOOM_FIT_WINDOW, OnUpdateIfCanvasValid)
    ON_COMMAND_RANGE(ID_TOP_ZOOM_OUT, ID_TOP_ZOOM_IN, OnTopZoomInOut)
    ON_UPDATE_COMMAND_UI_RANGE(ID_TOP_ZOOM_OUT, ID_TOP_ZOOM_IN, OnUpdateIfCanvasValid)
    // file/edit
    ON_COMMAND(ID_FILE_PRINT, OnFilePrint)
    ON_UPDATE_COMMAND_UI(ID_FILE_PRINT, OnUpdateIfCanvasValid)
    ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
    ON_UPDATE_COMMAND_UI(ID_EDIT_COPY, OnUpdateIfCanvasValid)
END_MESSAGE_MAP()

Canvas* CMainView::GetCanvas() const
{
    auto   doc = (CMainDoc*)m_pDocument;
    return doc ? doc->GetCanvas() : nullptr;
}

std::optional<ViewportContext> CMainView::CreateViewportContext() const
{
    if (auto canvas = GetCanvas())
        return ViewportContext(*canvas, *this);
    return std::nullopt;
}

void CMainView::OnDraw(CDC* paintdc)
{
    const CRect   view_rect = FCWnd::GetClientRect(m_hWnd);
    if (view_rect.IsRectEmpty())
        return;

    // 不需要恢复view原点，OnDraw这个函数完了paintdc就销毁了
    paintdc->SetViewportOrg(0, 0);

    BufferedPaintDC   memdc(*paintdc, view_rect);

    // 画布区域外颜色
    CBCGPVisualManager::GetInstance()->OnFillPopupWindowBackground(&memdc, view_rect);

    if (auto ctx = CreateViewportContext())
    {
        GetCanvas()->Draw(memdc, theRuntime.m_canvas_back, *ctx);

        if (auto tool = theToolManager.GetActiveTool())
        {
            tool->OnDrawToolOverlay(memdc, *ctx);
        }
    }
    else
    {
        if (m_welcome)
            m_welcome->Draw(memdc);
    }
}

void CMainView::OnInitialUpdate()
{
    m_welcome = nullptr;
    SetScrollSizes(MM_TEXT, CSize()); // 去掉前一张图片的滚动条影响

    if (auto canvas = GetCanvas())
    {
        double   ratio = canvas->CalcFitViewRatio(m_hWnd); // 居中显示
        UpdateZoomRatio(ratio, ZoomChangedBy::InitView); // 里面会触发OnUpdate，因此不用调用__super了
        canvas->MarkCurrentFitView();
        return;
    }
    else
    {
        m_welcome = make_unique<main_view::WelcomeRender>(m_hWnd);
    }
    __super::OnInitialUpdate(); // default to call OnUpdate to set scroll size
}

void CMainView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
    if (auto canvas = GetCanvas())
    {
        // MFC底层有问题，如果选择800%，最右边和最下边会画不上，原因是计算客户区宽度少了1像素，但如果滚动条已经显示再设置那么没问题
        BOOL   horz, vert;
        CheckScrollBars(horz, vert);
        SetScrollSizes(MM_TEXT, canvas->ZoomedSize());
        if (!horz && !vert)
        {
            CheckScrollBars(horz, vert);
            if (horz || vert)
            {
                SetScrollSizes(MM_TEXT, canvas->ZoomedSize());
            }
        }
    }

    __super::OnUpdate(pSender, lHint, pHint);
}

void CMainView::OnFilePrint()
{
    // TODO: 实现打印
    if (auto canvas = GetCanvas())
    {
    }
}

void CMainView::OnEditCopy()
{
    if (auto canvas = GetCanvas())
    {
        CWaitCursor   wait_cursor;
        auto   img = canvas->BuildCanvasImage();
        phoxo::effect::CopyToClipboard   fx;
        img.ApplyEffect(fx);
    }
}

void CMainView::OnUpdateIfCanvasValid(CCmdUI* ui)
{
    ui->Enable(GetCanvas() != nullptr);
}

void CMainView::OnContextMenu(CWnd*, CPoint point)
{
    if (CBCGPPopupMenu::GetActiveMenu() != NULL)
        return;

    if (auto tool = theToolManager.GetActiveTool(); tool && theRuntime.GetCurrentCanvas())
    {
        tool->OnContextMenu(point);
    }
}

void CMainView::OnLButtonDown(UINT nFlags, CPoint point)
{
    auto   ctx = CreateViewportContext();
    auto   tool = theToolManager.GetActiveTool();
    if (ctx && tool)
    {
        tool->OnLButtonDown(*ctx, nFlags, point);
    }
    Invalidate();
    SetCapture();
}

void CMainView::OnLButtonUp(UINT nFlags, CPoint point)
{
    auto   ctx = CreateViewportContext();
    auto   tool = theToolManager.GetActiveTool();
    if (ctx && tool)
    {
        tool->OnLButtonUp(*ctx, nFlags, point);
    }
    Invalidate();

    if (::GetCapture() == m_hWnd)
    {
        ::ReleaseCapture();
    }
}

LRESULT CMainView::OnCaptureChanged(WPARAM, LPARAM lParam)
{
    if (auto tool = theToolManager.GetActiveTool())
    {
        tool->OnCaptureChanged();
    }
    return 0;
}

BOOL CMainView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
    auto   ctx = CreateViewportContext();
    auto   tool = theToolManager.GetActiveTool();
    if (tool && ctx && (nHitTest == HTCLIENT))
    {
        ::SetCursor(tool->GetToolCursor(*ctx));
        return TRUE;
    }
    return __super::OnSetCursor(pWnd, nHitTest, message);
}

void CMainView::OnMouseMove(UINT nFlags, CPoint point)
{
    auto   ctx = CreateViewportContext();
    auto   tool = theToolManager.GetActiveTool();
    if (ctx && tool)
    {
        tool->OnMouseMove(*ctx, nFlags, point);
    }
    __super::OnMouseMove(nFlags, point);
}

void CMainView::OnMouseLeave()
{
    auto   ctx = CreateViewportContext();
    auto   tool = theToolManager.GetActiveTool();
    if (ctx && tool)
    {
        tool->OnMouseMove(*ctx, 0, { -0xFFFFFF,-0xFFFFFF });
    }
    __super::OnMouseLeave();
}

void CMainView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    if (auto tool = theToolManager.GetActiveTool())
    {
        if (tool->OnKeyDown(nChar, nFlags))
            return;
    }
    __super::OnKeyDown(nChar, nRepCnt, nFlags);
}
