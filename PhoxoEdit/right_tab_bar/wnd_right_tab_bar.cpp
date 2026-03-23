#include "pch.h"
#include "PhoxoEdit.h"
#include "wnd_right_tab_bar.h"

namespace
{
    constexpr int   BAR_WIDTH = 56;
    constexpr SIZE   BUTTON_SIZE{ 30, 64 };

    CString LoadTip(UINT id, int sub_index)
    {
        LanguageTextGroup   group(L"TAB", id);
        CString   t = group[sub_index];
        if (sub_index == 1) // tooltip may contain \n, which needs to be replaced with actual newlines
        {
            t.Replace(LR"(\n)", L"\n");
        }
        return t;
    }
}

BEGIN_MESSAGE_MAP(WndRightTabBar, CBCGPOutlookBar)
    ON_MESSAGE(WM_DPICHANGED_AFTERPARENT, OnDPIChangedAfterParent)
END_MESSAGE_MAP()

BOOL WndRightTabBar::ButtonPanel::OnUserToolTip(CBCGPToolbarButton* button, CString& tip) const
{
    if (CString s = LoadTip(button->m_nID, 0); !s.IsEmpty())
    {
        tip = s;
        return TRUE;
    }
    return FALSE;
}

void WndRightTabBar::ButtonPanel::GetMessageString(UINT nID, CString& desp) const
{
    desp = LoadTip(nID, 1);
}

void WndRightTabBar::Create(CWnd* parent)
{
    int   init_width = DPICalculator::Cast(BAR_WIDTH);
    DWORD   dock = theConfig.PanelDockStyle();
    CBCGPOutlookBar::Create(L"", parent, CRect(0, 0, init_width, init_width), ID_RIGHT_TAB_BAR, WS_CHILD | WS_VISIBLE | dock);

    auto   container = DYNAMIC_DOWNCAST(CBCGPOutlookWnd, GetUnderlinedWindow());
    container->HideSingleTab(true);

    // Create first page:
    m_panel.Create(this, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_HIDE_INPLACE, ID_RIGHT_TAB_BAR_PANEL);
    m_panel.SetOwner(parent);
    m_panel.SetButtonFullRowWidth(CBCGPOutlookBarPane::ButtonFullRowWidth_Enabled);

    m_panel.LoadIcons(IDSVG_RIGHT_TAB_ICONS, BUTTON_SIZE);

    //for (int idx = 0; UINT id : {ID_TAB_CROP_ROTATE, ID_TAB_TEXT, ID_TAB_ADJUST, ID_TAB_EFFECT, ID_TAB_WIDGET, ID_TAB_FRAME})
    for (int idx = 0; UINT id : {ID_TAB_CROP_ROTATE})
    {
        m_panel.AddButton(L"", id, idx++);
    }

    container->AddTab(&m_panel, L"", (UINT)-1, FALSE);
}

LRESULT WndRightTabBar::OnDPIChangedAfterParent(WPARAM w, LPARAM l)
{
    return __super::OnDPIChangedAfterParent(w, l);
}
