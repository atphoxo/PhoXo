#pragma once
#include "resource.h"       // main symbols
#include "ExplorerMenu_i.h"

class ATL_NO_VTABLE CPhoXoExplorerMenu :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CPhoXoExplorerMenu, &CLSID_PhoXoExplorerMenu>,
    public IExplorerCommand
{
public:
    DECLARE_REGISTRY_RESOURCEID(106)

    BEGIN_COM_MAP(CPhoXoExplorerMenu)
        COM_INTERFACE_ENTRY(IExplorerCommand)
    END_COM_MAP()

    // Inherited via IExplorerCommand
    IFACEMETHODIMP GetTitle(IShellItemArray*, PWSTR* name) override;
    IFACEMETHODIMP GetIcon(IShellItemArray*, PWSTR* icon) override;
    IFACEMETHODIMP GetToolTip(IShellItemArray*, PWSTR*) override { return E_NOTIMPL; }
    IFACEMETHODIMP GetCanonicalName(GUID* guidCommandName) override;
    IFACEMETHODIMP GetState(IShellItemArray*, BOOL, EXPCMDSTATE* cmdState) override;
    IFACEMETHODIMP Invoke(IShellItemArray*, IBindCtx*) override;
    IFACEMETHODIMP GetFlags(EXPCMDFLAGS* flags) override;
    IFACEMETHODIMP EnumSubCommands(IEnumExplorerCommand** enumCommands) override { return E_NOTIMPL; }
};

OBJECT_ENTRY_AUTO(__uuidof(PhoXoExplorerMenu), CPhoXoExplorerMenu)
