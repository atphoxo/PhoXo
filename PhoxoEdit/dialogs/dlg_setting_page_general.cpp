#include "pch.h"
#include "dlg_setting_page_general.h"
#include "local.h"
using namespace settings;

namespace
{
    enum
    {
        IDC_RENDER_PREFERENCE_TEXT = 3000,
        IDC_RENDER_PREFERENCE_COMBOBOX = 3001,

        IDC_PANEL_DOCK_TEXT = 3002,
        IDC_PANEL_DOCK_COMBOBOX = 3003,

        IDC_MAX_ZOOM_TEXT = 3004,
        IDC_MAX_ZOOM_EDIT = 3005,
    };
}

BEGIN_MESSAGE_MAP(DlgSettingPageGeneral, CBCGPPropertyPage)
    ON_CBN_SELCHANGE(IDC_RENDER_PREFERENCE_COMBOBOX, OnActivateUpdateData)
    ON_CBN_SELCHANGE(IDC_PANEL_DOCK_COMBOBOX, OnActivateUpdateData)
    ON_EN_CHANGE(IDC_MAX_ZOOM_EDIT, OnActivateUpdateData)
END_MESSAGE_MAP()

DlgSettingPageGeneral::DlgSettingPageGeneral()
    : CBCGPPropertyPage(IDD_SETTING_GENERAL)
    , m_title{ LoadText(1) }
{
    m_psp.pszTitle = m_title;
    m_psp.dwFlags |= PSP_USETITLE;

    EnableVisualManagerStyle();
}

BOOL DlgSettingPageGeneral::OnInitDialog()
{
    BOOL   result = __super::OnInitDialog();

    SetDlgItemText(IDC_RENDER_PREFERENCE_TEXT, LoadText(2));
    FCWnd::AddComboString(GetDlgItem(IDC_RENDER_PREFERENCE_COMBOBOX), LoadText(3));

    SetDlgItemText(IDC_PANEL_DOCK_TEXT, LoadText(4));
    FCWnd::AddComboString(GetDlgItem(IDC_PANEL_DOCK_COMBOBOX), LoadText(5));

    SetDlgItemText(IDC_MAX_ZOOM_TEXT, LoadText(6));
    SendDlgItemMessage(IDC_MAX_ZOOM_EDIT, EM_SETLIMITTEXT, 2, 0);

    SetControlInfoTip(IDC_PANEL_DOCK_COMBOBOX, LoadText(7), DT_CENTER);

    return result;
}

void DlgSettingPageGeneral::DoDataExchange(CDataExchange* pDX)
{
    __super::DoDataExchange(pDX);
    DDX_CBIndex(pDX, IDC_RENDER_PREFERENCE_COMBOBOX, (int&)g_config.m_render_preference);
    DDX_CBIndex(pDX, IDC_PANEL_DOCK_COMBOBOX, (int&)g_config.m_panel_dock);

    FCWnd::DDX_Text_NoTip(pDX, IDC_MAX_ZOOM_EDIT, g_config.m_max_zoom_ratio, 16);
    g_config.m_max_zoom_ratio = std::clamp(g_config.m_max_zoom_ratio, 4, 99);
    phoxo::ZoomMapper::s_max_ratio = g_config.m_max_zoom_ratio;
}
