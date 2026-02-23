#pragma once

_PHOXO_BEGIN

/// Move a single layer to a new position
class CmdMoveLayer : public Command
{
private:
    shared_ptr<Layer>   m_layer;
    CPoint   m_new_pos; // Target position or backup of previous position

public:
    CmdMoveLayer(const shared_ptr<Layer>& layer, CPoint new_pos)
        : m_layer{ layer }
        , m_new_pos{ new_pos }
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
        CPoint   dst = m_new_pos;
        m_new_pos = m_layer->Position();
        m_layer->SetPosition(dst);
    }
};

_PHOXO_NAMESPACE_END
