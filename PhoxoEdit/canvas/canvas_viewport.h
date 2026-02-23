#pragma once
#include "viewport_context.h"

_PHOXO_BEGIN

class CanvasViewport
{
private:
    const Canvas&   m_canvas;

    // 还有内容 Canvas 内容变化（图片切换或编辑）需要加上
    CRect   m_rect_on_zoomed_canvas; // (scroll_pos , view_size)
    CSize   m_canvas_total_zoomed_size; // 用来监测zoom改变的情况
    int   m_canvas_content_ver = 0;

    Image   m_cache;

public:
    explicit CanvasViewport(const Canvas& canvas) : m_canvas(canvas) {}

    void Draw(HDC hdc, HBRUSH background, const ViewportContext& ctx);

private:
    bool IsCacheValid(const ViewportContext& ctx) const;
    void RebuildCache(const ViewportContext& ctx, HBRUSH background);
};

_PHOXO_NAMESPACE_END
