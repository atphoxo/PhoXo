#pragma once

namespace main_view
{
    class WelcomeRender
    {
    private:
        HWND   m_wnd;
        FCImage   m_icon;
        CFont   m_font;
        const CString   m_text{ theConfig.AppText(L"welcome") };

    public:
        WelcomeRender(HWND wnd) : m_wnd{ wnd }
        {
            FCIcon   ico{ IDR_MAINFRAME, DPICalculator::Cast(100) };
            m_icon = phoxo::ImageHandler::Make(ico.CreateBitmap(), WICPremultiplied32bpp);
            m_font.CreatePointFont(280, L"Georgia");
        }

        COLORREF TitleColor() const
        {
            BOOL   is_dark = CBCGPVisualManager::GetInstance()->IsDarkTheme();
            FCColor   c{ is_dark ? Gdiplus::Color::White : 0x202020 };
            return c.ToCOLORREF();
        }

        void DrawIcon(CDC& dc) const
        {
            CPoint   center = FCWnd::GetClientRect(m_wnd).CenterPoint();
            int   x = center.x - m_icon.Width() / 2;
            int   y = center.y - m_icon.Height() - DPICalculator::Cast(10);
            phoxo::ImageDrawer::Draw(dc, { x,y }, m_icon);
        }

        void DrawText(CDC& dc) const
        {
            CRect   rc = FCWnd::GetClientRect(m_wnd);
            rc.top = rc.CenterPoint().y + DPICalculator::Cast(10);

            auto   oldft = SelectObject(dc, m_font);
            dc.SetTextColor(TitleColor());
            dc.SetBkMode(TRANSPARENT);
            dc.DrawText(m_text, rc, DT_TOP | DT_CENTER | DT_SINGLELINE);
            SelectObject(dc, oldft);
        }

        void Draw(CDC& dc) const
        {
            DrawIcon(dc);
            DrawText(dc);
        }
    };
}
