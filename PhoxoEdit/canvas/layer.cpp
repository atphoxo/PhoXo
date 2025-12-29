#include "pch.h"
#include "layer.h"

_PHOXO_BEGIN

void Layer::RenderPixel(GPointF canvas_pt, Color& target, double ratio) const
{
    GPointF   pt = CanvasToLayer(canvas_pt);
    if (ratio < 1)
    {
        if(theConfig.m_render_preference == RenderPreference::Quality)
            AreaBlend(pt, target, ratio);
        else
            NearestBlend(pt, target);
    }
    else
    {
        NearestBlend(pt, target);
    }
}

void Layer::NearestBlend(GPointF layer_pt, Color& target) const
{
    CPoint   pt{ Math::Floor(layer_pt.X), Math::Floor(layer_pt.Y) };
    if (m_image.IsInside(pt))
    {
        auto   c = *(const Color*)m_image.GetPixel(pt);
        PixelFunc::BlendStraightAlpha(target, c);
    }
}

/* 暂时不用双线性采样
void Layer::BilinearBlend(GPointF layer_pt, Color& target) const
{
    auto   c = SamplingBilinear::Get(m_image, layer_pt);
    PixelFunc::BlendStraightAlpha(target, c);
}*/

void Layer::AreaBlend(GPointF layer_pt, Color& target, double ratio) const
{
    auto   c = SamplingAreaBox::Get(m_image, layer_pt, ratio);
    PixelFunc::BlendStraightAlpha(target, c);
}

_PHOXO_NAMESPACE_END
