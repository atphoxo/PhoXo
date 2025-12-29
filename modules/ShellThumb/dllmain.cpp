#include "pch.h"
#include "framework.h"
#include "resource.h"
#include "ShellThumb_i.h"
#include "dllmain.h"

CShellThumbModule _AtlModule;

// DLL Entry Point
extern "C" BOOL WINAPI DllMain(HINSTANCE, DWORD dwReason, LPVOID lpReserved)
{
	return _AtlModule.DllMain(dwReason, lpReserved);
}
