#pragma once

_PHOXO_BEGIN

class ViewportComposer : public effect::PixelIterator<ViewportComposer>
{
private:
    const Canvas&   m_canvas;
    const CPoint   m_scroll_pos;

public:
    ViewportComposer(const Canvas& canvas, CPoint scroll_pos)
        : m_canvas(canvas)
        , m_scroll_pos(scroll_pos)
    {
        EnableParallel(true);
    }

    void Process(CPoint pt, Color& px) const
    {
        GPointF   canvas_pt = m_canvas.ZoomMapper().ToOriginal(pt + m_scroll_pos);
        for (auto& layer : m_canvas.AllLayers())
        {
            layer->RenderPixel(canvas_pt, px, m_canvas.ZoomRatio());
        }
    }

    static void HandlePixel(Image& img, int x, int y, Color* px, ViewportComposer& eff)
    {
        eff.Process(CPoint(x, y), *px);
    }

private:
    SIZE GetScanLineCountPerTask(const Image& img) override
    {
        return { 0, 50 };
    }
};

_PHOXO_NAMESPACE_END
