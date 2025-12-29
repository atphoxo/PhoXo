#pragma once

#define TINYEXR_USE_OPENMP 0
#define TINYEXR_USE_STB_ZLIB 1
#include <tinyexr.h>

class TinyExrCodec
{
private:
    struct AutoFreeLib
    {
        float   * m_ptr = nullptr;
        PCSTR   m_err = nullptr;
        int   m_width = 0;
        int   m_height = 0;

        int Load(const auto& vec)
        {
            return LoadEXRFromMemory(&m_ptr, &m_width, &m_height, vec.data(), vec.size(), &m_err);
        }

        void Write(FCColor* dest) const
        {
            auto   src = m_ptr;
            for (int i = 0; i < (m_width * m_height); i++)
            {
                dest->r = (BYTE)(src[0] * 255);
                dest->g = (BYTE)(src[1] * 255);
                dest->b = (BYTE)(src[2] * 255);
                dest->a = (BYTE)(src[3] * 255);
                dest++; src += 4;
            }
        }

        ~AutoFreeLib()
        {
            free(m_ptr);
            FreeEXRErrorMessage(m_err);
        }
    };

public:
    static IWICBitmap* Load(PCWSTR filepath, IWICImagingFactory* factory)
    {
        std::vector<BYTE>   buf;
        FCFile::Read(filepath, buf);
        if (buf.size())
        {
            AutoFreeLib   out;
            if ((out.Load(buf) == TINYEXR_SUCCESS) && out.m_ptr)
            {
                IWICBitmapPtr   bmp;
                factory->CreateBitmap(out.m_width, out.m_height, WICNormal32bpp, WICBitmapCacheOnLoad, &bmp);
                if (WIC::BitmapLock lock(bmp); lock.IsFormat32bpp())
                {
                    out.Write((FCColor*)lock.m_data);
                    return bmp.Detach();
                }
            }
        }
        return nullptr;
    }
};
