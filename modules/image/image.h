#pragma once

#ifdef PHOXODLL_EXPORTS
#define IMAGE_API __declspec(dllexport)
#else
#define IMAGE_API __declspec(dllimport)
#endif

extern "C"
{
    // heif需要初始化
    IMAGE_API void oxo_3rd_init();
    IMAGE_API void oxo_3rd_deinit();

    IMAGE_API IWICBitmap* oxo_load_heif_memory(LPCVOID ptr, UINT length, IWICImagingFactory* factory);
    IMAGE_API IWICBitmap* oxo_load_heif_file(PCWSTR filepath, IWICImagingFactory* factory);

    // 只能渲染 Premultiplied 32bpp
    IMAGE_API HANDLE oxo_svg_load_memory(LPCVOID ptr, UINT length, D2D_SIZE_F* svg_size);
    IMAGE_API HANDLE oxo_svg_load_file(PCWSTR filepath, D2D_SIZE_F* svg_size);
    IMAGE_API void oxo_svg_render(IWICBitmap* target, HANDLE hsvg, float scale);
    IMAGE_API void oxo_svg_close(HANDLE hsvg);

    IMAGE_API IWICBitmap* oxo_stb_load_memory(LPCVOID ptr, UINT length, IWICImagingFactory* factory);

    IMAGE_API IWICBitmap* oxo_3rd_load(PCWSTR filepath, IWICImagingFactory* factory);
    IMAGE_API BOOL oxo_3rd_save(PCWSTR filepath, IWICBitmap* img, IWICImagingFactory* factory);
}
