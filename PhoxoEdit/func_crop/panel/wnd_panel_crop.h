#pragma once
#include "page_expander.h"

class WndPanelCrop : public CBCGPDialogBar,
                     public IEventObserverBase
{
private:
    int   m_ratio_index = 0;
    int   m_lock_aspect = FALSE;

    CBCGPEdit   m_width_edit, m_height_edit;
    std::map<int, unique_ptr<BCGImageButton>>   m_image_buttons;

    // 初始shape panel隐藏，点击按钮后展开
    crop::PageExpander   m_shape_panel;

public:
    static inline WndPanelCrop*   s_panel = nullptr; // global pointer

    WndPanelCrop();
    ~WndPanelCrop() { s_panel = nullptr; }
    void Create(CWnd* parent);
    void OnViewKeyDown(UINT nChar);
    void OnViewContextMenu(CPoint point);

private:
    void ReloadButtonIcon();
    BOOL PreTranslateMessage(MSG* pMsg) override;

    BCGImageButton& AddImageButton(int id);

    void InitSizeEdit();
    void UpdateSizeEdit();
    void UpdateKeepAspectButton();
    void ApplyCropSizeFromSingleEdit(HWND edit_ctrl);

    void DoDataExchange(CDataExchange* pDX) override;
    void OnObserveEvent(ObservedEvent& event) override;
    void OnEventCanvasReloaded();
    // COLORREF GetBorderColor(BOOL) const override{ return globalData.clrBarDkShadow; }

    afx_msg void OnWidthEditKillFocus();
    afx_msg void OnHeightEditKillFocus();
    afx_msg void OnEnableIfCropValid(CCmdUI* pCmdUI);
    afx_msg void OnEnableIfCanvasValid(CCmdUI* pCmdUI);
    afx_msg void OnPostUpdateKeepAspect();
    afx_msg void OnRatioButton(UINT id);
    afx_msg void OnKeepAspect();
    afx_msg void OnCancelCrop();
    afx_msg void OnApplyCrop();
    afx_msg LRESULT OnChangeVisualManager(WPARAM wp, LPARAM lp);
    DECLARE_MESSAGE_MAP()
};
