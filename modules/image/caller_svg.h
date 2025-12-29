#pragma once

class CSVGStream
{
private:
    HANDLE   m_svg;
    D2D_SIZE_F   m_image_size{};

public:
    CSVGStream(PCWSTR filepath)
    {
        m_svg = oxo_svg_load_file(filepath, &m_image_size);
    }

    ~CSVGStream()
    {
        oxo_svg_close(m_svg);
    }

    IWICBitmapPtr CreateBitmap(float scale = 1) const
    {
        int   w = (int)ceil(m_image_size.width * scale);
        int   h = (int)ceil(m_image_size.height * scale);
        if (m_svg && w && h)
        {
            if (IWICBitmapPtr bmp = WIC::CreateBitmap(CSize(w, h), WICPremultiplied32bpp))
            {
                oxo_svg_render(bmp, m_svg, scale);

                auto   t2 = WIC::ConvertFormat(bmp, WICNormal32bpp);
                return WIC::CreateBitmapFromSource(t2);
            }
        }
        return nullptr;
    }

    IWICBitmapPtr CreateBitmapWithWidth(int output_width) const
    {
        if (m_image_size.width > 0)
        {
            return CreateBitmap((float)output_width / m_image_size.width);
        }
        return nullptr;
    }
};
