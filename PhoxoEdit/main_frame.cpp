#include "pch.h"
#include "PhoxoEdit.h"
#include "main_frame.h"
#include "main_frame_utils.h"

static_assert(ID_TAB_CROP_ROTATE == 20000); // ID_TAB_CROP_ROTATE 必须是range第一个

IMPLEMENT_DYNCREATE(CMainFrame, CBCGPFrameWnd)
BEGIN_MESSAGE_MAP(CMainFrame, CBCGPFrameWnd)
    ON_WM_CREATE()
    ON_WM_GETMINMAXINFO()
    ON_WM_SIZE()
    ON_WM_CLOSE()
    ON_MESSAGE(MSG_POST_LOAD_FIRST, OnPostLoadFirst)
    // right tab group
    ON_COMMAND_RANGE(ID_TAB_CROP_ROTATE, ID_TAB_LAST_ID, OnRightTab)
    ON_UPDATE_COMMAND_UI_RANGE(ID_TAB_CROP_ROTATE, ID_TAB_LAST_ID, OnUpdateRightTab)
END_MESSAGE_MAP()

namespace
{
}

// SetPersistantFrame(false); // 不要设置，否则不能保存位置了
int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    DPICalculator::g_current_dpi() = GetDpiForWindow(m_hWnd);

    int   ret = __super::OnCreate(lpCreateStruct);

    m_top_toolbar.Create(this);
    m_right_tab_bar.Create(this);

    ChangeWindowMessageFilterEx(m_hWnd, WM_DROPFILES, MSGFLT_ALLOW, NULL);
    ChangeWindowMessageFilterEx(m_hWnd, 0x0049, MSGFLT_ALLOW, NULL); // WM_COPYGLOBALDATA, 当自己高权限，允许低限权explorer拖进来
    DragAcceptFiles(TRUE);

    OnRightTab(ID_TAB_CROP_ROTATE);
    return ret;
}

void CMainFrame::OnRightTab(UINT id)
{
    m_func_panel_manager.OnClickTab(*this, id);
}

void CMainFrame::OnUpdateRightTab(CCmdUI* pCmdUI)
{
    pCmdUI->SetCheck(pCmdUI->m_nID == m_func_panel_manager.CurrentTabID());
}

void CMainFrame::OnEnableIfCanvasValid(CCmdUI* pCmdUI)
{
    pCmdUI->Enable(theApp.GetCurrentCanvas() != NULL);
}

LRESULT CMainFrame::OnPostLoadFirst(WPARAM wParam, LPARAM lParam)
{
    unique_ptr<CString>   cmd{ (CString*)wParam };
    theApp.OpenDocumentFile(*cmd);
    return 0;
}

void CMainFrame::OnGetMinMaxInfo(MINMAXINFO* info)
{
    info->ptMinTrackSize = CMainFrameUtils::GetMinMainWndSize();
    __super::OnGetMinMaxInfo(info);
}

void CMainFrame::OnSize(UINT nType, int cx, int cy)
{
    __super::OnSize(nType, cx, cy);
    if (nType == SIZE_MINIMIZED)
        return;

    if (auto canvas = theApp.GetCurrentCanvas(); canvas && canvas->IsCurrentFitView())
    {
        SendMessage(WM_COMMAND, ID_TOP_ZOOM_FIT_WINDOW);
    }
}

void CMainFrame::OnClose()
{
    theConfig.Save();
    __super::OnClose();
}
