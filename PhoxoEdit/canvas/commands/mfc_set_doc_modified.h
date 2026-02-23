#pragma once

_PHOXO_BEGIN

/// Mark document as modified.
class CmdMFCSetDocModified : public Command
{
private:
    BOOL   m_flag = TRUE;

public:
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
    static CDocument* GetActiveDocument()
    {
        if (auto main = (CFrameWnd*)AfxGetMainWnd())
            if (auto view = main->GetActiveView())
                return view->GetDocument();
        return nullptr;
    }

    void Swap()
    {
        if (auto doc = GetActiveDocument())
        {
            BOOL   dst = m_flag;
            m_flag = doc->IsModified();
            doc->SetModifiedFlag(dst);
        }
    }
};

_PHOXO_NAMESPACE_END
