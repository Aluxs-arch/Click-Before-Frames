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
