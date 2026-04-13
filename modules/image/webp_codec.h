#pragma once

IWICBitmap* oxo_webp_load_from_memory(LPCVOID data, SIZE_T size, IWICImagingFactory* factory);
BOOL oxo_webp_save(PCWSTR filepath, float quality, IWICBitmap* src);
