#pragma once

class ImageFileIO
{
public:
    static void Cleanup();

    static phoxo::Image LoadFile(PCWSTR filepath);
    static bool SaveFile(PCWSTR filepath, const phoxo::Image& img);
};
