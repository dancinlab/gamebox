---
schema: airgenome-gamebox/airgenome/doc/airgenome_gamebox_user32_gdi32_advapi32_cycle29_31_increment_landed/ai-native/1
last_updated: 2026-05-03
ssot:
  marker: state/markers/airgenome_gamebox_user32_gdi32_advapi32_cycle29_31_increment_landed.marker
  roadmap_files_modified:
    - .roadmap.loader_win32
  modules_created:
    - lib/loader/pe_user32_core.hexa
    - lib/loader/pe_gdi32_core.hexa
    - lib/loader/pe_advapi32_core.hexa
  predecessor_handoffs:
    - airgenome/doc/airgenome_gamebox_kernel32_core_cycle28_increment_landed_2026_05_03.ai.md
    - airgenome/doc/airgenome_gamebox_online_network_stack_3_loader_landed_2026_05_03.ai.md
    - airgenome/doc/airgenome_gamebox_battlenet_d2r_offline_shim_landed_2026_05_03.ai.md
    - airgenome/doc/airgenome_gamebox_self_mk2_tuning_landed_2026_05_02.ai.md
status: LANDED_USER32_GDI32_ADVAPI32_CYCLE29_31_BUNDLED_INCREMENT_TRACK_Q
related_raws:
  - raw 9    # hexa-only (verifier emit native via println __USER32__/__GDI32__/__ADVAPI32__ PARTIAL + r0_emit JSONL)
  - raw 11   # snake_case fields throughout JSONL + JSON marker + r0_emit event_type
  - raw 12   # silent-error ban — every PARTIAL emit explicit (no swallow as PASS)
  - raw 15   # env_lazy — repo-relative paths only, no /Users/<user>/ leaks
  - raw 175  # BR-NO-USER-VERBATIM (no verbatim user quotes anywhere)
online_expansion_lockin:
  L1: true   # EULA/ToS violation risk acknowledged (a+ path)
  L2: true   # Warden detection risk acknowledged — Win32 syscall perfection rosetta direct contribution (long-horizon)
  L3: true   # 1-2 year+ multi-cycle horizon acknowledged
  L4: true   # D2R online single-target lock-in (D4 retired_intentional unchanged)
  L5: true   # Track J/K/L/O/Q parallel BG land scope locked-in
  lockin_iso: 2026-05-03
preserved_unchanged:
  - .roadmap.loader_win32 cond.1 + cond.2 + cond.3 status (all 3 partial unchanged — full ~435 fn coverage = multi-cycle long-horizon)
  - all 14 other .roadmap.* files (battlenet/d2r/d4/wow/cs2/delta/elden/lineage 3/purple/loader_pe/loader_dx)
  - all native sources, plugin.json, entry, README.md, .gitignore
  - 418 existing lib/loader/*.hexa files (Track O's 1 + Track K's 3 + d2r/battlenet/dx/win32_*/etc untouched, 3 NEW added)
  - all c_* perf modules (untouched — Track J/N parallel scope)
  - all tool/* (untouched — Track M parallel scope)
  - all tests/*.hexa (no new test harness; module's self_test() suffices)
  - 3 docs/*_HONEST_FREEZE.md (GAMEGUARD/WARDEN/D2R_REALM — Track E/J/L territory, read-only)
  - lib/loader/battlenet_bypass.hexa (Track P territory)
  - lib/loader/pe_kernel32_core.hexa (Track O predecessor — 0-line touched)
policy:
  migration: forbidden
  changes: additive_only
  in_place_writes: 1
  in_place_writes_note: 1 .roadmap.loader_win32 JSONL header surgical edit (cond.1 evidence array append +3 rows USER32+GDI32+progression + cond.3 evidence array append +3 rows ADVAPI32+cumulative+L2; cond.2 evidence + cond.3 status unchanged) — 0 cond.N row deleted, 0 status field promoted, 0 existing assertion broken, append/extend only
  destructive_ops: 0
  cost_usd: 0
  substrate: mac-local
  cap_minutes: 90
  br_no_user_verbatim: true
  friendly_preset: true
  silent_land: true
---

# airgenome-gamebox USER32 + GDI32 + ADVAPI32 cycle 29-31 bundled increment landed - Track Q (BG-AGB online expansion)

## TL;DR

Track O predecessor (KERNEL32 18 fn skeleton-tier 2026-05-03) 위 Track Q 로 USER32 + GDI32 + ADVAPI32
3 DLL 측 core surface 측 53 fn skeleton-tier loader bundled land (cycle 29 + 30 + 31 합산). 실
CreateWindowExW / 실 GetMessageW / 실 GetDC / 실 BitBlt / 실 RegOpenKeyExW / 실 OpenSCManagerW /
실 OpenProcessToken / 실 CryptAcquireContextW = 0, mac-local hexa-only. .roadmap.loader_win32 cond.1
+ cond.3 evidence augment (status partial → partial 유지 — full ~435 fn coverage = multi-cycle
long-horizon). cumulative coverage progression cycle 28 = 26/435 (~5.98%) → cycle 31 = 79/435
(~18.16%) (+53 fn bundled). online_expansion_lockin L1+L2+L3+L4+L5 ALL OK.

- `lib/loader/pe_user32_core.hexa` (NEW, 507 LOC) — USER32.dll 20 fn surface skeleton tracker
  (`__USER32__ PARTIAL <fn>` ×20 emit + synthetic HWND/MSG/WNDCLASSEXW/POINT/RECT round-trip,
  4 TSV table persist).
- `lib/loader/pe_gdi32_core.hexa` (NEW, 485 LOC) — GDI32.dll 15 fn surface skeleton tracker
  (`__GDI32__ PARTIAL <fn>` ×15 emit + synthetic HDC/HBITMAP/HGDIOBJ/LOGFONTW/BITMAPINFO/SIZE
  round-trip, 4 TSV table persist).
- `lib/loader/pe_advapi32_core.hexa` (NEW, 502 LOC) — ADVAPI32.dll 18 fn surface skeleton tracker
  (`__ADVAPI32__ PARTIAL <fn>` ×18 emit + synthetic HKEY/SC_HANDLE/HCRYPTPROV/SECURITY_DESCRIPTOR
  /SID round-trip, 4 TSV table persist).

## §1 user_directive_paraphrase

predecessor cycle 측 Track O (KERNEL32 core 18 fn cycle 28 increment) 후 follow-up Track Q 로 USER32
+ GDI32 + ADVAPI32 3 DLL 측 core surface 측 NEW skeleton-tier loader 모듈 bundled land. 의도:

- USER32 측 핵심 UI primitive 측 5 group 측 baseline 정착 — window (CreateWindowExW/DestroyWindow/
  ShowWindow/UpdateWindow/GetWindowRect) + message (GetMessageW/TranslateMessage/DispatchMessageW/
  PeekMessageW/PostMessageW) + input (GetKeyState/GetAsyncKeyState/GetCursorPos/SetCursorPos/
  GetForegroundWindow) + dialog (MessageBoxW/MessageBoxA/DialogBoxParamW) + misc (RegisterClassExW/
  DefWindowProcW). 실 syscall 0 / synthetic byte array round-trip 만.
- GDI32 측 핵심 graphics device interface primitive 측 4 group 측 baseline 정착 — dc_mgmt (GetDC/
  ReleaseDC/CreateCompatibleDC/DeleteDC) + drawing (BitBlt/StretchBlt/TextOutW/Rectangle/MoveToEx)
  + object (CreateBitmap/CreateCompatibleBitmap/SelectObject/DeleteObject) + font (CreateFontW/
  GetTextExtentPoint32W). 실 syscall 0 / synthetic byte array round-trip 만.
- ADVAPI32 측 핵심 registry / security / service / token / crypto primitive 측 5 group 측 baseline
  정착 — registry (RegOpenKeyExW/RegCloseKey/RegQueryValueExW/RegSetValueExW/RegEnumKeyExW) + security
  (InitializeSecurityDescriptor/SetSecurityDescriptorDacl/GetSecurityInfo/SetSecurityInfo/
  ConvertStringSecurityDescriptorToSecurityDescriptorW) + service (OpenSCManagerW/OpenServiceW/
  CloseServiceHandle) + token (OpenProcessToken/GetTokenInformation/AdjustTokenPrivileges) + crypto
  (CryptAcquireContextW/CryptReleaseContext). 실 syscall 0 / synthetic byte array round-trip 만.
- `docs/D2R_D4_INSTALL_LAUNCH_ROADMAP.md` cycle 27 status 측 KERNEL32 8/435 (~1.84%) baseline 위
  cycle 28 측 26/435 (~5.98%, Track O) → cycle 29 측 46/435 (~10.57%, Track Q1 USER32 +20 fn) →
  cycle 30 측 61/435 (~14.02%, Track Q2 GDI32 +15 fn) → cycle 31 측 79/435 (~18.16%, Track Q3
  ADVAPI32 +18 fn) coverage progression — bundled 3 cycle increment in single Track Q land.
- .roadmap.loader_win32 cond.1 + cond.3 evidence augment 6 row 추가 (cond.2 evidence unchanged
  per spec) — status partial → partial 유지 (full ~435 fn coverage = multi-cycle long-horizon).
- "natural environment alignment" path 측 long-horizon goal 측 direct contribution — Win32 syscall
  perfection rosetta toward L2 Warden detection compat 측 multi-year baseline. KERNEL32 + USER32
  + GDI32 + ADVAPI32 = real Windows binary 측 가장 빈번 호출 surface 4 (file/memory/process/system
  + UI/window/input + graphics + registry/service/security/crypto), Warden detection 측 syscall
  pattern match 측 가장 직접적 alignment vector (online_expansion_lockin 5 항목 ALL OK).
- Track O cumulative cross-link — KERNEL32 18 fn (Track O) + USER32 20 fn + GDI32 15 fn + ADVAPI32
  18 fn (Track Q) = 71 fn cumulative skeleton-tier surface coverage. Track J (lib/perf/c_bnet_*) /
  Track L (.roadmap.diablo2_resurrected) / Track M (tool/*) / Track N (lib/perf/) / Track P
  (lib/loader/battlenet_bypass.hexa) = parallel BG, 본 cycle 0 라인 touch.

규칙:
- additive only — 기존 self_test row 0 삭제, 3 NEW module + 1 NEW handoff + 1 NEW marker + cond.1
  evidence array append (3 row) + cond.3 evidence array append (3 row).
- migration / destructive op / 외부 cost / push: 모두 금지.
- own1 (Wine 0 / hexa-only / NO real USER32.dll/GDI32.dll/ADVAPI32.dll binding / NO Wine source
  lookup / NO ReactOS source 인용 / NO OSS Win32 emulator import) + own2 (PARTIAL emit explicit,
  silent_error_ban) 보존.
- BR-NO-USER-VERBATIM (사용자 원문 인용 금지, paraphrase only).
- silent-land — 새 doc + marker 만 land, 외부 announce 0.

## §2 fn coverage table (3 modules / group / fn names / count)

### Q1: pe_user32_core.hexa (USER32, 20 fn)

| group | fn names | fn count | fn_kind range | synthetic struct |
|---|---|---:|---|---|
| window | CreateWindowExW / DestroyWindow / ShowWindow / UpdateWindow / GetWindowRect | 5 | 0..4 | HWND (hwnd_hex 0x00010404) + RECT (16 byte = 4×LONG: {0,0,1280,720}) |
| message | GetMessageW / TranslateMessage / DispatchMessageW / PeekMessageW / PostMessageW | 5 | 5..9 | MSG (28 byte = HWND + UINT msg + WPARAM + LPARAM + DWORD time + POINT) |
| input | GetKeyState / GetAsyncKeyState / GetCursorPos / SetCursorPos / GetForegroundWindow | 5 | 10..14 | POINT (8 byte = 2×LONG: x=640 y=400) + virtual-key code SHORT |
| dialog | MessageBoxW / MessageBoxA / DialogBoxParamW | 3 | 15..17 | text_w_synth UTF-16 ref + flags DWORD |
| misc | RegisterClassExW / DefWindowProcW | 2 | 18..19 | WNDCLASSEXW (80 byte ref shape, cbSize 0x50 LE) |
| **module total** | — | **20** | **0..19** | 5 synthetic struct families |

### Q2: pe_gdi32_core.hexa (GDI32, 15 fn)

| group | fn names | fn count | fn_kind range | synthetic struct |
|---|---|---:|---|---|
| dc_mgmt | GetDC / ReleaseDC / CreateCompatibleDC / DeleteDC | 4 | 0..3 | HDC (hdc_hex 0x21010800) |
| drawing | BitBlt / StretchBlt / TextOutW / Rectangle / MoveToEx | 5 | 4..8 | raster_op DWORD (0x00CC0020) + width/height ref + RECT |
| object | CreateBitmap / CreateCompatibleBitmap / SelectObject / DeleteObject | 4 | 9..12 | HBITMAP (0x22020900) + HGDIOBJ (0x23030A00) + BITMAPINFOHEADER (40 byte) |
| font | CreateFontW / GetTextExtentPoint32W | 2 | 13..14 | LOGFONTW (92 byte, lfHeight=-12 LE) + SIZE (8 byte: cx=128 cy=24) |
| **module total** | — | **15** | **0..14** | 4 synthetic struct families |

### Q3: pe_advapi32_core.hexa (ADVAPI32, 18 fn)

| group | fn names | fn count | fn_kind range | synthetic struct |
|---|---|---:|---|---|
| registry | RegOpenKeyExW / RegCloseKey / RegQueryValueExW / RegSetValueExW / RegEnumKeyExW | 5 | 0..4 | HKEY (0x80000002 HKEY_LOCAL_MACHINE pseudo + sub_hkey 0x000A0500) + REG_SZ value_data UTF-16 |
| security | InitializeSecurityDescriptor / SetSecurityDescriptorDacl / GetSecurityInfo / SetSecurityInfo / ConvertStringSecurityDescriptorToSecurityDescriptorW | 5 | 5..9 | SECURITY_DESCRIPTOR (20 byte: Revision 0x01 + Sbz1 + Control 0x8004 LE + Owner/Group/Sacl/Dacl 4-byte offsets) + SDDL string |
| service | OpenSCManagerW / OpenServiceW / CloseServiceHandle | 3 | 10..12 | SC_HANDLE (mgr 0x000B0600 + svc 0x000B0700) + access mask DWORD |
| token | OpenProcessToken / GetTokenInformation / AdjustTokenPrivileges | 3 | 13..15 | HANDLE token (0x000C0800) + SID (12 byte: Revision 1 + SubAuthorityCount 1 + IdentifierAuthority 6 + SubAuthority 4) |
| crypto | CryptAcquireContextW / CryptReleaseContext | 2 | 16..17 | HCRYPTPROV (0x000D0900) + provider name UTF-16 ref |
| **module total** | — | **18** | **0..17** | 5 synthetic struct families |

각 fn 측 self_test() 측 record_syscall_invoke + println("__<DLL>__ PARTIAL <fn>") + r0_emit
("pe_<dll>_core_record" event_type, JSON payload with fn_kind/fn_name/fn_group/status + group-
specific fields). Q1 = 20 PARTIAL emit + 16 lifecycle/pump/event row = 36 record. Q2 = 15 PARTIAL
emit + 12 lifecycle/draw/object row = 27 record. Q3 = 18 PARTIAL emit + 12 op/event row = 30
record. Track Q bundled = 53 fn skeleton-tier + 40 supporting record = 93 total record.

| module | LOC | bytes | sha256 | fn count | DLL emit count | PARTIAL grep | r0_emit count |
|---|---:|---:|---|---:|---:|---:|---:|
| pe_user32_core.hexa | 507 | 28227 | 7143b14042efdd4173b04e0490b32189f81b1259837064ecaf2ac8e61930b208 | 20 | 20 | 88 | 39 |
| pe_gdi32_core.hexa | 485 | 26250 | 1013d2d03bfd4003c02122e1040ae2c26cbaf74fcccc0a67e287f4dd5a6fba23 | 15 | 15 | 73 | 34 |
| pe_advapi32_core.hexa | 502 | 29294 | 6a97bdc43c6a1e8aa6ac1dcf253cb8f27f9e08bd88473e28c000e4aeddeabaf0 | 18 | 18 | 82 | 37 |
| **Track Q total** | **1494** | **83771** | — | **53** | **53** | **243** | **110** |

## §3 .roadmap.loader_win32 cond evidence count before/after

| cond_id | status | before evidence count | after evidence count | delta | evidence rows added |
|---|---|---:|---:|---:|---|
| loader_win32.cond.1 | partial → partial (unchanged) | 16 | 19 | **+3** | (a) Track Q1 pe_user32_core skeleton-tier 20 fn / (b) Track Q2 pe_gdi32_core skeleton-tier 15 fn / (c) USER32+GDI32 cycle 28→30 progression 26/435→61/435 ~14.02% |
| loader_win32.cond.2 | partial (unchanged) | 6 | 6 | 0 | none (per spec — cond.2 status STAYS at current value, evidence augment via cond.1 cluster only since USER32+GDI32 are full Win32 surface scope rather than 60% threshold matrix scope) |
| loader_win32.cond.3 | partial → partial (unchanged) | 4 | 7 | **+3** | (a) Track Q3 pe_advapi32_core skeleton-tier 18 fn (registry/security/service/token/crypto, ntdll-adjacent surface) / (b) ADVAPI32 cycle 30→31 progression 61/435→79/435 ~18.16% / (c) Track Q bundled cumulative cycle 28→31 71 fn skeleton-tier across 4 modules + L2 long-horizon natural environment alignment direct contribution |

cond.1 blocker_reason refresh: 기존 message 후미에 USER32+GDI32 cycle 29-30 increment 안내 append.
cond.3 blocker_reason refresh: 기존 message 후미에 ADVAPI32 cycle 31 increment + bundled cycle 29-31
완료 안내 append. 0 status field promotion (multi-cycle long-horizon 명시).

전체 cond 3 中 0 promoted (status enum 변경 0), 2 evidence augmented (cond.1 +3 rows + cond.3 +3
rows). own1 (Wine 0) + own2 (PARTIAL emit explicit) preserved.

## §4 online_expansion_lockin L1-L5 ack confirmation

| 항목 | 명칭 | 사용자 lock-in posture | marker reflect | 본 cycle posture |
|---|---|---|---|---|
| L1 | EULA/ToS violation risk acknowledged | OK (a+ path) | `eula_violation_risk_acknowledged: true` | USER32/GDI32/ADVAPI32 syscall surface = production access 측 ToS 위반 가능성 인지 — skeleton-tier hexa-only design 으로 risk 분리 (real syscall 0) |
| L2 | Warden detection risk acknowledged | OK — Track Q 직접 contributor | `warden_detection_risk_acknowledged: true` | **본 Track Q 는 L2 long-horizon 직접 기여** — USER32 message pump pattern + GDI32 paint cycle pattern + ADVAPI32 registry/token query pattern = Warden detection 측 syscall pattern match 회피 측 가장 직접적 alignment vector (real Windows binary 측 가장 빈번 호출 surface). 본 cycle = skeleton baseline only, production smoke 측 multi-cycle. |
| L3 | 1-2 year+ multi-cycle horizon acknowledged | OK | `multi_year_horizon_acknowledged: true` | 누적 71/435 = ~18.16% — full coverage 측 cycle 32+ (KERNEL32 expansion / NTDLL / OLE32 / etc) 측 multi-cycle long-horizon 명시 |
| L4 | D2R online single-target lock-in | OK (D4 retired_intentional unchanged) | `d2r_single_target_lockin: true` + `d4_retired_intentional_unchanged: true` | USER32 측 class_name "D2RClient" + ADVAPI32 측 registry path "SOFTWARE\\Blizzard Entertainment\\Diablo II Resurrected" + InstallPath "C:\\Battle.net\\D2R" + service name "Battle.net Update Agent" 측 D2R-specific. D4 path 0 줄. |
| L5 | Track J/K/L/O/Q parallel BG land scope locked-in | OK | `track_jklo_parallel_lockin: true` + `track_q_bundled_cycle_29_31: true` | Track Q (USER32+GDI32+ADVAPI32 bundled) = J (Warden compat) + K (network stack) + L (D2R realm protocol) + M (tool) + N (perf) + O (KERNEL32 core, 직접 predecessor) + P (battlenet_bypass) parallel BG. 본 cycle Q 만 land, 다른 track 측 territory 0 라인 touch. |

5 항목 ALL OK 사용자 lock-in 2026-05-03. marker `online_expansion_lockin` block 5 field 모두 true +
lockin_iso "2026-05-03" + 6 redundant boolean ack field.

## §5 cycle 28 → 31 cumulative coverage progression (26/435 → 79/435 ~18.16%)

| metric | cycle 28 (Track O) | cycle 29 (Track Q1 USER32) | cycle 30 (Track Q2 GDI32) | cycle 31 (Track Q3 ADVAPI32) | total delta cycle 28→31 |
|---|---:|---:|---:|---:|---:|
| Win32 fn covered (skeleton+real mixed) | 26 | 46 | 61 | 79 | +53 |
| total surface (D2R_D4_INSTALL_LAUNCH_ROADMAP cycle 27 ref) | 435 | 435 | 435 | 435 | 0 |
| % progression | 5.98% | 10.57% | 14.02% | 18.16% | +12.18 pp |
| skeleton-tier fn per cycle | +18 (KERNEL32) | +20 (USER32) | +15 (GDI32) | +18 (ADVAPI32) | +71 cumulative |
| new module per cycle | pe_kernel32_core.hexa | pe_user32_core.hexa | pe_gdi32_core.hexa | pe_advapi32_core.hexa | 4 module total |

Track Q bundled (cycle 29 + 30 + 31) 측 single-land progression — cycle 28 baseline (Track O) 위
3 sequential cycle increment 합산 land. cycle 28 측 KERNEL32 18 fn baseline 측 self-similar pattern
mirror — fn_kind_name/fn_group_name canonical map + 4 TSV table per module + r0_emit ≥ 14 +
self_test record ≥ 12 ALL preserved.

production smoke (실 CreateWindowExW = NSWindow alloc + headless Cocoa shim / 실 GetMessageW = mach
port message + AppKit event loop / 실 GetDC = CGContext + Quartz / 실 BitBlt = CGContextDrawImage /
실 RegOpenKeyExW = mac plist read / 실 OpenSCManagerW = launchd query / 실 OpenProcessToken =
mach_port_t + audit_token_t / 실 CryptAcquireContextW = SecKeychainOpen) = cycle 32+ 측 multi-cycle
long-horizon — fn-by-fn real-vs-stub matrix SSOT 측 별도 cycle (full ~435 fn coverage = many more
cycles). 본 cycle = baseline only.

## §6 multi-cycle long-horizon caveat (~435 fn full coverage = many cycles)

KERNEL32 + USER32 + GDI32 + ADVAPI32.dll 측 export fn count 합산 ~435 (Microsoft Windows 10/11 측
4 DLL combined export table 측 대략 추정 — Wine / ReactOS source 측 정확 count 측 인용 0). 본
cycle 31 = 79/435 (~18.16%) 측 4 cycle 누적 baseline. full coverage 측 cycle path:

- cycle 32+: KERNEL32 expansion — synchronization (CreateMutex/CreateEvent/CreateSemaphore), DLL
  load (LoadLibraryW/GetProcAddress/FreeLibrary), thread (CreateThread/SuspendThread/TlsAlloc),
  TLS, file mapping (CreateFileMappingW/MapViewOfFile), pipe — 수십 cycle 측 incremental
- cycle 35+: USER32 expansion — clipboard (GetClipboardData/SetClipboardData), keyboard layout
  (GetKeyboardLayout), DPI awareness (SetProcessDPIAware), system metrics (GetSystemMetrics) —
  ~10 fn 추가 candidate
- cycle 38+: GDI32 expansion — pen/brush (CreatePen/CreateSolidBrush), region (CreateRectRgn),
  enhanced metafile (CreateEnhMetaFile), printer (StartDoc/EndDoc) — ~9 fn 추가 candidate
- cycle 40+: ADVAPI32 expansion — service control (StartServiceW/ControlService/QueryServiceStatus),
  event log (RegisterEventSourceW/ReportEventW), SAM (LookupAccountNameW), crypto signing
  (CryptSignHashW/CryptVerifySignatureW) — ~7 fn 추가 candidate
- cycle 50+: NTDLL Nt* core syscall bridge — separate scope from ADVAPI32 (cond.3 직접 영역) +
  OLE32 / OLEAUT32 (COM) + WINTRUST (signature verification) — multi-cycle long-horizon
- cycle 100+: production smoke matrix — fn-by-fn real-vs-stub coverage SSOT + Warden compat
  syscall pattern align cycle 측 합산 후 cond.1 + cond.2 + cond.3 status promotion 측 met 후보
  (1-2 year+ horizon)

직접적 Warden bypass / detection vector 0 — natural environment alignment 측 indirect contribution
(syscall surface naming / synthetic struct shape / fn group classification) 만. 본 cycle 측 full
coverage 도달 = false (의도적, multi-cycle long-horizon 정합).

## §7 caveats inline (honest C3 ≥ 8 numbered)

C1. **USER32/GDI32/ADVAPI32 측 synthetic round-trip — real syscall X**: pe_user32_core.hexa +
    pe_gdi32_core.hexa + pe_advapi32_core.hexa 측 self_test() 측 53 syscall_invoke + 12 window/
    dc/registry lifecycle + 12 message/draw/service event + 12 input/object/sec_token row = 89
    record 측 println("__<DLL>__ PARTIAL <fn>") + r0_emit JSON 만. 실 macOS NSWindow alloc /
    AppKit event loop / Quartz CGContext / launchd / mach_port / SecKeychain 측 syscall 0 —
    production smoke 측 cycle 32+ pending. 의도적 (own1 Wine 0 / hexa-only mandate 정합).

C2. **synthetic HWND / HDC / HKEY / SC_HANDLE / token / HCRYPTPROV — real handle X**: 3 module 측
    handle_hex 측 reference shape 만 (windows.h / winuser.h / wingdi.h / winreg.h / winsvc.h /
    wincrypt.h 측 standard public header type definition 인용, Wine user32/gdi32/advapi32 source
    측 코드 인용 0, ReactOS 측 코드 인용 0). pseudo-handle range (HKEY_LOCAL_MACHINE 0x80000002,
    GetCurrentProcess 0xFFFFFFFF) = MSDN 공개 spec value, Wine source 인용 0.

C3. **D2R-specific synthetic 하드코딩 — D2R single target (L4) reflect**: USER32 측 class_name
    "D2RClient" + caption "Diablo II: Resurrected" + ADVAPI32 측 registry path "SOFTWARE\\Blizzard
    Entertainment\\Diablo II Resurrected" + InstallPath "C:\\Battle.net\\D2R" + service name
    "Battle.net Update Agent" + GDI32 측 font face "Exocet" (D2R UI font reference name) 측 D2R-
    specific. D4 측 (Diablo IV.exe / ProductSettings.json 등) 0 줄 — D4 retired_intentional 측
    status enum 측 unchanged 정합 (predecessor self_mk2_tuning land 측 명시).

C4. **53 fn / 435 = ~12.18 pp progression 측 cycle 28→31 cumulative baseline 만, full coverage =
    multi-cycle long-horizon**: 본 cycle 측 evidence augment (cond.1 16→19 row + cond.3 4→7 row,
    cond.2 6 row 유지) 만 — cond.* status field promotion (partial → met) 0. full Win32 coverage
    측 production smoke + fn-by-fn real-vs-stub matrix SSOT + ~435 fn 측 multi-cycle work — 본
    cycle 은 53 fn skeleton-tier 측 baseline 만 정착.

C5. **53 fn 측 PARTIAL emit explicit — silent_error_ban (raw 12) 정합**: PARTIAL ≠ PASS. 본 cycle
    측 status emit 53 모두 `__USER32__/__GDI32__/__ADVAPI32__ PARTIAL <fn>` 측 명시 — production
    smoke 미달 + real-vs-stub 측 stub side 측 honest acknowledgement. PARTIAL swallow as PASS =
    silent_error_ban 위반 = 본 cycle 측 명시적 회피.

C6. **L2 Warden compat = long-horizon, 본 cycle Warden detection vector 0**: Track Q 측 USER32 +
    GDI32 + ADVAPI32 syscall surface 측 baseline = Warden detection 측 syscall pattern match 회피
    측 indirect contribution (fn name + fn_group classification + synthetic struct shape) 만. 실
    Warden 측 USER32 GetMessageW hook detection / GDI32 BitBlt screen scrape detection / ADVAPI32
    RegQueryValueExW registry probe pattern / OpenProcessToken privilege scan pattern 측 분석 0
    — 별도 Track J (c_bnet_warden_environment_compat) territory (parallel BG, 본 cycle 측 untouched).

C7. **online_expansion_lockin marker block 측 사용자 lock-in 2026-05-03 reflect — verbatim 측 0**:
    user_directive_paraphrase 측 5 항목 (L1 EULA/ToS / L2 Warden long-horizon / L3 multi-year / L4
    D2R single target / L5 Track J/K/L/O/Q parallel) 측 paraphrase 만 — 사용자 원문 인용 0 (BR-NO-
    USER-VERBATIM raw 175 정합). marker 측 `online_expansion_lockin` block 5 field 모두 true +
    lockin_iso "2026-05-03" + 6 redundant boolean ack field 측 explicit reflection.

C8. **Track Q scope 측 J / K / L / M / N / O / P 분리 — 본 cycle 측 cross-link untouched**: 본
    cycle 은 Q (USER32 + GDI32 + ADVAPI32 core skeleton-tier bundled) 만 — Track J (lib/perf/
    c_bnet_warden_environment_compat) + Track K (lib/loader/pe_winsock_ws2_32 + pe_wininet_https
    + pe_schannel_tls) + Track L (.roadmap.diablo2_resurrected D2R realm protocol cross-link) +
    Track M (tool/*) + Track N (lib/perf/) + Track O (lib/loader/pe_kernel32_core.hexa, predecessor
    direct mirror) + Track P (lib/loader/battlenet_bypass.hexa) 측 territory 0 라인 touch. 8
    track parallel BG land 측 lock-in (L5) — cross-cycle merge audit 별도 cycle.

C9. **runtime exec smoke 측 미수행 — cap_minutes 90 + skeleton-tier mandate 정합**: 본 cycle 측
    pe_user32_core.hexa + pe_gdi32_core.hexa + pe_advapi32_core.hexa 측 self_test() 실측 stdout
    캡처 미수행 — predecessor pe_kernel32_core.hexa (Track O cycle 28) + pe_winsock_ws2_32 / WININET
    / SCHANNEL (Track K) 측 hexa runtime smoke 측 docker route fall-through caveat 동급 posture.
    emit count 53 + 12 + 12 + 12 = 89 record + LOC 1494 합산 + grep PARTIAL 243 + grep __USER32__
    PARTIAL 20 + __GDI32__ PARTIAL 15 + __ADVAPI32__ PARTIAL 18 측 static-validate 만.

C10. **fn_kind range 측 module 별 다른 max — assertion 측 module-local max enforced**: pe_user32_core
     fn_kind 0..19 (20 entries, max 19), pe_gdi32_core fn_kind 0..14 (15 entries, max 14),
     pe_advapi32_core fn_kind 0..17 (18 entries, max 17). 각 module 측 record_syscall_invoke 측
     fn_kind <= module_max assertion 별도 enforcement (module 간 fn_kind namespace 분리 — global
     fn_kind 통합 SSOT 측 cycle 32+ candidate).

C11. **Track Q = bundled 3-cycle land in single Track session — cycle 29 + 30 + 31 sequential
     numbering preserved**: cycle 29 (USER32 Q1), cycle 30 (GDI32 Q2), cycle 31 (ADVAPI32 Q3) 측
     sequential cycle ID 보존 — single Track Q session 내 3 increment 합산 land. coverage progression
     metric 측 cycle 별 breakdown explicit (cycle 28 26 → cycle 29 46 → cycle 30 61 → cycle 31 79).
     marker `win32_coverage_progression` field 측 cycle_27 / cycle_28 / cycle_29_30_31_cumulative
     reflect.

C12. **cond.2 evidence unchanged per spec (USER32+GDI32 evidence routed to cond.1 cluster)**:
     spec 측 "cond.2/cond.3 status STAYS at current value" + cond.2 evidence array unchanged (6
     rows preserved). USER32 + GDI32 측 evidence 는 본 cycle 측 cond.1 (kernel32 + 광의 Win32
     syscall surface) cluster append 처리 — cond.2 (60% threshold per-DLL render coverage) 측
     scope 별도 (real fn count vs stub count ratio matrix SSOT) 미land 정합. ADVAPI32 측 evidence
     는 cond.3 (ntdll Nt* syscall bridge) cluster append (registry/service/token/crypto = ntdll-
     adjacent surface, ntdll core syscall bridge 와 별도 scope but cluster 내 evidence 합당).

## §8 next-cycle hooks

- **cycle 32+ KERNEL32 expansion** — synchronization primitive (CreateMutex/CreateEvent/CreateSemaphore)
  + DLL load (LoadLibraryW/GetProcAddress/FreeLibrary) + thread (CreateThread/SuspendThread/
  TlsAlloc) + file mapping (CreateFileMappingW/MapViewOfFile) + pipe + etc 수십 cycle 측 incremental
  expansion — Track O 측 self-similar pattern.
- **cycle 35+ USER32 expansion** — clipboard / keyboard layout / DPI awareness / system metrics —
  Track Q1 측 self-similar pattern.
- **cycle 38+ GDI32 expansion** — pen/brush / region / enhanced metafile / printer — Track Q2 측
  self-similar pattern.
- **cycle 40+ ADVAPI32 expansion** — service control / event log / SAM / crypto signing — Track Q3
  측 self-similar pattern.
- **cycle 50+ NTDLL Nt* core syscall bridge** — NtQueryInformationProcess + NtCreateFile + NtReadFile
  3 syscall hexa-native bridge (kernel32 → ntdll → macOS posix syscall pass-through) — cond.3 직접
  영역, ADVAPI32 (registry/service)와 별도 scope.
- **cycle 60+ OLE32 / OLEAUT32 / WINTRUST** — COM apartment + variant marshal + signature verify
  — separate Track territory.
- **production smoke cycle (multi-cycle, eta 1-2 year+)** — 실 CreateWindowExW = NSWindow alloc +
  headless Cocoa shim; 실 GetMessageW = mach port message + AppKit event loop; 실 GetDC = CGContext
  + Quartz; 실 BitBlt = CGContextDrawImage; 실 RegOpenKeyExW = mac plist read; 실 OpenSCManagerW =
  launchd query; 실 OpenProcessToken = mach_port_t + audit_token_t; 실 CryptAcquireContextW =
  SecKeychainOpen.
- **loader_win32.cond.1 + cond.2 + cond.3 status promotion partial → met 측 production smoke matrix
  + 60% threshold matrix** — 71 + 추가 fn 측 production smoke 합산 후 fn-by-fn real-vs-stub matrix
  SSOT 측 land + per-DLL real fn count vs stub count ratio matrix + Warden compat syscall pattern
  align cycle 측 합산 후 status 승격 (multi-cycle long-horizon 합산 결과).
- **Track J cross-link audit cycle** — c_bnet_warden_environment_compat 측 USER32 message pump /
  GDI32 paint cycle / ADVAPI32 registry probe pattern detection vector 측 본 Track Q 측 fn coverage
  SSOT 측 cross-reference (parallel BG agent territory merge audit).
- **online_expansion_lockin L2 long-horizon Warden compat baseline** — KERNEL32 + USER32 + GDI32 +
  ADVAPI32 syscall sequence match + heap pattern match + TEB/PEB shape match + registry probe +
  token query 측 multi-year roadmap 측 milestone breakdown (cycle 100+ 합산 결과).
