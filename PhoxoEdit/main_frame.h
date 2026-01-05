#pragma once
#include "top_toolbar/wnd_top_toolbar.h"
#include "right_tab_bar/wnd_right_tab_bar.h"
#include "function_panels/panel_manager.h"

class CMainFrame : public CBCGPFrameWnd
{
private:
    WndTopToolbar   m_top_toolbar;
    WndRightTabBar   m_right_tab_bar;
    PanelManager   m_func_panel_manager;

public:
    const WndTopToolbar& TopToolbar() const { return m_top_toolbar; }

private:
    afx_msg void OnClose();
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

    // commands
    afx_msg void OnRightTab(UINT id);
    afx_msg void OnUpdateRightTab(CCmdUI* pCmdUI);
    afx_msg void OnEnableIfCanvasValid(CCmdUI* pCmdUI);

    afx_msg LRESULT OnPostLoadFirst(WPARAM wParam, LPARAM lParam);
    afx_msg void OnGetMinMaxInfo(MINMAXINFO* info);
    afx_msg void OnSize(UINT nType, int cx, int cy);
    DECLARE_MESSAGE_MAP()
    DECLARE_DYNCREATE(CMainFrame)

    friend class CMainFrameUtils;
};
