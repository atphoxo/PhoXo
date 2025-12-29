#pragma once

// 一共四种形态：NoSB , H-SB , V-SB , H+V-SB
struct ScrollViewDrawTarget : public phoxo::CanvasDrawTarget
{
    ScrollViewDrawTarget(const Canvas& canvas, const CScrollView& view)
    {
        const CSize   view_size = FCWnd::GetClientSize(view);
        const CSize   zoomed_canvas_size = canvas.ZoomedSize();

        // 先充满整个区域再调整
        dst_rect_on_view = CRect({}, view_size);
        src_rect_on_zoomed_canvas = CRect({}, zoomed_canvas_size);

        BOOL   horz, vert;
        view.CheckScrollBars(horz, vert);
        if (!horz)
        {
            CenterAlign(view_size.cx, zoomed_canvas_size.cx, dst_rect_on_view.left, dst_rect_on_view.right);
        }
        else
        {
            src_rect_on_zoomed_canvas.left = view.GetScrollPos(SB_HORZ);
            src_rect_on_zoomed_canvas.right = src_rect_on_zoomed_canvas.left + view_size.cx;
        }

        if (!vert)
        {
            CenterAlign(view_size.cy, zoomed_canvas_size.cy, dst_rect_on_view.top, dst_rect_on_view.bottom);
        }
        else
        {
            src_rect_on_zoomed_canvas.top = view.GetScrollPos(SB_VERT);
            src_rect_on_zoomed_canvas.bottom = src_rect_on_zoomed_canvas.top + view_size.cy;
        }
    }

    void SetHdcAndBrush(HDC hdc, HBRUSH brush)
    {
        dst_hdc = hdc;
        background_brush = brush;
    }

    static GPointF ViewToCanvas(const CScrollView& view, CPoint view_pt, const Canvas& canvas)
    {
        ScrollViewDrawTarget   info(canvas, view); // 利用它来计算位置
        CPoint   tmp = view_pt - info.dst_rect_on_view.TopLeft() + info.src_rect_on_zoomed_canvas.TopLeft();
        return canvas.ZoomMapper().ToOriginal(tmp);
    }

private:
    static void CenterAlign(int view, int canvas, LONG& begin, LONG& end)
    {
        int   diff = view - canvas; ASSERT(diff >= 0);
        begin = diff / 2;
        end = begin + canvas;
    }
};
