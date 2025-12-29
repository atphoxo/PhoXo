#pragma once
#include "zoom_mapper.h"
#include "layer_manager.h"
#include "canvas_viewport.h"

_PHOXO_BEGIN

// UndoManager m_undo
// int   m_active_layer_index = -1;
// bool   m_dirty = false; // 是否有未保存更改
// 图层混合模式：Blend Mode、透明度
class Canvas
{
private:
    CSize   m_canvas_size; // 原始大小，例如 20000x20000，一旦定下来不轻易改动，缩放/调整边界/trim等改动
    LayerManager   m_layer_mgr;
    ZoomMapper   m_zoom;
    CanvasViewport   m_viewport;
    int   m_content_version = 1; // canvas变化时递增，从1开始，0为非法
    bool   m_is_current_fitview = false; // 用于缩放时保持fit view

public:
    explicit Canvas(CSize canvas_size);

    int ContentVersion() const { return m_content_version; }
    CSize OriginalSize() const { return m_canvas_size; }
    CSize ZoomedSize() const;

    // zoom 相关
    const auto& ZoomMapper() const { return m_zoom; }
    double ZoomRatio() const { return m_zoom.m_ratio; }
    void SetZoomRatio(double ratio);

    bool IsCurrentFitView() const { return m_is_current_fitview; }
    void MarkCurrentFitView() { m_is_current_fitview = true; }
    double CalcFitViewRatio(HWND view) const;

    // layer 相关
    // Layer* ActiveLayer() const;
    void AddLayer(const shared_ptr<Layer>& layer, int index = -1);
    const auto& AllLayers() const { return m_layer_mgr.all(); }

    void Draw(const CanvasDrawTarget& target);

    // Render full canvas at original size (no zoom, no viewport)
    Image BuildCanvasImage() const;
};

_PHOXO_NAMESPACE_END
