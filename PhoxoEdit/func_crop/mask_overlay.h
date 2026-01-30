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
            float   rounded_rect_radius_percent; // in [0, 1]
            bool   draw_grid = true;
        };

        MaskOverlay();

        void Draw(HDC dc, const CRect& crop_on_view, CSize view_size, const DrawParams& params);

    private:
        void UpdateOverlayMask(const CRect& crop_on_view, const DrawParams& params);
        void DrawGridLines(const CRect& crop_on_view);
        void FillShapeMask(CD2DRectF crop_on_view, const DrawParams& params);
    };
}
