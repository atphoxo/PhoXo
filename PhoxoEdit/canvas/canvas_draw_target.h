#pragma once

_PHOXO_BEGIN

struct CanvasDrawTarget
{
    HDC   dst_hdc{};
    HBRUSH   background_brush{};

    // canvas（全部或区域）显示在HDC上区域
    CRect   dst_rect_on_view;

    // 1. 如果小于view，居中显示 ({0,0} , zoomed_canvas)
    // 2. 大于view，此矩形表示 (scroll_pos , view_size)
    CRect   src_rect_on_zoomed_canvas;

    void FillImageCheckerboard(Image& img) const
    {
        BitmapHDC   dc(img);
        ::SetBrushOrgEx(dc, -src_rect_on_zoomed_canvas.left, -src_rect_on_zoomed_canvas.top, NULL); // 加入滚动条偏移
        ::FillRect(dc, CRect({}, img.Size()), background_brush);
    }

    void DrawImage(const Image& img) const
    {
        CPoint   pt = dst_rect_on_view.TopLeft();
        ::BitBlt(dst_hdc, pt.x, pt.y, img.Width(), img.Height(), BitmapHDC(img), 0, 0, SRCCOPY);
    }
};

_PHOXO_NAMESPACE_END
