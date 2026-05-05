---
schema: airgenome-gamebox/handoff/win32_cycle40_43_bundled_increment_landed/1
completion_iso: 2026-05-04
track: AM
domain: loader_win32
cycles: [40, 41, 42, 43]
sub_tracks: [AM1, AM2, AM3, AM4]
modules_new: 4
modules_extend: 0
loc_total: 1884
fn_surface_delta: 42
fn_groups_total: 13
roadmap_evidence_added: 8
caveat_count: 14
threshold_50pct_breached: true
online_expansion_lockin: {L1: true, L2: true, L3: true, L4: true, L5: true, lockin_iso: 2026-05-03}
policy:
  migration: forbidden
  changes: additive_only
  destructive_ops: 0
  cost_usd: 0
  substrate: mac-local
  cap_minutes: 90
  br_no_user_verbatim: true
  friendly_preset: true
  silent_land: true
---

# §1 Mission paraphrase

Track AM — Win32 cycle 40 ~ 43 bundled 4 sub-cycle skeleton-tier loader land. 4 NEW
loader module 측 시야 baseline coverage 추가, **50% threshold breach milestone**:

- AM1 cycle 40 — `lib/loader/pe_winmm_core.hexa` (WINMM multimedia/timer, 12 fn 4 group) —
  game frame-pacing critical surface
- AM2 cycle 41 — `lib/loader/pe_xaudio2_core.hexa` (XAudio2 game audio engine, 10 fn 3 group) —
  D2R/D4 audio engine prerequisite
- AM3 cycle 42 — `lib/loader/pe_imm32_core.hexa` (IMM32 IME input, 10 fn 3 group) —
  ko-KR/ja-JP/zh-CN composition baseline
- AM4 cycle 43 — `lib/loader/pe_setupapi_core.hexa` (SETUPAPI device installer, 10 fn 3 group) —
  D2R/D4 installer + Battle.net Update Agent baseline

Win32 coverage progression cycle 39 = 176/435 (40.46%) → cycle 43 = 218/435 (50.11%) 누적
+42 fn skeleton-tier delta across 4 modules (Track AM bundled 4-cycle increment).
**50% threshold breached at cycle 43** — first crossing of 50% Win32 fn coverage since cycle
27 baseline (8/435 1.84%).
`.roadmap.loader_win32` cond.1 evidence +3 row + cond.2 evidence +2 row + cond.3 evidence
+3 row = 8 row additive augment, status STAYS at current (partial × 3, full ~435 fn
coverage = multi-cycle long-horizon).

# §2 Bundled 4-cycle table

| cycle | track | module | LOC | bytes | fn | groups | sha256 |
|-------|-------|--------|-----|-------|----|--------|--------|
| 40 | AM1 | lib/loader/pe_winmm_core.hexa | 482 | 26592 | 12 | 4 (timer 4 + wave 4 + midi 2 + mixer 2) | `9c31598d869c5857c559360cb2e8680bdd7041ed3223c13ab100b3da7ca920c1` |
| 41 | AM2 | lib/loader/pe_xaudio2_core.hexa | 470 | 27643 | 10 | 3 (engine 3 + voice 4 + effects 3) | `add6934436339c7adbb8405c51c11d7e102ff40b81aa244a1feafe7c7ccdebe7` |
| 42 | AM3 | lib/loader/pe_imm32_core.hexa | 465 | 25955 | 10 | 3 (context 3 + composition 4 + ime 3) | `565faf396f1cc6028cb18eb6cf0c005152ada4c937de63d85f605bc280fce888` |
| 43 | AM4 | lib/loader/pe_setupapi_core.hexa | 467 | 27200 | 10 | 3 (devinfo 4 + driver 3 + inf 3) | `2d636d807e8b624a29568cff41b256575ed6cd8c4c5916b9dbcd8f056dc9730f` |
| **total** | **AM bundled** | **4 modules** | **1884** | **107390** | **42** | **13 groups** | — |

# §3 Coverage progression — 50% threshold breach

cycle 27 = 8/435 (1.84%) → cycle 28 = 26/435 (5.98%) +18 KERNEL32 (Track O) → cycle 29 =
46/435 (10.57%) +20 USER32 (Q1) → cycle 30 = 61/435 (14.02%) +15 GDI32 (Q2) → cycle 31 =
79/435 (18.16%) +18 ADVAPI32 (Q3) → cycle 32 = 94/435 (21.61%) +15 KERNEL32 EXTEND (W1) →
cycle 33 = 109/435 (25.06%) +15 NTDLL (W2) → cycle 34 = 121/435 (27.82%) +12 OLE32 (W3) →
cycle 35 = 132/435 (30.34%) +11 OLEAUT32 (W4) → cycle 36 = 144/435 (33.10%) +12 MSVCRT
(AH1) → cycle 37 = 156/435 (35.86%) +12 SHELL32 (AH2) → cycle 38 = 166/435 (38.16%) +10
COMCTL32 (AH3) → cycle 39 = 176/435 (40.46%) +10 DXGI (AH4) → cycle 40 = 188/435 (43.22%)
+12 WINMM (AM1) → cycle 41 = 198/435 (45.52%) +10 XAUDIO2 (AM2) → cycle 42 = 208/435
(47.82%) +10 IMM32 (AM3) → **cycle 43 = 218/435 (50.11%) +10 SETUPAPI (AM4) — 50% THRESHOLD
BREACHED**.

Track AM 4-sub-cycle delta = +42 fn (+9.65 pp). 50% milestone significance: **first time
Win32 fn coverage crosses 50% threshold since cycle 27 baseline**, achieved via Track AM4
SETUPAPI 10 fn delta during cycle 43 increment.

# §4 fn surface table per module

## AM1 — WINMM (12 fn) — frame-pacing critical
- timer (4): timeBeginPeriod / timeEndPeriod / timeGetTime / timeSetEvent
- wave (4): waveOutOpen / waveOutWrite / waveOutClose / waveOutGetVolume
- midi (2): midiOutOpen / midiOutClose
- mixer (2): mixerOpen / mixerClose

## AM2 — XAudio2 (10 fn) — D2R/D4 audio engine prerequisite
- engine (3): XAudio2Create / IXAudio2_CreateMasteringVoice / IXAudio2_CreateSourceVoice
- voice (4): IXAudio2SourceVoice_SubmitSourceBuffer / IXAudio2SourceVoice_Start /
  IXAudio2SourceVoice_Stop / IXAudio2SourceVoice_SetVolume
- effects (3): IXAudio2SourceVoice_SetEffectChain /
  IXAudio2SubmixVoice_SetEffectParameters / IXAudio2MasteringVoice_SetVolume

## AM3 — IMM32 (10 fn) — ko/ja/zh IME baseline
- context (3): ImmGetContext / ImmReleaseContext / ImmAssociateContext
- composition (4): ImmGetCompositionStringW / ImmSetCompositionStringW / ImmNotifyIME /
  ImmGetCandidateListW
- ime (3): ImmGetDefaultIMEWnd / ImmIsIME / ImmInstallIMEW

## AM4 — SETUPAPI (10 fn) — installer baseline
- devinfo (4): SetupDiCreateDeviceInfoList / SetupDiDestroyDeviceInfoList /
  SetupDiEnumDeviceInfo / SetupDiGetDeviceRegistryPropertyW
- driver (3): SetupDiBuildDriverInfoList / SetupDiGetDriverInfoDetailW /
  SetupDiCallClassInstaller
- inf (3): SetupOpenInfFileW / SetupGetLineByIndexW / SetupCloseInfFile

# §5 .roadmap.loader_win32 evidence augment

`.roadmap.loader_win32` JSONL header surgical edit (additive_only, evidence array append
only, status field preserved):

| cond_id | status_before | status_after | evidence_before | evidence_after | delta |
|---------|---------------|--------------|-----------------|----------------|-------|
| loader_win32.cond.1 | partial | partial (unchanged) | 24 | 27 | +3 (AM1 WINMM + AM2 XAUDIO2 + cycle 39→41 progression) |
| loader_win32.cond.2 | partial | partial (unchanged) | 11 | 13 | +2 (AM3 IMM32 + UI/IME family cumulative) |
| loader_win32.cond.3 | partial | partial (unchanged) | 12 | 15 | +3 (AM4 SETUPAPI + Track AM bundled cycle 40-43 + 50% threshold breach milestone) |

Total evidence augment = +8 row. blocker_reason refresh on cond.1/2/3 acknowledging Win32
cycle 40-43 bundled increment landed (WINMM + XAudio2 + IMM32 + SETUPAPI) and 50% threshold
breach milestone. Existing evidence rows + blocker_reason text preserved (additive append
only). header line 1-4 comments + blockers array (loader_win32.blk.1 kernel-AC class) +
cross_link section + status active + since 2026-05-03 모두 보존.

# §6 Synthetic struct families

| module | family | byte size | example value |
|--------|--------|-----------|---------------|
| WINMM | WAVEFORMATEX | 18 | wFormatTag 0x0001 PCM, nChannels 2, nSamplesPerSec 44100, nAvgBytesPerSec 176400, nBlockAlign 4, wBitsPerSample 16 |
| WINMM | HWAVEOUT | n/a | 0x94010000 |
| WINMM | HMIDIOUT | n/a | 0x94020000 |
| WINMM | HMIXER | n/a | 0x94030000 |
| WINMM | timer_id | n/a | 0x94040001 (TIME_PERIODIC delay 16ms resolution 1ms) |
| WINMM | timeGetTime DWORD | 4 | 0x12345678 (synth) |
| XAUDIO2 | IID_IXAudio2 GUID LE | 16 | 8BCF1F58-9FE7-4583-8AC6-E2ADC465C8BB |
| XAUDIO2 | XAUDIO2_VOICE_DETAILS | 16 | CreationFlags 0, ActiveFlags 0, InputChannels 2, InputSampleRate 44100 |
| XAUDIO2 | XAUDIO2_BUFFER | 44 | Flags 0, AudioBytes 65536, pAudioData 0x95010100 |
| XAUDIO2 | IXAudio2 handle | n/a | engine 0x95000000 / mastering 0x95010000 / source 0x95020000 / submix 0x95030000 |
| IMM32 | HIMC | n/a | 0x96010000 (input context) / 0x96010100 (associated) |
| IMM32 | COMPOSITIONFORM | 28 | dwStyle 0x02 CFS_POINT, ptCurrentPos (100,200), rcArea (0,0,800,600) |
| IMM32 | composition string WCHAR | var | ko 안녕 4B (0xC548 0xB155) / en test 8B / ja テスト / zh 你好 |
| IMM32 | CANDIDATELIST | 28 | header 28-byte + offsets WORD + 3 candidates |
| IMM32 | HKL | 4 | ko-KR 0x04120412 / ja-JP 0x04110411 / zh-CN 0x04040404 |
| SETUPAPI | HDEVINFO | n/a | 0x97010000 |
| SETUPAPI | SP_DEVINFO_DATA | 32 | cbSize 32, ClassGuid GUID_DEVCLASS_DISPLAY 16-byte 4D36E968 LE, DevInst 0x00000A1B |
| SETUPAPI | SP_DRVINFO_DATA_W | 1606 | DriverType SPDIT_CLASSDRIVER 1, Description "Apple_M_series_Display_Driver", InfFileName display.inf |
| SETUPAPI | HINF | n/a | 0x97020000 |
| SETUPAPI | INFCONTEXT | 24 | Inf 0x97020000, Section "Manufacturer", Line 5 |

# §7 Caveats inline (C1-C14)

- C1. PARTIAL ≠ PASS — own2 silent_error_ban 측 명시적 회피, 4 module 모두 PARTIAL emit
  status 만 (production smoke 측 미수행).
- C2. real timeBeginPeriod / 실 timeGetTime / 실 timeSetEvent = mach_absolute_time / 실
  CADisplayLink / 실 dispatch_source_t timer bridge 미수행 (long-horizon WINMM frame-pacing
  production smoke pending separate cycle).
- C3. real waveOutOpen / 실 waveOutWrite / 실 midiOutOpen / 실 mixerOpen = AudioUnit /
  AudioToolbox / AVAudioEngine / CoreAudio bridge 미수행 (long-horizon WINMM audio
  production smoke pending separate cycle).
- C4. real XAudio2Create / 실 IXAudio2_CreateMasteringVoice / 실 SubmitSourceBuffer = D2R/D4
  audio engine prerequisite, AVAudioEngine + AudioUnit bridge 미수행 (long-horizon XAudio2
  production smoke pending separate cycle, XAudio2 SDK / DirectX SDK / xaudio2redist 0
  binding).
- C5. real ImmGetContext / 실 ImmGetCompositionStringW / 실 ImmNotifyIME = NSTextInputClient
  + NSTextInputContext + IMKInputController + TISInputSource bridge 미수행 (long-horizon
  IMM32 ko/ja/zh IME composition production smoke pending separate cycle).
- C6. real SetupDiEnumDeviceInfo / 실 SetupDiCallClassInstaller / 실 SetupOpenInfFileW =
  IOKit IORegistry + kext loader + INF parser bridge 미수행 (long-horizon SETUPAPI installer
  production smoke pending separate cycle).
- C7. Wine source 인용 0 / ReactOS source 인용 0 / OSS Win32 emulator import 0 / XAudio2 SDK
  binding 0 / DirectX SDK binding 0 / IME service binding 0 — synthetic byte arrays inline
  ONLY (own1 정합).
- C8. macOS libSystem mach_absolute_time / AudioUnit / AVAudioEngine / NSTextInputClient /
  IMKInputController / IOKit / NSObject / 직접 binding 0 (own1 (c) macOS Apple binary
  boundary 정합).
- C9. fn_kind canonical map module 별 다른 max (AM1 0..11 12-fn / AM2 0..9 10-fn / AM3
  0..9 10-fn / AM4 0..9 10-fn) — 본 module 내 sanity assertion only.
- C10. Track AM bundled 4-cycle land in single Track session — handoff doc + marker = single
  pair, 4 cycle entry 별 separate marker 미생성 (per-cycle marker fan-out = 별도 cycle 측
  separate Track territory).
- C11. cond.1 cond.2 cond.3 status partial → partial 유지 (status promotion 미수행, evidence
  array append 만) — 50% threshold breach = coverage milestone NOT status promotion
  criterion; production smoke matrix + 60% threshold matrix + Nt* syscall bridge 실측 = 별도
  cycle.
- C12. L4 D2R single target 측 path 하드코딩 ("C:\\Windows\\System32\\ko_kr_ime.ime" +
  "C:\\Windows\\INF\\display.inf" + "Apple_M_series_Synthetic_Adapter") synth_only — D4
  retired_intentional unchanged.
- C13. L5 J/K/L/M/N/O/P/Q/W/AH parallel BG cross-link untouched — Track AM 측 별도 BG agent
  territory (out-of-scope merge audit).
- C14. raw 9 (hexa-only) + raw 11 (snake_case) + raw 12 (silent_error_ban) + raw 15 (env
  lazy / repo-relative) + raw 175 (BR-NO-USER-VERBATIM) 모두 enforce — 4 module + 1
  roadmap edit + 1 handoff doc + 1 marker 측 0 verbatim user quote, 0 absolute /Users/...
  leak, 0 silent fail.

# §8 Natural environment alignment posture

Track AM continues L2 long-horizon Warden natural-environment-alignment via Win32 syscall
expansion. real Windows binary 측 가장 빈번 호출 surface +4 family 추가:

- WINMM multimedia/timer — timeBeginPeriod 1ms = high precision frame-pacing baseline
  (D2R/D4 game frame-timing prerequisite, 60Hz/120Hz/144Hz refresh sync).
- XAudio2 game audio engine — D2R/D4 audio engine prerequisite (IXAudio2 + voice graph +
  effect chain baseline, PCM 44100 stereo 16bit foundation).
- IMM32 IME input — ko-KR/ja-JP/zh-CN/en-US composition baseline (D2R chat input + Lineage
  한글 입력 baseline + 3-candidate list + composition string lifecycle).
- SETUPAPI device installer — D2R/D4 installer + Battle.net Update Agent + GUID_DEVCLASS_DISPLAY
  + GUID_DEVCLASS_NET enumeration baseline (INF parser + driver detail + class installer).

direct_warden_bypass_vector: none_in_this_cycle. indirect_contribution: fn_naming
baseline +42 fn delta / +210 cumulative delta since cycle 28 / fn_group_classification +13
group / synthetic_struct_shape WAVEFORMATEX+XAUDIO2_BUFFER+VOICE_DETAILS+COMPOSITIONFORM+
CANDIDATELIST+SP_DEVINFO_DATA+SP_DRVINFO_DATA_W+INFCONTEXT+IID_IXAudio2_GUID+HKL+CLASSGUID.

# §9 Predecessors

- `state/markers/airgenome_gamebox_win32_cycle36_39_bundled_increment_landed.marker` (Track
  AH marker, coverage progression schema cycle 27..39 reference)
- `state/markers/airgenome_gamebox_win32_cycle32_35_bundled_increment_landed.marker` (Track
  W marker)
- `state/markers/airgenome_gamebox_user32_gdi32_advapi32_cycle29_31_increment_landed.marker`
  (Track Q marker)
- `state/markers/airgenome_gamebox_kernel32_core_cycle28_increment_landed.marker` (Track O
  marker)
- `lib/loader/pe_dxgi_core.hexa` (Track AH4, mirror style reference)
- `lib/loader/pe_msvcrt_core.hexa` (Track AH1, mirror style reference)

# §10 Files created/modified

| path | kind | LOC | bytes | sha256 |
|------|------|-----|-------|--------|
| lib/loader/pe_winmm_core.hexa | NEW loader | 482 | 26592 | `9c31598d869c5857c559360cb2e8680bdd7041ed3223c13ab100b3da7ca920c1` |
| lib/loader/pe_xaudio2_core.hexa | NEW loader | 470 | 27643 | `add6934436339c7adbb8405c51c11d7e102ff40b81aa244a1feafe7c7ccdebe7` |
| lib/loader/pe_imm32_core.hexa | NEW loader | 465 | 25955 | `565faf396f1cc6028cb18eb6cf0c005152ada4c937de63d85f605bc280fce888` |
| lib/loader/pe_setupapi_core.hexa | NEW loader | 467 | 27200 | `2d636d807e8b624a29568cff41b256575ed6cd8c4c5916b9dbcd8f056dc9730f` |
| .roadmap.loader_win32 | MODIFIED | 5 | 28045 | `a6053999dd4f9f3179235bc9845b471014a9a73cf62f88c17d3bbf0acbd0eb50` |
| airgenome/doc/airgenome_gamebox_win32_cycle40_43_bundled_increment_landed_2026_05_04.ai.md | NEW handoff | self | self | self |
| state/markers/airgenome_gamebox_win32_cycle40_43_bundled_increment_landed.marker | NEW marker | self | self | self |

(self-pinned files: handoff + marker — sha256 not pinned in own body)

# §11 Next cycle actions (long-horizon)

- cycle 44+ WINMM extend — wave_in (waveInOpen/waveInRead/waveInClose) + midi_in
  (midiInOpen/midiInClose) + joystick (joyGetPos/joyGetPosEx/joyGetDevCaps) + extra timer
  (timeGetSystemTime/timeKillEvent) — Track AM1 self-similar pattern (multi-cycle, separate
  Track session).
- cycle 45+ XAudio2 extend — IXAudio2_CommitChanges / IXAudio2SourceVoice_GetState /
  IXAudio2SourceVoice_SetFrequencyRatio / X3DAudio (positional audio) / XAPO custom effect —
  Track AM2 self-similar pattern.
- cycle 46+ IMM32 extend — ImmGetConversionStatus / ImmSetConversionStatus / ImmGetOpenStatus
  / ImmSetOpenStatus / ImmGetCompositionFontW / ImmSetCompositionFontW / ImmGetGuideLineW —
  Track AM3 self-similar pattern.
- cycle 47+ SETUPAPI extend — SetupDiOpenDevRegKey / SetupDiSetDeviceRegistryPropertyW /
  SetupDiGetClassDescription / SetupInstallFromInfSection / CM_Get_Device_ID — Track AM4
  self-similar pattern.
- cycle 50+ D3D11 — D3D11CreateDeviceAndSwapChain + ID3D11Device VFTable + ID3D11DeviceContext
  + ID3D11Texture2D — DXGI prerequisite consumed (D3DMetal binary licence multi-year
  horizon dependent).
- production smoke cycle (multi-cycle, eta 1-2 year+) — 실 timeBeginPeriod = mach_absolute_time
  + dispatch_source_t / 실 waveOutOpen = AudioUnit + AudioToolbox / 실 XAudio2Create =
  AVAudioEngine + AudioUnit / 실 ImmGetContext = NSTextInputClient + IMKInputController /
  실 SetupDiEnumDeviceInfo = IOKit IORegistry / 실 SetupOpenInfFileW = NSFileManager + INF
  parser.
- loader_win32.cond.1 + cond.2 + cond.3 status promotion partial → met = production smoke
  matrix + 60% threshold matrix + Nt* syscall bridge 실측 합산 후 fn-by-fn real-vs-stub
  matrix SSOT 측 land (multi-cycle long-horizon 합산 결과). 50% threshold breach = coverage
  milestone, NOT status promotion criterion.
- 60% threshold target — Track AN/AO/AP cycle 44-50+ 측 IPHLPAPI / WBEM / MSCTF / D3D9 /
  XInput extend / DirectInput cumulative delta ~+60-70 fn → ~280/435 (~64%) target (multi-
  cycle long-horizon, separate Track session).
- Track AM cross-link audit cycle — c_bnet_warden_environment_compat 측 WINMM timer pattern
  / XAudio2 voice graph pattern / IMM32 composition string pattern / SETUPAPI INF parse
  pattern 측 본 Track AM 측 fn coverage SSOT 측 cross-reference (parallel BG agent territory
  merge audit).
- online_expansion_lockin L2 long-horizon Warden compat baseline — KERNEL32 + USER32 +
  GDI32 + ADVAPI32 + NTDLL + OLE32 + OLEAUT32 + MSVCRT + SHELL32 + COMCTL32 + DXGI + WINMM
  + XAudio2 + IMM32 + SETUPAPI syscall sequence match + heap pattern match + TEB/PEB shape
  match + sync primitive pattern + COM apartment pattern + IDispatch automation pattern +
  CRT runtime pattern + Shell APIs pattern + Common Controls pattern + DXGI swapchain
  pattern + WINMM timer/audio pattern + XAudio2 voice graph pattern + IMM32 composition
  pattern + SETUPAPI INF/devinfo pattern 측 multi-year roadmap 측 milestone breakdown
  (cycle 100+ 합산 결과).
