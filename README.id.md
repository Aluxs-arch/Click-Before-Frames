# Click Before Frames

[English](README.md) | **Bahasa Indonesia**

> **Developer:** Aluxs &nbsp;•&nbsp; **GitHub:** [@Aluxs-arch](https://github.com/Aluxs-arch)
> Mod Geode untuk Geometry Dash 2.2081 (Windows, Android, macOS, iOS).

Mod Geode yang mencatat klik **sebelum** setiap frame visual, bukan di tengah
frame. Mod ini merupakan fork dari
[Click Between Frames](https://github.com/theyareonit/Click-Between-Frames)
karya theyareonit, dengan penjadwal input baru, dua bahasa (English + Bahasa
Indonesia), dan identitas Aluxs.

## Apa itu "Before Frame"

Geometry Dash vanilla (dan CBF) mengumpulkan event input di antara dua frame
yang dirender, lalu menyimulasikan step fisika untuk frame berikutnya. CBF
klasik memutar ulang setiap klik pada posisi sub-frame yang **persis**, sehingga
batas presisi 240 TPS hilang.

**Click Before Frames** melangkah lebih jauh: setiap klik digeser lebih awal
sebesar **jeda (lead)** yang bisa diatur, lalu dibatasi ke awal interval fisika
frame tersebut. Klik pun diterapkan *sebelum* frame visual, bukan tepat pada
posisi saat frame itu digambar:

* Dengan jeda bawaan **4,17 ms** (satu step 240 TPS pada 60 FPS), klik yang
  terlambat diterapkan satu step fisika lebih awal, sehingga jeda input
  berkurang hingga 4,17 ms.
* Dengan jeda **16,67 ms** (satu frame penuh 60 FPS), semua klik dalam interval
  diterapkan pada step fisika **pertama** — sebelum frame digambar.

Ini juga bisa dipakai untuk mengompensasi latensi layar atau perangkat input.
Karena transformasinya monoton, urutan tekan/lepas selalu terjaga.

Ada dua aturan pengaman agar fisika tetap masuk akal:

* Klik tidak pernah digeser ke sebelum awal interval fisika saat ini (fisika
  itu sudah disimulasikan).
* Klik yang terjadi pada/setelah batas frame **tidak** ditarik mundur ke frame
  ini; klik tersebut tetap mengantre untuk frame berikutnya, sama seperti CBF
  vanilla.

Mematikan **Mode Before Frame** mengembalikan perilaku klasik di antara frame,
jadi mod ini juga bisa dipakai sebagai pengganti CBF. Pergeseran juga otomatis
dimatikan pada mode **Klik per Step** agar fisikanya tetap sama dengan vanilla.

> ⚠️ Menggeser klik lebih awal mengubah waktu input. Sebagian list dan
> leaderboard yang membolehkan CBF mungkin **tidak** membolehkan ini. Pakai
> **Mode Aman** dan cek aturan list/leaderboard sebelum mengirim run.

## Dua bahasa / Two languages

Mod ini menyediakan tepat dua bahasa:

| Kode | Bahasa | Berkas dalam game |
| ---- | ------ | ----------------- |
| `en` | English | [`resources/lang/en.lang`](resources/lang/en.lang) |
| `id` | Bahasa Indonesia | [`resources/lang/id.lang`](resources/lang/id.lang) |

Ganti lewat pengaturan **Language / Bahasa**. Pengaturan ini memengaruhi label
status dalam game, caption/kredit di layar akhir, dan popup error Linux/Wine.
String juga ditanam di biner sebagai cadangan, jadi mod tetap berfungsi walau
berkas `.lang` hilang. Nama dan deskripsi pengaturan ditulis dalam dua bahasa
sekaligus karena UI pengaturan Geode belum mendukung terjemahan per bahasa.

Dokumentasi juga dua bahasa: [`README.md`](README.md) berbahasa Inggris,
berkas ini berbahasa Indonesia, sedangkan [`about.md`](about.md) dan
[`changelog.md`](changelog.md) memuat keduanya.

## Instalasi

1. Pasang [Geode](https://geode-sdk.org/install/) **5.6.0 atau lebih baru**.
2. Pasang **Click Before Frames** (dari index Geode, atau bangun paket `.geode`
   sendiri — lihat [Membangun](#membangun)).
3. Jangan aktifkan Click Between Frames atau CBF Lite bersamaan; keduanya
   ditandai tidak kompatibel karena melakukan hook pada fungsi yang sama.

## Pengaturan

| Pengaturan | ID | EN |
| ---------- | -- | -- |
| `language` | Bahasa | Language |
| `soft-toggle` | Nonaktifkan mod tanpa restart | Disable the mod without restarting |
| `before-frame` | Geser klik lebih awal (sebelum frame) | Pull clicks earlier (before the frame) |
| `before-frame-lead` | Jeda dalam ms (0 = klasik, 4,17 = satu step 240 TPS, 16,67 = satu frame penuh 60 FPS) | Lead in ms |
| `show-indicator` | Label status kecil sesuai bahasa | Small localized status label |
| `safe-mode` | Nonaktifkan progres pada level ber-rating | Disable progress on rated levels |
| `click-on-steps` | Presisi 240 TPS, fisika mirip vanilla | 240 TPS precision |
| `mouse-fix` / `precision-fix` | Perbaikan performa Windows | Windows performance fixes |
| `physics-bypass` / `bypass-mode` | Physics Bypass bawaan (opsional) | Optional Physics Bypass |
| `wine-workaround` / `right-click` | Opsi input Linux/Wine | Linux/Wine input options |

## Cara pakai

* Kalau ikon otomatis melompat saat respawn, matikan **Stop Triggers on Death**
  di Mega Hack.
* Untuk mengubah keybind, buka Options → **Keys** (butuh Custom Keybinds).
* Pakai Physics Bypass atau salah satu FPS: 60, 80, 120, 240. GD 2.2 stutter
  pada nilai lain kecuali Physics Bypass aktif.
* Matikan TPS Bypass / Draw Divide karena tidak berguna dengan mod ini. Mod
  mengabaikan opsi vanilla *Click On Steps* / *Click Between Steps*.
* Di Linux/Wine, kalau input tidak bekerja, jalankan
  `sudo usermod -aG input $USER` (sedikit mengurangi keamanan sistem) lalu
  restart.
* Mulai dengan **Jeda Awal** `4,17` (satu step) dan naikkan bertahap bila
  perlu kompensasi latensi lebih besar; `0` mengembalikan presisi klasik.

## Masalah yang diketahui

* Tidak bekerja dengan bot.
* Dukungan controller Linux/Wine masih eksperimental.
* Karena klik diterapkan lebih awal, replay/macro dari mod input lain bisa
  desinkronisasi.

## Membangun

Butuh [Geode SDK](https://docs.geode-sdk.org/getting-started/), CMake 3.21+,
dan kompiler C++23.

```bash
geode build            # atau: cmake -B build -DGEODE_SDK=... && cmake --build build
bash tests/run.sh      # tes logika + i18n + JSON tanpa SDK
```

`resources/linux-input.so` adalah helper hasil build untuk Linux/Wine (diabaikan
Git). Bangun secara lokal dengan `bash src/linux/build.sh` (butuh
`libevdev-dev`), atau biarkan CI yang membuatnya.
`.github/workflows/multi-platform.yml` menjalankan tes unit, membangun paket
Windows, macOS, Android32, Android64 (plus iOS sebagai upaya terbaik), lalu
menggabungkannya menjadi satu file `.geode` dan menerbitkannya sebagai aset
GitHub Release.

## Rilis

Paket siap pakai ada di
[GitHub Releases](https://github.com/Aluxs-arch/Click-Before-Frames/releases):
`click-before-frames-v1.0.0.geode` (Windows, macOS, Android32, Android64).
Pasang dengan membuka file `.geode` lewat Geode, atau masukkan ke folder mods
Geode.

> Binary iOS saat ini tertahan oleh masalah upstream Geode bindings (tipe FMOD
> pada source bindings hasil generate), bukan kode mod ini. Kode `src/main.cpp`
> dan `src/i18n.cpp` tetap bisa dikompilasi untuk iOS; binary iOS akan
> ditambahkan setelah toolchain upstream diperbaiki.

## Kredit & lisensi

* **Developer:** Aluxs — GitHub [@Aluxs-arch](https://github.com/Aluxs-arch)
* Mod asli: [Click Between Frames](https://github.com/theyareonit/Click-Between-Frames)
  oleh theyareonit (syzzi), lisensi MIT. Kode hook input/fisika di `src/`
  diturunkan dari mod tersebut.
* Kode input Android, macOS & iOS berbasis
  [Click on Steps oleh zmx](https://github.com/qimiko/click-on-steps), dipakai
  dengan izin pada proyek aslinya. Port Android oleh
  [mat](https://github.com/matcool), port macOS & iOS oleh
  [Jasmine](https://github.com/hiimjasmine00). Ikon asli oleh alex/sincos.
* Fork ini menambahkan penjadwal Before Frame, sistem dua bahasa, UI
  terlokalisasi, dokumentasi, dan tes.

MIT — lihat [LICENSE](LICENSE). Notis hak cipta asli tetap dipertahankan.
