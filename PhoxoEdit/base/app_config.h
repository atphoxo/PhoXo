#pragma once

class AppConfig : public FCSingleton<AppConfig>
{
public:
    PanelDock   m_panel_dock = PanelDock::Right;
    int       m_theme_index = 0;
    int       m_max_zoom_ratio = 8;
    RenderPreference   m_render_preference = RenderPreference::Quality;
    //int  svgѯ�ʴ򿪳ߴ�

    AppConfig();

    void Save();
    CString AppText(PCWSTR key) { return LanguageText::Get(L"app", key); }

    CBCGPWinApp::BCGP_VISUAL_THEME GetVisualTheme() const;

    static bool LoadSeeIgnoreICC();
    static int LoadSeeJpegQuality();

private:
    void ProcessIntConfigs(FCRegKey& reg, bool is_load);
};

#define theConfig AppConfig::GetInstance()
