#pragma once
#include "command.h"

_PHOXO_BEGIN

class CommandManager final
{
private:
    std::vector<unique_ptr<Command>>   m_undo, m_redo;

public:
    const auto& UndoList() const { return m_undo; }

    bool CanUndo() const { return !m_undo.empty(); }
    bool CanRedo() const { return !m_redo.empty(); }

    void Undo(Canvas& canvas)
    {
        if (!CanUndo()) return;
        auto   cmd = std::move(m_undo.back());
        m_undo.pop_back();
        cmd->Undo(canvas);
        m_redo.push_back(std::move(cmd));
    }

    void Redo(Canvas& canvas)
    {
        if (!CanRedo()) return;
        auto   cmd = std::move(m_redo.back());
        m_redo.pop_back();
        cmd->Redo(canvas);
        m_undo.push_back(std::move(cmd));
    }

    /// Execute a command immediately on canvas and add to undo list.
    void Execute(unique_ptr<Command>&& cmd, Canvas& canvas, IProgressListener* progress)
    {
        if (!cmd) return;

        m_redo.clear(); // like bintree
        cmd->Execute(canvas, progress);

        // If user canceled, do not store
        if (progress && progress->IsUserCanceled())
        {
            cmd->Undo(canvas);
            return;
        }

        // Transfer ownership into undo list
        m_undo.push_back(std::move(cmd));
    }
};

_PHOXO_NAMESPACE_END
