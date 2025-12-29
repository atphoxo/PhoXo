#include "pch.h"
#include <winrt/Windows.Management.Deployment.h>
#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Windows.ApplicationModel.h>
#pragma comment(lib, "windowsapp.lib")

using namespace winrt;
using namespace Windows::Foundation;
using namespace Windows::Management::Deployment;

static constexpr PCWSTR PACKAGE_NAME = L"PhoXo.ExplorerMenu";

void RegisterPackage()
{
    CString   selfpath = FCFile::GetModuleFolder();
    CString   msixfile = selfpath + L"ExplorerMenu.msix";

    PackageManager   pm;
    AddPackageOptions   opts;
    opts.ExternalLocationUri(Uri((PCWSTR)selfpath));
    pm.AddPackageByUriAsync(Uri((PCWSTR)msixfile), opts).get(); // synchronously wait for completion
}

void UnregisterPackage()
{
    PackageManager   pm;
    for (const auto& pkg : pm.FindPackagesForUser(L""))
    {
        if (pkg.Id().Name() == PACKAGE_NAME)
        {
            pm.RemovePackageAsync(pkg.Id().FullName()).get(); // synchronously wait for completion
            return;
        }
    }
}

int __stdcall wWinMain(HINSTANCE, HINSTANCE, LPWSTR lpCmdLine, int)
{
    init_apartment();

    try
    {
        if (StrCmpI(lpCmdLine, L"/register") == 0)
        {
            RegisterPackage();
            return 0;
        }

        if (StrCmpI(lpCmdLine, L"/unregister") == 0)
        {
            UnregisterPackage();
            return 0;
        }
    }
    catch (...) {}
    return 0;
}
