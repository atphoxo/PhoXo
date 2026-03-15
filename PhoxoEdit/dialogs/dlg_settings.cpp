#include "pch.h"
#include "dlg_settings.h"
#include "local.h"
using namespace settings;

DlgSettings::DlgSettings() : CBCGPPropertySheet(LoadText(0), NULL)
{
    m_psh.dwFlags |= PSH_NOAPPLYNOW; // hide Apply button

    int   list_width = DPICalculator::CastForWindow(160, AfxGetMainWnd());
    SetLook(CBCGPPropertySheet::PropSheetLook_List, list_width);

    SetIconsList(IDSVG_SETTING_ICON, 20, (COLORREF)-1, TRUE /* Auto-scale */);

    AddPage(&m_page_general);

    EnableVisualManagerStyle(TRUE, TRUE);
    EnablePageTransitionEffect(CBCGPPageTransitionManager::BCGPPageTransitionNone);
}

BOOL DlgSettings::OnInitDialog()
{
    ::ShowWindow(::GetDlgItem(m_hWnd, IDOK), SW_HIDE);
    SetDlgItemText(IDCANCEL, theConfig.AppText(L"close"));

    return __super::OnInitDialog();
}
