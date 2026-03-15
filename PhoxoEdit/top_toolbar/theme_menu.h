#pragma once

namespace topbar
{
    class ThemeMenu : public FCMenu
    {
    private:
        LanguageTextGroup   m_items;

    public:
        ThemeMenu(const CString& txt) : m_items{ txt }
        {
        }

    private:
        bool OnTranslateMenuItem(CMenu* sub_menu, UINT item_id, CString& txt) override
        {
            if (sub_menu)
                txt = m_items[0];
            else
                txt = m_items[item_id - ID_THEME_01 + 1];
            return true;
        }
    };
}
