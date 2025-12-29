#pragma once

class SvgImageLoader
{
private:
    HANDLE   m_svg{};
    D2D_SIZE_F   m_size{};

public:
    SvgImageLoader(const std::vector<BYTE>& vec)
    {
        if (UINT len = (UINT)vec.size())
        {
            m_svg = oxo_svg_load_memory(vec.data(), len, &m_size);
        }
    }

    ~SvgImageLoader()
    {
        oxo_svg_close(m_svg);
    }

    CSize GetSize() const
    {
        return { (int)ceil(m_size.width), (int)ceil(m_size.height) };
    }

    IWICBitmapPtr CreateBitmap(SIZE imgsize) const
    {
        IWICBitmapPtr   bmp = WIC::CreateBitmap(imgsize, WICPremultiplied32bpp);
        if (bmp && (m_size.width > 0))
        {
            oxo_svg_render(bmp, m_svg, imgsize.cx / m_size.width);
        }
        return bmp;
    }

    HBITMAP CreateDIB(SIZE imgsize) const
    {
        static int   auto_back = GetAutoSvgBackground();
        auto   bmp = ImageHandler::Make(CreateBitmap(imgsize), WICNormal32bpp);
        if (bmp && auto_back)
        {
            ImproveVisualContrast(bmp);
        }
        return bmp.Detach();
    }

private:
    void ImproveVisualContrast(FCImage& img) const;

    static int GetAutoSvgBackground()
    {
        int   var = true;
        DWORD   len = sizeof(int);
        RegGetValue(HKEY_CURRENT_USER, L"Software\\PhoXoSee\\config", L"auto_svg_background", RRF_RT_DWORD, NULL, &var, &len);
        return var;
    }
};
