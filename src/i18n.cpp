#include "i18n.hpp"

#include <filesystem>
#include <fstream>
#include <utility>
#include <vector>

namespace bf {

// ---------------------------------------------------------------------------
// Built-in string tables. These are always available, even when the .lang
// resource files are missing (for example in a development build).
// ---------------------------------------------------------------------------
static const std::unordered_map<std::string, std::string> STRINGS_EN = {
    { "indicator.before-frame",   "BEFORE FRAME" },
    { "indicator.between-frames", "BETWEEN FRAMES" },
    { "indicator.ms",             "ms" },
    { "indicator.vanilla",        "VANILLA 240 TPS" },
    { "endscreen.caption",        "Before Frame" },
    { "endscreen.credit",         "by Aluxs - github.com/Aluxs-arch" },
    { "linux.title",              "Click Before Frames - Linux" },
    { "linux.body",
        "Failed to read input devices.\n"
        "On most distributions, this can be resolved with the following command: "
        "<cr>sudo usermod -aG input $USER</c> (reboot afterward; this will make your system slightly less secure).\n"
        "If the issue persists, please open an issue on github.com/Aluxs-arch/Click-Before-Frames." },
};

static const std::unordered_map<std::string, std::string> STRINGS_ID = {
    { "indicator.before-frame",   "SEBELUM FRAME" },
    { "indicator.between-frames", "DI ANTARA FRAME" },
    { "indicator.ms",             "ms" },
    { "indicator.vanilla",        "VANILLA 240 TPS" },
    { "endscreen.caption",        "Sebelum Frame" },
    { "endscreen.credit",         "oleh Aluxs - github.com/Aluxs-arch" },
    { "linux.title",              "Click Before Frames - Linux" },
    { "linux.body",
        "Gagal membaca perangkat input.\n"
        "Pada sebagian besar distro, ini bisa diatasi dengan perintah: "
        "<cr>sudo usermod -aG input $USER</c> (restart sesudahnya; ini sedikit mengurangi keamanan sistem).\n"
        "Jika masalah berlanjut, silakan buka issue di github.com/Aluxs-arch/Click-Before-Frames." },
};

static const std::unordered_map<std::string, std::string>& builtinStrings(Language lang) {
    return lang == Language::Indonesian ? STRINGS_ID : STRINGS_EN;
}

Language languageFromSetting(std::string_view settingValue) {
    // Values come from the "language" string setting in mod.json.
    return settingValue == "Bahasa Indonesia" || settingValue == "id" || settingValue == "id-ID"
        ? Language::Indonesian
        : Language::English;
}

std::string_view languageFile(Language lang) {
    return lang == Language::Indonesian ? "lang/id.lang" : "lang/en.lang";
}

// Replace the two-character sequence "\n" with a real newline.
static std::string unescape(std::string_view value) {
    std::string out;
    out.reserve(value.size());
    for (size_t i = 0; i < value.size(); ++i) {
        if (value[i] == '\\' && i + 1 < value.size() && value[i + 1] == 'n') {
            out.push_back('\n');
            ++i;
        }
        else {
            out.push_back(value[i]);
        }
    }
    return out;
}

// Parse a simple key=value language file. Blank lines and lines starting with
// '#' are ignored; everything after the first '=' belongs to the value.
static void overlayFile(std::unordered_map<std::string, std::string>& strings, std::string const& path) {
    std::ifstream file(path);
    if (!file.is_open()) return;

    std::string line;
    while (std::getline(file, line)) {
        if (!line.empty() && line.back() == '\r') line.pop_back();
        if (line.empty() || line[0] == '#') continue;

        size_t equals = line.find('=');
        if (equals == std::string::npos) continue;

        std::string key = line.substr(0, equals);
        std::string value = unescape(line.substr(equals + 1));
        if (key.empty()) continue;

        // Trim leading/trailing spaces around the key only.
        size_t first = key.find_first_not_of(" \t");
        size_t last = key.find_last_not_of(" \t");
        if (first == std::string::npos) continue;
        key = key.substr(first, last - first + 1);

        strings[key] = std::move(value);
    }
}

Localizer::Localizer() : m_strings(STRINGS_EN) {}

Localizer& Localizer::get() {
    static Localizer instance;
    return instance;
}

void Localizer::setLanguage(Language lang, std::string const& resourcesDir) {
    m_language = lang;
    m_strings = builtinStrings(lang);

    if (resourcesDir.empty()) return;

    // Geode copies resources/ folders into the mod's runtime resources dir,
    // keeping the "lang/" folder (mod.json -> resources.folders). Also accept
    // a flat file (resources.files) and the repository layout, so the same
    // code works in packaged mods, dev builds and unit tests.
    auto relative = std::filesystem::path(std::string(languageFile(lang)));
    auto root = std::filesystem::path(resourcesDir);
    const std::vector<std::filesystem::path> candidates = {
        root / relative,
        root / relative.filename(),
        root / "resources" / relative,
    };

    for (auto const& candidate : candidates) {
        std::ifstream probe(candidate);
        if (!probe.is_open()) continue;
        overlayFile(m_strings, candidate.string());
        break;
    }
}

std::string Localizer::get(std::string_view key) const {
    auto it = m_strings.find(std::string(key));
    if (it != m_strings.end()) return it->second;

    // Fallback to English if a translation (or an overlay) is missing a key.
    auto fallback = STRINGS_EN.find(std::string(key));
    if (fallback != STRINGS_EN.end()) return fallback->second;

    return std::string(key);
}

} // namespace bf
