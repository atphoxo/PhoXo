#include "pch.h"
#include "PhoxoEdit.h"
#include "main_frame.h"
#include "main_doc.h"
#include "main_view.h"
#include "framework/main_doc_manager.h"

CPhoxoEditApp theApp;

BEGIN_MESSAGE_MAP(CPhoxoEditApp, CBCGPWinApp)
 	ON_COMMAND(ID_CREATE_IMAGE, OnCreateImage)
    ON_COMMAND(ID_FILE_OPEN, CBCGPWinApp::OnFileOpen)
    ON_COMMAND(ID_NEW_FROM_CLIPBOARD, OnPasteFromClipboard)
    ON_UPDATE_COMMAND_UI(ID_NEW_FROM_CLIPBOARD, OnUpdatePasteFromClipboard)
    ON_COMMAND(ID_NEW_FROM_SCANNER, OnNewFromScanner)
    ON_UPDATE_COMMAND_UI(ID_NEW_FROM_SCANNER, OnUpdateNewFromScanner)
    ON_COMMAND(ID_CLEAR_RECENT_FILES, OnClearRecentFiles)
    ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
END_MESSAGE_MAP()

namespace
{
}

CPhoxoEditApp::CPhoxoEditApp()
{
    AppConfig::GetInstance(); // init load config
	m_bSaveState = FALSE; // 不保存界面元素，一堆 BCG... 开头的键值
    m_bAfxStoreDockSate = FALSE; // 禁止保存 MFC 框架的停靠窗口状态 Settings\ControlBars-Summary
    m_bMSAASupport = FALSE;

	EnableLoadKeyboardAccelerators(FALSE);
    SetVisualTheme(theConfig.GetVisualTheme());
}

BOOL CPhoxoEditApp::InitInstance()
{
    phoxo::CoreLib::Init();
    EnableD2DSupport(D2D1_FACTORY_TYPE_MULTI_THREADED);
    phoxo::CoreLib::InitD2D(AfxGetD2DState()->GetDirect2dFactory());

	__super::InitInstance();

	EnableTaskbarInteraction(FALSE);
	SetRegistryKey(L"PhoXo");
    free((void*)m_pszProfileName);
    m_pszProfileName = _tcsdup(L"edit");
	SetRegistryBase(L"Settings");
	LoadStdProfileSettings(10);  // Load standard INI file options (including MRU)

    // Register the application's document templates.
    ASSERT(!m_pDocManager);
    m_pDocManager = new CMainDocManager;
	AddDocTemplate(new CSingleDocTemplate(IDR_MAINFRAME, RUNTIME_CLASS(CMainDoc), RUNTIME_CLASS(CMainFrame), RUNTIME_CLASS(CMainView)));

	CCommandLineInfo   cmd;
    ProcessShellCommand(cmd); // 先打开一个空的
    ParseCommandLine(cmd); // 解析cmd

    // Performs some delayed initialization (e.g., opening an image)
    m_pMainWnd->PostMessage(MSG_MAINWND_POST_INIT, (WPARAM)new CString(cmd.m_strFileName));

	// The one and only window has been initialized, so show and update it
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();
	return TRUE;
}

int CPhoxoEditApp::ExitInstance()
{
    ImageFileIO::Cleanup();
    phoxo::CoreLib::Uninit();
    return __super::ExitInstance();
}

BOOL CPhoxoEditApp::LoadWindowPlacement(CRect& normal, int& nFflags, int& nShowCmd)
{
    return __super::LoadWindowPlacement(normal, nFflags, nShowCmd);
}

void CPhoxoEditApp::OnAppAbout()
{
    ShellExecute(NULL, L"open", L"https://www.phoxo.com", NULL, NULL, SW_SHOWNORMAL);
}

void CPhoxoEditApp::OnClearRecentFiles()
{
    if (auto* obj = m_pRecentFileList)
    {
        for (int i = 0; i < obj->GetSize(); i++)
        {
            obj->m_arrNames[i].Empty();
        }
        obj->WriteList(); // 写回注册表
    }
}

void CPhoxoEditApp::OnCreateImage()
{
    m_pending_new.Create(500, 300);
    phoxo::ImageFastPixel::FillColor(m_pending_new, FCColor(Gdiplus::Color::Orange));
    OnFileNew();
}

void CPhoxoEditApp::OnPasteFromClipboard()
{
    m_pending_new.Destroy();
    phoxo::effect::GetClipboard   fx;
    m_pending_new.ApplyEffect(fx);
    OnFileNew();
}

void CPhoxoEditApp::OnNewFromScanner()
{
    CString   tmpfile = FCFile::GetTempFolder() + phoxo::Utils::NewGUIDString();
    PhoxoUtils::WIAPopScanImageDialog(tmpfile);
    m_pending_new = phoxo::CodecGdiplus::LoadFile(tmpfile);
    if (m_pending_new)
    {
        OnFileNew();
    }
    DeleteFile(tmpfile);
}

void CPhoxoEditApp::OnUpdatePasteFromClipboard(CCmdUI* pCmdUI)
{
    pCmdUI->Enable(::IsClipboardFormatAvailable(CF_BITMAP));
}

void CPhoxoEditApp::OnUpdateNewFromScanner(CCmdUI* pCmdUI)
{
    pCmdUI->Enable(PhoxoUtils::WIAIsCanScanImage());
}
