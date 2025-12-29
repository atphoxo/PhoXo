#pragma once

#define STBI_ONLY_PSD
#define STBI_ONLY_TGA
#define STBI_ONLY_HDR
#define STBI_ONLY_PNM
#define STB_IMAGE_IMPLEMENTATION
#define STBI_WINDOWS_UTF8
#include <stb_image.h>

#define __STDC_LIB_EXT1__
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

class StbCodec
{
private:
    struct AutoFreeStb
    {
        BYTE   * m_ptr = nullptr;
        ~AutoFreeStb() { if (m_ptr) { stbi_image_free(m_ptr); } }
    };

    static IWICBitmap* CreateBitmap(int width, int height, BYTE* src, IWICImagingFactory* factory)
    {
        IWICBitmapPtr   bmp;
        factory->CreateBitmap(width, height, GUID_WICPixelFormat32bppRGBA, WICBitmapCacheOnLoad, &bmp); // <- note: stb output RGBA
        if (WIC::BitmapLock lock(bmp); lock.IsFormat32bpp())
        {
            memcpy(lock.m_data, src, width * height * 4);
        }
        return bmp.Detach();
    }

public:
    static IWICBitmap* Load(PCWSTR filepath, IWICImagingFactory* factory)
    {
        AutoFreeStb   stb;
        int   width = 0, height = 0, actual_channels = 0;
        if (stb.m_ptr = stbi_load(CW2A(filepath, CP_UTF8), &width, &height, &actual_channels, 4))
        {
            return CreateBitmap(width, height, stb.m_ptr, factory);
        }
        return nullptr;
    }

    static IWICBitmap* LoadMemory(const BYTE* ptr, int length, IWICImagingFactory* factory)
    {
        AutoFreeStb   stb;
        int   width = 0, height = 0, actual_channels = 0;
        if (stb.m_ptr = stbi_load_from_memory(ptr, length, &width, &height, &actual_channels, 4))
        {
            return CreateBitmap(width, height, stb.m_ptr, factory);
        }
        return nullptr;
    }

    static bool SaveTga(PCWSTR filepath, IWICBitmap* srcbmp, IWICImagingFactory* factory)
    {
        auto   rgb_order = WIC::ConvertFormat(srcbmp, GUID_WICPixelFormat32bppRGBA); // note: input is RGBA
        IWICBitmapPtr   src;
        factory->CreateBitmapFromSource(rgb_order, WICBitmapCacheOnLoad, &src);
        if (WIC::BitmapLock lock(src); lock.IsFormat32bpp())
        {
            return stbi_write_tga(CW2A(filepath, CP_UTF8), lock.m_width, lock.m_height, 4, lock.m_data);
        }
        return false;
    }
};
