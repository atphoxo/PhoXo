#pragma once

class AppConfig : public FCSingleton<AppConfig>
{
public:
    PanelDock   m_panel_dock = PanelDock::Right;
    int       m_follow_phoxosee_theme = true;
    int       m_max_zoom_ratio = 8;
    RenderPreference   m_render_preference = RenderPreference::Speed;
    //int  svgѯ�ʴ򿪳ߴ�

    AppConfig();

    void Save();
    CString AppText(PCWSTR key) { return LanguageText::Get(L"app", key); }

private:
    void ProcessIntConfigs(FCRegKey& reg, bool is_load);
};

#define theConfig AppConfig::GetInstance()
