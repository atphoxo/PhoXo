#pragma once

_PHOXO_BEGIN

// bool   m_locked = false; // 是否锁定（无法编辑）
// m_blend_mode  	混合模式（Normal / Multiply / ...）
// m_effects	图层特效列表（如阴影、描边）
// PVOID extra
// int   m_flag = 0; 例如底层背景
// 
class Layer
{
private:
    CString   m_name; // layer name
    CPoint   m_pos; // layer offset on the canvas
    Image   m_image; // raster image (vector layers may also render to raster)
    BYTE   m_opacity = 100; // opacity 0–100
    bool   m_visible = true; // visibility flag

public:
    Layer(const Layer&) = delete;

    Layer(Image&& img)
        : m_image{ std::move(img) }
    {
    }

    virtual ~Layer() = default;

    int Opacity() const { return m_opacity; }
    void SetOpacity(int opacity) { m_opacity = (BYTE)opacity; }

    CPoint Position() const { return m_pos; }
    void SetPosition(CPoint pos) { m_pos = pos; }
    CRect Bounds() const { return CRect(m_pos, m_image.Size()); }

    const Image& RasterImage() const { return m_image; }
    Image& ImageRef() { return m_image; }

    void RenderPixel(GPointF canvas_pt, Color& target, double ratio) const;

private:
    GPointF CanvasToLayer(GPointF canvas_pt) const
    {
        return { canvas_pt.X - (float)m_pos.x, canvas_pt.Y - (float)m_pos.y };
    }

    void NearestBlend(GPointF layer_pt, Color& target) const;
    void AreaBlend(GPointF layer_pt, Color& target, double ratio) const;
};

_PHOXO_NAMESPACE_END
