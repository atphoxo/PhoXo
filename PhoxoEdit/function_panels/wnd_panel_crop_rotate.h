#pragma once

class WndPanelCropRotate : public CBCGPDialogBar
{
private:
    int   m_ratio_index = 0;
    std::map<UINT, unique_ptr<CBCGPButton>>   m_ratio_buttons;

    CBCGPComboBox   m_presets_combo;
    CFont   m_size_edit_font;
    CBCGPEdit   m_width_edit, m_height_edit;

public:
    WndPanelCropRotate();
    void Create(CWnd* parent);

private:
    void InitSizeEdit();
    void ResetSizeEdit();

    void ResetPresetsCombo();

    void DoDataExchange(CDataExchange* pDX) override;

    void OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHandler) override
    {
        __super::OnUpdateCmdUI(pTarget, FALSE); // enableÈ«²¿¿Ø¼þ
    }

    afx_msg void OnRatioButton(UINT id);
    DECLARE_MESSAGE_MAP()
};
