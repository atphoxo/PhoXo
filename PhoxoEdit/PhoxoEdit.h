#pragma once

class CPhoxoEditApp : public CBCGPWinApp
{
public:
    FCImage   m_pending_new;

    CPhoxoEditApp();
    Canvas* GetCurrentCanvas();

private:
    BOOL InitInstance() override;
    int ExitInstance() override;
    HINSTANCE LoadAppLangResourceDLL() override { return NULL; } // not used
    BOOL LoadSysPolicies() override { m_dwPolicies = 0; return TRUE; } // not used
    //CDocument* OpenDocumentFile(LPCTSTR lpszFileName) override;

    afx_msg void OnClearRecentFiles();
    afx_msg void OnCreateImage();
    afx_msg void OnPasteFromClipboard();
    afx_msg void OnNewFromScanner();
    afx_msg void OnUpdatePasteFromClipboard(CCmdUI* pCmdUI);
    afx_msg void OnUpdateNewFromScanner(CCmdUI* pCmdUI);
    DECLARE_MESSAGE_MAP()
};

extern CPhoxoEditApp theApp;
