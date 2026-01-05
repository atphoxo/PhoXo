#pragma once

class AppConfig
{
public:
    // int   m_startup_page; // 启动页
    PanelDock   m_panel_dock = PanelDock::Right;
    int       m_follow_see_theme = true;
    int       m_max_zoom_ratio = 8;
    RenderPreference   m_render_preference = RenderPreference::Speed;
    //int  svg询问打开尺寸
    //int  undolevel,gimp是undo内存

    // temporary variables for runtime
    CBrush   m_runtime_canvas_back;

    AppConfig();

    static AppConfig& GetInstance()
    {
        static AppConfig   v;
        return v;
    }

    void Save();
    CString AppText(PCWSTR key) { return LanguageText::Get(L"app", key); }

private:
    void ProcessIntConfigs(FCRegKey& reg, bool is_load);
};

#define  theConfig  AppConfig::GetInstance()
