#include "pch.h"
#include "app_config.h"

namespace
{
    struct ConfigReg : public FCRegKey
    {
        ConfigReg()
        {
            DWORD   disposition = 0;
            Create(HKEY_CURRENT_USER, AppDefine::CONFIG_REGISTRY, REG_NONE, REG_OPTION_NON_VOLATILE, KEY_READ | KEY_WRITE, NULL, &disposition); ASSERT(m_hKey);
        }
    };

    struct PhoXoSeeConfigReg : public FCRegKey
    {
        PhoXoSeeConfigReg()
        {
            Open(HKEY_CURRENT_USER, LR"(Software\PhoXoSee\config)");
        }
    };

    void LoadLanguage()
    {
        CString   lang;
        PhoXoSeeConfigReg().LoadString(L"language_id", lang);
        if (lang.IsEmpty())
            lang = LanguageText::GetSystemLanguageID();

        LanguageText::SetCurrentLanguageID(lang);
    }
}

AppConfig::AppConfig()
{
    LoadLanguage();

    if (ConfigReg k{})
    {
        ProcessIntConfigs(k, true);
    }

    phoxo::ZoomMapper::s_max_ratio = m_max_zoom_ratio;
}

void AppConfig::Save()
{
    if (ConfigReg k{})
    {
        ProcessIntConfigs(k, false);
    }
}

CBCGPWinApp::BCGP_VISUAL_THEME AppConfig::GetVisualTheme() const
{
    switch (m_theme_index)
    {
        case 1: return CBCGPWinApp::BCGP_VISUAL_THEME_VS_2026_DARK;
        case 2: return CBCGPWinApp::BCGP_VISUAL_THEME_VS_2026_SUNNY_DAY;
        case 3: return CBCGPWinApp::BCGP_VISUAL_THEME_VS_2026_ICY_MINT;
        case 4: return CBCGPWinApp::BCGP_VISUAL_THEME_VS_2026_SILKY_PINK;
        case 5: return CBCGPWinApp::BCGP_VISUAL_THEME_VS_2026_JUICY_PLUM;
        case 6: return CBCGPWinApp::BCGP_VISUAL_THEME_VS_2026_MOONLIGHT_GLOW;
        case 7: return CBCGPWinApp::BCGP_VISUAL_THEME_VS_2026_MANGO_PARADISE;
        case 8: return CBCGPWinApp::BCGP_VISUAL_THEME_VS_2026_MYSTICAL_FOREST;
        case 9: return CBCGPWinApp::BCGP_VISUAL_THEME_VS_2026_COOL_SLATE;
    }
    return CBCGPWinApp::BCGP_VISUAL_THEME_OFFICE_2022_WHITE; // default
}

bool AppConfig::LoadSeeIgnoreICC()
{
    int   v = false;
    if (PhoXoSeeConfigReg k{})
    {
        k.LoadInt(L"ignore_embedded_icc", v);
    }
    return v;
}

int AppConfig::LoadSeeJpegQuality()
{
    int   v = 80;
    if (PhoXoSeeConfigReg k{})
    {
        k.LoadInt(L"save_jpeg_quality", v);
    }
    return v;
}

void AppConfig::ProcessIntConfigs(FCRegKey& reg, bool is_load)
{
    struct Item
    {
        PCWSTR key;
        int*   value;
    };

    Item   items[] =
    {
        { L"max_zoom_ratio",       &m_max_zoom_ratio },
        { L"theme_index",          &m_theme_index },

        // enum / int casts
        { L"panel_dock",           (int*)&m_panel_dock },
        { L"render_preference",    (int*)&m_render_preference },
    };

    for (auto& it : items)
    {
        if (is_load)
            reg.LoadInt(it.key, *it.value);
        else
            reg.SetDWORDValue(it.key, *it.value);
    }
}
