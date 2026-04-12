#include "pch.h"
#include "cmd_rect_crop.h"
#include "tool_crop.h"
#include "mask_overlay.h"

_PHOXO_NAMESPACE(cmd)
using namespace D2D1;

namespace
{
    struct LayerCropResult
    {
    private:
        const CRect   m_crop_on_canvas;

    public:
        CPoint   m_new_layer_pos;
        Image   m_cropped;

    public:
        LayerCropResult(const Layer& layer, const CRect& crop_on_canvas) : m_crop_on_canvas(crop_on_canvas)
        {
            CRect   rc;
            rc.IntersectRect(layer.Bounds(), crop_on_canvas);

            // If empty, the layer should be removed in the future.
            // Currently kept to preserve the layer structure.
            ASSERT(!rc.IsRectEmpty());

            m_new_layer_pos = rc.TopLeft() - crop_on_canvas.TopLeft();

            CRect   on_layer = rc - layer.Position(); // rect relative to the layer
            m_cropped = phoxo::ImageHandler::GetRegion(layer.RasterImage(), on_layer);
            ApplyShapeMaskToAlpha();
        }

    private:
        void GenerateShapeMask(HDC dc) const
        {
            auto   target = D2D::CreateDCRenderTarget(D2D1_RENDER_TARGET_TYPE_SOFTWARE, dc);
            ID2D1SolidColorBrushPtr   br;
            target->CreateSolidColorBrush(ColorF(ColorF::White), &br);

            CRect   rc{ -m_new_layer_pos, m_crop_on_canvas.Size() };
            target->BeginDraw();
            target->Clear(ColorF(0, 0));
            crop::MaskOverlay::FillShapeMask(target, br, rc, ToolCrop::s_crop_shape, ToolCrop::s_roundness);
            target->EndDraw();
        }

        void ApplyShapeMaskToAlpha()
        {
            if (ToolCrop::s_crop_shape == CropShape::Rectangle)
                return;

            Image   mask;
            mask.Create(m_cropped.Size(), 32);
            GenerateShapeMask(BitmapHDC(mask));

            auto   src = (const Color*)mask.PixelBase();
            auto   dst = (Color*)m_cropped.PixelBase();
            int   count = m_cropped.Width() * m_cropped.Height();
            std::for_each_n(src, count, [&dst](Color c) {
                dst->a = (BYTE)(dst->a * c.r / 255);
                dst++;
            });
        }
    };
}

CmdRectCrop::CmdRectCrop(const CRect& crop_on_canvas, const Canvas& canvas, int ratio_index)
    : m_crop_on_canvas(crop_on_canvas)
    , m_aspect_ratio(ToolCrop::s_aspect_ratio)
    , m_ratio_index(ratio_index)
{
    SetDescription(LanguageText::Get(L"cmd", L"crop"));
    for (auto& layer : canvas.LayerMgr().all())
    {
        LayerCropResult   result(*layer, crop_on_canvas);
        AddCommand(make_unique<CmdMoveLayer>(layer, result.m_new_layer_pos));
        AddCommand(make_unique<CmdModifyLayerImage>(layer, std::move(result.m_cropped)));
    }
    AddCommand(make_unique<CmdSetCanvasSize>(crop_on_canvas.Size()));
    AddCommand(make_unique<CmdMFCSetDocModified>());
}

void CmdRectCrop::Execute(Canvas& canvas, IProgressListener* progress)
{
    __super::Execute(canvas, progress);
    ToolCrop::SetCropOnCanvas(CRect());
}

void CmdRectCrop::Undo(Canvas& canvas)
{
    __super::Undo(canvas);

    ToolCrop::SetCropOnCanvas(m_crop_on_canvas);
    ToolCrop::s_aspect_ratio = m_aspect_ratio;

    IEventObserverBase::FireEvent(AppEvent::CropUndoCompleted, m_ratio_index);
    AfxGetMainWnd()->PostMessage(WM_COMMAND, ID_TOP_ZOOM_FIT_WINDOW);
}

void CmdRectCrop::Redo(Canvas& canvas)
{
    __super::Redo(canvas);
    ToolCrop::SetCropOnCanvas(CRect());
}

_PHOXO_NAMESPACE_END
