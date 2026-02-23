#pragma once

_PHOXO_BEGIN

/// Directly set the canvas size
class CmdSetCanvasSize : public Command
{
private:
    CSize   m_new_size; // Target canvas size or backup of previous size during swap

public:
    explicit CmdSetCanvasSize(CSize new_size) : m_new_size(new_size)
    {
    }

    void Execute(Canvas& canvas, IProgressListener*) override
    {
        Swap(canvas);
    }

    void Undo(Canvas& canvas) override
    {
        Swap(canvas);
    }

    void Redo(Canvas& canvas) override
    {
        Swap(canvas);
    }

private:
    void Swap(Canvas& canvas)
    {
        CSize   dst = m_new_size;
        m_new_size = canvas.Size();
        canvas.SetSizeDirect(dst);
    }
};

_PHOXO_NAMESPACE_END
