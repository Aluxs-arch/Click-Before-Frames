# Click Before Frames

**Developer: Aluxs** — GitHub: [@Aluxs-arch](https://github.com/Aluxs-arch)
Source: [Aluxs-arch/Click-Before-Frames](https://github.com/Aluxs-arch/Click-Before-Frames)

**English** / **Bahasa Indonesia** — this mod ships with two languages. Pick one
in the mod settings (**Language / Bahasa**).

---

## [EN] English

### What does it do

This mod registers your clicks **before** each visual frame instead of halfway
through it. Input events collected between two rendered frames are pulled
earlier by a configurable lead: with the default 4.17 ms (one 240 TPS step at
60 FPS) a late click is applied one physics step earlier, and with a 16.67 ms
(one full frame) lead every click is applied on the **first physics step**,
before the frame is drawn. This removes input delay and lets you compensate for
monitor/device latency.

Turn **Before Frame Mode / Mode Before Frame** off to get the classic
between-frames precision of the original CBF instead.

### Settings

* **Before Frame Mode** — pull clicks earlier, before the visual frame.
* **Pre-Frame Lead (ms)** — how far clicks are pulled earlier (0–16.67 ms,
  default 4.17 ms = one 240 TPS step). `0` = classic precision; `16.67` = every
  click on the first step at 60 FPS.
* **Show Status Label** — small label in the chosen language.
* **Language / Bahasa** — `English` or `Bahasa Indonesia`.
* **Disable Mod / Safe Mode / Click on Steps / Physics Bypass** — same safety and
  compatibility options as CBF.

### How to use

Use Physics Bypass or an FPS of 60, 80, 120 or 240. Disable TPS Bypass and Draw
Divide. If the icon jumps on respawn, disable "Stop Triggers on Death" in Mega
Hack. On Linux/Wine, input may need `sudo usermod -aG input $USER`.

<cy>Warning:</cy> applying clicks earlier changes input timing. Some lists and
leaderboards that allow CBF will **not** allow this mod. Use **Safe Mode** if
you play rated levels. This mod does not work with bots and is incompatible with
Click Between Frames / CBF Lite.

### Credits

Fork of [Click Between Frames](https://github.com/theyareonit/Click-Between-Frames)
by theyareonit (MIT). Android/macOS/iOS input code based on Click on Steps by
zmx; ports by mat and Jasmine. Before Frame scheduler, localization, docs and
tests by Aluxs.

---

## [ID] Bahasa Indonesia

### Apa yang dilakukan

Mod ini mencatat klik **sebelum** setiap frame visual, bukan di tengah frame.
Event input yang dikumpulkan di antara dua frame digeser lebih awal sebesar jeda
yang bisa diatur: dengan bawaan 4,17 ms (satu step 240 TPS pada 60 FPS), klik
yang terlambat diterapkan satu step fisika lebih awal; dengan jeda 16,67 ms
(satu frame penuh), semua klik diterapkan pada **step fisika pertama** sebelum
frame digambar. Ini mengurangi jeda input dan bisa mengompensasi latensi
monitor/perangkat.

Matikan **Before Frame Mode / Mode Before Frame** untuk kembali ke presisi
klasik di antara frame seperti CBF asli.

### Pengaturan

* **Mode Before Frame** — geser klik lebih awal, sebelum frame visual.
* **Jeda Awal (ms)** — seberapa jauh klik digeser lebih awal (0–16,67 ms,
  bawaan 4,17 ms = satu step 240 TPS). `0` = presisi klasik; `16,67` = semua
  klik di step pertama pada 60 FPS.
* **Tampilkan Label Status** — label kecil sesuai bahasa pilihan.
* **Language / Bahasa** — `English` atau `Bahasa Indonesia`.
* **Nonaktifkan Mod / Mode Aman / Klik per Step / Physics Bypass** — opsi
  keamanan dan kompatibilitas seperti CBF.

### Cara pakai

Pakai Physics Bypass atau FPS 60, 80, 120, atau 240. Matikan TPS Bypass dan
Draw Divide. Kalau ikon melompat saat respawn, matikan "Stop Triggers on Death"
di Mega Hack. Di Linux/Wine, input mungkin memerlukan
`sudo usermod -aG input $USER`.

<cy>Perhatian:</cy> klik yang diterapkan lebih awal mengubah waktu input.
Sebagian list dan leaderboard yang membolehkan CBF **tidak** membolehkan mod
ini. Pakai **Mode Aman** saat main level ber-rating. Mod ini tidak bekerja
dengan bot dan tidak kompatibel dengan Click Between Frames / CBF Lite.

### Kredit

Fork dari [Click Between Frames](https://github.com/theyareonit/Click-Between-Frames)
oleh theyareonit (MIT). Kode input Android/macOS/iOS berbasis Click on Steps
oleh zmx; port oleh mat dan Jasmine. Penjadwal Before Frame, lokalisasi,
dokumentasi, dan tes oleh Aluxs.
