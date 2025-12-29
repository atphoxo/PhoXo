#pragma once

class CShellThumbModule : public ATL::CAtlDllModuleT< CShellThumbModule >
{
public :
	DECLARE_LIBID(LIBID_ShellThumbLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_SHELLTHUMB, "{3533d239-e6f8-4377-9038-eb773fc05215}")
};

extern class CShellThumbModule _AtlModule;
