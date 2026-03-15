#pragma once

class DlgSettingPageGeneral : public CBCGPPropertyPage
{
private:
    const CString   m_title;

public:
    DlgSettingPageGeneral();

private:
    BOOL OnInitDialog() override;
    void DoDataExchange(CDataExchange* pDX) override;
    afx_msg void OnActivateUpdateData() { UpdateData(); }
    DECLARE_MESSAGE_MAP()
};
