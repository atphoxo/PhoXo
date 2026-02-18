#include "pch.h"
#include "PhoxoEdit.h"
#include "tool_crop.h"
#include "main_view.h"
using namespace crop;

namespace
{
    void ZoomForCropMode(const Canvas& canvas)
    {
        CMainView&   view = *theRuntime.GetActiveView();
        CRect   rc = FCWnd::GetClientRect(view);
        rc.DeflateRect(DPICalculator::Cast(10), DPICalculator::Cast(10)); // margin
        float   ratio = phoxo::Utils::CalcFitZoomRatio(rc.Size(), canvas.Size());
        view.UpdateZoomRatio(ratio, ZoomChangedBy::Other);
    }

    CRect CropOnView(const ViewportContext& ctx)
    {
        GPointF   tl = ctx.CanvasToView(ToolCrop::s_crop_on_canvas.TopLeft());
        GPointF   br = ctx.CanvasToView(ToolCrop::s_crop_on_canvas.BottomRight());
        return { (int)floor(tl.X), (int)floor(tl.Y), (int)floor(br.X), (int)floor(br.Y) };
    }
}

ToolCrop::ToolCrop()
{
    ResetForNewImage();
}

void ToolCrop::SetCropOnCanvas(const CRect& rc)
{
    if (auto canvas = theRuntime.GetCurrentCanvas(); canvas && !rc.IsRectEmpty())
    {
        s_crop_on_canvas.IntersectRect(CRect({}, canvas->Size()), rc);
    }
    else
    {
        s_crop_on_canvas = {};
    }

    // 用户可能输入一个无效的rc，但仍会触发刷新/事件以便 UI 恢复显示
    theRuntime.InvalidateView();
    IEventObserverBase::FireEvent(AppEvent::CropRectChanged);
}

void ToolCrop::ApplyCropAspectRatio(int width, int height)
{
    if (auto canvas = theRuntime.GetCurrentCanvas(); canvas && width && height)
    {
        s_aspect_ratio.Lock(width, height);
        SetCropOnCanvas(s_aspect_ratio.FitCanvas(canvas->Size()));
    }
}

HCURSOR ToolCrop::GetToolCursor(const ViewportContext& ctx)
{
    CPoint   pt;
    ::GetCursorPos(&pt);
    ::ScreenToClient(ctx.m_view, &pt);
    if (HCURSOR cursor = m_handle_overlay.GetCursor(pt, CropOnView(ctx)))
        return cursor;
    return __super::GetToolCursor(ctx);
}

void ToolCrop::OnLButtonDown(const ViewportContext& ctx, UINT nFlags, CPoint point)
{
    if (!HasCropRect() || (nFlags & MK_CONTROL))
    {
        // create new crop rect
        m_move_strategy.emplace(GripType::None, ctx.ViewToCanvas(point), CRect(), ctx.m_canvas.Size());
    }
    else
    {
        auto   type = m_handle_overlay.HitTest(point, CropOnView(ctx));
        if (type == GripType::None)
            return;

        // modify existing crop rect
        m_move_strategy.emplace(type, ctx.ViewToCanvas(point), s_crop_on_canvas, ctx.m_canvas.Size());
    }
}

void ToolCrop::OnLButtonUp(const ViewportContext& ctx, UINT nFlags, CPoint point)
{
    m_move_strategy = std::nullopt;
}

void ToolCrop::OnMouseMove(const ViewportContext& ctx, UINT, CPoint point)
{
    if (m_move_strategy)
    {
        SetCropOnCanvas(m_move_strategy->HandleMouseMove(ctx.ViewToCanvas(point), s_aspect_ratio));
    }
    else
    {
        if (m_handle_overlay.OnMouseMove(point, CropOnView(ctx)))
        {
            theRuntime.InvalidateView();
        }
    }
}

void ToolCrop::OnCaptureChanged()
{
    // ASSERT(!m_move_strategy.has_value());
    m_move_strategy = std::nullopt;
}

void ToolCrop::OnDrawToolOverlay(HDC hdc, const ViewportContext& ctx)
{
    if (!ToolCrop::HasCropRect())
        return;

    MaskOverlay::DrawParams   params{
        .shape = s_crop_shape,
        .rounded_rect_radius_percent = (float)s_roundness / 100.f,
        .is_interacting = m_move_strategy.has_value()
    };

    CRect   rc = CropOnView(ctx);
    m_mask_overlay.Draw(hdc, rc, FCWnd::GetClientSize(ctx.m_view), params);
    m_handle_overlay.Draw(hdc, rc);
}

void ToolCrop::OnCanvasReloaded()
{
    ResetForNewImage();
}

void ToolCrop::ResetForNewImage()
{
    s_crop_on_canvas = {};
    s_aspect_ratio.Unlock();

    if (auto canvas = theRuntime.GetCurrentCanvas())
    {
        ZoomForCropMode(*canvas);
        s_crop_on_canvas = CRect({}, canvas->Size());
        IEventObserverBase::FireEvent(AppEvent::CanvasReloaded, canvas);
    }
}
