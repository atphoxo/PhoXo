#pragma once

_PHOXO_BEGIN

class ZoomMapper
{
private:
    double   m_ratio = 1;

    void set(double r)
    {
        m_ratio = std::clamp(r, s_min_ratio, s_max_ratio);
    }

public:
    static inline double   s_min_ratio = 0.01;
    static inline double   s_max_ratio = 8; // 0.01 <= ratio <= s_max_ratio

    GPointF ToOriginal(CPoint zoomed) const
    {
        return { (float)(zoomed.x / m_ratio), (float)(zoomed.y / m_ratio) };
    }

    GPointF ToZoomed(CPoint original) const
    {
        return { (float)(original.x * m_ratio), (float)(original.y * m_ratio) };
    }

    GPointF ToZoomed(GPointF original) const
    {
        return { (float)(original.X * m_ratio), (float)(original.Y * m_ratio) };
    }

    double CalcRatioByMouseWheel(int wheel_delta, double step) const
    {
        double   coef = 1;
        if (wheel_delta >= 1)
            coef = std::pow(step, wheel_delta);
        else if (wheel_delta <= -1)
            coef = std::pow(1 / step, -wheel_delta);

        return std::clamp(m_ratio * coef, 0.05, s_max_ratio);
    }

    friend class Canvas;
};

_PHOXO_NAMESPACE_END
