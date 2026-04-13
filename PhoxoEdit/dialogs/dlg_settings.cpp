#include "pch.h"
#include "dlg_settings.h"
#include "local.h"
using namespace settings;

DlgSettings::DlgSettings() : CBCGPPropertySheet(LoadText(0), NULL)
{
    m_psh.dwFlags |= PSH_NOAPPLYNOW; // hide Apply button

    int   list_width = DPICalculator::Cast(160);
    SetLook(CBCGPPropertySheet::PropSheetLook_List, list_width);

    SetIconsList(IDSVG_SETTING_ICON, 20, (COLORREF)-1, TRUE /* Auto-scale */);

    AddPage(&m_page_general);

    BOOL   NCArea = !globalData.bIsWindows11;
    EnableVisualManagerStyle(TRUE, NCArea); // Use Win32 to set caption for Windows 11 and later

    EnablePageTransitionEffect(CBCGPPageTransitionManager::BCGPPageTransitionNone);
}

BOOL DlgSettings::OnInitDialog()
{
    ::ShowWindow(::GetDlgItem(m_hWnd, IDOK), SW_HIDE);
    SetDlgItemText(IDCANCEL, g_config.AppText(L"close"));

    BCGUtils::SyncCaptionWithVisualManager(m_hWnd);

    return __super::OnInitDialog();
}
