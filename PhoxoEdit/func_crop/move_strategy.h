#pragma once
#include "grip_handle.h"

namespace crop
{
    class MoveStrategy
    {
    private:
        enum class Action
        {
            Create,
            Modify
        };

    private:
        // All coordinates are in CANVAS SPACE
        Action    m_action;
        GripType  m_type;
        CRect     m_begin;
        GPointF   m_anchor;
        const CSize   m_canvas_size;

    public:
        MoveStrategy(GripType type, GPointF anchor, const CRect& begin_rect, CSize canvas_size)
            : m_action(begin_rect.IsRectEmpty() ? Action::Create : Action::Modify)
            , m_type(type)
            , m_begin(begin_rect)
            , m_anchor(anchor)
            , m_canvas_size(canvas_size)
        {
            // 1. 限制在canvas范围内，否则在canvas外点击拖拽也会生成裁剪框，鼠标指针不跟随裁剪区域
            // 2. 不要用canvas_size - 1，否则从右往左选取后再NormalizeRect最右边会有一行选不上
            if (m_action == Action::Create)
            {
                m_anchor.X = std::clamp(anchor.X, 0.f, (float)canvas_size.cx);
                m_anchor.Y = std::clamp(anchor.Y, 0.f, (float)canvas_size.cy);
            }
        }

        // Returns the computed crop rectangle
        CRect HandleMouseMove(GPointF cur, const FixedAspectRatio& ratio);

    private:
        CRect CreateCrop(GPointF cur, const FixedAspectRatio& ratio);

        CRect MoveCrop(GPointF delta) const;
        int ClampEdge(double v, GripType edge) const;
        void MoveSingleEdge(CRect& rc, GPointF delta, GripType edge) const;
        void BackCalculateEdge(CRect& rc, double ratio, GripType edge) const;
        CRect ResizeEdge(GPointF delta, GripType edge, const FixedAspectRatio& ratio, GripType fixed_side) const;

        CRect ResizeCornerLocked(GPointF delta, const FixedAspectRatio& ratio) const;
        CRect ResizeCornerUnlocked(GPointF delta) const;
    };
}
