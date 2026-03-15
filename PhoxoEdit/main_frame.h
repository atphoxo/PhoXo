#pragma once
#include "top_toolbar/wnd_top_toolbar.h"
#include "right_tab_bar/wnd_right_tab_bar.h"
#include "panel_manager.h"

class CMainFrame : public CBCGPFrameWnd
{
private:
    WndTopToolbar   m_top_toolbar;
    WndRightTabBar   m_right_tab_bar;
    PanelManager   m_func_panel_manager;

public:
    const WndTopToolbar& TopToolbar() const { return m_top_toolbar; }

private:
    BOOL OnShowPopupMenu(CBCGPPopupMenu* popmenu) override;

    afx_msg void OnClose();
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

    // commands
    afx_msg void OnAppSettings();
    afx_msg void OnAppThemes();
    afx_msg void OnSelectTheme(UINT id);
    afx_msg void OnRightTab(UINT id);
    afx_msg void OnUpdateRightTab(CCmdUI* pCmdUI);
    afx_msg void OnEnableIfCanvasValid(CCmdUI* pCmdUI);

    afx_msg LRESULT OnPostCanvasReloaded(WPARAM, LPARAM);
    afx_msg LRESULT OnPostLoadFirst(WPARAM, LPARAM);
    afx_msg void OnGetMinMaxInfo(MINMAXINFO* info);
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg LRESULT OnDPIChanged(WPARAM, LPARAM);
    afx_msg void OnUpdateSelectTheme(CCmdUI* pCmdUI);
    DECLARE_MESSAGE_MAP()
    DECLARE_DYNCREATE(CMainFrame)
};
