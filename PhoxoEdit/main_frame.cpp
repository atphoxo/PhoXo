#include "pch.h"
#include "PhoxoEdit.h"
#include "main_frame.h"
#include "tool_manager.h"
#include "dialogs/dlg_settings.h"

static_assert(ID_TAB_CROP_ROTATE == 20000); // ID_TAB_CROP_ROTATE 必须是range第一个

IMPLEMENT_DYNCREATE(CMainFrame, CBCGPFrameWnd)
BEGIN_MESSAGE_MAP(CMainFrame, CBCGPFrameWnd)
    ON_WM_CREATE()
    ON_WM_GETMINMAXINFO()
    ON_WM_SIZE()
    ON_WM_DESTROY()
    ON_MESSAGE(WM_DPICHANGED, OnDPIChanged)
    ON_MESSAGE(MSG_POST_LOAD_FIRST, OnPostLoadFirst)
    ON_MESSAGE(MSG_POST_CANVAS_RELOADED, OnPostCanvasReloaded)
    // right tab group
    ON_COMMAND_RANGE(ID_TAB_CROP_ROTATE, ID_TAB_LAST_ID, OnRightTab)
    ON_UPDATE_COMMAND_UI_RANGE(ID_TAB_CROP_ROTATE, ID_TAB_LAST_ID, OnUpdateRightTab)
    ON_COMMAND(ID_APP_SETTINGS, OnAppSettings)
    ON_COMMAND(ID_APP_THEMES, OnNoOp)
    ON_COMMAND(ID_TOPBAR_MENU, OnNoOp)
    ON_COMMAND_RANGE(ID_THEME_01, ID_THEME_10, OnSelectTheme)
    ON_UPDATE_COMMAND_UI_RANGE(ID_THEME_01, ID_THEME_10, OnUpdateSelectTheme)
END_MESSAGE_MAP()

// SetPersistantFrame(false); // 不要设置，否则不能保存位置了
int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    DPICalculator::g_current_dpi() = GetDpiForWindow(m_hWnd);

    __super::OnCreate(lpCreateStruct);

    m_top_toolbar.Create(this);
    m_right_tab_bar.Create(this);

    ChangeWindowMessageFilterEx(m_hWnd, WM_DROPFILES, MSGFLT_ALLOW, NULL);
    ChangeWindowMessageFilterEx(m_hWnd, 0x0049, MSGFLT_ALLOW, NULL); // WM_COPYGLOBALDATA, 当自己高权限，允许低限权explorer拖进来
    DragAcceptFiles(TRUE);

    OnRightTab(ID_TAB_CROP_ROTATE);
    return 0;
}

void CMainFrame::OnAppSettings()
{
    DlgSettings   dlg;
    dlg.DoModal();

    if (auto canvas = theRuntime.GetCurrentCanvas())
    {
        canvas->InvalidateViewport();
        theRuntime.InvalidateView();
    }
}

void CMainFrame::OnSelectTheme(UINT id)
{
    int   new_theme_index = id - ID_THEME_01;
    if (theConfig.m_theme_index == new_theme_index)
        return;

    theConfig.m_theme_index = new_theme_index;
    theApp.SetVisualTheme(theConfig.GetVisualTheme());
    theRuntime.OnThemeChanged();
}

void CMainFrame::OnRightTab(UINT id)
{
    m_func_panel_manager.OnClickTab(*this, id);

    if (auto doc = GetActiveDocument())
        doc->UpdateAllViews(NULL);
}

void CMainFrame::OnUpdateRightTab(CCmdUI* pCmdUI)
{
    pCmdUI->SetCheck(pCmdUI->m_nID == m_func_panel_manager.CurrentTabID());
}

void CMainFrame::OnEnableIfCanvasValid(CCmdUI* pCmdUI)
{
    pCmdUI->Enable(theRuntime.GetCurrentCanvas() != NULL);
}

LRESULT CMainFrame::OnPostCanvasReloaded(WPARAM, LPARAM)
{
    if (auto tool = theToolManager.GetActiveTool())
    {
        tool->OnCanvasReloaded();
    }
    return 0;
}

LRESULT CMainFrame::OnPostLoadFirst(WPARAM wParam, LPARAM)
{
    unique_ptr<CString>   cmd{ (CString*)wParam };
    if (cmd->GetLength())
    {
        theApp.OpenDocumentFile(*cmd);
    }
    return 0;
}

namespace
{
    CPoint GetMinMainWndSize()
    {
        CRect   rc;
        SystemParametersInfo(SPI_GETWORKAREA, sizeof(RECT), rc, 0);
        rc.DeflateRect(200, 150);

        int   x = (std::min)(DPICalculator::Cast(500), rc.Width()); // 如果屏幕很小的情况下做个保护
        int   y = (std::min)(DPICalculator::Cast(400), rc.Height());
        return { x, y };
    }
}

void CMainFrame::OnGetMinMaxInfo(MINMAXINFO* info)
{
    info->ptMinTrackSize = GetMinMainWndSize();
    __super::OnGetMinMaxInfo(info);
}

void CMainFrame::OnSize(UINT nType, int cx, int cy)
{
    __super::OnSize(nType, cx, cy);
    if (nType == SIZE_MINIMIZED)
        return;

    if (auto canvas = theRuntime.GetCurrentCanvas(); canvas && canvas->IsCurrentFitView())
    {
        SendMessage(WM_COMMAND, ID_TOP_ZOOM_FIT_WINDOW);
    }
}

BOOL CMainFrame::OnShowPopupMenu(CBCGPPopupMenu* popmenu)
{
    return __super::OnShowPopupMenu(popmenu);
}

void CMainFrame::OnDestroy()
{
    theToolManager.Shutdown();
    theConfig.Save();
    __super::OnDestroy();
}

LRESULT CMainFrame::OnDPIChanged(WPARAM wParam, LPARAM lParam)
{
    DPICalculator::g_current_dpi() = GetDpiForWindow(m_hWnd);
    return __super::OnDPIChanged(wParam, lParam);
}

void CMainFrame::OnUpdateSelectTheme(CCmdUI* pCmdUI)
{
    int   curr = (int)(pCmdUI->m_nID - ID_THEME_01);
    pCmdUI->SetCheck(theConfig.m_theme_index == curr);
}
