#include "pch.h"
#include "main_view.h"
#include "main_frame.h"
#include "top_toolbar/zoom_slider_mapper.h"

namespace
{
    double CalcRatioByMouseWheel(double current, short zDelta, double step)
    {
        int   wheel_delta = FCWnd::WheelDeltaToStep(zDelta);
        double   coef = 1;
        if (wheel_delta >= 1)
            coef = std::pow(step, wheel_delta);
        else if (wheel_delta <= -1)
            coef = std::pow(1 / step, -wheel_delta);

        return std::clamp(current * coef, 0.05, phoxo::ZoomMapper::s_max_ratio);
    }
}

void CMainView::UpdateZoomRatio(double new_ratio, ZoomChangedBy from, std::optional<CPoint> view_anchor)
{
    auto   canvas = GetCanvas();
    if (!canvas)
        return;

    std::optional<ScrollViewAnchorRestore>   restore;
    if (view_anchor)
        restore.emplace(*this, *canvas, *view_anchor);

    canvas->SetZoomRatio(new_ratio);
    IEventObserverBase::FireEvent(AppEvent::ZoomRatioChanged, from);
    GetDocument()->UpdateAllViews(NULL);

    if (restore)
        restore->Restore();
}

void CMainView::OnTopZoomActual()
{
    CPoint   center = FCWnd::GetClientRect(m_hWnd).CenterPoint();
    UpdateZoomRatio(1, ZoomChangedBy::ZoomButton, center);
}

void CMainView::OnTopZoomInOut(UINT id)
{
    auto   canvas = GetCanvas();
    if (!canvas)
        return;

    CPoint   center = FCWnd::GetClientRect(m_hWnd).CenterPoint();
    auto   dir = (id == ID_TOP_ZOOM_IN) ? ZoomDirection::ZoomIn : ZoomDirection::ZoomOut;
    double   next_ratio = ZoomSliderMapper::NextRatio(canvas->ZoomRatio(), dir);
    UpdateZoomRatio(next_ratio, ZoomChangedBy::ZoomButton, center);
}

void CMainView::OnTopZoomRatioComboList()
{
    auto   canvas = GetCanvas();
    if (!canvas)
        return;

    auto&   top_toolbar = ((CMainFrame*)AfxGetMainWnd())->TopToolbar();
    double   ratio = canvas->ZoomRatio();
    if (double curr = top_toolbar.GetComboDisplayedRatio(); curr > 0)
    {
        // case: 用户输入 22.4，然后点击 combo list，失去焦点。仅在显示比例不同的情况下才更新 ratio
        if (ZoomSliderMapper::IsDisplayedZoomDifferent(ratio, curr))
            ratio = curr;
    }

    // 无论是否改变，都会刷新 combo 显示。处理用户输入空、无效字符，或者 22.00 等情况
    CPoint   center = FCWnd::GetClientRect(m_hWnd).CenterPoint();
    UpdateZoomRatio(ratio, ZoomChangedBy::TopCombobox, center);
}

void CMainView::OnTopZoomFitWindow()
{
    auto   canvas = GetCanvas();
    if (!canvas)
        return;

    SetScrollSizes(MM_TEXT, CSize()); // 先去掉滚动条

    double   ratio = canvas->CalcFitViewRatio(m_hWnd);
    UpdateZoomRatio(ratio, ZoomChangedBy::ZoomButton);
    canvas->MarkCurrentFitView();
}

void CMainView::OnTopZoomSlider()
{
    auto&   top_toolbar = ((CMainFrame*)AfxGetMainWnd())->TopToolbar();
    CPoint   center = FCWnd::GetClientRect(m_hWnd).CenterPoint();
    UpdateZoomRatio(top_toolbar.GetRatioFromSlider(), ZoomChangedBy::TopSlider, center);
}

BOOL CMainView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
    auto   canvas = GetCanvas();
    if (!canvas)
        return 0;

    if (nFlags & MK_CONTROL)
    {
        ScreenToClient(&pt); // screen -> view
        double   newzoom = CalcRatioByMouseWheel(canvas->ZoomRatio(), zDelta, 1.25);
        UpdateZoomRatio(newzoom, ZoomChangedBy::MouseWheel, pt);
        return 0;
    }

    // 横向滚动
    if (nFlags & MK_SHIFT)
    {
        ScrollHorizByWheel(zDelta);
        return 0;
    }

    return __super::OnMouseWheel(nFlags, zDelta, pt);
}
