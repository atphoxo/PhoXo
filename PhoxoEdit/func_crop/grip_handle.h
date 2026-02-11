#pragma once
#include "crop_aspect_ratio.h"

namespace crop
{
    enum class GripType
    {
        None,
        TopLeft,
        Top,
        TopRight,
        Left,
        Right,
        BottomLeft,
        Bottom,
        BottomRight,
        Move,   // 点击中心，移动整个裁剪框
    };

    class GripHandle
    {
        bool   m_hovered = false;
    public:
        const GripType   m_type;

        GripHandle(GripType type) : m_type(type) {}

        static bool IsInnerMoveOnly(const CRect& crop_on_view);

        bool IsHovered() const { return m_hovered; }
        CRect GetHitZone(const CRect& crop_on_view) const;
        void Draw(HDC hdc, const CRect& crop_on_view) const;
        bool UpdateHoverState(CPoint cursor_on_view, const CRect& crop_on_view, GripHandle* first_hover);
    };
}
