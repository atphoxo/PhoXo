#include "pch.h"
#include "move_strategy.h"

// lock ratio时：
// 调整一个grip，它的对角总是保持不变，比如调整top-left，则bottom-right始终固定。调整top，则bottom固定（这时候你需要再指定一个边left or right固定）

_PHOXO_NAMESPACE(crop)

using enum GripType;

int MoveStrategy::ClampEdge(double v, GripType edge) const
{
    if (edge == Top)     return std::clamp((long)v, 0L, m_begin.bottom - 1);
    if (edge == Bottom)  return std::clamp((long)v, m_begin.top + 1, m_canvas_size.cy);
    if (edge == Left)    return std::clamp((long)v, 0L, m_begin.right - 1);
    assert(edge == Right);
    return std::clamp((long)v, m_begin.left + 1, m_canvas_size.cx);
}

void MoveStrategy::MoveSingleEdge(CRect& rc, GPointF delta, GripType edge) const
{
    if (edge == Top)          rc.top = ClampEdge(rc.top + (int)delta.Y, edge);
    else if (edge == Bottom)  rc.bottom = ClampEdge(rc.bottom + (int)delta.Y, edge);
    else if (edge == Left)    rc.left = ClampEdge(rc.left + (int)delta.X, edge);
    else if (edge == Right)   rc.right = ClampEdge(rc.right + (int)delta.X, edge);
}

void MoveStrategy::BackCalculateEdge(CRect& rc, double ratio, GripType edge) const
{
    if (edge == Top)          rc.top = ClampEdge(rc.bottom - rc.Width() / ratio, edge);
    else if (edge == Bottom)  rc.bottom = ClampEdge(rc.top + rc.Width() / ratio, edge);
    else if (edge == Left)    rc.left = ClampEdge(rc.right - rc.Height() * ratio, edge);
    else if (edge == Right)   rc.right = ClampEdge(rc.left + rc.Height() * ratio, edge);
}

CRect MoveStrategy::ResizeEdge(GPointF delta, GripType edge, const CropAspectRatio& ratio, GripType fixed_side) const
{
    CRect   rc = m_begin;
    MoveSingleEdge(rc, delta, edge);
    if (!ratio.IsLocked())
        return rc;

    bool   backcalc = false;
    if ((edge == Top) || (edge == Bottom))
    {
        const double   dstwidth = rc.Height() * ratio.Value();
        if (fixed_side == Left)
        {
            double   newright = rc.left + dstwidth; // may exceed canvas bounds
            rc.right = ClampEdge(newright, Right); // final clamped value
            backcalc = (newright > m_canvas_size.cx);
        }
        else
        {
            double   newleft = rc.right - dstwidth;
            rc.left = ClampEdge(newleft, Left);
            backcalc = (newleft < 0);
        }
    }
    else if ((edge == Left) || (edge == Right))
    {
        const double   dstheight = rc.Width() / ratio.Value();
        if (fixed_side == Top)
        {
            double   newbottom = rc.top + dstheight;
            rc.bottom = ClampEdge(newbottom, Bottom);
            backcalc = (newbottom > m_canvas_size.cy);
        }
        else
        {
            double   newtop = rc.bottom - dstheight;
            rc.top = ClampEdge(newtop, Top);
            backcalc = (newtop < 0);
        }
    }

    if (backcalc)
    {
        // left-right 最终结果限制好了，肯定比目标尺寸小，现在返回去重新计算top/bottom
        BackCalculateEdge(rc, ratio.Value(), edge);
    }
    return rc;
}

CRect MoveStrategy::MoveCrop(GPointF delta) const
{
    CRect   rc = m_begin;
    rc.OffsetRect((int)delta.X, (int)delta.Y);
    FCWnd::MoveRectInside(rc, m_canvas_size);
    return rc;
}

CRect MoveStrategy::ResizeCornerLocked(GPointF delta, const CropAspectRatio& ratio) const
{
    switch (m_type)
    {
        case TopLeft:
            delta.Y = (float)ratio.MapDragToVertical(delta);
            return ResizeEdge(delta, Top, ratio, Right);

        case TopRight:
            delta.Y = (float)ratio.MapDragToVertical({ -delta.X, delta.Y });
            return ResizeEdge(delta, Top, ratio, Left);

        case BottomLeft:
            delta.Y = (float)ratio.MapDragToVertical({ -delta.X, delta.Y });
            return ResizeEdge(delta, Bottom, ratio, Right);

        case BottomRight:
            delta.Y = (float)ratio.MapDragToVertical(delta);
            return ResizeEdge(delta, Bottom, ratio, Left);
    }
    return {};
}

CRect MoveStrategy::ResizeCornerUnlocked(GPointF delta) const
{
    CRect   rc = m_begin;
    switch (m_type)
    {
        case TopLeft:
            MoveSingleEdge(rc, delta, Top);
            MoveSingleEdge(rc, delta, Left);
            return rc;

        case TopRight:
            MoveSingleEdge(rc, delta, Top);
            MoveSingleEdge(rc, delta, Right);
            return rc;

        case BottomLeft:
            MoveSingleEdge(rc, delta, Bottom);
            MoveSingleEdge(rc, delta, Left);
            return rc;

        case BottomRight:
            MoveSingleEdge(rc, delta, Bottom);
            MoveSingleEdge(rc, delta, Right);
            return rc;
    }
    return {};
}

CRect MoveStrategy::HandleMouseMove(GPointF cur, const CropAspectRatio& ratio)
{
    if (m_action == Action::Create)
        return CreateCrop(cur, ratio);

    const GPointF   delta = cur - m_anchor;

    // Move whole crop
    if (m_type == Move)
        return MoveCrop(delta);

    // Resize by 4 edges (Top / Bottom / Left / Right)
    if ((m_type == Top) || (m_type == Bottom))
        return ResizeEdge(delta, m_type, ratio, Left);

    if ((m_type == Left) || (m_type == Right))
        return ResizeEdge(delta, m_type, ratio, Top);

    // Resize by 4 corners (TopLeft / TopRight / BottomLeft / BottomRight)
    if (ratio.IsLocked())
        return ResizeCornerLocked(delta, ratio);
    else
        return ResizeCornerUnlocked(delta);
}

namespace
{
    auto DetermineCorner(GPointF cur, GPointF anchor)
    {
        if (cur.X < anchor.X && cur.Y < anchor.Y)
            return std::make_pair(TopLeft, CSize(-1, -1));

        if (cur.X >= anchor.X && cur.Y < anchor.Y)
            return std::make_pair(TopRight, CSize(1, -1));

        if (cur.X < anchor.X && cur.Y >= anchor.Y)
            return std::make_pair(BottomLeft, CSize(-1, 1));

        // cur.X >= anchor.X && cur.Y >= anchor.Y
        return std::make_pair(BottomRight, CSize(1, 1));
    }
}

CRect MoveStrategy::CreateCrop(GPointF cur, const CropAspectRatio& ratio)
{
    auto [corner, size] = DetermineCorner(cur, m_anchor);

    // 初始化 begin，使其等效于拖拽一个 1x1 的 rect
    m_type = corner;
    m_begin = { CPoint((int)m_anchor.X, (int)m_anchor.Y), size };
    m_begin.NormalizeRect();
    FCWnd::MoveRectInside(m_begin, m_canvas_size);

    m_action = Action::Modify;
    CRect   rc = HandleMouseMove(cur, ratio);
    m_action = Action::Create;
    return rc;
}

_PHOXO_NAMESPACE_END
