// dllmain.h : Declaration of module class.

class CExplorerMenuModule : public ATL::CAtlDllModuleT< CExplorerMenuModule >
{
public :
	DECLARE_LIBID(LIBID_ExplorerMenuLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_EXPLORERMENU, "{a91b92ba-e47c-4917-830b-4694af670684}")
};

extern class CExplorerMenuModule _AtlModule;
