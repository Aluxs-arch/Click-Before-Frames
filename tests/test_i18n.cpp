#include "../src/i18n.hpp"

#include <cstdio>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <string>

static int failures = 0;

static void check(bool condition, std::string const& name) {
    if (!condition) {
        std::fprintf(stderr, "FAIL: %s\n", name.c_str());
        ++failures;
    }
    else {
        std::fprintf(stdout, "ok  : %s\n", name.c_str());
    }
}

int main(int argc, char** argv) {
    auto& loc = bf::Localizer::get();

    check(bf::languageFromSetting("English") == bf::Language::English, "English setting -> English");
    check(bf::languageFromSetting("Bahasa Indonesia") == bf::Language::Indonesian, "ID setting -> Indonesian");
    check(bf::languageFromSetting("id") == bf::Language::Indonesian, "id shorthand -> Indonesian");
    check(std::string(bf::languageFile(bf::Language::Indonesian)) == "lang/id.lang", "Indonesian file path");

    // Built-in strings work with no resources directory at all.
    loc.setLanguage(bf::Language::English, "");
    check(loc.language() == bf::Language::English, "English selected");
    check(loc.get("indicator.before-frame") == "BEFORE FRAME", "built-in English label");

    loc.setLanguage(bf::Language::Indonesian, "");
    check(loc.language() == bf::Language::Indonesian, "Indonesian selected");
    check(loc.get("indicator.before-frame") == "SEBELUM FRAME", "built-in Indonesian label");
    check(loc.get("endscreen.caption") == "Sebelum Frame", "built-in Indonesian endscreen caption");
    check(loc.get("endscreen.credit").find("Aluxs") != std::string::npos, "developer credit is present");
    check(loc.get("indicator.vanilla") == "VANILLA 240 TPS", "vanilla/CoS label is available");
    check(loc.get("indicator.auto") == "OTOMATIS", "v2 AUTO label is localized");
    check(loc.get("debug.saved") == "hemat", "v2 debug key is localized");
    check(loc.get("endscreen.stats-prefix") == "Klik", "v2 endscreen stats key is localized");
    check(loc.get("does.not.exist") == "does.not.exist", "unknown key falls back to the key");

    // Overlay files: write a fake resources/lang tree in a temp directory.
    auto temp = std::filesystem::temp_directory_path() / "click-before-frames-i18n-test";
    std::filesystem::create_directories(temp / "lang");
    {
        std::ofstream(temp / "lang" / "en.lang") <<
            "# comment\n"
            "indicator.before-frame=OVERRIDDEN EN\n"
            "endscreen.caption=Line one\\nLine two\n";
        std::ofstream(temp / "lang" / "id.lang") <<
            "indicator.before-frame=SEBELUM FRAME (ID file)\n";
    }

    loc.setLanguage(bf::Language::English, temp.string());
    check(loc.get("indicator.before-frame") == "OVERRIDDEN EN", "en.lang overrides built-in strings");
    check(loc.get("endscreen.caption") == "Line one\nLine two", "\\n is unescaped in overlay");
    check(loc.get("indicator.ms") == "ms", "keys missing from overlay use built-ins");

    loc.setLanguage(bf::Language::Indonesian, temp.string());
    check(loc.get("indicator.before-frame") == "SEBELUM FRAME (ID file)", "id.lang overrides built-in strings");
    check(loc.get("indicator.between-frames") == "DI ANTARA FRAME", "Indonesian built-ins remain available");

    std::filesystem::remove_all(temp);

    // Verify the two language files that actually ship with the mod.
    // tests/run.sh passes the repository root, which exercises the
    // "<root>/resources/lang/<lang>.lang" dev layout.
    if (argc > 1) {
        std::string root = argv[1];
        loc.setLanguage(bf::Language::English, root);
        check(loc.get("meta.locale") == "en", "shipped en.lang is loaded");
        check(loc.get("debug.moved") == "1st", "shipped en.lang has v2 debug keys");
        check(loc.get("indicator.auto") == "AUTO", "shipped en.lang has v2 AUTO key");
        loc.setLanguage(bf::Language::Indonesian, root);
        check(loc.get("meta.locale") == "id", "shipped id.lang is loaded");
        check(loc.get("meta.language") == "Bahasa Indonesia", "shipped Indonesian metadata");
        check(loc.get("linux.body").find('\n') != std::string::npos, "shipped Indonesian popup has unescaped newlines");
        check(loc.get("endscreen.credit").find("Aluxs") != std::string::npos, "shipped credit names Aluxs");
        check(loc.get("debug.saved") == "hemat", "shipped id.lang has v2 debug keys");
    }
    else {
        std::fprintf(stdout, "note: pass the repo root to also test the shipped .lang files\n");
    }

    if (failures) {
        std::fprintf(stderr, "%d test(s) failed\n", failures);
        return EXIT_FAILURE;
    }
    std::fprintf(stdout, "All i18n tests passed\n");
    return EXIT_SUCCESS;
}
