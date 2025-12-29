#include "pch.h"
#include "heif_codec.h"

namespace
{
    void CopyPixelWithConvert(FCColor* dst, const FCColor* src, int pixel_count)
    {
        for (int i = 0; i < pixel_count; i++, dst++, src++)
        {
            *dst = *src;
            dst->b = src->r; // swap R & B
            dst->r = src->b;
        }
    }

    struct HeifImage
    {
        heif_image   * m_img = nullptr;
        CSize   m_image_size;

        HeifImage(const heif_image_handle* hh)
        {
            heif_decode_image(hh, &m_img, heif_colorspace_RGB, heif_chroma_interleaved_RGBA, nullptr);
            if (m_img)
            {
                m_image_size.cx = heif_image_get_width(m_img, heif_channel_interleaved);
                m_image_size.cy = heif_image_get_height(m_img, heif_channel_interleaved);
            }
        }

        ~HeifImage()
        {
            heif_image_release(m_img);
        }

        bool Write(IWICBitmapPtr wicbmp) const
        {
            int   stride = 0;
            auto   pixel = heif_image_get_plane_readonly(m_img, heif_channel_interleaved, &stride);

            WIC::BitmapLock   lock(wicbmp);

            // libheif strideÓÃ16 bytes¶ÔÆë
            if (pixel && lock.IsFormat32bpp() && (stride >= (int)lock.m_stride))
            {
                for (UINT y = 0; y < lock.m_height; y++)
                {
                    CopyPixelWithConvert((FCColor*)lock.m_data, (const FCColor*)pixel, lock.m_width);
                    pixel += stride;
                    lock.m_data += lock.m_stride;
                }
                return true;
            }
            return false;
        }
    };
}

IWICBitmapPtr CLibHeifCodec::CreateWICBitmap(IWICImagingFactory* factory)
{
    if (m_primary)
    {
        HeifImage   img(m_primary);
        IWICBitmapPtr   wicbmp;
        factory->CreateBitmap(img.m_image_size.cx, img.m_image_size.cy, WICNormal32bpp, WICBitmapCacheOnLoad, &wicbmp);
        if (img.Write(wicbmp))
        {
            return wicbmp;
        }
    }
    assert(false);
    return nullptr;
}
