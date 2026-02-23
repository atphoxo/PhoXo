#pragma once
#include "zoom_mapper.h"
#include "layer_manager.h"
#include "canvas_viewport.h"
#include "command_manager.h"

_PHOXO_BEGIN

// int   m_active_layer_index = -1;
// bool   m_dirty = false; // 是否有未保存更改
// 图层混合模式：Blend Mode、透明度
class Canvas
{
private:
    CSize   m_canvas_size; // Original size (e.g., 20000x20000), rarely changed once set; modified only by adjustment/trim/crop operations
    LayerManager   m_layer_mgr;
    ZoomMapper   m_zoom;
    CanvasViewport   m_viewport;
    CommandManager   m_command_mgr;
    int   m_content_version = 1; // Incremented when canvas changes, starts from 1, 0 is invalid
    bool   m_is_current_fitview = false; // 用于缩放时保持fit view

public:
    Canvas(const Canvas&) = delete;

    explicit Canvas(CSize canvas_size);

    int ContentVersion() const { return m_content_version; }
    CSize Size() const { return m_canvas_size; }
    CSize ZoomedSize() const;
    void SetSizeDirect(CSize new_size) { m_canvas_size = new_size; }

    // undo/redo
    const auto& CommandMgr() const { return m_command_mgr; }
    void Execute(unique_ptr<Command>&& cmd, IProgressListener* progress);
    void Undo();
    void Redo();

    // zoom
    const auto& ZoomMapper() const { return m_zoom; }
    double ZoomRatio() const { return m_zoom.m_ratio; }
    void SetZoomRatio(double ratio);

    bool IsCurrentFitView() const { return m_is_current_fitview; }
    void MarkCurrentFitView() { m_is_current_fitview = true; }
    double CalcFitViewRatio(HWND view) const;

    // layer
    const auto& LayerMgr() const { return m_layer_mgr; }
    void AddLayer(const shared_ptr<Layer>& layer, int index = -1);

    void Draw(HDC hdc, HBRUSH background, const ViewportContext& ctx);

    // Render full canvas at original size (no zoom, no viewport)
    Image BuildCanvasImage() const;
};

_PHOXO_NAMESPACE_END
