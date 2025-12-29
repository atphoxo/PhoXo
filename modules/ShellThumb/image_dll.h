#pragma once

class ImageDllLoader
{
private:
    IWICBitmapPtr   m_image;

public:
    ImageDllLoader(const std::vector<BYTE>& vec, ImageFormat fmt)
    {
        switch (fmt)
        {
            case ImageFormat::Heif:
                oxo_3rd_init();
                m_image.Attach(oxo_load_heif_memory(vec.data(), (UINT)vec.size(), WIC::g_factory));
                oxo_3rd_deinit();
                break;

            case ImageFormat::Tga:
            case ImageFormat::Psd:
                m_image.Attach(oxo_stb_load_memory(vec.data(), (UINT)vec.size(), WIC::g_factory));
                break;
        }
    }

    CSize GetSize() const
    {
        return WIC::GetBitmapSize(m_image);
    }

    HBITMAP CreateBitmap(SIZE imgsize) const
    {
        auto   bmp = WIC::ScaleBitmap(m_image, imgsize, WICBitmapInterpolationModeFant);
        return ImageHandler::Make(bmp, WICNormal32bpp).Detach();
    }
};
