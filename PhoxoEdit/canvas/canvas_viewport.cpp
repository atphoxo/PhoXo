#include "pch.h"
#include "canvas.h"
#include "viewport_composer.h"

_PHOXO_BEGIN

void CanvasViewport::Draw(const CanvasDrawTarget& target)
{
    if (!IsCacheValid(target))
    {
        RebuildCache(target);
    }

    target.DrawImage(m_cache);
}

bool CanvasViewport::IsCacheValid(const CanvasDrawTarget& target) const
{
    return m_cache
        && (m_rect_on_zoomed_canvas == target.src_rect_on_zoomed_canvas) // zoom区域大小改变，或者滚动条位置改变
        && (m_canvas_total_zoomed_size == m_canvas.ZoomedSize()) // zoom改变
        && (m_canvas_content_ver == m_canvas.ContentVersion()); // canvas内容改变
}

void CanvasViewport::RebuildCache(const CanvasDrawTarget& target)
{
    // 更新缓存帧信息
    m_rect_on_zoomed_canvas = target.src_rect_on_zoomed_canvas;
    m_canvas_total_zoomed_size = m_canvas.ZoomedSize();
    m_canvas_content_ver = m_canvas.ContentVersion();

    // 比如滚动图片/图片画线，缓存尺寸是不变的，这里不需要重新创建位图
    if (CSize sz = m_rect_on_zoomed_canvas.Size(); m_cache.Size() != sz)
        m_cache.Create(sz);

    // 填充背景棋盘格
    target.FillImageCheckerboard(m_cache);

    // 渲染 Canvas 到缓存
    ViewportComposer   fx(m_canvas, m_rect_on_zoomed_canvas.TopLeft());
    m_cache.ApplyEffect(fx);
}

_PHOXO_NAMESPACE_END
