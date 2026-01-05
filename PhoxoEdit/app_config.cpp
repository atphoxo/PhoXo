#include "pch.h"
#include "PhoxoEdit.h"
#include "app_config.h"

namespace
{
    class ConfigReg : public FCRegKey
    {
    public:
        ConfigReg()
        {
            DWORD   disposition = 0;
            Create(HKEY_CURRENT_USER, AppDefine::CONFIG_REGISTRY, REG_NONE, REG_OPTION_NON_VOLATILE, KEY_READ | KEY_WRITE, NULL, &disposition); ASSERT(m_hKey);
        }
    };

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

AppConfig::AppConfig()
{
    LoadLanguage();
    CreateCanvasGridBack(m_runtime_canvas_back);

    ConfigReg   k;
    if (k)
    {
        ProcessIntConfigs(k, true);
    }

    phoxo::ZoomMapper::s_max_ratio = m_max_zoom_ratio;
}

void AppConfig::Save()
{
    ConfigReg   k;
    if (!k)
        return;

    ProcessIntConfigs(k, false);
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

        // enum / int casts
        { L"panel_dock",           (int*)&m_panel_dock },
        { L"render_preference",           (int*)&m_render_preference },
    };

    for (auto& it : items)
    {
        if (is_load)
            reg.LoadInt(it.key, *it.value);
        else
            reg.SetDWORDValue(it.key, *it.value);
    }
}
