#pragma once

#include <algorithm>

// Pure, engine-independent logic for the "Before Frame" input scheduler.
// Kept free of Geode/Cocos headers so it can be unit tested on any platform
// (see tests/test_before_frame.cpp).
namespace bf {

using Timestamp = double;

// Convert a millisecond value from the mod settings into seconds, which is the
// unit used by the game's PlayerButtonCommand timestamps.
constexpr double msToSeconds(double milliseconds) {
    return milliseconds / 1000.0;
}

// "Before Frame" positioning of an input inside a visual frame.
//
// The game collects OS input events in [intervalStart, frameStart) and then
// simulates the physics steps of the visual frame that starts at frameStart.
// Classic CBF replays every click at its exact sub-frame position. Before Frame
// instead pulls every click `lead` seconds EARLIER, so the click is applied
// before the visual frame rather than at the point where it was rendered:
//
//   * lead = 4.17 ms (default, one 240 TPS step at 60 FPS): a late click is
//     applied one physics step earlier;
//   * lead = 16.67 ms (one whole 60 FPS frame): every click in the interval
//     is applied on the FIRST physics step, i.e. before the frame is drawn.
//
// Two safety rules:
//   * Never move a click before intervalStart (the physics of the previous
//     frame has already been simulated; going further back is impossible).
//     This also keeps input order intact, since the transform is monotonic.
//   * Never pull a click that happened at/after frameStart back into this
//     frame (that would mean reacting to an event that has not happened yet).
//     Those inputs stay in the queue for the next frame, like vanilla CBF.
//
// Set enabled=false (or lead<=0) to get the original between-frames behaviour.
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
// Useful for diagnostics / tests; the real step queue uses the same formula.
constexpr double inputStepProgress(Timestamp inputTime, Timestamp intervalStart, double stepDelta) {
    if (stepDelta <= 0.0) return 0.0;
    double elapsed = inputTime - intervalStart;
    double progress = elapsed - stepDelta * static_cast<double>(static_cast<long long>(elapsed / stepDelta));
    return std::clamp(progress / stepDelta, 0.0, 1.0);
}

} // namespace bf
