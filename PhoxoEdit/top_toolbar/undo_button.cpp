#include "pch.h"
#include "undo_button.h"

void topbar::UndoButton::OnShowPopupMenu()
{
    CleanUpUndoList();
    if (auto canvas = theRuntime.GetCurrentCanvas())
    {
        for (auto& cmd : canvas->CommandMgr().UndoList() | std::views::reverse)
        {
            AddUndoAction(cmd->Description());
        }
    }

    __super::OnShowPopupMenu();
}
