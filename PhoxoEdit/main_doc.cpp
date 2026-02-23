#include "pch.h"
#include "PhoxoEdit.h"
#include "main_frame.h"
#include "main_doc.h"

IMPLEMENT_DYNCREATE(CMainDoc, CDocument)
BEGIN_MESSAGE_MAP(CMainDoc, CDocument)
    ON_COMMAND(ID_EDIT_UNDO, OnUndo)
    ON_UPDATE_COMMAND_UI(ID_EDIT_UNDO, OnUpdateUndo)
    ON_COMMAND(ID_EDIT_REDO, OnRedo)
    ON_UPDATE_COMMAND_UI(ID_EDIT_REDO, OnUpdateRedo)
    ON_UPDATE_COMMAND_UI(ID_FILE_SAVE, OnUpdateFileSave)
    ON_UPDATE_COMMAND_UI(ID_FILE_SAVE_AS, OnUpdateFileSave)
END_MESSAGE_MAP()

namespace
{
    // canvas 改变但是view还没准备好，不能直接用观察者广播，所以这里用 POST 消息通知
    void PostCanvasReloadedToMainWnd()
    {
        if (auto w = AfxGetMainWnd())
            w->PostMessageW(MSG_POST_CANVAS_RELOADED);
    }
}

void CMainDoc::Execute(unique_ptr<phoxo::Command>&& cmd, phoxo:: IProgressListener* progress)
{
    CWaitCursor   waitcursor;
    if (m_canvas)
        m_canvas->Execute(std::move(cmd), progress);
    UpdateAllViews(NULL);
}

BOOL CMainDoc::OnNewDocument()
{
    if (!__super::OnNewDocument())
        return FALSE;

    SetModifiedFlag(FALSE);
    SetTitle(theConfig.AppText(L"untitled"));

    // 启动时候创建的空doc直接返回，避免post image changed消息
    if (!m_canvas && !theApp.m_pending_new)
        return TRUE;

    m_canvas = nullptr;
    if (auto& img = theApp.m_pending_new; img)
    {
        m_canvas = make_unique<Canvas>(img.Size());
        m_canvas->AddLayer(make_shared<Layer>(std::move(img)));
        SetModifiedFlag(TRUE); // 来自剪贴板等要保存
    }
    PostCanvasReloadedToMainWnd();
    return TRUE;
}

BOOL CMainDoc::OnOpenDocument(LPCTSTR filepath)
{
    m_canvas = nullptr;

    if (FCImage img = ImageFileIO::LoadFile(filepath))
    {
        m_canvas = make_unique<Canvas>(img.Size());
        m_canvas->AddLayer(make_shared<Layer>(std::move(img)));
        SetPathName(filepath);
        PostCanvasReloadedToMainWnd();
        return TRUE;
    }
    else
    {
        CString   key = PathFileExists(filepath) ? L"load_error" : L"not_exist";
        ::BCGPMessageLightBox(filepath, MB_OK | MB_ICONWARNING, NULL, NULL, LanguageText::Get(L"FILE", key));
        UpdateAllViews(NULL);
        SetPathName(filepath, FALSE);
        PostCanvasReloadedToMainWnd();
        return FALSE;
    }
}

BOOL CMainDoc::OnSaveDocument(LPCTSTR filepath)
{
    if (!m_canvas)
        return FALSE;

    const FCImage   img = m_canvas->BuildCanvasImage();

    CWaitCursor   wait_cursor;
    FileSaveAgent   agent(filepath);
    CString   error_text;
    if (ImageFileIO::SaveFile(agent.GetTempFile(), img))
    {
        if (agent.CommitReplace(&error_text))
        {
            SetModifiedFlag(FALSE);
            return TRUE;
        }
    }

    if (!error_text.IsEmpty())
        error_text = L"\r\n" + error_text;

    ::BCGPMessageLightBox(filepath + error_text, MB_OK | MB_ICONWARNING, NULL, NULL, LanguageText::Get(L"FILE", L"save_error"));
    return FALSE;
}

void CMainDoc::OnUndo()
{
    CWaitCursor   waitcursor;
    int   count = ((CMainFrame*)AfxGetMainWnd())->TopToolbar().UndoCount();
    for (int i = 0; i < count; i++)
    {
        if (m_canvas)
            m_canvas->Undo();
    }
    UpdateAllViews(NULL);
}

void CMainDoc::OnUpdateUndo(CCmdUI* ui)
{
    ui->Enable(m_canvas && m_canvas->CommandMgr().CanUndo());
}

void CMainDoc::OnRedo()
{
    CWaitCursor   waitcursor;
    if (m_canvas)
        m_canvas->Redo();
    UpdateAllViews(NULL);
}

void CMainDoc::OnUpdateRedo(CCmdUI* ui)
{
    ui->Enable(m_canvas && m_canvas->CommandMgr().CanRedo());
}

void CMainDoc::OnUpdateFileSave(CCmdUI* ui)
{
    ui->Enable(m_canvas != nullptr);
}
