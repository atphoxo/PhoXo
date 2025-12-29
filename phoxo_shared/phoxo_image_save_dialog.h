#pragma once

class PhoxoImageSaveDialog : public CFileDialog
{
public:
    PhoxoImageSaveDialog(CWnd* parent = NULL, PCWSTR init_file = NULL)
        : CFileDialog(FALSE
        , L"jpg"
        , (init_file && *init_file) ? GetInitFile(init_file) : NULL
        , OFN_OVERWRITEPROMPT | OFN_PATHMUSTEXIST
        , GetFormatFilter()
        , parent)
    {
        m_ofn.nFilterIndex = GetFilterIndex(init_file);
    }

private:
    static CString GetInitFile(PCWSTR init_file)
    {
        if (GetFilterIndex(init_file) != 0)
            return init_file;

        // svg, heic, raw 等不支持的格式，改为默认 jpg
        CPath   t(init_file);
        t.RenameExtension(L".jpg");
        return t;
    }

    static int GetFilterIndex(PCWSTR init_file)
    {
        using enum phoxo::ImageFormat;

        switch (phoxo::ImageFileExtParser::GetType(init_file))
        {
            case Jpeg: return 1;
            case Png:  return 2;
            case Bmp:  return 3;
            case Gif:  return 4;
            case Tiff: return 5;
            case Webp: return 6;
            case Tga:  return 7;
        }
        return 0;
    }

    static CString GetFormatFilter()
    {
        CString   t;
        t.Append(L"JPEG|*.jpg|");
        t.Append(L"PNG|*.png|");
        t.Append(L"BMP|*.bmp|");
        t.Append(L"GIF|*.gif|");
        t.Append(L"TIFF|*.tiff|");
        t.Append(L"WEBP|*.webp|");
        t.Append(L"TGA|*.tga|");
        t.Append(L"DirectDraw Surface (DDS)|*.dds|");

        if (!WIC::IsEncoderMissing(WIC::GUID_ContainerFormat_Jxl))
            t.Append(L"JXL|*.jxl|");

        t += L"|";
        return t;
    }
};
