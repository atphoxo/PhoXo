#include "pch.h"
#include "canvas.h"

_PHOXO_BEGIN

Canvas::Canvas(CSize canvas_size)
    : m_canvas_size{ canvas_size }
    , m_viewport{ *this }
{
}

CSize Canvas::ZoomedSize() const
{
    GPointF   pt = m_zoom.ToZoomed(m_canvas_size);
    return { (int)(std::max)(1.0f, pt.X), (int)(std::max)(1.0f, pt.Y) };
}

void Canvas::Execute(unique_ptr<Command>&& cmd, IProgressListener* progress)
{
    m_command_mgr.Execute(std::move(cmd), *this, progress);
    m_content_version++;
}

void Canvas::Undo()
{
    m_command_mgr.Undo(*this);
    m_content_version++;
}

void Canvas::Redo()
{
    m_command_mgr.Redo(*this);
    m_content_version++;
}

void Canvas::AddLayer(const shared_ptr<Layer>& layer, int index)
{
    m_layer_mgr.add(layer, index);
    m_content_version++;
    //layer->SetPosition(CPoint(200, 200));
}

void Canvas::Draw(HDC hdc, HBRUSH background, const ViewportContext& ctx)
{
    m_viewport.Draw(hdc, background, ctx);
}

Image Canvas::BuildCanvasImage() const
{
    Image   img;
    img.Create(m_canvas_size);
    for (auto& layer : m_layer_mgr.all())
    {
        effect::CompositeStraight   fx(layer->RasterImage(), layer->Position(), layer->Opacity());
        fx.EnableParallel(true);
        img.ApplyEffect(fx);
    }
    return img;
}

void Canvas::SetZoomRatio(double ratio)
{
    m_zoom.set(ratio);
    m_is_current_fitview = false;
}

double Canvas::CalcFitViewRatio(HWND view) const
{
    return Utils::CalcFitZoomRatio(FCWnd::GetClientSize(view), m_canvas_size);
}

_PHOXO_NAMESPACE_END
