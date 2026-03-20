#pragma once

_PHOXO_BEGIN

/// Generic canvas rotate command
template<typename Effect>
class CmdRotateCanvas : public BatchCommand
{
private:
    static CPoint CalcNewPosition(CSize canvas_size, const Layer& layer)
    {
        CPoint   pos = layer.Position();
        CSize   sz = layer.RasterImage().Size();
        if constexpr (std::is_same_v<Effect, effect::Rotate90>)
        {
            return { canvas_size.cy - pos.y - sz.cy, pos.x };
        }
        else // Rotate270
        {
            return { pos.y, canvas_size.cx - pos.x - sz.cx };
        }
    }

public:
    CmdRotateCanvas(const Canvas& canvas, const CString& desc)
    {
        SetDescription(desc);

        Effect fx;
        fx.EnableParallel();

        const CSize   canvas_size = canvas.Size();
        for (auto& layer : canvas.LayerMgr().all())
        {
            // 1. rotate image
            Image   dst = layer->RasterImage();
            dst.ApplyEffect(fx);
            AddCommand(make_unique<CmdModifyLayerImage>(layer, std::move(dst)));

            // 2. move layer position
            AddCommand(make_unique<CmdMoveLayer>(layer, CalcNewPosition(canvas_size, *layer)));
        }

        AddCommand(make_unique<CmdSetCanvasSize>(CSize(canvas_size.cy, canvas_size.cx)));
        AddCommand(make_unique<CmdMFCSetDocModified>());
    }
};

// type aliases
using CmdRotate90Canvas = CmdRotateCanvas<effect::Rotate90>;
using CmdRotate270Canvas = CmdRotateCanvas<effect::Rotate270>;

_PHOXO_NAMESPACE_END
