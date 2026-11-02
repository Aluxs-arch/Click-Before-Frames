# Click Before Frames

**English** | [Bahasa Indonesia](README.id.md)

> **Developer:** Aluxs &nbsp;•&nbsp; **GitHub:** [@Aluxs-arch](https://github.com/Aluxs-arch)
> A Geode mod for Geometry Dash 2.2081 (Windows, Android, macOS, iOS).

A Geode mod that registers your clicks **ahead of** each visual frame instead
of somewhere in the middle of it. It is a fork of
[Click Between Frames](https://github.com/theyareonit/Click-Between-Frames) by
theyareonit, with a new input scheduler, two languages (English + Bahasa
Indonesia), and Aluxs branding.

## What "Before Frame" means

Vanilla Geometry Dash (and CBF) collects input events during the interval
between two rendered frames and then simulates the physics steps of the next
frame. Classic CBF replays every click at its *exact* sub-frame position, which
removes the 240 TPS precision limit.

**Click Before Frames** goes one step further: every click is pulled earlier by
a configurable **lead**, clamped to the start of the frame's physics interval.
The click is therefore applied *before* the visual frame rather than at the
exact point where it was rendered:

* With the default **4.17 ms** lead (one 240 TPS step at 60 FPS), a late click
  is applied one physics step earlier, removing up to 4.17 ms of input delay.
* With a **16.67 ms** lead (one whole 60 FPS frame), every click in the
  interval is applied on the **first** physics step — before the frame is drawn.

This can also be used to compensate for display or device latency. Because the
transform is monotonic, press/release order is always preserved.

Two safety rules keep the physics honest:

* A click is never moved before the start of the current physics interval (that
  physics has already been simulated).
* A click that happened at/after the frame boundary is **not** pulled back into
  the frame; it stays queued for the next frame, exactly like vanilla CBF.

Turning **Before Frame Mode** off gives you the original between-frames
behaviour, so this mod can also be used as a drop-in CBF replacement. The shift
is also disabled automatically in **Click on Steps** mode so that mode keeps
vanilla physics parity.

> ⚠️ Shifting clicks earlier changes the timing of your inputs. Some lists and
> leaderboards that allow CBF may **not** allow this. Use **Safe Mode** and
> check the rules of your list/leaderboard before submitting runs.

## Two languages / Dua bahasa

The mod ships with exactly two languages:

| Code | Language | In-game file |
| ---- | -------- | ------------ |
| `en` | English | [`resources/lang/en.lang`](resources/lang/en.lang) |
| `id` | Bahasa Indonesia | [`resources/lang/id.lang`](resources/lang/id.lang) |

Switch with **Language / Bahasa** in the mod settings. The setting affects the
in-game status label, the end-screen caption/credit, and the Linux/Wine error
popup. Strings are also compiled into the binary as a fallback, so the mod
still works if the `.lang` files are missing. Mod-setting names and
descriptions are written in both languages inline, because Geode's settings UI
has no per-language lookup.

Documentation is bilingual too: this file is English,
[`README.id.md`](README.id.md) is Indonesian, and
[`about.md`](about.md) / [`changelog.md`](changelog.md) contain both.

## Installation

1. Install [Geode](https://geode-sdk.org/install/).
2. Install **Click Before Frames** (from the Geode index, or build the `.geode`
   package yourself — see [Building](#building)).
3. Do **not** enable Click Between Frames or CBF Lite at the same time; they are
   marked as incompatible because they hook the same functions.

## Settings

| Setting | EN | ID |
| ------- | -- | -- |
| `language` | Language | Bahasa |
| `soft-toggle` | Disable the mod without restarting | Nonaktifkan mod tanpa restart |
| `before-frame` | Pull clicks earlier (before the frame) | Geser klik lebih awal (sebelum frame) |
| `before-frame-lead` | Lead in ms (0 = classic, 4.17 = one 240 TPS step, 16.67 = whole frame at 60 FPS) | Jeda dalam ms |
| `show-indicator` | Small localized status label | Label status kecil |
| `safe-mode` | Disable progress/stats on rated levels | Nonaktifkan progres pada level ber-rating |
| `click-on-steps` | 240 TPS precision, vanilla-like physics | Presisi 240 TPS |
| `mouse-fix` / `precision-fix` | Windows performance fixes | Perbaikan performa Windows |
| `physics-bypass` / `bypass-mode` | Optional built-in Physics Bypass | Physics Bypass bawaan (opsional) |
| `wine-workaround` / `right-click` | Linux/Wine input options | Opsi input Linux/Wine |

## How to use

* If the icon automatically jumps when you respawn, disable **Stop Triggers on
  Death** in Mega Hack.
* To edit keybinds, open GD Options → **Keys** (requires Custom Keybinds).
* Use Physics Bypass or one of these FPS values: 60, 80, 120, 240. 2.2 stutters
  on other values unless you enable Physics Bypass.
* Disable TPS Bypass / Draw Divide — they are pointless with this mod. The mod
  overrides the vanilla *Click On Steps* / *Click Between Steps* options.
* On Linux/Wine, if input does not work, run `sudo usermod -aG input $USER`
  (this slightly reduces system security) and reboot.
* Start with **Pre-Frame Lead** at `4.17` (one step) and increase it gradually
  if you need more latency compensation; `0` restores classic precision.

## Known issues

* Does not work with bots.
* Linux/Wine controller support is experimental.
* Because clicks are applied earlier, replays/macro recordings made with other
  input mods may desync.

## Building

Requires the [Geode SDK](https://docs.geode-sdk.org/getting-started/) and
CMake 3.21+ / a C++23 compiler.

```bash
geode build            # or: cmake -B build -DGEODE_SDK=... && cmake --build build
bash tests/run.sh      # engine-independent logic + i18n + JSON checks
```

`resources/linux-input.so` is a generated helper for Linux/Wine builds (it is
gitignored). Build it locally with `bash src/linux/build.sh` (needs
`libevdev-dev`), or let CI generate it.
`.github/workflows/multi-platform.yml` runs the unit tests and builds Windows,
macOS, iOS, Android32 and Android64 packages, plus that helper.

## Credits & license

* **Developer:** Aluxs — GitHub [@Aluxs-arch](https://github.com/Aluxs-arch)
* Original mod: [Click Between Frames](https://github.com/theyareonit/Click-Between-Frames)
  by theyareonit (syzzi), MIT licensed. The physics/input hook code in `src/`
  is derived from it.
* Android, macOS & iOS input code based on
  [Click on Steps by zmx](https://github.com/qimiko/click-on-steps), used with
  permission by the original project. Android port by
  [mat](https://github.com/matcool), macOS & iOS ports by
  [Jasmine](https://github.com/hiimjasmine00). Original icon by alex/sincos.
* This fork adds the Before Frame scheduler, the two-language system, the
  localized UI, documentation and tests.

MIT — see [LICENSE](LICENSE). Original copyright notices are preserved.
