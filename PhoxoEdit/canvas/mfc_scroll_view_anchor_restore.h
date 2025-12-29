#pragma once
#include "mfc_scroll_view_draw_target.h"

class ScrollViewAnchorRestore
{
private:
    CScrollView&   m_view;
    const Canvas&   m_canvas;
    const CPoint   m_old_view_pt;
    const GPointF   m_old_canvas_pt;

public:
    // 尽量保持canvas上的某个点在view上的位置不变
    ScrollViewAnchorRestore(CScrollView& view, const Canvas& canvas, CPoint old_view)
        : m_view{ view }
        , m_canvas{ canvas }
        , m_old_view_pt{ old_view }
        , m_old_canvas_pt{ ScrollViewDrawTarget::ViewToCanvas(view, old_view, canvas) }
    {
    }

    void Restore() const
    {
        GPointF   zoomed_pt = m_canvas.ZoomMapper().ToZoomed(m_old_canvas_pt);
        int   x = std::lround(zoomed_pt.X) - m_old_view_pt.x; // 解等式 zoomed_pt - scroll_offset == view_pt
        int   y = std::lround(zoomed_pt.Y) - m_old_view_pt.y;

        DWORD   style = m_view.GetStyle();
        if (style & WS_HSCROLL) m_view.SetScrollPos(SB_HORZ, x);
        if (style & WS_VSCROLL) m_view.SetScrollPos(SB_VERT, y);

        m_view.Invalidate();
    }
};
