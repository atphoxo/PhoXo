#include "pch.h"
#include "framework.h"
#include "image.h"
#include "heif_codec.h"

void oxo_3rd_init()
{
    heif_init(nullptr);
}

void oxo_3rd_deinit()
{
    heif_deinit();
}

IWICBitmap* oxo_load_heif_memory(LPCVOID ptr, UINT length, IWICImagingFactory* factory)
{
    CLibHeifCodec   loader(ptr, length);
    if (loader.m_image_size.cx)
    {
        return loader.CreateWICBitmap(factory).Detach();
    }
    return nullptr;
}

IWICBitmap* oxo_load_heif_file(PCWSTR filepath, IWICImagingFactory* factory)
{
    std::vector<BYTE>   buf;
    FCFile::Read(filepath, buf);
    if (UINT len = (UINT)buf.size())
    {
        return oxo_load_heif_memory(buf.data(), len, factory);
    }
    return nullptr;
}

/*
HANDLE oxo_create_image_loader(PCWSTR filepath, SIZE* image_size)
{
    switch (CImageFileExtParser::GetType(filepath))
    {
        case ImageFormat::Heif:
        {
            auto   t = std::make_unique<CLibHeifCodec>(filepath);
            *image_size = t->m_image_size;
            if (image_size->cx)
                return t.release();
        }
        break;
    }
    return nullptr;
}

void oxo_delete_image_loader(HANDLE loader)
{
    delete (CCodecBase*)loader;
}

IWICBitmap* oxo_load_image(HANDLE loader, IWICImagingFactory* factory)
{
    auto   bmp = ((CCodecBase*)loader)->CreateWICBitmap(factory);
    return bmp.Detach();
}
*/
