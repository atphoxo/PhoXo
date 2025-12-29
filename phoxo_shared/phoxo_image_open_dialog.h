#pragma once

class PhoxoImageOpenDialog : public CFileDialog
{
public:
    PhoxoImageOpenDialog(CWnd* parent, PCWSTR all_images, PCWSTR init_folder = NULL)
        : CFileDialog(TRUE
        , NULL
        , init_folder
        , OFN_FILEMUSTEXIST
        , TypeFilter(all_images)
        , parent)
    {
    }

private:
    static CString TypeFilter(PCWSTR all_images)
    {
        PCWSTR   filter =
            L"%s (*.*)|"
            L"*.png;*.bmp;*.gif;*.ico;*.cur;*.webp;*.tiff;*.tif;"
            L"*.jpeg;*.jpe;*.jpg;"
            L"*.heif;*.heic;*.avif;"
            L"*.dng;*.dds;*.arw;*.cr2;*.cr3;*.nef;*.rw2;*.raf;"
            L"*.jxl;*.exr;*.hdr;*.jp2;"
            L"*.psd;*.svg;*.tga||";

        CString   fmt;
        fmt.Format(filter, lstrlen(all_images) ? all_images : L"All Images");
        return fmt;
    }
};
