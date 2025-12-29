#include "pch.h"
#include "framework.h"
#include "SetupHelper.h"

namespace
{
    namespace AppDefine
    {
        inline constexpr PCWSTR  UNIQUE_NAME_FLAG = L"0768610E2E4F487181BC73C7016829E1";
    }

    void ForceKillAllRunning()
    {
        for (int i = 0; i < 100; i++)
        {
            HWND   wnd = ::FindWindowEx(HWND_MESSAGE, NULL, NULL, AppDefine::UNIQUE_NAME_FLAG);
            if (!wnd)
                break;

            DWORD   pid = 0;
            GetWindowThreadProcessId(wnd, &pid);
            if (HANDLE process = OpenProcess(PROCESS_TERMINATE, FALSE, pid))
            {
                TerminateProcess(process, 0);
            }
            Sleep(100);
        }
    }
}

void __stdcall RunBeforeInstall()
{
    ForceKillAllRunning();

    // 强制替换不能删除的文件
    CString   folder = FCFile::GetModuleFolder((HMODULE)&__ImageBase);
    for (PCWSTR name : {L"ShellThumb.dll", L"ShellMenu.dll", L"image.dll", L"PhoXoSee.exe"})
    {
        FCFile::ForceDeleteOnReboot(folder + name);
    }
}
