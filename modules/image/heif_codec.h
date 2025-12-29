#pragma once
#include <thread>
#include "codec_base.h"

struct HeifContextDeleter
{
    void operator()(heif_context* p)
    {
        heif_context_free(p);
    }
};

class CLibHeifCodec : public CCodecBase
{
private:
    std::unique_ptr<heif_context, HeifContextDeleter>   m_context;
    heif_image_handle   * m_primary = nullptr;

public:
    CLibHeifCodec(LPCVOID buf, size_t length) : m_context(heif_context_alloc())
    {
        if (buf && length)
        {
            heif_context_set_max_decoding_threads(m_context.get(), (int)std::thread::hardware_concurrency());
            auto   err = heif_context_read_from_memory_without_copy(m_context.get(), buf, length, nullptr);
            if (err.code == heif_error_Ok)
            {
                heif_context_get_primary_image_handle(m_context.get(), &m_primary);
                m_image_size.cx = heif_image_handle_get_width(m_primary);
                m_image_size.cy = heif_image_handle_get_height(m_primary);
            }
        }
    }

    ~CLibHeifCodec()
    {
        heif_image_handle_release(m_primary);
    }

    IWICBitmapPtr CreateWICBitmap(IWICImagingFactory* factory) override;
};
