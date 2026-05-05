---
schema: airgenome-gamebox/docs/airgenome_gamebox_kernel32_core_cycle28_increment_landed/ai-native/1
last_updated: 2026-05-03
ssot:
  marker: state/markers/airgenome_gamebox_kernel32_core_cycle28_increment_landed.marker
  roadmap_files_modified:
    - .roadmap.loader_win32
  modules_created:
    - lib/loader/pe_kernel32_core.hexa
  predecessor_handoffs:
    - docs/airgenome_gamebox_online_network_stack_3_loader_landed_2026_05_03.ai.md
    - docs/airgenome_gamebox_battlenet_d2r_offline_shim_landed_2026_05_03.ai.md
    - docs/airgenome_gamebox_self_mk2_tuning_landed_2026_05_02.ai.md
status: LANDED_KERNEL32_CORE_CYCLE28_INCREMENT_TRACK_O
related_raws:
  - raw 9    # hexa-only (verifier emit native via println __KERNEL32__ PARTIAL + r0_emit JSONL)
  - raw 11   # snake_case fields throughout JSONL + JSON marker + r0_emit event_type
  - raw 12   # silent-error ban — every PARTIAL emit explicit (no swallow as PASS)
  - raw 15   # env_lazy — repo-relative paths only, no /Users/<user>/ leaks
  - raw 175  # BR-NO-USER-VERBATIM (no verbatim user quotes anywhere)
online_expansion_lockin:
  L1: true   # EULA/ToS violation risk acknowledged (a+ path)
  L2: true   # Warden detection risk acknowledged — Win32 syscall perfection rosetta direct contribution (long-horizon)
  L3: true   # 1-2 year+ multi-cycle horizon acknowledged
  L4: true   # D2R online single-target lock-in (D4 retired_intentional unchanged)
  L5: true   # Track J/K/L/O scope locked-in for parallel BG land
  lockin_iso: 2026-05-03
preserved_unchanged:
  - .roadmap.loader_win32 cond.1 status (partial unchanged — full ~435 fn coverage = multi-cycle long-horizon)
  - .roadmap.loader_win32 cond.2 + cond.3 (status partial, evidence array unchanged)
  - all 14 other .roadmap.* files (battlenet/d2r/d4/wow/cs2/delta/elden/lineage 3/purple/loader_pe/loader_dx)
  - all native sources, plugin.json, entry, README.md, .gitignore
  - 417 existing lib/loader/*.hexa files (Track K's 3 + d2r/battlenet/dx/win32_*/etc untouched, 1 NEW added)
  - all c_* perf modules (untouched — Track J/N parallel scope)
  - all tool/* (untouched — Track M parallel scope)
  - all tests/*.hexa (no new test harness; module's self_test() suffices)
  - 3 docs/*_HONEST_FREEZE.md (GAMEGUARD/WARDEN/D2R_REALM — Track E/J/L territory, read-only)
policy:
  migration: forbidden
  changes: additive_only
  in_place_writes: 1
  in_place_writes_note: 1 .roadmap.loader_win32 JSONL header surgical edit (cond.1 evidence array append +4 rows + blocker_reason refresh) — 0 cond.N row deleted, 0 status field promoted, append/extend only
  destructive_ops: 0
  cost_usd: 0
  substrate: mac-local
  cap_minutes: 90
  br_no_user_verbatim: true
  friendly_preset: true
  silent_land: true
---

# airgenome-gamebox KERNEL32 core cycle 28 increment landed - Track O (BG-AGB online expansion)

## TL;DR

Track K predecessor (3 network DLL skeleton-tier loader 2026-05-03) 위 Track O 로 KERNEL32.dll 측
core syscall surface 측 4 group (file I/O / memory / process / system) 측 18 fn skeleton-tier
loader land. real CreateFileW / 실 HeapAlloc / 실 CreateProcessW / 실 GetSystemTime = 0, mac-local
hexa-only. .roadmap.loader_win32 cond.1 evidence augment (12 → 16, status partial → partial 유지 —
full ~435 fn coverage = multi-cycle long-horizon). KERNEL32 coverage progression cycle 27 = 8/435
(~1.84%) → cycle 28 = 26/435 (~5.98%) (+18 fn). online_expansion_lockin L1+L2+L3+L4+L5 ALL OK.

- `lib/loader/pe_kernel32_core.hexa` (NEW, 500 LOC) — KERNEL32.dll 18 fn surface skeleton tracker
  (`__KERNEL32__ PARTIAL <fn>` ×18 emit + synthetic FILE_HANDLE/PROCESS_INFORMATION/SYSTEMTIME
  round-trip, 4 TSV table persist).

## §1 user_directive_paraphrase

predecessor cycle 측 Track K (online network stack 3 loader land) 후 follow-up Track O 로 KERNEL32.dll
측 core syscall surface 측 NEW skeleton-tier loader 모듈 land. 의도:

- KERNEL32 측 핵심 system primitive 측 4 group 측 baseline 정착 — file I/O (CreateFileW/CreateFileA/
  ReadFile/WriteFile/CloseHandle) + memory (HeapAlloc/HeapFree/VirtualAlloc/VirtualFree/
  GetProcessHeap) + process (CreateProcessW/OpenProcess/GetCurrentProcess/TerminateProcess/
  WaitForSingleObject) + system (GetSystemTime/GetTickCount64/Sleep). 실 syscall 0 / synthetic
  byte array round-trip 만.
- `.roadmap.D2R_D4_INSTALL_LAUNCH_ROADMAP.md` cycle 27 status 측 KERNEL32 8/435 (~1.84%) → cycle
  28 측 26/435 (~5.98%) coverage progression — +18 fn cycle 28 increment, 본 cycle 은 multi-cycle
  long-horizon 측 한 cycle 만.
- .roadmap.loader_win32 cond.1 (kernel32 + 광의 Win32 syscall surface coverage) 측 evidence augment
  4 row 추가 — status partial → partial 유지 (full ~435 fn coverage = multi-cycle long-horizon).
- "natural environment alignment" path 측 long-horizon goal 측 direct contribution — Win32 syscall
  perfection rosetta toward L2 Warden detection compat 측 multi-year baseline. KERNEL32 = real
  Windows binary 측 가장 빈번 호출 surface, Warden detection 측 syscall pattern match 측 가장 직접적
  alignment vector (online_expansion_lockin 5 항목 ALL OK).
- Track K cross-link — KERNEL32 18 fn + Winsock 20 fn + WININET 12 fn + SCHANNEL 8 fn = 58 fn
  online stack baseline (cumulative skeleton-tier surface coverage). Track J (lib/perf/c_bnet_*) /
  Track L (.roadmap.diablo2_resurrected) / Track M (tool/*) / Track N (lib/perf/) / Track P
  (lib/loader/battlenet_bypass.hexa) = parallel BG, 본 cycle 0 라인 touch.

규칙:
- additive only — 기존 self_test row 0 삭제, 1 NEW module + 1 NEW handoff + 1 NEW marker + cond.1
  evidence array append (4 row).
- migration / destructive op / 외부 cost / push: 모두 금지.
- own1 (Wine 0 / hexa-only / NO real KERNEL32.dll binding / NO Wine syscall lookup / NO OSS Win32
  emulator) + own2 (PARTIAL emit explicit, silent_error_ban) 보존.
- BR-NO-USER-VERBATIM (사용자 원문 인용 금지, paraphrase only).
- silent-land — 새 doc + marker 만 land, 외부 announce 0.

## §2 fn coverage table (group / fn names / count)

| group | fn names | fn count | fn_kind range | synthetic struct |
|---|---|---:|---|---|
| file_io | CreateFileW / CreateFileA / ReadFile / WriteFile / CloseHandle | 5 | 0..4 | FILE_HANDLE (handle_hex 0x00000400 + path_w_synth UTF-16 reference shape + access/share/create_kind DWORD) |
| memory | HeapAlloc / HeapFree / VirtualAlloc / VirtualFree / GetProcessHeap | 5 | 5..9 | heap byte buffer (heap_base 0x10000000 + alloc_size 4096 + region_kind MEM_COMMIT\|RESERVE + protect_kind PAGE_READWRITE) |
| process | CreateProcessW / OpenProcess / GetCurrentProcess / TerminateProcess / WaitForSingleObject | 5 | 10..14 | PROCESS_INFORMATION (16 byte: hProcess 0x00000800 + hThread 0x00000804 + dwProcessId 0x1234 + dwThreadId 0x1238) |
| system | GetSystemTime / GetTickCount64 / Sleep | 3 | 15..17 | SYSTEMTIME (16 byte = 8 × WORD: 2026-05-03 12:00:00 UTC LE encoded) |
| **module total** | — | **18** | **0..17** | 4 synthetic struct families |

각 fn 측 self_test() 측 record_syscall_invoke + println("__KERNEL32__ PARTIAL <fn>") + r0_emit
("pe_kernel32_core_record" event_type, JSON payload with fn_kind/fn_name/fn_group/status +
group-specific fields) ×18 = 18 PARTIAL emit total. 4 TSV persist table (syscall_invoke /
file_io_round_trip / memory_lifecycle / proc_sys_event), 18 + 4 + 4 + 4 = 30 total record.

## §3 .roadmap.loader_win32 cond evidence count before/after

| cond_id | status | before evidence count | after evidence count | delta | evidence rows added |
|---|---|---:|---:|---:|---|
| loader_win32.cond.1 | partial → partial (unchanged) | 12 | 16 | **+4** | (a) Track O pe_kernel32_core skeleton-tier 18 fn / (b) cycle 27→28 KERNEL32 coverage progression 8/435→26/435 ~5.98% / (c) L2 long-horizon natural env alignment direct contribution / (d) Track O cross-link to Track K (3 network = 58 fn cumulative online stack baseline) |
| loader_win32.cond.2 | partial (unchanged) | 6 | 6 | 0 | none (user32+gdi32 render scope, Track O out-of-scope; cycle 29 USER32 candidate) |
| loader_win32.cond.3 | partial (unchanged) | 4 | 4 | 0 | none (ntdll Nt* syscall scope, Track O out-of-scope) |

cond.1 blocker_reason refresh: 기존 message 후미에 "KERNEL32 cycle 28 increment landed via Track O
(lib/loader/pe_kernel32_core.hexa, +18 fn skeleton-tier file_io/memory/process/system, 8/435→26/435
~5.98%) — full ~435 fn coverage = multi-cycle long-horizon (separate cycle)" append. 0 status field
promotion (multi-cycle long-horizon 명시).

전체 cond 3 中 0 promoted (status enum 변경 0), 1 evidence augmented (cond.1 +4 rows). own1 (Wine 0)
+ own2 (PARTIAL emit explicit) preserved.

## §4 online_expansion_lockin L1-L5 ack confirmation

| 항목 | 명칭 | 사용자 lock-in posture | marker reflect | 본 cycle posture |
|---|---|---|---|---|
| L1 | EULA/ToS violation risk acknowledged | OK (a+ path) | `eula_violation_risk_acknowledged: true` | KERNEL32 syscall surface = production access 측 ToS 위반 가능성 인지 — skeleton-tier hexa-only design 으로 risk 분리 (real syscall 0) |
| L2 | Warden detection risk acknowledged | OK — Track O 직접 contributor | `warden_detection_risk_acknowledged: true` | **본 Track O 는 L2 long-horizon 직접 기여** — KERNEL32 syscall pattern match = Warden detection 회피 측 가장 직접적 alignment vector (real Windows binary 측 가장 빈번 호출 surface). 본 cycle = skeleton baseline only, production smoke 측 multi-cycle. |
| L3 | 1-2 year+ multi-cycle horizon acknowledged | OK | `multi_year_horizon_acknowledged: true` | KERNEL32 18/435 = ~5.98% — full coverage 측 cycle 29-N (USER32 / GDI32 / ADVAPI32 / etc) 측 multi-cycle long-horizon 명시 |
| L4 | D2R online single-target lock-in | OK (D4 retired_intentional unchanged) | `d2r_single_target_lockin: true` + `d4_retired_intentional_unchanged: true` | KERNEL32 측 synthetic CreateFileW path "C:\\Battle.net\\D2R\\Save\\test.d2s" + CreateProcessW image_path "C:\\Battle.net\\Agent.exe" 측 D2R-specific. D4 path 0 줄. |
| L5 | Track J/K/L/O parallel BG land scope locked-in | OK | `track_jklo_parallel_lockin: true` | Track O (KERNEL32 core) = J (Warden compat) + K (network stack) + L (D2R realm protocol) + M (tool) + N (perf) + P (battlenet_bypass) parallel BG. 본 cycle O 만 land, 다른 track 측 territory 0 라인 touch. |

5 항목 ALL OK 사용자 lock-in 2026-05-03. marker `online_expansion_lockin` block 5 field 모두 true +
lockin_iso "2026-05-03" + 6 redundant boolean ack field.

## §5 cycle 27→28 KERNEL32 coverage progression (8/435 → 26/435 ~5.98%)

| metric | cycle 27 (predecessor) | cycle 28 (Track O) | delta | % progression |
|---|---:|---:|---:|---|
| KERNEL32 fn covered | 8 | 26 | +18 | 1.84% → 5.98% (+4.14 pp) |
| KERNEL32 total surface | 435 | 435 | 0 | (D2R_D4_INSTALL_LAUNCH_ROADMAP.md cycle 27 ref) |
| coverage source modules | win32_kernel32_stage1..5 + tier1..4 (8 fn aggregate) | + lib/loader/pe_kernel32_core.hexa (18 fn skeleton-tier) | +1 module | — |
| skeleton-tier vs production | (mixed) | skeleton-tier only (synthetic round-trip) | — | full coverage = multi-cycle long-horizon |

cycle 27 측 KERNEL32 8 fn coverage 측 source = predecessor win32_kernel32_stage1 / stage2 / stage3 /
stage4 / stage5 + tier1_real / tier2_real / tier3_struct / tier4_heap 측 aggregate (mk1 narrative
phase_3_kernel32 측 진척, mixed real-vs-stub). cycle 28 = 본 Track O 측 skeleton-tier +18 fn 추가.

production smoke (real CreateFileW / 실 HeapAlloc / 실 CreateProcessW) = cycle 29+ 측 multi-cycle
long-horizon — fn-by-fn real-vs-stub matrix SSOT 측 별도 cycle (full ~435 fn coverage = many more
cycles). 본 cycle = baseline only.

## §6 multi-cycle long-horizon caveat (~435 fn full coverage = many cycles)

KERNEL32.dll 측 export fn count ~435 (Microsoft Windows 10/11 측 standard kernel32 export table 측
대략 추정 — Wine / ReactOS source 측 정확 count 측 인용 0). 본 cycle 28 = 26/435 (~5.98%) 측 최
초기 baseline. full coverage 측 cycle path:

- cycle 29: USER32 (window/message pump) skeleton-tier — ~30 fn 추가, KERNEL32 영향 0
- cycle 30: GDI32 (drawing/font) skeleton-tier — ~24 fn 추가, KERNEL32 영향 0
- cycle 31: ADVAPI32 (registry/security) skeleton-tier — ~25 fn 추가, KERNEL32 영향 0
- cycle 32+: KERNEL32 expansion — synchronization (CreateMutex/CreateEvent/CreateSemaphore), DLL
  load (LoadLibraryW/GetProcAddress/FreeLibrary), thread (CreateThread/ResumeThread/SuspendThread/
  TlsAlloc), TLS, file mapping (CreateFileMappingW/MapViewOfFile), pipe, etc — 수십 cycle 측
  partial increment 합산 후 ~50% threshold 도달 가능 (1-2 year+ horizon)
- cycle 50+: production smoke matrix — fn-by-fn real-vs-stub coverage SSOT + Warden compat syscall
  pattern align cycle 측 합산 후 cond.1 status promotion 측 met 후보

직접적 Warden bypass / detection vector 0 — natural environment alignment 측 indirect contribution
(syscall surface naming / synthetic struct shape / fn group classification) 만. 본 cycle 측 full
coverage 도달 = false (의도적, multi-cycle long-horizon 정합).

## §7 caveats inline (honest C3 ≥ 8 numbered)

C1. **KERNEL32 측 synthetic round-trip — real syscall X**: pe_kernel32_core.hexa 측 self_test() 측
    18 syscall_invoke + 4 file_io_round_trip + 4 memory_lifecycle + 4 proc_sys_event = 30 record 측
    println("__KERNEL32__ PARTIAL <fn>") + r0_emit JSON 만. 실 macOS posix open(2)/read(2)/mmap/
    fork+exec/gettimeofday(2) 측 syscall 0 — production smoke 측 cycle 29+ pending. 의도적 (own1
    Wine 0 / hexa-only mandate 정합).

C2. **synthetic FILE_HANDLE / PROCESS_INFORMATION / SYSTEMTIME — real struct byte X**: pe_kernel32_core.hexa
    측 handle_hex "0x00000400" / pi_synth_hex 16 byte / systemtime_hex 16 byte 측 reference shape
    만 (windows.h / winbase.h / processthreadsapi.h 측 standard public header type definition 인용,
    Wine kernel32 source 측 코드 인용 0, ReactOS 측 코드 인용 0).

C3. **CreateProcessW 측 D2R Battle.net Agent.exe 측 synthetic image path 하드코딩 — D2R single
    target (L4) reflect**: pe_kernel32_core.hexa 측 image_path_w_synth "C:\\Battle.net\\Agent.exe"
    + CreateFileW path_w_synth "C:\\Battle.net\\D2R\\Save\\test.d2s" 측 D2R-specific. D4 측 process
    image (Diablo IV.exe) 0 줄 — D4 retired_intentional 측 status enum 측 unchanged 정합 (predecessor
    self_mk2_tuning land 측 명시).

C4. **18 fn / 435 = ~5.98% 측 coverage = cycle 28 baseline 만, full coverage = multi-cycle long-
    horizon**: 본 cycle 측 evidence augment (12 → 16 row) 만 — cond.1 status field promotion (partial
    → met) 0. full Win32 coverage 측 production smoke + fn-by-fn real-vs-stub matrix SSOT + ~435
    fn 측 multi-cycle work — 본 cycle 은 18 fn skeleton-tier 측 baseline 만 정착.

C5. **18 module 측 PARTIAL emit explicit — silent_error_ban (raw 12) 정합**: PARTIAL ≠ PASS. 본 cycle
    측 status emit 18 모두 `__KERNEL32__ PARTIAL <fn>` 측 명시 — production smoke 미달 + real-vs-stub
    측 stub side 측 honest acknowledgement. PARTIAL swallow as PASS = silent_error_ban 위반 = 본
    cycle 측 명시적 회피.

C6. **L2 Warden compat = long-horizon, 본 cycle Warden detection vector 0**: Track O 측 KERNEL32
    syscall surface 측 baseline = Warden detection 측 syscall pattern match 회피 측 indirect
    contribution (fn name + fn_group classification + synthetic struct shape). 실 Warden 측 syscall
    hooking pattern detection / TEB/PEB shape walk / heap layout match / NtQueryInformationProcess
    response normalize 측 분석 0 — 별도 Track J (c_bnet_warden_environment_compat) territory
    (parallel BG, 본 cycle 측 untouched).

C7. **online_expansion_lockin marker block 측 사용자 lock-in 2026-05-03 reflect — verbatim 측 0**:
    user_directive_paraphrase 측 5 항목 (L1 EULA/ToS / L2 Warden long-horizon / L3 multi-year / L4
    D2R single target / L5 Track J/K/L/O parallel) 측 paraphrase 만 — 사용자 원문 인용 0 (BR-NO-USER-
    VERBATIM raw 175 정합). marker 측 `online_expansion_lockin` block 5 field 모두 true + lockin_iso
    "2026-05-03" + 6 redundant boolean ack field 측 explicit reflection.

C8. **Track O scope 측 J / K / L / M / N / P 분리 — 본 cycle 측 cross-link untouched**: 본 cycle 은
    O (KERNEL32 core skeleton-tier) 만 — Track J (lib/perf/c_bnet_warden_environment_compat) +
    Track K (lib/loader/pe_winsock_ws2_32 + pe_wininet_https + pe_schannel_tls) + Track L
    (.roadmap.diablo2_resurrected D2R realm protocol cross-link) + Track M (tool/*) + Track N
    (lib/perf/c_bnet_*) + Track P (lib/loader/battlenet_bypass.hexa) 측 territory 0 라인 touch. 7
    track parallel BG land 측 lock-in (L5) — cross-cycle merge audit 별도 cycle.

C9. **runtime exec smoke 측 미수행 — cap_minutes 90 + skeleton-tier mandate 정합**: 본 cycle 측
    pe_kernel32_core.hexa 측 self_test() 실측 stdout 캡처 미수행 — predecessor pe_winsock_ws2_32 +
    pe_wininet_https + pe_schannel_tls (Track K) 측 hexa runtime smoke 측 docker route fall-through
    caveat 동급 posture (predecessor online_network_stack_3_loader landing 측 C11 reflection).
    emit count 18+4+4+4 = 30 record + LOC 500 + grep PARTIAL 82 + grep __KERNEL32__ PARTIAL 18 측
    static-validate 만.

C10. **fn_kind 0..17 = 18 entries (not 17) — comment header initial draft 측 17 fn 측 typo, 본 cycle
     final 18 정합**: 초기 spec 측 "16-20 core fn" range 측 18 fn 안착. fn_kind_name(k) 측 0..17 =
     18 entries (CreateFileW / CreateFileA / ReadFile / WriteFile / CloseHandle / HeapAlloc /
     HeapFree / VirtualAlloc / VirtualFree / GetProcessHeap / CreateProcessW / OpenProcess /
     GetCurrentProcess / TerminateProcess / WaitForSingleObject / GetSystemTime / GetTickCount64 /
     Sleep). assertion 측 fn_kind <= 17 + count == 18.

## §8 next-cycle hooks

- **cycle 29 USER32** — lib/loader/pe_user32_core.hexa skeleton-tier (window primitive: CreateWindowExW
  / RegisterClassExW / ShowWindow / DefWindowProcW / GetMessageW / TranslateMessage / DispatchMessageW
  / DestroyWindow / SendMessageW / PostMessageW / etc ~30 fn skeleton). Track O 측 KERNEL32 18 fn
  baseline 측 self-similar pattern.
- **cycle 30 GDI32** — lib/loader/pe_gdi32_core.hexa skeleton-tier (drawing primitive: GetDC / ReleaseDC
  / CreateCompatibleDC / DeleteDC / SelectObject / DeleteObject / TextOutW / SetBkColor / SetTextColor
  / Rectangle / etc ~24 fn skeleton). Track O 측 self-similar pattern.
- **cycle 31 ADVAPI32** — lib/loader/pe_advapi32_core.hexa skeleton-tier (registry/security primitive:
  RegOpenKeyExW / RegQueryValueExW / RegSetValueExW / RegCloseKey / OpenSCManagerW / CreateServiceW /
  StartServiceW / OpenProcessToken / GetTokenInformation / etc ~25 fn skeleton). Track O 측
  self-similar pattern.
- **cycle 32+ KERNEL32 expansion** — synchronization primitive (CreateMutex/CreateEvent/CreateSemaphore)
  + DLL load (LoadLibraryW/GetProcAddress/FreeLibrary) + thread (CreateThread/SuspendThread/TlsAlloc)
  + file mapping (CreateFileMappingW/MapViewOfFile) + pipe + etc 수십 cycle 측 incremental expansion.
- **production smoke cycle (multi-cycle, eta 1-2 year+)** — 실 CreateFileW = posix open(2) +
  GetLastError = errno → Win32 error code mapping; 실 HeapAlloc = malloc(3) + heap chunk header;
  실 CreateProcessW = posix_spawnp(3) + 실 SYSTEMTIME = gettimeofday(2) → SYSTEMTIME struct fill.
- **loader_win32.cond.1 status promotion partial → met 측 production smoke matrix** — 18 + 추가 fn
  측 production smoke 합산 후 fn-by-fn real-vs-stub matrix SSOT 측 land + Warden compat syscall
  pattern align cycle 측 합산 후 status 승격 (multi-cycle long-horizon 합산 결과).
- **Track J cross-link audit cycle** — c_bnet_warden_environment_compat 측 KERNEL32 syscall pattern
  detection vector 측 본 Track O 측 fn coverage SSOT 측 cross-reference (parallel BG agent territory
  merge audit).
- **online_expansion_lockin L2 long-horizon Warden compat baseline** — KERNEL32 syscall sequence
  match + heap pattern match + TEB/PEB shape match 측 multi-year roadmap 측 milestone breakdown
  (cycle 50+ 합산 결과).
