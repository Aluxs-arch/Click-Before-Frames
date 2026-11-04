# How "Before Frame" works / Cara kerja "Before Frame"

**English** | [Bahasa Indonesia](#bahasa-indonesia)

Module logic lives in [`src/before_frame.hpp`](../src/before_frame.hpp) (pure C++,
unit-tested without the Geode SDK) and is applied in
[`src/main.cpp`](../src/main.cpp) inside `buildStepQueue()`.

---

## English

### 1. Where a click normally lands

Geometry Dash renders one visual frame every `frameInterval` seconds (16.67 ms at
60 FPS) and simulates `stepCount` physics steps inside that frame (4 at 60 FPS,
240 steps per second). The game collects OS input events during the interval
`[intervalStart, frameStart)` and replays them while simulating the frame that
starts at `frameStart`.

```
time ───────────────────────────────────────────────────────────────►

        intervalStart (T-1)                    frameStart (T)
              │                                     │
   physics    │  step 0  │  step 1  │  step 2  │  step 3  │
   steps      ├──────────┼──────────┼──────────┼──────────┤
              │                                     │
   OS input   │      ▲ click @ 15.7 ms             │
   collected  │      │                             │
              │                                     ▼
              │                              ► frame T is drawn
```

Classic CBF replays the click at its **exact** sub-step position (here, near the
end of step 3). That removes the 240 TPS precision limit, but the click still
takes effect at the *end* of the frame's simulation.

### 2. What Before Frame changes

Every input timestamp is pulled earlier by a **lead** and clamped to
`intervalStart`:

```
effective = max(inputTime - lead, intervalStart)
```

The transform is monotonic, so press/release order is preserved. Two hard rules:

* never go before `intervalStart` (that physics has already been simulated);
* an input at/after `frameStart` is **not** pulled back — it stays queued for the
  next frame (no reacting to events that have not happened yet).

```
              intervalStart                        frameStart
                    │                                  │
   steps            │ step 0 │ step 1 │ step 2 │ step 3│
                    ├────────┼────────┼────────┼───────┤
   classic click    │                            ▲ 15.7 ms
   lead = 1 step    │                  ▲ 11.5 ms
   lead = 1 frame   │ ▲ 0.0 ms (first step, before the frame is drawn)
```

### 3. v2.0.0: where the lead comes from

| Source | Lead |
| ------ | ---- |
| `Lead Mode = Auto` (default) | one real physics step: `stepDelta = frameInterval / stepCount` |
| `Lead Mode = Manual` | the `Manual Lead (ms)` setting |
| Preset `Vanilla` | `0` (classic between-frames precision) |
| Preset `One Step` | `stepDelta` |
| Preset `Half Frame` | `frameInterval / 2` |
| Preset `Full Frame` | `frameInterval` (all clicks on step 0) |

`Auto` is genuinely adaptive: `stepDelta` already includes timewarp, lag
compensation and Physics Bypass step counts, so the lead is not a fixed 4.17 ms
guess. Every resolved lead is clamped to `[0, frameInterval]`.

With **Separate Leads** enabled:

```
lead(input) = base × (isPush ? 1 : releaseScale) × (isPlayer2 ? p2Scale : 1)
```

`releaseScale = 0%` keeps releases (orb holds, dash orb releases) at their exact
sub-frame time; `p2Scale = 0%` does the same for P2 in duals.

### 4. Safety & diagnostics

Leads are disabled when the mod is soft-disabled, in `Click on Steps` mode,
on death, while paused, on the end screen, or in the editor. The localized debug
overlay shows measured `frameInterval`, the step an input landed on, how many
clicks were moved to step 0, and the accumulated latency saved; the same numbers
are summarized on the end screen.

### 5. Files

| File | Role |
| ---- | ---- |
| `src/before_frame.hpp` | pure lead resolution + timestamp transform |
| `src/i18n.*`, `resources/lang/*.lang` | EN/ID strings |
| `src/main.cpp` | `buildStepQueue()`, HUD, end screen, settings |
| `tests/test_before_frame.cpp` | 50+ assertions incl. monotonicity |
| `docs/HOW_IT_WORKS.md` | this document |

---

## Bahasa Indonesia

Logika modul ada di [`src/before_frame.hpp`](../src/before_frame.hpp) (C++ murni,
dites tanpa Geode SDK) dan dipakai di [`src/main.cpp`](../src/main.cpp) dalam
`buildStepQueue()`.

### 1. Di mana klik biasanya mendarat

Geometry Dash menggambar satu frame visual setiap `frameInterval` detik (16,67 ms
pada 60 FPS) dan menyimulasikan `stepCount` step fisika di dalam frame itu (4
pada 60 FPS, 240 step per detik). Event input OS dikumpulkan selama interval
`[intervalStart, frameStart)` lalu diputar ulang saat frame yang dimulai di
`frameStart` disimulasikan.

```
waktu ──────────────────────────────────────────────────────────────►

        intervalStart (T-1)                    frameStart (T)
              │                                     │
   step       │  step 0  │  step 1  │  step 2  │  step 3  │
   fisika     ├──────────┼──────────┼──────────┼──────────┤
              │                                     │
   input OS   │      ▲ klik @ 15,7 ms              │
              │                                     ▼
              │                              ► frame T digambar
```

CBF klasik memutar ulang klik pada posisi sub-step yang **persis** (di sini,
menjelang akhir step 3). Batas presisi 240 TPS hilang, tetapi klik tetap
berpengaruh di *akhir* simulasi frame.

### 2. Yang diubah Before Frame

Setiap timestamp input digeser lebih awal sebesar **jeda** lalu dibatasi ke
`intervalStart`:

```
efektif = max(waktuInput - jeda, intervalStart)
```

Transformasinya monoton, jadi urutan tekan/lepas tetap terjaga. Dua aturan keras:

* tidak pernah mundur sebelum `intervalStart` (fisika itu sudah disimulasikan);
* input pada/setelah `frameStart` **tidak** ditarik mundur — tetap mengantre
  untuk frame berikutnya (tidak bereaksi pada event yang belum terjadi).

```
              intervalStart                        frameStart
                    │                                  │
   step             │ step 0 │ step 1 │ step 2 │ step 3│
                    ├────────┼────────┼────────┼───────┤
   klik klasik      │                            ▲ 15,7 ms
   jeda = 1 step    │                  ▲ 11,5 ms
   jeda = 1 frame   │ ▲ 0,0 ms (step pertama, sebelum frame digambar)
```

### 3. v2.0.0: dari mana jeda berasal

| Sumber | Jeda |
| ------ | ---- |
| `Mode Jeda = Auto` (bawaan) | satu step fisika asli: `stepDelta = frameInterval / stepCount` |
| `Mode Jeda = Manual` | nilai `Jeda Manual (ms)` |
| Preset `Vanilla` | `0` (presisi klasik di antara frame) |
| Preset `One Step` | `stepDelta` |
| Preset `Half Frame` | `frameInterval / 2` |
| Preset `Full Frame` | `frameInterval` (semua klik di step 0) |

`Auto` benar-benar adaptif: `stepDelta` sudah mencakup timewarp, kompensasi lag,
dan jumlah step Physics Bypass, sehingga jedanya bukan tebakan 4,17 ms yang
kaku. Semua jeda dibatasi ke `[0, frameInterval]`.

Dengan **Jeda Terpisah** aktif:

```
jeda(input) = dasar × (tekan ? 1 : skalaLepas) × (P2 ? skalaP2 : 1)
```

`skalaLepas = 0%` membuat pelepasan (tahan orb, dash orb) tetap di waktu
sub-frame persisnya; `skalaP2 = 0%` melakukan hal yang sama untuk P2 saat dual.

### 4. Keamanan & diagnostik

Jeda dimatikan saat mod dinonaktifkan, pada mode `Klik per Step`, saat mati,
pause, layar akhir, atau di editor. Overlay debug terlokalisasi menampilkan
`frameInterval` terukur, step tempat klik mendarat, jumlah klik yang pindah ke
step 0, dan akumulasi latensi yang dihemat; angka yang sama dirangkum di layar
akhir.

### 5. Berkas

| Berkas | Peran |
| ------ | ----- |
| `src/before_frame.hpp` | resolusi jeda + transform timestamp |
| `src/i18n.*`, `resources/lang/*.lang` | string EN/ID |
| `src/main.cpp` | `buildStepQueue()`, HUD, layar akhir, pengaturan |
| `tests/test_before_frame.cpp` | 50+ assertion termasuk monotonisitas |
| `docs/HOW_IT_WORKS.md` | dokumen ini |
