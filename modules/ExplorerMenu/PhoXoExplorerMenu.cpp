#include "pch.h"
#include "PhoXoExplorerMenu.h"
#include "multi_lang_string.h"

using namespace phoxo;
_COM_SMARTPTR_TYPEDEF(IShellItem, __uuidof(IShellItem));

namespace
{
    CString GetSelectedFilepath(IShellItemArray* selection)
    {
        DWORD   count = 0;
        if (SUCCEEDED(selection->GetCount(&count)) && (count == 1))
        {
            IShellItemPtr   item;
            if (SUCCEEDED(selection->GetItemAt(0, &item)) && item)
            {
                CComHeapPtr<WCHAR>   pszPath;
                if (SUCCEEDED(item->GetDisplayName(SIGDN_FILESYSPATH, &pszPath)))
                {
                    return CString(pszPath);
                }
            }
        }
        return L"";
    }

    bool IsShellMenuHidden()
    {
        DWORD   val = 0, len = sizeof(DWORD);
        auto   err = RegGetValue(HKEY_CURRENT_USER, LR"(Software\PhoXoSee\config)", L"hide_shell_menu", RRF_RT_REG_DWORD, NULL, &val, &len);
        return (err == ERROR_SUCCESS) && (val != 0);
    }
}

IFACEMETHODIMP CPhoXoExplorerMenu::GetTitle(IShellItemArray*, PWSTR* name)
{
    return SHStrDup(PhoXoMenuTitle::Get(), name);
}

IFACEMETHODIMP CPhoXoExplorerMenu::GetIcon(IShellItemArray*, LPWSTR* icon)
{
    CString   path = FCFile::GetModuleFolder((HMODULE)&__ImageBase) + L"PhoXoSee.exe,-128";
    return SHStrDup(path, icon);
}

IFACEMETHODIMP CPhoXoExplorerMenu::GetCanonicalName(GUID* guidCommandName)
{
    *guidCommandName = CLSID_PhoXoExplorerMenu;
    return S_OK;
}

IFACEMETHODIMP CPhoXoExplorerMenu::GetState(IShellItemArray* selection, BOOL, EXPCMDSTATE* cmdState)
{
    CString   filepath = GetSelectedFilepath(selection);
    auto   imagetype = ImageFileExtParser::GetType(filepath);

    if (!IsShellMenuHidden() && (imagetype != ImageFormat::Unknown))
        *cmdState = ECS_ENABLED;
    else
        *cmdState = ECS_HIDDEN;
    return S_OK;
}

IFACEMETHODIMP CPhoXoExplorerMenu::Invoke(IShellItemArray* selection, IBindCtx*)
{
    CString   exepath = FCFile::GetModuleFolder((HMODULE)&__ImageBase) + L"PhoXoSee.exe";
    CString   filepath = GetSelectedFilepath(selection);
    ShellExecute(NULL, L"open", exepath, L"\"" + filepath + L"\"", NULL, SW_SHOW);
    return S_OK;
}

IFACEMETHODIMP CPhoXoExplorerMenu::GetFlags(EXPCMDFLAGS* flags)
{
    *flags = ECF_DEFAULT;
    return S_OK;
}
