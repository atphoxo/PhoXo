#include "pch.h"
#include "ThumbnailHandler.h"
#include "svg.h"
#include "image_dll.h"

namespace
{
    struct AutoWICCom
    {
        AutoWICCom() { WIC::g_factory.CreateInstance(CLSID_WICImagingFactory); }
        ~AutoWICCom() { WIC::g_factory = nullptr; }
    };

    struct AutoSTATSTG : public STATSTG
    {
        UINT   m_length = 0;

        AutoSTATSTG(IStream* stream) : STATSTG{}
        {
            if (stream && SUCCEEDED(stream->Stat(this, STATFLAG_DEFAULT)))
            {
                if (auto len = cbSize.QuadPart; len && (len < 200 * 1000 * 1000))
                {
                    m_length = (UINT)len;
                }
            }
        }

        ~AutoSTATSTG()
        {
            if (pwcsName) { CoTaskMemFree(pwcsName); }
        }
    };

    CSize CalcFitSize(UINT cx, SIZE imgsize)
    {
        CRect   rc(0, 0, cx, cx);
        return Utils::CalculateFitWindow(imgsize, rc).Size();
    }
}

HBITMAP CThumbnailHandler::LoadSvg(UINT cx) const
{
    SvgImageLoader   loader(m_data);
    CSize   imgsize = CalcFitSize(cx, loader.GetSize());
    return loader.CreateDIB(imgsize);
}

HBITMAP CThumbnailHandler::LoadFromImageDll(UINT cx, ImageFormat fmt) const
{
    ImageDllLoader   loader(m_data, fmt);
    CSize   imgsize = CalcFitSize(cx, loader.GetSize());
    return loader.CreateBitmap(imgsize);
}

STDMETHODIMP CThumbnailHandler::Initialize(IStream* stream, DWORD grfMode)
{
    if (AutoSTATSTG stat(stream); stat.m_length)
    {
        stream->Seek(LARGE_INTEGER{}, STREAM_SEEK_SET, NULL);

        m_data.resize(stat.m_length);
        ULONG   read = 0;
        if (SUCCEEDED(stream->Read(m_data.data(), stat.m_length, &read)) && read == m_data.size())
        {
            m_filename = stat.pwcsName;
        }
        else
        {
            m_data.clear();
        }
    }
    return S_OK;
}

STDMETHODIMP CThumbnailHandler::GetThumbnail(UINT cx, HBITMAP *phbmp, WTS_ALPHATYPE *pdwAlpha)
{
    using enum ImageFormat;
    AutoWICCom   auto_wic;
    if (m_data.size() && phbmp && pdwAlpha)
    {
        *pdwAlpha = WTSAT_ARGB;
        switch (auto fmt = ImageFileExtParser::GetType(m_filename))
        {
            case Svg:
                if (*phbmp = LoadSvg(cx))
                    return S_OK;
                break;

            case Heif:
            case Tga:
            case Psd:
                if (*phbmp = LoadFromImageDll(cx, fmt))
                    return S_OK;
                break;
        }

        *pdwAlpha = WTSAT_UNKNOWN; //  ß∞‹¡À£¨ª÷∏¥ƒ¨»œ÷µ
    }
    return E_FAIL;
}
