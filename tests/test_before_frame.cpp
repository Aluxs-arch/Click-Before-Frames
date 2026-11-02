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

    if (failures) {
        std::fprintf(stderr, "%d test(s) failed\n", failures);
        return EXIT_FAILURE;
    }
    std::fprintf(stdout, "All before_frame tests passed\n");
    return EXIT_SUCCESS;
}
