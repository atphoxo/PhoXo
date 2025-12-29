#include "pch.h"
#include "PhoxoEdit.h"
#include "main_doc.h"

IMPLEMENT_DYNCREATE(CMainDoc, CDocument)
BEGIN_MESSAGE_MAP(CMainDoc, CDocument)
    ON_UPDATE_COMMAND_UI(ID_FILE_SAVE, OnUpdateFileSave)
    ON_UPDATE_COMMAND_UI(ID_FILE_SAVE_AS, OnUpdateFileSave)
END_MESSAGE_MAP()

BOOL CMainDoc::OnNewDocument()
{
    if (!__super::OnNewDocument())
        return FALSE;

    m_canvas = nullptr;
    SetModifiedFlag(FALSE);
    SetTitle(theConfig.AppText(L"untitled"));

    if (auto& img = theApp.m_pending_new; img)
    {
        m_canvas = make_unique<Canvas>(img.Size());
        m_canvas->AddLayer(make_shared<Layer>(std::move(img)));
        SetModifiedFlag(TRUE); // 来自剪贴板等要保存
    }
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
        return TRUE;
    }
    else
    {
        CString   key = PathFileExists(filepath) ? L"load_error" : L"not_exist";
        ::BCGPMessageLightBox(filepath, MB_OK | MB_ICONWARNING, NULL, NULL, LanguageText::Get(L"FILE", key));
        UpdateAllViews(NULL);
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

void CMainDoc::OnUpdateFileSave(CCmdUI* ui)
{
    ui->Enable(m_canvas != nullptr);
}
