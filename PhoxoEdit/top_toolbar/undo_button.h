#pragma once

namespace topbar
{
    class UndoButton : public CBCGPRibbonUndoButton
    {
    public:
        using CBCGPRibbonUndoButton::CBCGPRibbonUndoButton;
        void OnShowPopupMenu() override;
    };
}
