#pragma once
#include "dlg_setting_page_general.h"

class DlgSettings : public CBCGPPropertySheet
{
public:
    DlgSettings();

private:
    DlgSettingPageGeneral   m_page_general;

    BOOL OnInitDialog() override;
};
