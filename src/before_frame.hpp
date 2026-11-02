#pragma once

#include <algorithm>
#include <string_view>

// Pure, engine-independent logic for the "Before Frame" input scheduler.
// Kept free of Geode/Cocos headers so it can be unit tested on any platform
// (see tests/test_before_frame.cpp).
namespace bf {

using Timestamp = double;

// ---------------------------------------------------------------------------
// v2.0.0: adaptive / per-input lead profiles
// ---------------------------------------------------------------------------

enum class LeadMode {
    Auto,    // lead = one physics step of the current frame
    Manual,  // lead = the user's millisecond value
};

enum class Preset {
    Custom,     // use the individual settings below
    Vanilla,    // no lead at all (classic between-frames precision)
    OneStep,    // one physics step (same as Auto)
    HalfFrame,  // half of the measured frame interval
    FullFrame,  // every input on the first physics step
};

constexpr LeadMode leadModeFromSetting(std::string_view value) {
    return value == "Auto" ? LeadMode::Auto : LeadMode::Manual;
}

constexpr Preset presetFromSetting(std::string_view value) {
    if (value == "Vanilla") return Preset::Vanilla;
    if (value == "One Step") return Preset::OneStep;
    if (value == "Half Frame") return Preset::HalfFrame;
    if (value == "Full Frame") return Preset::FullFrame;
    return Preset::Custom;
}

// Convert a millisecond value from the mod settings into seconds, which is the
// unit used by the game's PlayerButtonCommand timestamps.
constexpr double msToSeconds(double milliseconds) {
    return milliseconds / 1000.0;
}

struct LeadProfile {
    bool enabled = true;
    LeadMode mode = LeadMode::Auto;
    double manualLead = msToSeconds(4.17);  // seconds
    bool separateLeads = false;
    double releaseScale = 1.0;  // 0.0 .. 2.0 (1.0 = 100%)
    double p2Scale = 1.0;       // 0.0 .. 2.0
    Preset preset = Preset::Custom;
};

// One physics step of the current frame, never larger than the frame itself.
// stepDelta already accounts for timewarp / lag / Physics Bypass, which makes
// this genuinely adaptive instead of a hard-coded 4.17 ms.
constexpr double autoLead(double stepDelta, double frameInterval) {
    if (stepDelta <= 0.0 || frameInterval <= 0.0) return 0.0;
    return std::min(stepDelta, frameInterval);
}

// Resolve the lead (in seconds) for one input event.
// `isPush` / `isPlayer2` are only used when separateLeads is on, so releases
// (orb holds, dash orb releases) and P2 in duals can keep their own timing.
// The result is always clamped to the current frame interval: the mod can
// never simulate physics from a previous frame.
constexpr double resolveLead(
    LeadProfile const& profile,
    bool isPush,
    bool isPlayer2,
    double stepDelta,
    double frameInterval
) {
    if (!profile.enabled || frameInterval <= 0.0) return 0.0;

    double base = 0.0;
    switch (profile.preset) {
        case Preset::Vanilla:   return 0.0;
        case Preset::OneStep:   base = autoLead(stepDelta, frameInterval); break;
        case Preset::HalfFrame: base = frameInterval * 0.5; break;
        case Preset::FullFrame: base = frameInterval; break;
        case Preset::Custom:
            base = (profile.mode == LeadMode::Auto)
                ? autoLead(stepDelta, frameInterval)
                : std::max(0.0, profile.manualLead);
            break;
    }

    if (profile.separateLeads) {
        double scale = (isPush ? 1.0 : std::max(0.0, profile.releaseScale))
                     * (isPlayer2 ? std::max(0.0, profile.p2Scale) : 1.0);
        base *= scale;
    }

    return std::clamp(base, 0.0, frameInterval);
}

// ---------------------------------------------------------------------------
// Timestamp transform (unchanged from v1, but now fed a resolved lead)
// ---------------------------------------------------------------------------

// Pull a click `lead` seconds earlier, clamped to the start of the physics
// interval that belongs to the visual frame starting at frameStart.
//
// Safety rules:
//   * never earlier than intervalStart (that physics already happened);
//   * never pull an input that happened at/after frameStart back into this
//     frame (that would mean reacting to an event that has not happened yet).
//
// enabled=false (or lead<=0) = classic between-frames behaviour.
constexpr Timestamp beforeFrameTimestamp(
    Timestamp inputTime,
    Timestamp frameStart,
    Timestamp intervalStart,
    double lead,
    bool enabled
) {
    if (!enabled || lead <= 0.0 || inputTime >= frameStart) return inputTime;
    return std::max(inputTime - lead, intervalStart);
}

// The proportion of a physics step that has elapsed when the input is applied.
constexpr double inputStepProgress(Timestamp inputTime, Timestamp intervalStart, double stepDelta) {
    if (stepDelta <= 0.0) return 0.0;
    double elapsed = inputTime - intervalStart;
    double progress = elapsed - stepDelta * static_cast<double>(static_cast<long long>(elapsed / stepDelta));
    return std::clamp(progress / stepDelta, 0.0, 1.0);
}

// 0-based physics step index an input belongs to (used by the debug overlay).
constexpr int inputStepIndex(Timestamp inputTime, Timestamp intervalStart, double stepDelta, int stepCount) {
    if (stepDelta <= 0.0 || stepCount <= 0) return 0;
    int index = static_cast<int>((inputTime - intervalStart) / stepDelta);
    return std::clamp(index, 0, stepCount - 1);
}

} // namespace bf
