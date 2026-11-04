#include "../src/before_frame.hpp"

#include <cmath>
#include <cstdio>
#include <cstdlib>
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

static bool near(double a, double b, double eps = 1e-9) {
    return std::fabs(a - b) < eps;
}

int main() {
    // A 60 FPS visual frame: the physics interval [0.0, 1/60) is simulated
    // when the frame at 1/60 is drawn.
    constexpr double intervalStart = 0.0;
    constexpr double frameStart = 1.0 / 60.0;
    constexpr double lead = bf::msToSeconds(4.17); // one 240 TPS step

    check(near(bf::msToSeconds(4.17), 0.00417), "ms -> seconds conversion");

    // Mode off (or zero lead) must be a byte-for-byte no-op: classic CBF.
    check(near(bf::beforeFrameTimestamp(0.015, frameStart, intervalStart, lead, false), 0.015),
        "disabled mode keeps the exact timestamp");
    check(near(bf::beforeFrameTimestamp(0.015, frameStart, intervalStart, 0.0, true), 0.015),
        "zero lead keeps the exact timestamp");

    // Enabled: the click is pulled `lead` seconds earlier.
    check(near(bf::beforeFrameTimestamp(0.015, frameStart, intervalStart, lead, true), 0.015 - lead),
        "enabled mode shifts the click earlier by the lead");

    // Clicks within one lead of the interval start are applied on the first step.
    check(near(bf::beforeFrameTimestamp(intervalStart + 0.001, frameStart, intervalStart, lead, true), intervalStart),
        "early click is clamped to the frame start");

    // Never simulate physics that already happened.
    check(near(bf::beforeFrameTimestamp(0.0005, frameStart, intervalStart, lead, true), 0.0),
        "result never precedes the physics interval");

    // A lead of one whole frame snaps every click to the first physics step.
    check(near(bf::beforeFrameTimestamp(frameStart - 0.001, frameStart, intervalStart, frameStart, true), intervalStart),
        "full-frame lead applies a late click on the first step");

    // Never react to an input that has not happened yet: it stays queued for
    // the next frame, exactly like vanilla between-frames handling.
    check(near(bf::beforeFrameTimestamp(frameStart, frameStart, intervalStart, lead, true), frameStart),
        "input exactly at frame start is not pulled back");
    check(near(bf::beforeFrameTimestamp(frameStart + 0.002, frameStart, intervalStart, lead, true), frameStart + 0.002),
        "future input is not pulled back");

    // At 60 FPS there are 4 physics steps per frame. A click just before the
    // frame boundary normally belongs to the last (4th) step; a full-frame
    // Before Frame lead moves it to the first one.
    constexpr double stepDelta = frameStart / 4.0;
    check(near(bf::inputStepProgress(frameStart - 0.001, intervalStart, stepDelta), (stepDelta - 0.001) / stepDelta, 1e-6),
        "classic click belongs to the last physics step");
    check(near(bf::inputStepProgress(
            bf::beforeFrameTimestamp(frameStart - 0.001, frameStart, intervalStart, frameStart, true),
            intervalStart, stepDelta), 0.0),
        "full-frame lead belongs to the first physics step");
    double classicTime = frameStart - 0.001;
    double ledTime = bf::beforeFrameTimestamp(classicTime, frameStart, intervalStart, lead, true);
    check(static_cast<int>(classicTime / stepDelta) == 3 && static_cast<int>(ledTime / stepDelta) == 2,
        "one-step lead moves a late click one physics step earlier");

    // -----------------------------------------------------------------------
    // v2.0.0: adaptive lead, presets, per-input scales, step index
    // -----------------------------------------------------------------------
    check(bf::leadModeFromSetting("Auto") == bf::LeadMode::Auto, "Auto lead mode parsed");
    check(bf::leadModeFromSetting("Manual") == bf::LeadMode::Manual, "Manual lead mode parsed");
    check(bf::presetFromSetting("Vanilla") == bf::Preset::Vanilla, "Vanilla preset parsed");
    check(bf::presetFromSetting("One Step") == bf::Preset::OneStep, "One Step preset parsed");
    check(bf::presetFromSetting("Half Frame") == bf::Preset::HalfFrame, "Half Frame preset parsed");
    check(bf::presetFromSetting("Full Frame") == bf::Preset::FullFrame, "Full Frame preset parsed");
    check(bf::presetFromSetting("Custom") == bf::Preset::Custom, "Custom preset parsed");
    check(bf::presetFromSetting("unknown") == bf::Preset::Custom, "unknown preset falls back to Custom");

    // Auto lead equals exactly one physics step, clamped to the frame.
    check(near(bf::autoLead(stepDelta, frameStart), stepDelta), "auto lead = one physics step");
    check(near(bf::autoLead(frameStart * 4.0, frameStart), frameStart), "auto lead clamped to frame interval");
    check(near(bf::autoLead(0.0, frameStart), 0.0), "auto lead with zero step is 0");
    check(near(bf::autoLead(stepDelta, 0.0), 0.0), "auto lead with zero frame is 0");

    bf::LeadProfile profile; // defaults: enabled, Auto, Custom
    check(near(bf::resolveLead(profile, true, false, stepDelta, frameStart), stepDelta),
        "default profile uses adaptive one-step lead");

    profile.mode = bf::LeadMode::Manual;
    profile.manualLead = bf::msToSeconds(4.17);
    check(near(bf::resolveLead(profile, true, false, stepDelta, frameStart), 0.00417),
        "manual profile uses the millisecond value");

    // Presets override the individual settings.
    profile.preset = bf::Preset::Vanilla;
    check(near(bf::resolveLead(profile, true, false, stepDelta, frameStart), 0.0), "Vanilla preset = no lead");
    profile.preset = bf::Preset::HalfFrame;
    check(near(bf::resolveLead(profile, true, false, stepDelta, frameStart), frameStart * 0.5), "Half Frame preset");
    profile.preset = bf::Preset::FullFrame;
    check(near(bf::resolveLead(profile, true, false, stepDelta, frameStart), frameStart), "Full Frame preset");
    profile.preset = bf::Preset::OneStep;
    check(near(bf::resolveLead(profile, true, false, stepDelta, frameStart), stepDelta), "One Step preset");

    // Every lead is clamped to the frame interval (no time travel).
    profile.preset = bf::Preset::Custom;
    profile.mode = bf::LeadMode::Manual;
    profile.manualLead = 1.0;
    check(near(bf::resolveLead(profile, true, false, stepDelta, frameStart), frameStart), "lead clamped to frame");
    profile.enabled = false;
    check(near(bf::resolveLead(profile, true, false, stepDelta, frameStart), 0.0), "disabled profile = 0");

    // Separate release / P2 scales only apply when enabled.
    profile.enabled = true;
    profile.manualLead = bf::msToSeconds(4.0);
    profile.releaseScale = 0.0;
    profile.p2Scale = 0.5;
    check(near(bf::resolveLead(profile, true, false, stepDelta, frameStart), 0.004), "press lead unaffected by scales");
    profile.separateLeads = true;
    check(near(bf::resolveLead(profile, false, false, stepDelta, frameStart), 0.0), "release scale 0% keeps exact timing");
    check(near(bf::resolveLead(profile, true, true, stepDelta, frameStart), 0.002), "P2 scale 50%");
    check(near(bf::resolveLead(profile, false, true, stepDelta, frameStart), 0.0), "release + P2 scales combine");

    // Full-frame lead really moves a late click to step 0.
    profile.separateLeads = false;
    profile.preset = bf::Preset::FullFrame;
    double fullLead = bf::resolveLead(profile, true, false, stepDelta, frameStart);
    check(bf::inputStepIndex(
            bf::beforeFrameTimestamp(frameStart - 0.001, frameStart, 0.0, fullLead, true),
            0.0, stepDelta, 4) == 0,
        "Full Frame preset lands on the first physics step");
    check(bf::inputStepIndex(frameStart - 0.001, 0.0, stepDelta, 4) == 3,
        "classic late click lands on the last physics step");

    // Monotonicity: sorted inputs must stay sorted after the transform,
    // otherwise a press/release pair could be swapped.
    bf::LeadProfile mono;
    mono.mode = bf::LeadMode::Manual;
    mono.manualLead = bf::msToSeconds(6.0);
    mono.separateLeads = true;
    mono.releaseScale = 0.25;
    mono.p2Scale = 1.5;
    bool monotonic = true;
    for (int trial = 0; trial < 200; ++trial) {
        double t0 = 0.001 + (trial % 13) * 0.001;
        double t1 = t0 + 0.0007;
        double l0 = bf::resolveLead(mono, true, false, stepDelta, frameStart);
        double l1 = bf::resolveLead(mono, false, true, stepDelta, frameStart);
        double a = bf::beforeFrameTimestamp(t0, frameStart, 0.0, l0, true);
        double b = bf::beforeFrameTimestamp(t1, frameStart, 0.0, l1, true);
        if (a > b) { monotonic = false; break; }
    }
    check(monotonic, "timestamp transform preserves input order across 200 trials");

    if (failures) {
        std::fprintf(stderr, "%d test(s) failed\n", failures);
        return EXIT_FAILURE;
    }
    std::fprintf(stdout, "All before_frame tests passed\n");
    return EXIT_SUCCESS;
}
