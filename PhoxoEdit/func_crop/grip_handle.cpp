#include "pch.h"
#include "grip_handle.h"

_PHOXO_NAMESPACE(crop)

using enum GripType;

namespace
{
    int GripLength() { return DPICalculator::Cast(30); }
    int GripThickness() { return DPICalculator::Cast(3); }
    int BorderGap() { return DPICalculator::Cast(2); }

    void CenterSpan(LONG& begin, LONG& end, LONG center)
    {
        begin = center - GripLength() / 2;
        end = begin + GripLength();
    }

    void CutH(CRect& rc, bool from_left)
    {
        from_left ? rc.right = rc.left + GripLength() : rc.left = rc.right - GripLength();
    }

    void CutV(CRect& rc, bool from_top)
    {
        from_top ? rc.bottom = rc.top + GripLength() : rc.top = rc.bottom - GripLength();
    }

    class GripBorder
    {
    private:
        CPoint   center;
        CRect   top, bottom, left, right, outer;

    public:
        GripBorder(const CRect& crop_on_view) : center{ crop_on_view.CenterPoint() }
        {
            CRect   inner = crop_on_view;
            inner.InflateRect(BorderGap(), BorderGap());

            outer = inner;
            outer.InflateRect(GripThickness(), GripThickness());

            top = bottom = left = right = outer;
            top.bottom = inner.top;
            bottom.top = inner.bottom;
            left.right = inner.left;
            right.left = inner.right;
        }

        CRect CalcSideRect(GripType type) const
        {
            CRect   rc;
            switch (type)
            {
                case Top:
                case Bottom:
                    rc = (type == Top) ? top : bottom;
                    CenterSpan(rc.left, rc.right, center.x);
                    break;

                case Left:
                case Right:
                    rc = (type == Left) ? left : right;
                    CenterSpan(rc.top, rc.bottom, center.y);
                    break;
            }
            rc.IntersectRect(rc, outer);
            return rc;
        }

        std::pair<CRect, CRect> CalcCornerRect(GripType type) const
        {
            CRect   r1, r2;
            if (type == TopLeft)
            {
                CutH(r1 = top, true);
                CutV(r2 = left, true);
            }
            else if (type == TopRight)
            {
                CutH(r1 = top, false);
                CutV(r2 = right, true);
            }
            else if (type == BottomLeft)
            {
                CutH(r1 = bottom, true);
                CutV(r2 = left, false);
            }
            else if (type == BottomRight)
            {
                CutH(r1 = bottom, false);
                CutV(r2 = right, false);
            }
            r1.IntersectRect(r1, outer);
            r2.IntersectRect(r2, outer);
            return { r1,r2 };
        }
    };
}

void GripHandle::Draw(HDC hdc, const CRect& crop_on_view) const
{
    HBRUSH   paint = m_hovered ? theRuntime.m_accent_brush : (HBRUSH)GetStockObject(WHITE_BRUSH);
    GripBorder   calc(crop_on_view);
    switch (m_type)
    {
        case Top:
        case Bottom:
        case Left:
        case Right:
        {
            CRect   rc = calc.CalcSideRect(m_type);
            ::FillRect(hdc, rc, paint);
        }
        break;

        case TopLeft:
        case TopRight:
        case BottomLeft:
        case BottomRight:
        {
            auto [r1, r2] = calc.CalcCornerRect(m_type);
            ::FillRect(hdc, r1, paint);
            ::FillRect(hdc, r2, paint);
        }
        break;
    }
}

namespace
{
    CRect CornerHitRect(CPoint pt)
    {
        CSize   sz(GripLength(), GripLength());
        return { pt - sz, pt + sz };
    }

    CRect SideHitRect(CPoint pt1, CPoint pt2, bool vertical)
    {
        if (vertical)
        {
            CRect   r1 = CornerHitRect(pt1), r2 = CornerHitRect(pt2);
            return { r1.left, r1.bottom, r1.right, r2.top };
        }
        else
        {
            CRect   r1 = CornerHitRect(pt1), r2 = CornerHitRect(pt2);
            return { r1.right, r1.top, r2.left, r1.bottom };
        }
    }
}

bool GripHandle::IsInnerMoveOnly(const CRect& crop_on_view)
{
    int   limit = 4 * GripLength();
    return (crop_on_view.Width() < limit) || (crop_on_view.Height() < limit);
}

// rc is cropped rect on view
CRect GripHandle::GetHitZone(const CRect& rc) const
{
    CPoint   tl{ rc.TopLeft() }, tr{ rc.right, rc.top };
    CPoint   bl{ rc.left, rc.bottom }, br{ rc.BottomRight() };
    switch (m_type)
    {
        case Top: return SideHitRect(tl, tr, false);
        case Bottom: return SideHitRect(bl, br, false);
        case Left: return SideHitRect(tl, bl, true);
        case Right: return SideHitRect(tr, br, true);
        case TopLeft: return CornerHitRect(tl);
        case TopRight: return CornerHitRect(tr);
        case BottomLeft: return CornerHitRect(bl);
        case BottomRight: return CornerHitRect(br);
    }
    return {};
}

bool GripHandle::UpdateHoverState(CPoint cursor_on_view, const CRect& crop_on_view, GripHandle* first_hover)
{
    bool   hovered = GetHitZone(crop_on_view).PtInRect(cursor_on_view);

    // 特化规则 1：crop 太小，inner 区域不算 grip
    if (IsInnerMoveOnly(crop_on_view) && crop_on_view.PtInRect(cursor_on_view))
        hovered = false;

    // 特化规则 2：只允许第一个 hittest hovering，防止很小的 crop 都命中
    if (first_hover)
        hovered = false;

    if (hovered != m_hovered)
    {
        m_hovered = hovered;
        return true;
    }
    return false;
}

_PHOXO_NAMESPACE_END
