#pragma once

#include <string>
#include <string_view>
#include <unordered_map>

// Tiny built-in localisation system (English + Bahasa Indonesia).
// It has no Geode dependencies so it can be compiled and unit tested on its
// own. Strings are built into the binary as a fallback, and can be overridden
// by resources/lang/en.lang and resources/lang/id.lang at runtime.
namespace bf {

enum class Language {
    English,
    Indonesian,
};

Language languageFromSetting(std::string_view settingValue);
std::string_view languageFile(Language lang);

class Localizer {
public:
    static Localizer& get();

    // Reloads the built-in strings for `lang` and overlays the matching
    // resources/lang/<lang>.lang file if it can be read from `resourcesDir`.
    void setLanguage(Language lang, std::string const& resourcesDir);

    Language language() const { return m_language; }

    // Missing keys fall back to English, then to the key itself.
    std::string get(std::string_view key) const;

private:
    Localizer();

    Language m_language = Language::English;
    std::unordered_map<std::string, std::string> m_strings;
};

inline std::string tr(std::string_view key) {
    return Localizer::get().get(key);
}

} // namespace bf
