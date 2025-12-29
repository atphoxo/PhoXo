#include "pch.h"
#include "svg.h"

namespace
{
    float GetLuminance(const FCColor& cr)
    {
        return 0.2126f * cr.r + 0.7152f * cr.g + 0.0722f * cr.b;
    }

    // 忽略弱透明的像素（a ≤ 64，即 25% 透明度以下），避免它们对平均亮度产生干扰
    double ComputeAverageLuminance(const FCImage& img, bool light_mode)
    {
        FCColor   back{ light_mode ? (int)0xFFFFFFFF : 0 };
        double   sum = 0;
        int   count = 0;
        std::for_each_n((const FCColor*)img.GetMemStart(), img.Width() * img.Height(), [&](auto& px) {
            if (px.a > 64)
            {
                FCColor   cr = back;
                phoxo::PixelFunc::BlendStraightAlpha(cr, px);
                sum += GetLuminance(cr);
                count++;
            }
        });
        return count ? (sum / count) : 0;
    }
}

/// Fill background (32 bit).
class FillBackground : public eff::PixelIterator<FillBackground>
{
private:
    FCColor   m_back;

    void Process(FCColor& px) const
    {
        if (px.a == 0xFF)
            return;
        if (px.a == 0)
        {
            px = m_back;
            return;
        }

        FCColor   fore = px;
        px = m_back;
        phoxo::PixelFunc::BlendStraightAlpha(px, fore);
    }

public:
    FillBackground(FCColor fill_back_color) : m_back(fill_back_color) {}

    static void HandlePixel(FCImage&, int, int, FCColor* px, FillBackground& eff)
    {
        eff.Process(*px);
    }
};

void SvgImageLoader::ImproveVisualContrast(FCImage& img) const
{
    static bool   light_mode = FCHelper::IsWindowsInLightMode();
    double   lum = ComputeAverageLuminance(img, light_mode);
    if (lum > 245 && light_mode)
    {
        FillBackground   cmd(FCColor(0xFF2C2C2C)); // 如果觉得太黑可以换 0xFF444444
        img.ApplyEffect(cmd);
    }
    else if (lum < 15 && !light_mode)
    {
        FillBackground   cmd(FCColor(0xFFC8C8C8));
        img.ApplyEffect(cmd);
    }
}
