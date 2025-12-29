#pragma once
#include "top_toolbar/wnd_top_toolbar.h"
#include "right_tab_bar/wnd_right_tab_bar.h"

class CMainFrame : public CBCGPFrameWnd
{
private:
    UINT   m_current_tab = ID_TAB_CROP_ROTATE;
    WndTopToolbar   m_top_toolbar;
    WndRightTabBar   m_right_tab_bar;

public:
    const WndTopToolbar& TopToolbar() const { return m_top_toolbar; }

private:
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
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
