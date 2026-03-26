#include "pch.h"
#include "PhoxoEdit.h"
#include "main_frame.h"
#include "tool_manager.h"
#include "dialogs/dlg_settings.h"
#include "base/command_line.h"

static_assert(ID_TAB_CROP_ROTATE == 20000); // ID_TAB_CROP_ROTATE 必须是range第一个

IMPLEMENT_DYNCREATE(CMainFrame, CBCGPFrameWnd)
BEGIN_MESSAGE_MAP(CMainFrame, CBCGPFrameWnd)
    ON_WM_CREATE()
    ON_WM_GETMINMAXINFO()
    ON_WM_SIZE()
    ON_WM_DESTROY()
    ON_MESSAGE(WM_DPICHANGED, OnDPIChanged)
    ON_MESSAGE(MSG_MAINWND_POST_INIT, OnPostInit)
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
    DPICalculator::Update(m_hWnd);

    __super::OnCreate(lpCreateStruct);
    SetMenu(NULL);
    PhoxoUtils::EnableWndDragDrop(m_hWnd);

    m_top_toolbar.Create(this);
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

LRESULT CMainFrame::OnPostInit(WPARAM, LPARAM)
{
    m_right_tab_bar.Create(this);
    OnRightTab(ID_TAB_CROP_ROTATE);

    CommandLine   cmds;
    theApp.ParseCommandLine(cmds);
    cmds.NotifyEditorReady();
    if (CString file = cmds.GetStartupOpenFile(); !file.IsEmpty())
    {
        theApp.OpenDocumentFile(file);
    }

    HICON   icon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
    SetIcon(icon, TRUE);	// Set big icon
    SetIcon(icon, FALSE);	// Set small icon

    theRuntime.m_post_init_finished = true;

    // Bug fix: when starting in full-screen mode, failing to refresh the title bar may cause issues
    if (IsZoomed())
    {
        RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW | RDW_ALLCHILDREN);
    }
    return 0;
}

void CMainFrame::OnGetMinMaxInfo(MINMAXINFO* info)
{
    // Note: This is called before OnCreate, so the window is not fully initialized yet
    DPICalculator::Update(m_hWnd);

    info->ptMinTrackSize = { DPICalculator::Cast(600), DPICalculator::Cast(400) };
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
    DPICalculator::Update(m_hWnd);
    return __super::OnDPIChanged(wParam, lParam);
}

void CMainFrame::OnUpdateSelectTheme(CCmdUI* pCmdUI)
{
    int   curr = (int)(pCmdUI->m_nID - ID_THEME_01);
    pCmdUI->SetCheck(theConfig.m_theme_index == curr);
}
