#pragma once

class WndRightTabBar : public CBCGPOutlookBar
{
private:
    struct ButtonPanel : public CBCGPOutlookBarPane
    {
        BOOL OnUserToolTip(CBCGPToolbarButton* button, CString& tip) const override;
        void GetMessageString(UINT nID, CString& desp) const override;
    };

    ButtonPanel   m_panel;

public:
    void Create(CWnd* parent);
};
