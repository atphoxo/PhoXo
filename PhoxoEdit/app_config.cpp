#include "pch.h"
#include "PhoxoEdit.h"
#include "app_config.h"

namespace
{
    void LoadLanguage()
    {
        CString   lang = FCRegKey::GetString(HKEY_CURRENT_USER, LR"(Software\PhoXo\config)", L"language_id");
        if (lang.IsEmpty())
            lang = FCRegKey::GetString(HKEY_CURRENT_USER, LR"(Software\PhoXoSee\config)", L"language_id");

        if (lang.IsEmpty())
            lang = LanguageText::GetSystemLanguageID();

        LanguageText::SetCurrentLanguageID(lang);
    }

    void CreateCanvasGridBack(CBrush& br)
    {
        using namespace Gdiplus;
        phoxo::effect::CheckerFill   eff(FCColor(Color::Silver), FCColor(Color::White), 16);
        FCImage   img;
        img.Create(32, 32);
        img.ApplyEffect(eff);
        br.Attach(::CreatePatternBrush(img));
    }
}

CAppConfig::CAppConfig()
{
    LoadLanguage();
    CreateCanvasGridBack(m_runtime_canvas_back);
    phoxo::ZoomMapper::s_max_ratio = m_max_zoom_ratio;
}
