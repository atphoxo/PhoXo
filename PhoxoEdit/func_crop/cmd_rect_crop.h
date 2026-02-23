#pragma once

_PHOXO_NAMESPACE(cmd)
using namespace phoxo;

/// Crop the whole canvas to a rectangle (in canvas space)
class CmdRectCrop : public BatchCommand
{
private:
    const CRect   m_crop_on_canvas;
    const int   m_ratio_index{};
    const FixedAspectRatio   m_aspect_ratio;

public:
    explicit CmdRectCrop(const CRect& crop_on_canvas, const Canvas& canvas, int ratio_index);

    void Execute(Canvas& canvas, IProgressListener* progress) override;
    void Undo(Canvas& canvas) override;
    void Redo(Canvas& canvas) override;
};

_PHOXO_NAMESPACE_END
