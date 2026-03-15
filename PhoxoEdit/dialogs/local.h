#pragma once

namespace settings
{
    inline CString LoadText(int key)
    {
        return LanguageText::Get(L"SETTINGS", key);
    }
}
