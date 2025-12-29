#pragma once

HANDLE oxo_avif_load_memory(LPCVOID ptr, UINT length, SIZE* image_size, int* frame_count);
IWICBitmap* oxo_avif_load_current_frame(HANDLE h, IWICImagingFactory* factory);
void oxo_avif_close(HANDLE h);
