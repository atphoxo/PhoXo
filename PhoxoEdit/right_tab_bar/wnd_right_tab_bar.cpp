#include "pch.h"
#include "PhoxoEdit.h"
#include "wnd_right_tab_bar.h"

namespace
{
    constexpr int   BAR_WIDTH = 56;
    const CSize   BUTTON_SIZE(30, 64);

    CString LoadTip(UINT id, int sub_index)
    {
        LanguageTextGroup   group(L"TAB", id - ID_TAB_CROP_ROTATE);
        return group[sub_index];
    }
}

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
    int   init_width = globalUtils.ScaleByDPI(BAR_WIDTH, this);
    CBCGPOutlookBar::Create(L"TAB", parent, CRect(0, 0, init_width, init_width), ID_RIGHT_TAB_BAR, WS_CHILD | WS_VISIBLE | CBRS_RIGHT, 0);

    auto   container = DYNAMIC_DOWNCAST(CBCGPOutlookWnd, GetUnderlinedWindow());
    container->HideSingleTab(true);

    // Create first page:
    m_panel.Create(this, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_HIDE_INPLACE, ID_RIGHT_TAB_BAR_PANEL);
    m_panel.SetOwner(parent);
    m_panel.EnableDocking(CBRS_ALIGN_RIGHT);
    m_panel.SetButtonFullRowWidth(CBCGPOutlookBarPane::ButtonFullRowWidth_Enabled);

    m_panel.LoadIcons(IDSVG_RIGHT_TAB_ICONS, BUTTON_SIZE);

    for (int idx = 0; UINT id : {ID_TAB_CROP_ROTATE, ID_TAB_ADJUST, ID_TAB_EFFECT, ID_TAB_TEXT, ID_TAB_WIDGET, ID_TAB_FRAME})
    {
        m_panel.AddButton(L"", id, idx++);
    }

    container->AddTab(&m_panel, L"", (UINT)-1, FALSE);
}
