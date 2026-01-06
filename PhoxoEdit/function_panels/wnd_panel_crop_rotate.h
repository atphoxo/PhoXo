#pragma once

class WndPanelCropRotate : public CBCGPDialogBar
{
private:
    int   m_ratio_index = 0;
    std::map<UINT, unique_ptr<BCGImageButton>>   m_image_buttons;

    int   m_keep_aspect = FALSE;
    CBCGPComboBox   m_presets_combo;
    CBCGPEdit   m_width_edit, m_height_edit;

public:
    WndPanelCropRotate();
    void Create(CWnd* parent);

private:
    BCGImageButton& AddImageButton(UINT id);

    void InitSizeEdit();
    void ResetSizeEdit();

    void UpdateKeepAspectButton();
    void ResetPresetsCombo();

    void DoDataExchange(CDataExchange* pDX) override;

    void OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHandler) override
    {
        __super::OnUpdateCmdUI(pTarget, FALSE); // enableÈ«²¿¿Ø¼þ
    }

    afx_msg void OnPostUpdateKeepAspect();
    afx_msg void OnRatioButton(UINT id);
    afx_msg void OnKeepAspect();
    DECLARE_MESSAGE_MAP()
};
