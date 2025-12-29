#include "pch.h"
#include "image.h"
#include "webp_codec.h"
#include "stb_codec.h"
#include "tinyexr_codec.h"
#include "jp2.h"

using enum ImageFormat;

IWICBitmap* oxo_3rd_load(PCWSTR filepath, IWICImagingFactory* factory)
{
    switch (ImageFileExtParser::GetType(filepath))
    {
        case Tga:
        case Psd:
        case StbLib:
            return StbCodec::Load(filepath, factory);

        case Exr:
            return TinyExrCodec::Load(filepath, factory);

        case OpenJpeg:
            return oxo_jpeg2000_load(filepath, factory);
    }
    return nullptr;
}

BOOL oxo_3rd_save(PCWSTR filepath, IWICBitmap* srcbmp, IWICImagingFactory* factory)
{
    switch (ImageFileExtParser::GetType(filepath))
    {
        case Webp:
            return oxo_webp_save(filepath, 75, srcbmp);

        case Tga:
            return StbCodec::SaveTga(filepath, srcbmp, factory);
    }
    return false;
}

IWICBitmap* oxo_stb_load_memory(LPCVOID ptr, UINT length, IWICImagingFactory* factory)
{
    return StbCodec::LoadMemory((const BYTE*)ptr, length, factory);
}
