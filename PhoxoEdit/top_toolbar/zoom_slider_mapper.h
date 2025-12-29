#pragma once

class ZoomSliderMapper
{
private:
    // slider 固定缩放比例
    static inline const std::vector<double>   m_ticks = {
        0.05,   // 1/20
        0.10,   // 1/10
        0.15,   // 15/100
        0.20,   // 1/5
        0.25,   // 1/4
        0.33,   // 1/3 ≈ 0.33
        0.50,   // 1/2
        0.67,   // 2/3 ≈ 0.67
        1.0,    // 1x
        1.5,    // 3/2
        2.0,    // 2x
        3.0,    // 3x
        4.0,    // 4x
        5.0,    // 5x
        6.0,    // 6x
        7.0,    // 7x
        8.0     // 8x
    };

public:
    // 0 <= pos <= max
    static int slider_range_max()
    {
        return (int)m_ticks.size() - 1;
    }

    // 给定 slider pos，返回对应缩放比例
    static double RatioAtSlider(int pos)
    {
        pos = std::clamp(pos, 0, slider_range_max());
        return m_ticks[pos];
    }

    // 返回最接近slider pos
    static int NearestSliderPos(double ratio)
    {
        auto it = std::ranges::min_element(m_ticks, [ratio](auto& a, auto& b) {
            return std::abs(a - ratio) < std::abs(b - ratio); // 计算距离最近的
        });
        return (int)(it - m_ticks.begin());
    }

    // 计算用户点击<放大> <缩小>按钮后给的比例
    static double NextRatio(double ratio, ZoomDirection dir)
    {
        int   idx = NearestSliderPos(ratio);
        double   tick = m_ticks[idx];

        // 在两个刻度之间，要特殊处理，跳到最近的刻度
        if (IsDisplayedZoomDifferent(tick, ratio)) // 不能让显示的值不变
        {
            if ((ratio > tick) && (dir == ZoomDirection::ZoomOut)) // 防止 110% 直接变成 67%，先回落 100%
                return tick;

            if ((ratio < tick) && (dir == ZoomDirection::ZoomIn))
                return tick;
        }

        // 否则进入下一个 tick
        return RatioAtSlider(idx + (dir == ZoomDirection::ZoomIn ? 1 : -1));
    }

    static bool IsDisplayedZoomDifferent(double r1, double r2)
    {
        return lround(r1 * 100) != lround(r2 * 100); // 四舍五入到整数百分比后比较
    }
};
