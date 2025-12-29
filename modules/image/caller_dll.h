#pragma once

class CImage3rdDLL
{
public:
    static inline int   s_constructor_count = 0; // 如果用户只看常见格式，那从头到尾image.dll都不会被load

public:
    CImage3rdDLL()
    {
        s_constructor_count++;
        oxo_3rd_init();
    }

    static void Deinit()
    {
        if (s_constructor_count)
        {
            oxo_3rd_deinit();
        }
    }

    IWICBitmapPtr LoadHeifImage(PCWSTR filepath)
    {
        IWICBitmapPtr   t;
        t.Attach(oxo_load_heif_file(filepath, WIC::g_factory)); assert(t);
        return t;
    }

    IWICBitmapPtr LoadFile(PCWSTR filepath)
    {
        IWICBitmapPtr   t;
        t.Attach(oxo_3rd_load(filepath, WIC::g_factory)); assert(t);
        return t;
    }

    BOOL SaveFile(PCWSTR filepath, IWICBitmapSource* img)
    {
        IWICBitmapPtr   t = WIC::EnsureBitmapCreated(img);
        return oxo_3rd_save(filepath, t, WIC::g_factory);
    }
};
