#pragma once

_PHOXO_BEGIN

/// Generic canvas flip command
template<typename Effect>
class CmdFlipCanvas : public BatchCommand
{
private:
    static CPoint CalcNewPosition(CSize canvas_size, const Layer& layer)
    {
        CPoint   pos = layer.Position();
        CSize   sz = layer.RasterImage().Size();
        if constexpr (std::is_same_v<Effect, effect::Mirror>)
        {
            return { canvas_size.cx - pos.x - sz.cx, pos.y };
        }
        else
        {
            return { pos.x, canvas_size.cy - pos.y - sz.cy };
        }
    }

public:
    CmdFlipCanvas(const Canvas& canvas, const CString& desc)
    {
        SetDescription(desc);

        Effect   fx;
        fx.EnableParallel();

        for (auto& layer : canvas.LayerMgr().all())
        {
            // 1. flip image
            Image   dst = layer->RasterImage();
            dst.ApplyEffect(fx);
            AddCommand(make_unique<CmdModifyLayerImage>(layer, std::move(dst)));

            // 2. move layer position
            AddCommand(make_unique<CmdMoveLayer>(layer, CalcNewPosition(canvas.Size(), *layer)));
        }
        AddCommand(make_unique<CmdMFCSetDocModified>());
    }
};

// type aliases
using CmdFlipHorizontalCanvas = CmdFlipCanvas<effect::Mirror>;
using CmdFlipVerticalCanvas = CmdFlipCanvas<effect::Flip>;

_PHOXO_NAMESPACE_END
