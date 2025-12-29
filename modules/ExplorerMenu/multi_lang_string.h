#pragma once

class PhoXoMenuTitle
{
public:
    static PCWSTR Get()
    {
        LANGID   lang = ::GetUserDefaultUILanguage();

        if (IsTraditionalChineseSubLang(lang))
            lang = LANG_CHINESE_TRADITIONAL;

        switch (PRIMARYLANGID(lang))
        {
            case LANG_BELARUSIAN:       return L"Выкарыстаць PhoXo";        // Belarusian
            case LANG_CHINESE:          return L"使用 PhoXo 查看";           // Simplified Chinese
            case LANG_CHINESE_TRADITIONAL: return L"使用 PhoXo 檢視";        // Traditional Chinese
            case LANG_CZECH:            return L"Otevřít s PhoXo";          // Czech
            case LANG_DUTCH:            return L"Open met PhoXo";           // Dutch
            case LANG_ENGLISH:          return L"Open with PhoXo";          // English
            case LANG_FRENCH:           return L"Ouvrir avec PhoXo";        // French
            case LANG_GERMAN:           return L"Öffnen mit PhoXo";         // German
            case LANG_GREEK:            return L"Άνοιγμα με PhoXo";         // Greek
            case LANG_HUNGARIAN:        return L"Megnyitás PhoXo-val";      // Hungarian
            case LANG_INDONESIAN:       return L"Buka dengan PhoXo";        // Indonesian
            case LANG_ITALIAN:          return L"Apri con PhoXo";           // Italian
            case LANG_JAPANESE:         return L"PhoXoで表示";               // Japanese
            case LANG_KOREAN:           return L"PhoXo로 보기";              // Korean
            case LANG_LATVIAN:          return L"Atvērt ar PhoXo";          // Latvian
            case LANG_POLISH:           return L"Otwórz za pomocą PhoXo";   // Polish
            case LANG_PORTUGUESE:       return L"Abrir com PhoXo";          // Portuguese (Brazil)
            case LANG_ROMANIAN:         return L"Deschide cu PhoXo";        // Romanian
            case LANG_RUSSIAN:          return L"Открыть с помощью PhoXo";  // Russian
            case LANG_SLOVAK:           return L"Otvoriť pomocou PhoXo";    // Slovak
            case LANG_SLOVENIAN:        return L"Odpri s PhoXo";            // Slovenian
            case LANG_SPANISH:          return L"Abrir con PhoXo";          // Spanish
            case LANG_SWEDISH:          return L"Öppna med PhoXo";          // Swedish
            case LANG_TURKISH:          return L"PhoXo ile aç";             // Turkish
            case LANG_UKRAINIAN:        return L"Відкрити за допомогою PhoXo"; // Ukrainian
            case LANG_VIETNAMESE:       return L"Mở bằng PhoXo";            // Vietnamese
            case LANG_ARABIC:           return L"فتح باستخدام PhoXo";        // Arabic
            case LANG_HEBREW:           return L"פתח עם PhoXo";             // Hebrew
            case LANG_THAI:             return L"เปิดด้วย PhoXo";              // Thai
            case LANG_FINNISH:          return L"Avaa PhoXo-ohjelmalla";    // Finnish
            case LANG_NORWEGIAN:        return L"Åpne med PhoXo";           // Norwegian
            case LANG_DANISH:           return L"Åbn med PhoXo";            // Danish
            case LANG_MALAY:            return L"Buka dengan PhoXo";        // Malay

            default:                    return L"Open with PhoXo";          // fallback
        }
    }

private:
    static bool IsTraditionalChineseSubLang(LANGID lang)
    {
        return (PRIMARYLANGID(lang) == LANG_CHINESE) &&
            (SUBLANGID(lang) == SUBLANG_CHINESE_TRADITIONAL ||
            SUBLANGID(lang) == SUBLANG_CHINESE_HONGKONG ||
            SUBLANGID(lang) == SUBLANG_CHINESE_MACAU);
    }
};
