#pragma once

_PHOXO_BEGIN
class Canvas; // forward declaration

/// Base class for all Canvas commands (Undo/Redo)
class Command
{
private:
    CString   m_description;

public:
    Command() = default;
    virtual ~Command() = default;

    void SetDescription(const CString& desc) { m_description = desc; }
    const CString& Description() const { return m_description; }

    /// Execute the command on a canvas
    virtual void Execute(Canvas& canvas, IProgressListener* progress) = 0;
    virtual void Undo(Canvas& canvas) = 0;
    virtual void Redo(Canvas& canvas) = 0;

    /// Estimated memory cost in bytes (undo + redo data)
    // 暂时没用上 -- virtual uint64_t MemoryCost() const { return 0; }
};

/// Composite command: executes multiple commands as one unit
class BatchCommand : public Command
{
private:
    std::vector<unique_ptr<Command>>   m_commands;

public:
    /// Add a sub-command to this composite
    void AddCommand(unique_ptr<Command>&& cmd)
    {
        assert(cmd);
        if (cmd)
        {
            m_commands.push_back(std::move(cmd));
        }
    }

    void Execute(Canvas& canvas, IProgressListener* progress) override
    {
        for (auto& cmd : m_commands)
            cmd->Execute(canvas, progress);
    }

    void Undo(Canvas& canvas) override
    {
        for (auto& cmd : m_commands | std::views::reverse)
            cmd->Undo(canvas);
    }

    void Redo(Canvas& canvas) override
    {
        for (auto& cmd : m_commands)
            cmd->Redo(canvas);
    }
};

_PHOXO_NAMESPACE_END
