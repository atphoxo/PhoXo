#include "pch.h"
#include "main_doc_manager.h"

IMPLEMENT_DYNAMIC(CMainDocManager, CDocManager)

BOOL CMainDocManager::DoPromptFileName(CString& fileName, UINT nIDSTitle, DWORD lFlags, BOOL bOpenFileDialog, CDocTemplate* pTemplate)
{
    unique_ptr<CFileDialog>   dlg;
    if (bOpenFileDialog)
    {
        dlg = make_unique<PhoxoImageOpenDialog>(AfxGetMainWnd(), LanguageText::Get(L"FILE", L"open_type"));
    }
    else
    {
        dlg = make_unique<PhoxoImageSaveDialog>(AfxGetMainWnd(), fileName);
    }

    auto   result = dlg->DoModal();
    fileName = dlg->GetPathName();
    return result == IDOK;
}
