#include "pch.h"
#include "image.h"
#include <webp/encode.h>
#include <webp/decode.h>
#include <webp/mux.h>

IWICBitmap* oxo_webp_load_from_memory(LPCVOID data, SIZE_T size, IWICImagingFactory* factory)
{
    int   w = 0, h = 0;
    uint8_t*   decoded = WebPDecodeBGRA((const uint8_t*)data, size, &w, &h);
    if (!decoded)
        return nullptr;

    IWICBitmap*   bmp = nullptr;
    factory->CreateBitmapFromMemory(
        w,
        h,
        WICNormal32bpp,
        w * 4,
        w * 4 * h,
        decoded,
        &bmp);
    WebPFree(decoded);
    return bmp;
}

BOOL oxo_webp_save(PCWSTR filepath, float quality, IWICBitmap* src)
{
    BOOL   result = FALSE;
    if (WIC::BitmapLock lock(src); lock.IsFormat32bpp())
    {
        uint8_t   * out = nullptr;
        if (size_t output_size = WebPEncodeBGRA(lock.m_data, lock.m_width, lock.m_height, lock.m_stride, quality, &out))
        {
            result = FCFile::Write(filepath, out, (DWORD)output_size);
        }
        WebPFree(out);
    }
    return result;
}

void* oxo_webp_save_to_memory(IWICBitmap* src, float quality, UINT32* out_size)
{
    if (WIC::BitmapLock lock(src); lock.IsFormat32bpp())
    {
        uint8_t*   out{};
        *out_size = (UINT32)WebPEncodeBGRA(lock.m_data, lock.m_width, lock.m_height, lock.m_stride, quality, &out);
        return out;
    }
    return nullptr;
}

void oxo_webp_free(void* data)
{
    WebPFree(data);
}
