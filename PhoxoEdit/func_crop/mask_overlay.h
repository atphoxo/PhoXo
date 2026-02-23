#pragma once

namespace crop
{
    // ÷ªª≠’⁄’÷≤„£¨Õ⁄ø’crop«¯”Ú
    class MaskOverlay
    {
    private:
        ID2D1DCRenderTargetPtr   m_target;
        ID2D1SolidColorBrushPtr   m_black_brush;
        ID2D1SolidColorBrushPtr   m_grid_brush;

        FCImage   m_buffer;

    public:
        struct DrawParams
        {
            CropShape   shape;
            int   rounded_rect_radius_percent; // 0®C100
            bool   is_interacting;
        };

        MaskOverlay();

        void Draw(HDC dc, const CRect& crop_on_view, CSize view_size, const DrawParams& params);

        static void FillShapeMask(
            ID2D1RenderTarget* target,
            ID2D1Brush* brush,
            CD2DRectF crop_on_view,
            CropShape shape,
            int rounded_rect_radius_percent);

    private:
        void UpdateOverlayMask(const CRect& crop_on_view, const DrawParams& params);
        void DrawGridLines(const CRect& crop_on_view);
    };
}
