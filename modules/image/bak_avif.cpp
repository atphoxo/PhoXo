#include "pch.h"
#include "avif.h"
#include <avif/avif_cxx.h>
#include <thread>

// ICC profile support has not been implemented yet
class AvifDecoder
{
public:
    avif::DecoderPtr   m_decoder{ avifDecoderCreate() }; // 里面是malloc(440)一定成功

public:
    AvifDecoder()
    {
        // m_decoder->strictFlags = AVIF_STRICT_DISABLED;
        m_decoder->maxThreads = (std::min)(32, (int)std::thread::hardware_concurrency());
    }

    bool Load(LPCVOID ptr, size_t length)
    {
        return (ptr && length) &&
            (avifDecoderSetIOMemory(m_decoder.get(), (const BYTE*)ptr, length) == AVIF_RESULT_OK) &&
            (avifDecoderParse(m_decoder.get()) == AVIF_RESULT_OK) &&
            m_decoder->image &&
            (m_decoder->imageCount >= 1);
    }

    IWICBitmapPtr DecoderNextImage(IWICImagingFactory* factory)
    {
        // 定位到第一帧
        if (avifDecoderNextImage(m_decoder.get()) == AVIF_RESULT_OK)
        {
            CSize   imgsize = GetSize();
            IWICBitmapPtr   bmp;
            factory->CreateBitmap(imgsize.cx, imgsize.cy, WICNormal32bpp, WICBitmapCacheOnLoad, &bmp);
            if (WIC::BitmapLock lock(bmp); lock.IsFormat32bpp())
            {
                avifRGBImage   rgb = Create_avifRGBImage(lock);
                if (avifImageYUVToRGB(m_decoder->image, &rgb) == AVIF_RESULT_OK)
                    return bmp;
            }
        }
        return nullptr;
    }

    CSize GetSize() const
    {
        return CSize(m_decoder->image->width, m_decoder->image->height);
    }

private:
    avifRGBImage Create_avifRGBImage(WIC::BitmapLock& lock)
    {
        avifRGBImage   img;
        avifRGBImageSetDefaults(&img, m_decoder->image);
        img.maxThreads = m_decoder->maxThreads;
        img.format = AVIF_RGB_FORMAT_BGRA;
        img.depth = 8;
        img.rowBytes = lock.m_stride;
        img.pixels = lock.m_data;
        return img;
    }
};

HANDLE oxo_avif_load_memory(LPCVOID ptr, UINT length, SIZE* image_size, int* frame_count)
{
    auto   img = std::make_unique<AvifDecoder>();
    if (img->Load(ptr, length))
    {
        *frame_count = img->m_decoder->imageCount;
        *image_size = img->GetSize();
        return img.release();
    }
    return nullptr;
}

IWICBitmap* oxo_avif_load_current_frame(HANDLE h, IWICImagingFactory* factory)
{
    if (auto img = (AvifDecoder*)h)
    {
        return img->DecoderNextImage(factory).Detach();
    }
    return nullptr;
}

void oxo_avif_close(HANDLE h)
{
    delete (AvifDecoder*)h;
}
