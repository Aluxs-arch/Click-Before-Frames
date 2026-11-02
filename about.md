# Click Before Frames — v2.0.0

**Developer: Aluxs** — GitHub: [@Aluxs-arch](https://github.com/Aluxs-arch)
Source: [Aluxs-arch/Click-Before-Frames](https://github.com/Aluxs-arch/Click-Before-Frames)

**English** / **Bahasa Indonesia** — this mod ships with two languages. Pick one
in the mod settings (**Language / Bahasa**).

---

## [EN] English

### What does it do

This mod registers your clicks **before** each visual frame instead of halfway
through it. Input events collected between two rendered frames are pulled
earlier by a lead and clamped to the start of the frame's physics interval, so
the click is applied before the frame is drawn.

**v2.0.0 makes the lead adaptive:** with `Lead Mode = Auto` (default) the lead
equals one *real* physics step of the current frame, so it follows your FPS,
timewarp, lag and Physics Bypass instead of a fixed 4.17 ms. With the
`Full Frame` preset every click lands on the first physics step. Leads are
always clamped to the measured frame interval, and press/release order is
preserved.

### v2.0.0 settings

* **Preset** — `Custom`, `Vanilla` (no lead), `One Step`, `Half Frame`,
  `Full Frame` (all clicks on step 0).
* **Lead Mode** — `Auto` (one real physics step) or `Manual`.
* **Manual Lead (ms)** — 0–16.67 ms, used with Custom + Manual.
* **Separate Leads** — own scales for releases (orb holds) and Player 2 in
  duals; `0%` keeps exact sub-frame timing.
* **Show Debug Overlay** — measured frame time, current step `n/total`, clicks
  moved to step 0 and total latency saved. Great for calibration.
* **Show Status Label**, **Language / Bahasa** — localized in-game UI.
* **Disable Mod / Safe Mode / Click on Steps / Physics Bypass** — same safety
  options as CBF. Leads are disabled in Click on Steps mode (vanilla parity).
* End screen shows localized `BF2` statistics for the attempt.

### How to use

Use Physics Bypass or an FPS of 60, 80, 120 or 240. Disable TPS Bypass and Draw
Divide. If the icon jumps on respawn, disable "Stop Triggers on Death" in Mega
Hack. On Linux/Wine, input may need `sudo usermod -aG input $USER`. Start with
`Auto` + `Custom`; enable the debug overlay to calibrate. For orb-heavy levels,
enable Separate Leads and set Release Lead to `0%`.

<cy>Warning:</cy> applying clicks earlier changes input timing. Some lists and
leaderboards that allow CBF will **not** allow this mod. Use **Safe Mode** if
you play rated levels. This mod does not work with bots and is incompatible with
Click Between Frames / CBF Lite.

### Credits

Fork of [Click Between Frames](https://github.com/theyareonit/Click-Between-Frames)
by theyareonit (MIT). Android/macOS/iOS input code based on Click on Steps by
zmx; ports by mat and Jasmine. Adaptive Before Frame scheduler, presets,
per-input leads, diagnostics, localization, docs and tests by Aluxs.

---

## [ID] Bahasa Indonesia

### Apa yang dilakukan

Mod ini mencatat klik **sebelum** setiap frame visual, bukan di tengah frame.
Event input yang dikumpulkan di antara dua frame digeser lebih awal sebesar jeda
lalu dibatasi ke awal interval fisika frame, sehingga klik diterapkan sebelum
frame digambar.

**v2.0.0 membuat jeda adaptif:** dengan `Mode Jeda = Auto` (bawaan), jeda sama
dengan satu step fisika *asli* frame saat ini, jadi mengikuti FPS, timewarp,
lag, dan Physics Bypass, bukan 4,17 ms yang kaku. Dengan preset `Full Frame`,
semua klik mendarat di step fisika pertama. Jeda selalu dibatasi ke interval
frame terukur dan urutan tekan/lepas tetap terjaga.

### Pengaturan v2.0.0

* **Preset** — `Custom`, `Vanilla` (tanpa jeda), `One Step`, `Half Frame`,
  `Full Frame` (semua klik di step 0).
* **Mode Jeda** — `Auto` (satu step fisika asli) atau `Manual`.
* **Jeda Manual (ms)** — 0–16,67 ms, dipakai dengan Custom + Manual.
* **Jeda Terpisah** — skala sendiri untuk pelepasan (tahan orb) dan Player 2
  saat dual; `0%` mempertahankan waktu sub-frame persisnya.
* **Overlay Debug** — interval frame terukur, step `n/total`, klik yang pindah
  ke step 0, dan total latensi yang dihemat. Berguna untuk kalibrasi.
* **Tampilkan Label Status**, **Language / Bahasa** — UI terlokalisasi.
* **Nonaktifkan Mod / Mode Aman / Klik per Step / Physics Bypass** — opsi
  keamanan seperti CBF. Jeda dimatikan pada mode Klik per Step (paritas vanilla).
* Layar akhir menampilkan statistik `BF2` terlokalisasi untuk attempt tersebut.

### Cara pakai

Pakai Physics Bypass atau FPS 60, 80, 120, atau 240. Matikan TPS Bypass dan
Draw Divide. Kalau ikon melompat saat respawn, matikan "Stop Triggers on Death"
di Mega Hack. Di Linux/Wine, input mungkin memerlukan
`sudo usermod -aG input $USER`. Mulai dengan `Auto` + `Custom`; aktifkan overlay
debug untuk kalibrasi. Untuk level yang banyak orb, aktifkan Jeda Terpisah dan
set Jeda Lepas ke `0%`.

<cy>Perhatian:</cy> klik yang diterapkan lebih awal mengubah waktu input.
Sebagian list dan leaderboard yang membolehkan CBF **tidak** membolehkan mod
ini. Pakai **Mode Aman** saat main level ber-rating. Mod ini tidak bekerja
dengan bot dan tidak kompatibel dengan Click Between Frames / CBF Lite.

### Kredit

Fork dari [Click Between Frames](https://github.com/theyareonit/Click-Between-Frames)
oleh theyareonit (MIT). Kode input Android/macOS/iOS berbasis Click on Steps
oleh zmx; port oleh mat dan Jasmine. Penjadwal Before Frame adaptif, preset,
jeda per-input, diagnostik, lokalisasi, dokumentasi, dan tes oleh Aluxs.
