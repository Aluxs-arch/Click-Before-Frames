# v2.0.0

**English**

* **Adaptive lead (Auto mode, now the default):** the lead is no longer a hard-coded 4.17 ms — it equals one *real* physics step of the current frame, so it automatically adapts to FPS, timewarp, lag spikes and Physics Bypass. It is always clamped to the measured frame interval.
* **Presets:** `Custom`, `Vanilla` (no lead), `One Step`, `Half Frame`, `Full Frame` (every click on the first physics step). Presets override the individual lead settings for quick switching.
* **Per-input leads:** optional separate scales for releases (orb holds, dash orb releases) and Player 2 in duals, 0–200%. `0%` keeps those inputs at their exact sub-frame time.
* **Calibration / debug overlay:** localized HUD with measured frame time, current physics step (`n/total`), clicks moved to the first step and total latency saved. Auto/Manual lead can be calibrated against real numbers.
* **End-of-attempt statistics:** localized end-screen line with click count, first-step count and saved milliseconds. Watermark is now `BF2` / `BF2+PB`.
* **Safer timing:** leads are clamped to the frame interval; disabled on death, pause, editor/practice transitions, `Disable Mod` and in `Click on Steps` mode (full vanilla parity). Press/release order is guaranteed monotonic and is now covered by randomized unit tests.
* All new UI strings are available in **English + Bahasa Indonesia** (`resources/lang/en.lang` / `id.lang`) with built-in fallbacks.
* New bilingual architecture doc: [`docs/HOW_IT_WORKS.md`](docs/HOW_IT_WORKS.md) with a frame/step timing diagram.
* Expanded engine-independent tests (50+ assertions): presets, adaptive lead, scales, clamping, step index and input-order monotonicity.
* Version bumped in `mod.json`, `CMakeLists.txt`, `about.md`, README (EN/ID) and the release workflow. Release package: `click-before-frames-v2.0.0.geode` + GitHub Release `v2.0.0`.
* Developer: **Aluxs** — GitHub **[@Aluxs-arch](https://github.com/Aluxs-arch)**.

**Bahasa Indonesia**

* **Jeda adaptif (mode Auto, kini bawaan):** jeda tidak lagi kaku 4,17 ms, melainkan sama dengan satu step fisika *asli* frame saat ini, sehingga otomatis menyesuaikan FPS, timewarp, lag, dan Physics Bypass. Selalu dibatasi ke interval frame terukur.
* **Preset:** `Custom`, `Vanilla` (tanpa jeda), `One Step`, `Half Frame`, `Full Frame` (semua klik di step fisika pertama). Preset menimpa pengaturan jeda individu untuk pergantian cepat.
* **Jeda per-input:** skala terpisah opsional untuk pelepasan (tahan orb, dash orb) dan Player 2 saat dual, 0–200%. `0%` membuat input tersebut tetap di waktu sub-frame persisnya.
* **Overlay kalibrasi/debug:** HUD terlokalisasi berisi interval frame terukur, step fisika saat ini (`n/total`), jumlah klik yang pindah ke step pertama, dan total latensi yang dihemat. Jeda Auto/Manual bisa dikalibrasi dengan angka nyata.
* **Statistik akhir attempt:** baris terlokalisasi di layar akhir berisi jumlah klik, jumlah yang masuk step pertama, dan milidetik yang dihemat. Watermark kini `BF2` / `BF2+PB`.
* **Waktu lebih aman:** jeda dibatasi ke interval frame; dimatikan saat mati, pause, transisi editor/practice, `Nonaktifkan Mod`, dan mode `Klik per Step` (paritas vanilla penuh). Urutan tekan/lepas dijamin monoton dan kini diuji dengan tes acak.
* Semua string UI baru tersedia dalam **English + Bahasa Indonesia** (`resources/lang/en.lang` / `id.lang`) dengan fallback bawaan.
* Dokumen arsitektur dua bahasa baru: [`docs/HOW_IT_WORKS.md`](docs/HOW_IT_WORKS.md) lengkap dengan diagram timing frame/step.
* Tes tanpa SDK diperluas (50+ assertion): preset, jeda adaptif, skala, clamp, indeks step, dan monotonisitas urutan input.
* Versi dinaikkan di `mod.json`, `CMakeLists.txt`, `about.md`, README (EN/ID), dan workflow rilis. Paket rilis: `click-before-frames-v2.0.0.geode` + GitHub Release `v2.0.0`.
* Developer: **Aluxs** — GitHub **[@Aluxs-arch](https://github.com/Aluxs-arch)**.

---

# v1.0.0

**English**

* First release of **Click Before Frames**, a fork of Click Between Frames v1.5.0 (GD 2.2081). Targets Geode 5.6.0+ (5.3.0 SDK headers contain a duplicate `CCFileUtilsWin32::init` declaration that breaks current builds).
* New **Before Frame** scheduler: clicks are pulled earlier by a configurable lead and clamped to the start of the frame's physics interval; a full-frame lead applies every click on the first physics step (`src/before_frame.hpp`).
* New **Pre-Frame Lead (ms)** setting (0–16.67 ms, default 4.17 ms = one 240 TPS step at 60 FPS).
* New **Before Frame Mode** toggle — switch back to classic between-frames precision at any time; the lead is skipped in Click on Steps mode to keep vanilla parity.
* Two languages: English and Bahasa Indonesia. In-game labels, end-screen caption/credit and the Linux/Wine popup are localized via `resources/lang/*.lang`; mod settings are written bilingually.
* New optional localized status label and "BF" / "BF+PB" end-screen watermark with developer credit.
* Developer: **Aluxs** — GitHub **[@Aluxs-arch](https://github.com/Aluxs-arch)**.
* Marked Click Between Frames and CBF Lite as incompatible (identical hooks).
* Added engine-independent unit tests for the scheduler and localization, plus a CI test job.
* Added bilingual documentation: `README.md`, `README.id.md`, `about.md`, `support.md`.
* Release package: CI merges Windows, macOS, Android32 and Android64 binaries into `click-before-frames-v1.0.0.geode` and attaches it to the v1.0.0 GitHub Release. The iOS binary is pending an upstream Geode bindings fix.

**Bahasa Indonesia**

* Rilis perdana **Click Before Frames**, fork dari Click Between Frames v1.5.0 (GD 2.2081). Menargetkan Geode 5.6.0+ (header SDK 5.3.0 memiliki deklarasi ganda `CCFileUtilsWin32::init` yang menggagalkan build saat ini).
* Penjadwal **Before Frame** baru: klik digeser lebih awal sesuai jeda dan dibatasi ke awal interval fisika frame; jeda satu frame penuh menerapkan semua klik di step fisika pertama (`src/before_frame.hpp`).
* Pengaturan **Jeda Awal (ms)** baru (0–16,67 ms, bawaan 4,17 ms = satu step 240 TPS pada 60 FPS).
* Toggle **Mode Before Frame** baru — bisa kembali ke presisi klasik di antara frame kapan saja; jeda dilewati pada mode Klik per Step agar fisikanya tetap vanilla.
* Dua bahasa: English dan Bahasa Indonesia. Label dalam game, caption/kredit layar akhir, dan popup Linux/Wine dilokalisasi lewat `resources/lang/*.lang`; pengaturan mod ditulis dua bahasa.
* Label status opsional dan watermark layar akhir "BF" / "BF+PB" dengan kredit developer.
* Developer: **Aluxs** — GitHub **[@Aluxs-arch](https://github.com/Aluxs-arch)**.
* Click Between Frames dan CBF Lite ditandai tidak kompatibel (hook yang sama).
* Tes unit untuk penjadwal dan lokalisasi tanpa SDK, beserta job CI untuk tes.
* Dokumentasi dua bahasa: `README.md`, `README.id.md`, `about.md`, `support.md`.
* Paket rilis: CI menggabungkan binary Windows, macOS, Android32, dan Android64 menjadi `click-before-frames-v1.0.0.geode` lalu melampirkannya ke GitHub Release v1.0.0. Binary iOS menunggu perbaikan upstream Geode bindings.
