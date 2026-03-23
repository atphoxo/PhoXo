#pragma once

_PHOXO_BEGIN

/// Generic canvas resize command
class CmdResizeImage : public BatchCommand
{
public:
    CmdResizeImage(const Canvas& canvas, CSize new_size, const CString& desc)
    {
        SetDescription(desc);

        for (auto& layer : canvas.LayerMgr().all())
        {
            // 1. resize layer image
            Image   dst{ layer->RasterImage() };
            effect::ResizeWic   fx(new_size);
            dst.ApplyEffect(fx);
            AddCommand(make_unique<CmdModifyLayerImage>(layer, std::move(dst)));
        }

        AddCommand(make_unique<CmdSetCanvasSize>(new_size));
        AddCommand(make_unique<CmdMFCSetDocModified>());
    }
};

_PHOXO_NAMESPACE_END
