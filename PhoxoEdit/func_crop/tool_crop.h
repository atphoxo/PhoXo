#pragma once
#include "tool_base.h"
#include "mask_overlay.h"
#include "handle_overlay.h"
#include "move_strategy.h"

class ToolCrop : public ToolBase
{
private:
    crop::MaskOverlay   m_mask_overlay;
    crop::HandleOverlay   m_handle_overlay;
    std::optional<crop::MoveStrategy>   m_move_strategy;

public:
    static constexpr std::wstring_view   TOOL_NAME = L"crop";

    static inline CRect   s_crop_on_canvas;
    static inline crop::CropAspectRatio   s_aspect_ratio;
    static inline CropShape   s_crop_shape = CropShape::Rectangle;
    static inline int   s_roundness = 20; // 圆角百分比，0-100默认 20%

    static bool HasCropRect() { return !s_crop_on_canvas.IsRectEmpty(); }
    static void SetCropOnCanvas(const CRect& rc);
    static void ApplyCropAspectRatio(int width, int height);

    ToolCrop();
    std::wstring_view GetToolName() const override { return TOOL_NAME; }
    HCURSOR GetToolCursor(const ViewportContext& ctx) override;
    void OnLButtonDown(const ViewportContext& ctx, UINT nFlags, CPoint point) override;
    void OnLButtonUp(const ViewportContext& ctx, UINT nFlags, CPoint point) override;
    void OnMouseMove(const ViewportContext& ctx, UINT nFlags, CPoint point) override;
    void OnCaptureChanged() override;
    void OnDrawToolOverlay(HDC hdc, const ViewportContext& ctx) override;
    void OnCanvasReloaded() override;

private:
    void ResetForNewImage();
};
