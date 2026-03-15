#pragma once

class ImageFileIO
{
public:
    static void Cleanup();

    static phoxo::Image LoadFile(PCWSTR filepath, bool use_embedded_icc);
    static bool SaveFile(PCWSTR filepath, const phoxo::Image& img, int jpeg_quality);
};
