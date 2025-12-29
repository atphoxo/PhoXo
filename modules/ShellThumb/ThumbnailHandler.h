#pragma once
#include "resource.h"       // main symbols
#include "ShellThumb_i.h"

class ATL_NO_VTABLE CThumbnailHandler :
    public CComObjectRootEx<CComSingleThreadModel>,
    public CComCoClass<CThumbnailHandler, &CLSID_Thumbnail>,
    public IInitializeWithStream,
    public IThumbnailProvider
{
private:
	CString   m_filename;
	std::vector<BYTE>   m_data;

public:
	DECLARE_REGISTRY_RESOURCEID(IDR_THUMBNAIL_HANDLER)
	DECLARE_NOT_AGGREGATABLE(CThumbnailHandler)

	BEGIN_COM_MAP(CThumbnailHandler)
		COM_INTERFACE_ENTRY(IInitializeWithStream)
		COM_INTERFACE_ENTRY(IThumbnailProvider)
	END_COM_MAP()

	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease()
    {
	}

private:
    HBITMAP LoadSvg(UINT cx) const;
    HBITMAP LoadFromImageDll(UINT cx, ImageFormat fmt) const;

    STDMETHOD(Initialize)(IStream *pStream, DWORD grfMode) override;
    STDMETHOD(GetThumbnail)(UINT cx, HBITMAP *phbmp, WTS_ALPHATYPE *pdwAlpha) override;
};

OBJECT_ENTRY_AUTO(__uuidof(Thumbnail), CThumbnailHandler)
