#include "pch.h"
#include "image.h"
#include <lunasvg/lunasvg.h>

#pragma warning( disable : 4244 ) // warning from 'double' to 'float
#define NANOSVG_IMPLEMENTATION
#include "nanosvg.h"

#include <d2d1.h>
#pragma comment (lib, "D2d1.lib")

namespace
{
    bool NanoSVG_GetSize(LPCVOID ptr, UINT len, D2D_SIZE_F* svg_size)
    {
        if (ptr && len)
        {
            std::vector<char>   vec(len + 2); // 结尾0
            memcpy(vec.data(), ptr, len);

            if (auto nano = nsvgParse(vec.data(), "px", 96)) // nano返回永远不为空
            {
                *svg_size = { nano->width, nano->height };
                nsvgDelete(nano);
            }
        }
        return (svg_size->width > 0) && (svg_size->height > 0);
    }

    struct SvgImage
    {
        std::unique_ptr<lunasvg::Document>   m_luna;
        std::unique_ptr<D2D::SVGDrawer>   m_d2d_svg;

        SvgImage(std::unique_ptr<lunasvg::Document>&& t) : m_luna(std::move(t)) {}
        SvgImage(std::unique_ptr<D2D::SVGDrawer>&& t) : m_d2d_svg(std::move(t)) {}
    };
}

HANDLE oxo_svg_load_memory(LPCVOID ptr, UINT length, D2D_SIZE_F* svg_size)
{
    if (auto doc = lunasvg::Document::loadFromData((const char*)ptr, length))
    {
        *svg_size = { doc->width(), doc->height() };
        return new SvgImage(std::move(doc));
    }

    if (NanoSVG_GetSize(ptr, length, svg_size))
    {
        return new SvgImage(std::make_unique<D2D::SVGDrawer>(ptr, length));
    }
    return nullptr;
}

HANDLE oxo_svg_load_file(PCWSTR filepath, D2D_SIZE_F* svg_size)
{
    std::vector<BYTE>   buf;
    FCFile::Read(filepath, buf);
    if (UINT len = (UINT)buf.size())
    {
        return oxo_svg_load_memory(buf.data(), len, svg_size);
    }
    return nullptr;
}

void oxo_svg_render(IWICBitmap* target, HANDLE hsvg, float scale)
{
    auto   svgimg = (SvgImage*)hsvg;
    if (WIC::BitmapLock lock(target); lock.IsFormat32bpp() && svgimg)
    {
        if (svgimg->m_luna)
        {
            lunasvg::Matrix   mx;
            mx.scale(scale, scale);

            lunasvg::Bitmap   lsbmp(lock.m_data, lock.m_width, lock.m_height, lock.m_stride);
            svgimg->m_luna->render(lsbmp, mx);
        }
        else if (svgimg->m_d2d_svg)
        {
            lock.ReleaseLock(); // 上面有 BitmapLock 必须先释放

            ID2D1FactoryPtr   factory;
            ::D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &factory);
            svgimg->m_d2d_svg->DrawSvg(target, scale, factory);
        }
    }
}

void oxo_svg_close(HANDLE hsvg)
{
    delete (SvgImage*)hsvg;
}
