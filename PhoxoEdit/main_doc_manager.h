#pragma once

class CMainDocManager : public CDocManager
{
    DECLARE_DYNAMIC(CMainDocManager)

    BOOL DoPromptFileName(CString& fileName, UINT nIDSTitle, DWORD lFlags, BOOL bOpenFileDialog, CDocTemplate* pTemplate) override;
};
