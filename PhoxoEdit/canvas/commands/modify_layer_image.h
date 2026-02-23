#pragma once

_PHOXO_BEGIN

class CmdModifyLayerImage : public Command
{
private:
    Image   m_swap_image;
    shared_ptr<Layer>   m_layer;

public:
    CmdModifyLayerImage(const shared_ptr<Layer>& layer, Image&& img)
        : m_layer{ layer }
        , m_swap_image{ std::move(img) }
    {
    }

    void Execute(Canvas&, IProgressListener*) override
    {
        Swap();
    }

    void Undo(Canvas&) override
    {
        Swap();
    }

    void Redo(Canvas&) override
    {
        Swap();
    }

private:
    void Swap()
    {
        m_swap_image.Swap(m_layer->ImageRef());
    }
};

_PHOXO_NAMESPACE_END
