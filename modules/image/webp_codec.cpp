#include "pch.h"
#include "image.h"
#include <webp/encode.h>
#include <webp/mux.h>

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
