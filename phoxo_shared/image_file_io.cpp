#include "pch.h"
#include "image_file_io.h"
#include "../modules/image/caller_imagedll.h"
#pragma comment (lib, "../modules/image/bin/image.lib")

using namespace phoxo;

namespace
{
    CImage3rdDLL& GetImage3rdDLL()
    {
        static CImage3rdDLL   t;
        return t;
    }

    _COM_SMARTPTR_TYPEDEF(IThumbnailCache, __uuidof(IThumbnailCache));
    _COM_SMARTPTR_TYPEDEF(ISharedBitmap, __uuidof(ISharedBitmap));

    Image LoadIconFile(PCWSTR filepath)
    {
        try
        {
            CComPtr<IShellItem>   shname;
            SHCreateItemFromParsingName(filepath, NULL, IID_PPV_ARGS(&shname));

            IThumbnailCachePtr   loader;
            loader.CreateInstance(CLSID_LocalThumbnailCache, NULL, CLSCTX_INPROC);
            ISharedBitmapPtr   bmp;
            loader->GetThumbnail(shname, 256, WTS_FORCEEXTRACTION | WTS_EXTRACTDONOTCACHE, &bmp, NULL, NULL);
            HBITMAP   thumb{};
            bmp->GetSharedBitmap(&thumb);
            return ImageHandler::Make(WIC::CreateBitmapFromHBITMAP(thumb, WICBitmapUseAlpha), WICNormal32bpp);
        }
        catch (_com_error&) { assert(false); }
        return {};
    }

    Image LoadSvg(PCWSTR filepath)
    {
        CSVGStream   loader(filepath);
        if (auto img = loader.CreateBitmap())
        {
            return ImageHandler::Make(img, WICNormal32bpp);
        }
        return {};
    }

    Image LoadHeif(PCWSTR filepath)
    {
        if (auto img = GetImage3rdDLL().LoadHeifImage(filepath))
        {
            return ImageHandler::Make(img, WICNormal32bpp);
        }
        return {};
    }

    Image LoadWith3rd(PCWSTR filepath)
    {
        if (auto bmp = GetImage3rdDLL().LoadFile(filepath))
        {
            return ImageHandler::Make(bmp, WICNormal32bpp);
        }
        return {};
    }

    bool SaveWithWIC(PCWSTR filepath, const Image& img)
    {
        CWICFileEncoder   encoder(filepath);
        return encoder.WriteFile(WIC::CreateBitmapFromHBITMAP(img, WICBitmapUseAlpha));
    }
}

void ImageFileIO::Cleanup()
{
    CImage3rdDLL::Deinit();
}

Image ImageFileIO::LoadFile(PCWSTR filepath)
{
    switch (ImageFileExtParser::GetType(filepath))
    {
        case ImageFormat::Icon:
            return LoadIconFile(filepath);

        case ImageFormat::Svg:
            return LoadSvg(filepath);

        case ImageFormat::Heif:
            return LoadHeif(filepath);
    }

    if (auto img = CodecWIC::LoadFile(filepath, WICNormal32bpp, true)) // 常见格式 + webp, avif, jxl, raw...
        return img;

    if (auto img = LoadWith3rd(filepath)) // tga, psd...
        return img;

    return {};
}

bool ImageFileIO::SaveFile(PCWSTR filepath, const Image& img)
{
    using enum ImageFormat;

    switch (ImageFileExtParser::GetType(filepath))
    {
        case Bmp:
        case Jpeg:
        case Png:
        case Gif:
        case Tiff:
            return CodecGdiplus::SaveFile(filepath, img);
        case Webp:
        case Tga:
            return GetImage3rdDLL().SaveFile(filepath, WIC::CreateBitmapFromHBITMAP(img, WICBitmapUseAlpha));
        case Jxl:
        case Dds:
            return SaveWithWIC(filepath, img);
        default:
            assert(false);
            break;
    }
    return false;
}
