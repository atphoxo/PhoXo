#pragma once

class CAppConfig
{
public:
    int       m_follow_see_theme = true;
    int       m_max_zoom_ratio = 8;
    RenderPreference   m_render_preference = RenderPreference::Speed;
    //int  svg询问打开尺寸
    //int  undolevel,gimp是undo内存

    // temporary variables for runtime
    CBrush   m_runtime_canvas_back;

    CAppConfig();

    static CAppConfig& GetInstance()
    {
        static CAppConfig   v;
        return v;
    }

    CString AppText(PCWSTR key) { return LanguageText::Get(L"app", key); }
};

#define  theConfig  CAppConfig::GetInstance()
