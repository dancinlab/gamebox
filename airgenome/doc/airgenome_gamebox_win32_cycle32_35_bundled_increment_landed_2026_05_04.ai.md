---
schema: airgenome-gamebox/airgenome/doc/airgenome_gamebox_win32_cycle32_35_bundled_increment_landed/ai-native/1
last_updated: 2026-05-04
ssot:
  marker: state/markers/airgenome_gamebox_win32_cycle32_35_bundled_increment_landed.marker
  roadmap_files_modified:
    - .roadmap.loader_win32
  modules_extended:
    - lib/loader/pe_kernel32_core.hexa
  modules_created:
    - lib/loader/pe_ntdll_core.hexa
    - lib/loader/pe_ole32_core.hexa
    - lib/loader/pe_oleaut32_core.hexa
  predecessor_handoffs:
    - airgenome/doc/airgenome_gamebox_user32_gdi32_advapi32_cycle29_31_increment_landed_2026_05_03.ai.md
    - airgenome/doc/airgenome_gamebox_kernel32_core_cycle28_increment_landed_2026_05_03.ai.md
    - airgenome/doc/airgenome_gamebox_online_network_stack_3_loader_landed_2026_05_03.ai.md
    - airgenome/doc/airgenome_gamebox_self_mk2_tuning_landed_2026_05_02.ai.md
status: LANDED_WIN32_CYCLE32_35_BUNDLED_INCREMENT_TRACK_W
related_raws:
  - raw 9    # hexa-only (verifier emit native via println __KERNEL32__/__NTDLL__/__OLE32__/__OLEAUT32__ PARTIAL + r0_emit JSONL)
  - raw 11   # snake_case fields throughout JSONL + JSON marker + r0_emit event_type
  - raw 12   # silent-error ban — every PARTIAL emit explicit (no swallow as PASS)
  - raw 15   # env_lazy — repo-relative paths only, no /Users/<user>/ leaks
  - raw 175  # BR-NO-USER-VERBATIM (no verbatim user quotes anywhere)
online_expansion_lockin:
  L1: true   # EULA/ToS violation risk acknowledged (a+ path)
  L2: true   # Warden detection risk acknowledged — Win32 syscall perfection rosetta direct contribution (long-horizon)
  L3: true   # 1-2 year+ multi-cycle horizon acknowledged
  L4: true   # D2R online single-target lock-in (D4 retired_intentional unchanged)
  L5: true   # Track J/K/L/O/Q/W parallel BG land scope locked-in
  lockin_iso: 2026-05-03
preserved_unchanged:
  - .roadmap.loader_win32 cond.1 + cond.2 + cond.3 status (all 3 partial unchanged — full ~435 fn coverage = multi-cycle long-horizon)
  - all 14 other .roadmap.* files (battlenet/d2r/d4/wow/cs2/delta/elden/lineage 3/purple/loader_pe/loader_dx)
  - all native sources, plugin.json, entry, README.md, .gitignore
  - 418+ existing lib/loader/*.hexa files (pe_user32_core / pe_gdi32_core / pe_advapi32_core / pe_winsock_ws2_32 / pe_wininet_https / pe_schannel_tls / etc untouched, only pe_kernel32_core EXTENDED additively + 3 NEW added)
  - all c_* perf modules (untouched — Track J/N parallel scope)
  - all tool/* (untouched — Track M parallel scope)
  - all tests/*.hexa (no new test harness; module's self_test() suffices)
  - 3 docs/*_HONEST_FREEZE.md (GAMEGUARD/WARDEN/D2R_REALM — Track E/J/L territory, read-only)
  - lib/loader/battlenet_bypass.hexa (Track P territory)
policy:
  migration: forbidden
  changes: additive_only
  in_place_writes: 2
  in_place_writes_note: 1 .roadmap.loader_win32 JSONL header surgical edit (cond.1 evidence array append +5 rows W1+W3+W4+coverage+cumulative + cond.3 evidence array append +3 rows W2+coverage+cumulative + 2 blocker_reason refresh; cond.2 evidence + status unchanged) + 1 lib/loader/pe_kernel32_core.hexa additive extend (15 new fn + 1 new TSV table + 0 existing fn deletion / 0 existing assertion broken)
  destructive_ops: 0
  cost_usd: 0
  substrate: mac-local
  cap_minutes: 90
  br_no_user_verbatim: true
  friendly_preset: true
  silent_land: true
---

# airgenome-gamebox Win32 cycle 32-35 bundled increment landed - Track W (BG-AGB online expansion)

## TL;DR

Track Q predecessor (USER32 + GDI32 + ADVAPI32 53 fn skeleton-tier 2026-05-03) 위 Track W 로 KERNEL32
extend + NTDLL new + OLE32 new + OLEAUT32 new 4 sub-cycle 합산 53 fn skeleton-tier loader bundled
land (cycle 32 + 33 + 34 + 35). 실 EnterCriticalSection / 실 CreateMutexW / 실 NtCreateFile / 실
RtlAllocateHeap / 실 LdrLoadDll / 실 CoInitializeEx / 실 CoCreateInstance / 실 QueryInterface / 실
VariantInit / 실 SafeArrayCreate / 실 IDispatch::Invoke = 0, mac-local hexa-only. .roadmap.loader_win32
cond.1 + cond.3 evidence augment (status partial → partial 유지 — full ~435 fn coverage = multi-cycle
long-horizon). cumulative coverage progression cycle 31 = 79/435 (~18.16%) → cycle 35 = 132/435
(~30.34%) (+53 fn bundled). online_expansion_lockin L1+L2+L3+L4+L5 ALL OK.

- `lib/loader/pe_kernel32_core.hexa` (EXTEND, 500 → 689 LOC, +189 delta) — KERNEL32.dll 18 fn
  baseline + 15 NEW fn (sync_primitive 5 + locale 3 + file_extras 4 + thread 3) = 33 fn 8 group
  + 5번째 TSV table ext_event 추가. 기존 18 fn 0 라인 변경 (additive_only).
- `lib/loader/pe_ntdll_core.hexa` (NEW, 492 LOC) — NTDLL.dll 15 fn surface skeleton tracker
  (`__NTDLL__ PARTIAL <fn>` ×15 + synthetic UNICODE_STRING/OBJECT_ATTRIBUTES/IO_STATUS_BLOCK/
  NTSTATUS round-trip, 4 TSV table persist).
- `lib/loader/pe_ole32_core.hexa` (NEW, 478 LOC) — OLE32.dll 12 fn surface skeleton tracker
  (`__OLE32__ PARTIAL <fn>` ×12 + synthetic IID/CLSID 16-byte GUID + IUnknown vtable 24-byte
  3-slot + ref_count_synth round-trip, 4 TSV table persist).
- `lib/loader/pe_oleaut32_core.hexa` (NEW, 472 LOC) — OLEAUT32.dll 11 fn surface skeleton tracker
  (`__OLEAUT32__ PARTIAL <fn>` ×11 + synthetic VARIANT 16-byte + SAFEARRAYBOUND 8-byte + IDispatch
  vtable round-trip, 4 TSV table persist).

## §1 user_directive_paraphrase

predecessor cycle 측 Track Q (USER32 + GDI32 + ADVAPI32 core 53 fn cycle 29-31 increment) 후
follow-up Track W 로 KERNEL32 extension + NTDLL Nt* core + OLE32 COM + OLEAUT32 IDispatch 4
sub-cycle bundled land. 의도:

- KERNEL32 측 핵심 추가 primitive 측 4 NEW group 측 baseline 정착 — sync_primitive
  (InitializeCriticalSection/EnterCriticalSection/LeaveCriticalSection/DeleteCriticalSection/
  CreateMutexW) + locale (GetLocaleInfoW/SetThreadLocale/GetSystemDefaultLCID) + file_extras
  (GetFileAttributesW/SetFileAttributesW/DeleteFileW/MoveFileExW) + thread (CreateThread/
  GetCurrentThreadId/ResumeThread). 기존 18 fn 0 라인 변경 (additive_only).
- NTDLL 측 핵심 Nt*/Rtl*/Ldr core syscall bridge surface 측 3 group 측 baseline 정착 — nt_core
  (NtCreateFile/NtReadFile/NtWriteFile/NtClose/NtQueryInformationFile/NtSetInformationFile/
  NtAllocateVirtualMemory/NtFreeVirtualMemory) + rtl_utility (RtlInitUnicodeString/RtlAllocateHeap/
  RtlFreeHeap/RtlGetLastWin32Error) + ldr (LdrLoadDll/LdrGetProcedureAddress/LdrUnloadDll). 실
  syscall 0 / synthetic byte array round-trip 만.
- OLE32 측 핵심 COM init + IUnknown surface 측 4 group 측 baseline 정착 — com_init (CoInitialize/
  CoInitializeEx/CoUninitialize/CoCreateInstance) + iunknown (QueryInterface/AddRef/Release vtable
  simulation) + com_marshal (CoMarshalInterface/CoUnmarshalInterface/CoCreateInstanceEx) +
  apartment (CoGetCurrentProcess/CoGetCurrentLogicalThreadId). 실 syscall 0 / synthetic byte
  array round-trip 만.
- OLEAUT32 측 핵심 IDispatch + Variant + SafeArray surface 측 3 group 측 baseline 정착 — variant
  (VariantInit/VariantClear/VariantCopy/VariantChangeType) + safearray (SafeArrayCreate/Destroy/
  GetElement/PutElement) + idispatch (GetIDsOfNames/Invoke/GetTypeInfo vtable simulation). 실
  syscall 0 / synthetic byte array round-trip 만.
- predecessor cycle 31 status 측 79/435 (~18.16%) baseline 위 cycle 32 측 94/435 (~21.61%, Track
  W1 KERNEL32 +15 fn) → cycle 33 측 109/435 (~25.06%, Track W2 NTDLL +15 fn) → cycle 34 측 121/435
  (~27.82%, Track W3 OLE32 +12 fn) → cycle 35 측 132/435 (~30.34%, Track W4 OLEAUT32 +11 fn)
  coverage progression — bundled 4 sub-cycle increment in single Track W land.
- .roadmap.loader_win32 cond.1 + cond.3 evidence augment 8 row 추가 (cond.2 evidence + status
  unchanged) — status partial → partial 유지 (full ~435 fn coverage = multi-cycle long-horizon).
- "natural environment alignment" path 측 long-horizon goal 측 direct contribution — Win32 syscall
  perfection rosetta toward L2 Warden detection compat 측 multi-year baseline. CRITICAL_SECTION
  sync pattern + CreateMutex pattern + Nt* syscall bridge pattern + COM apartment pattern +
  IDispatch automation pattern = real Windows binary 측 빈번 호출 surface 추가 4 family 측
  Warden detection 측 syscall pattern match 회피 측 가장 직접적 alignment vector.
- Track Q + Track W cumulative cross-link — KERNEL32 33 fn (Track O+W1 EXTEND) + USER32 20 fn
  (Track Q1) + GDI32 15 fn (Track Q2) + ADVAPI32 18 fn (Track Q3) + NTDLL 15 fn (Track W2) +
  OLE32 12 fn (Track W3) + OLEAUT32 11 fn (Track W4) = 124 fn cumulative skeleton-tier surface
  coverage across 7 modules. Track J/K/L/M/N/O/P = parallel BG, 본 cycle 0 라인 touch.

규칙:
- additive only — 기존 self_test row 0 삭제, 1 EXTEND module + 3 NEW module + 1 NEW handoff +
  1 NEW marker + cond.1 evidence array append (5 row) + cond.3 evidence array append (3 row).
- migration / destructive op / 외부 cost / push: 모두 금지.
- own1 (Wine 0 / hexa-only / NO real KERNEL32.dll/NTDLL.dll/OLE32.dll/OLEAUT32.dll binding /
  NO Wine source lookup / NO ReactOS source 인용 / NO OSS Win32 emulator import) + own2 (PARTIAL
  emit explicit, silent_error_ban) 보존.
- BR-NO-USER-VERBATIM (사용자 원문 인용 금지, paraphrase only).
- silent-land — 새 doc + marker 만 land, 외부 announce 0.

## §2 fn coverage table (4 modules / group / fn names / count)

### W1: pe_kernel32_core.hexa (KERNEL32 EXTEND, +15 fn)

| group | fn names | fn count | fn_kind range | synthetic struct |
|---|---|---:|---|---|
| sync_primitive | InitializeCriticalSection / EnterCriticalSection / LeaveCriticalSection / DeleteCriticalSection / CreateMutexW | 5 | 18..22 | CRITICAL_SECTION (40 byte ref shape) at 0x30000100 + mutex_handle 0x00000C00 |
| locale | GetLocaleInfoW / SetThreadLocale / GetSystemDefaultLCID | 3 | 23..25 | LCID DWORD (0x00000412 ko-KR / 0x00000409 en-US) + locale_name UTF-16 |
| file_extras | GetFileAttributesW / SetFileAttributesW / DeleteFileW / MoveFileExW | 4 | 26..29 | path_w_synth UTF-16 + attr DWORD (0x00000020 archive) + flags DWORD |
| thread | CreateThread / GetCurrentThreadId / ResumeThread | 3 | 30..32 | thread_handle 0x00000D00 + tid 0x00001238 + start_addr 0x10010000 |
| **W1 delta total** | — | **+15** | **18..32** | 4 NEW synthetic struct families (preserves baseline 4 group + 18 fn from cycle 28) |

### W2: pe_ntdll_core.hexa (NTDLL, 15 fn)

| group | fn names | fn count | fn_kind range | synthetic struct |
|---|---|---:|---|---|
| nt_core | NtCreateFile / NtReadFile / NtWriteFile / NtClose / NtQueryInformationFile / NtSetInformationFile / NtAllocateVirtualMemory / NtFreeVirtualMemory | 8 | 0..7 | UNICODE_STRING (8 byte) + OBJECT_ATTRIBUTES (24 byte) + IO_STATUS_BLOCK (16 byte) + HANDLE 0x40000400 + NTSTATUS LE |
| rtl_utility | RtlInitUnicodeString / RtlAllocateHeap / RtlFreeHeap / RtlGetLastWin32Error | 4 | 8..11 | UNICODE_STRING + heap_handle 0x50000500 + heap_alloc_ptr 0x50001000 |
| ldr | LdrLoadDll / LdrGetProcedureAddress / LdrUnloadDll | 3 | 12..14 | dll_handle 0x60000600 + proc_addr 0x60010800 + dll_name_w "kernel32.dll" |
| **module total** | — | **15** | **0..14** | 6 synthetic struct families |

### W3: pe_ole32_core.hexa (OLE32, 12 fn)

| group | fn names | fn count | fn_kind range | synthetic struct |
|---|---|---:|---|---|
| com_init | CoInitialize / CoInitializeEx / CoUninitialize / CoCreateInstance | 4 | 0..3 | dwCoInit DWORD (0x00000002 APARTMENTTHREADED) + IID/CLSID 16-byte GUID LE + obj_out_hex 0x70030000 |
| iunknown | QueryInterface / AddRef / Release | 3 | 4..6 | IUnknown vtable 24-byte 3-slot (QI@0x70010000 / AddRef@0x70010008 / Release@0x70010010) + ref_count_synth 1→2→1 |
| com_marshal | CoMarshalInterface / CoUnmarshalInterface / CoCreateInstanceEx | 3 | 7..9 | HSTREAM 0x70020000 + COSERVERINFO ref 0x70040000 |
| apartment | CoGetCurrentProcess / CoGetCurrentLogicalThreadId | 2 | 10..11 | co_process_id 0x00001234 + logical_thread_id GUID 16-byte |
| **module total** | — | **12** | **0..11** | 4 synthetic struct families |

### W4: pe_oleaut32_core.hexa (OLEAUT32, 11 fn)

| group | fn names | fn count | fn_kind range | synthetic struct |
|---|---|---:|---|---|
| variant | VariantInit / VariantClear / VariantCopy / VariantChangeType | 4 | 0..3 | VARIANT (16 byte = VARTYPE 2 + reserved 6 + value 8) + VT_EMPTY/VT_I4/VT_R8/VT_BSTR + variant_addr 0x80020100 |
| safearray | SafeArrayCreate / SafeArrayDestroy / SafeArrayGetElement / SafeArrayPutElement | 4 | 4..7 | SAFEARRAYBOUND (8 byte = cElements + lLbound) + SAFEARRAY (16 byte) at 0x80040000 |
| idispatch | GetIDsOfNames / Invoke / GetTypeInfo | 3 | 8..10 | IDispatch vtable slot 4..6 (GetTypeInfo@0x80010028 / GetIDsOfNames@0x80010030 / Invoke@0x80010038) + DISPID + DISPPARAMS |
| **module total** | — | **11** | **0..10** | 5 synthetic struct families |

각 fn 측 self_test() 측 record_syscall_invoke + println("__<DLL>__ PARTIAL <fn>") + r0_emit
("pe_<dll>_core_record" event_type, JSON payload). W1 = +15 PARTIAL emit + 16 baseline KERNEL32
preserved + 8 ext_event row. W2 = 15 PARTIAL emit + 11 lifecycle/io/ldr row = 26 record. W3 = 12
PARTIAL emit + 10 init/vtable/marshal row = 22 record. W4 = 11 PARTIAL emit + 11 variant/safearray/
vtable row = 22 record. Track W bundled = 53 fn skeleton-tier (+15 W1 + 15 W2 + 12 W3 + 11 W4) +
58 supporting record = 111 total record (W1 ext only counts +15 + 8 ext_event = 23, W2/W3/W4 full
self-test counts).

| module | LOC | bytes | sha256 | fn count | DLL emit count | PARTIAL grep | r0_emit count |
|---|---:|---:|---|---:|---:|---:|---:|
| pe_kernel32_core.hexa (EXTEND, 500→689) | 689 | 44316 | caf98c2103b7b05ae4a8bcd1635275c3ae483fe710502bbb851c3998de834d95 | 33 (18 base + 15 W1) | 34 | 144 | 61 |
| pe_ntdll_core.hexa (NEW) | 492 | 28545 | 5885e0664a652b12cf98e0033f5b5fc55bb1cdec2132b16f9d128b6afae5c0de | 15 | 16 | 72 | 34 |
| pe_ole32_core.hexa (NEW) | 478 | 26687 | 9597af4cae3f1e86097abbc529f6e40a1f8c0628f7948f20de220d09faf2da49 | 12 | 13 | 61 | 30 |
| pe_oleaut32_core.hexa (NEW) | 472 | 26953 | b587a8e670fe92abed4290c296b8ac7a433057beda31f0e860ca5559b2a6c4d5 | 11 | 12 | 60 | 30 |
| **Track W total** | **2131** | **126501** | — | **53 (delta)** | **75** | **337** | **155** |

## §3 .roadmap.loader_win32 cond evidence count before/after

| cond_id | status | before evidence count | after evidence count | delta | evidence rows added |
|---|---|---:|---:|---:|---|
| loader_win32.cond.1 | partial → partial (unchanged) | 16 | 21 | **+5** | (a) Track W1 pe_kernel32_core EXTEND +15 fn / (b) Track W3 pe_ole32_core skeleton-tier 12 fn / (c) Track W4 pe_oleaut32_core skeleton-tier 11 fn / (d) Win32 cycle 32-35 bundled coverage progression 79/435→132/435 ~30.34% / (e) Track W bundled cumulative L2 long-horizon natural env alignment direct contribution |
| loader_win32.cond.2 | partial (unchanged) | 9 | 9 | 0 | none (per spec — cond.2 status STAYS at current value, evidence augment routed to cond.1 cluster only since KERNEL32+OLE32+OLEAUT32 are full Win32 surface scope rather than user32+gdi32 60% threshold matrix scope) |
| loader_win32.cond.3 | partial → partial (unchanged) | 7 | 10 | **+3** | (a) Track W2 pe_ntdll_core skeleton-tier 15 fn (Nt*/Rtl*/Ldr core syscall bridge, 본 cond.3 직접 영역) / (b) NTDLL coverage progression cycle 31→33 79/435→109/435 ~25.06% +15 fn / (c) Track W bundled cumulative cycle 32-35 4-sub-cycle progression + Track Q + Track W cumulative since cycle 28: 124 fn skeleton-tier across 7 modules |

cond.1 blocker_reason refresh: 기존 message 후미에 Win32 cycle 32-35 bundled increment 안내 append.
cond.3 blocker_reason refresh: 기존 message 후미에 NTDLL cycle 33 increment + Win32 bundled cycle
32-35 완료 안내 append. 0 status field promotion (multi-cycle long-horizon 명시).

전체 cond 3 中 0 promoted (status enum 변경 0), 2 evidence augmented (cond.1 +5 rows + cond.3 +3
rows). own1 (Wine 0) + own2 (PARTIAL emit explicit) preserved.

## §4 online_expansion_lockin L1-L5 ack confirmation

| 항목 | 명칭 | 사용자 lock-in posture | marker reflect | 본 cycle posture |
|---|---|---|---|---|
| L1 | EULA/ToS violation risk acknowledged | OK (a+ path) | `eula_violation_risk_acknowledged: true` | KERNEL32/NTDLL/OLE32/OLEAUT32 syscall surface = production access 측 ToS 위반 가능성 인지 — skeleton-tier hexa-only design 으로 risk 분리 (real syscall 0) |
| L2 | Warden detection risk acknowledged | OK — Track W 직접 contributor | `warden_detection_risk_acknowledged: true` | **본 Track W 는 L2 long-horizon 직접 기여** — KERNEL32 sync primitive (CRITICAL_SECTION + CreateMutex) pattern + NTDLL Nt* syscall sequence + OLE32 COM apartment pattern + OLEAUT32 IDispatch automation pattern = Warden detection 측 syscall pattern match 회피 측 가장 직접적 alignment vector (real Windows binary 측 빈번 호출 surface 추가 4 family). 본 cycle = skeleton baseline only, production smoke 측 multi-cycle. |
| L3 | 1-2 year+ multi-cycle horizon acknowledged | OK | `multi_year_horizon_acknowledged: true` | 누적 132/435 = ~30.34% (Track Q + Track W 합산 124 fn across 7 modules) — full coverage 측 cycle 36+ (KERNEL32 추가 expansion / WINTRUST / WINHTTP / etc) 측 multi-cycle long-horizon 명시 |
| L4 | D2R online single-target lock-in | OK (D4 retired_intentional unchanged) | `d2r_single_target_lockin: true` + `d4_retired_intentional_unchanged: true` | KERNEL32 측 mutex name "Global\\D2R_SingleInstance" + NTDLL 측 object_name "\\??\\C:\\Battle.net\\D2R\\Save\\test.d2s" + OLEAUT32 측 BSTR_synth "D2R\\0" 측 D2R-specific. D4 path 0 줄. |
| L5 | Track J/K/L/O/Q/W parallel BG land scope locked-in | OK | `track_jklo_parallel_lockin: true` + `track_q_bundled_cycle_29_31: true` + `track_w_bundled_cycle_32_35: true` | Track W (KERNEL32 + NTDLL + OLE32 + OLEAUT32 bundled) = J (Warden compat) + K (network stack) + L (D2R realm protocol) + M (tool) + N (perf) + O (KERNEL32 core baseline) + P (battlenet_bypass) + Q (USER32+GDI32+ADVAPI32) parallel BG. 본 cycle W 만 land, 다른 track 측 territory 0 라인 touch. |

5 항목 ALL OK 사용자 lock-in 2026-05-03. marker `online_expansion_lockin` block 5 field 모두 true +
lockin_iso "2026-05-03" + 6 redundant boolean ack field.

## §5 cycle 31 → 35 cumulative coverage progression (79/435 → 132/435 ~30.34%)

| metric | cycle 31 (Track Q3 ADVAPI32) | cycle 32 (Track W1 KERNEL32 extend) | cycle 33 (Track W2 NTDLL) | cycle 34 (Track W3 OLE32) | cycle 35 (Track W4 OLEAUT32) | total delta cycle 31→35 |
|---|---:|---:|---:|---:|---:|---:|
| Win32 fn covered (skeleton+real mixed) | 79 | 94 | 109 | 121 | 132 | +53 |
| total surface (D2R_D4_INSTALL_LAUNCH_ROADMAP cycle 27 ref) | 435 | 435 | 435 | 435 | 435 | 0 |
| % progression | 18.16% | 21.61% | 25.06% | 27.82% | 30.34% | +12.18 pp |
| skeleton-tier fn per cycle | (cum 79) | +15 (KERNEL32 W1) | +15 (NTDLL W2) | +12 (OLE32 W3) | +11 (OLEAUT32 W4) | +53 cumulative |
| module per cycle | pe_advapi32_core.hexa | pe_kernel32_core.hexa (EXTEND) | pe_ntdll_core.hexa (NEW) | pe_ole32_core.hexa (NEW) | pe_oleaut32_core.hexa (NEW) | 1 EXTEND + 3 NEW |

Track W bundled (cycle 32 + 33 + 34 + 35) 측 single-land progression — cycle 31 baseline (Track Q)
위 4 sequential cycle increment 합산 land. cycle 28 측 KERNEL32 18 fn baseline 측 self-similar
pattern mirror — fn_kind_name/fn_group_name canonical map + 4-5 TSV table per module + r0_emit ≥
14 + self_test record ≥ 12 ALL preserved.

production smoke (실 EnterCriticalSection = pthread_mutex_t / 실 CreateMutexW = sem_open + Darwin
mach_port_t / 실 NtCreateFile = Darwin open(2) syscall + namespace mapping / 실 RtlAllocateHeap =
malloc bridge / 실 LdrLoadDll = dlopen / 실 CoInitializeEx = NSXPCConnection bridge / 실
CoCreateInstance = NSObject alloc + IUnknown vtable rosetta / 실 VariantInit = NSValue init / 실
SafeArrayCreate = NSMutableArray + cFArrayRef / 실 IDispatch::Invoke = NSObject performSelector +
NSInvocation) = cycle 36+ 측 multi-cycle long-horizon — fn-by-fn real-vs-stub matrix SSOT 측 별도
cycle (full ~435 fn coverage = many more cycles). 본 cycle = baseline only.

## §6 multi-cycle long-horizon caveat (~435 fn full coverage = many cycles)

KERNEL32 + USER32 + GDI32 + ADVAPI32 + NTDLL + OLE32 + OLEAUT32.dll 측 export fn count 합산
~435+ (Microsoft Windows 10/11 측 7 DLL combined export table 측 대략 추정 — Wine / ReactOS source
측 정확 count 측 인용 0). 본 cycle 35 = 132/435 (~30.34%) 측 8 cycle 누적 baseline. full coverage
측 cycle path:

- cycle 36+: KERNEL32 추가 expansion — DLL load (LoadLibraryW/GetProcAddress/FreeLibrary), TLS
  (TlsAlloc/TlsGetValue/TlsSetValue/TlsFree), file mapping (CreateFileMappingW/MapViewOfFile),
  pipe — 수십 cycle 측 incremental
- cycle 38+: NTDLL 추가 expansion — NtMapViewOfSection + NtCreateSection + RtlNtStatusToDosError
  + LdrEnumerateLoadedModules — cond.3 직접 영역 추가 fn
- cycle 40+: OLE32 추가 expansion — CoTaskMemAlloc/CoTaskMemFree + CoGetClassObject +
  CoCreateGuid + IClassFactory vtable simulation + IStream
- cycle 42+: OLEAUT32 추가 expansion — SysAllocString/SysFreeString/SysStringLen + LoadTypeLib +
  RegisterTypeLib + ITypeInfo vtable simulation
- cycle 50+: WINTRUST + WINHTTP + USERENV + SHLWAPI 측 추가 surface — multi-cycle long-horizon
- cycle 100+: production smoke matrix — fn-by-fn real-vs-stub coverage SSOT + Warden compat
  syscall pattern align cycle 측 합산 후 cond.1 + cond.2 + cond.3 status promotion 측 met 후보
  (1-2 year+ horizon)

직접적 Warden bypass / detection vector 0 — natural environment alignment 측 indirect contribution
(syscall surface naming / synthetic struct shape / fn group classification) 만. 본 cycle 측 full
coverage 도달 = false (의도적, multi-cycle long-horizon 정합).

## §7 caveats inline (honest C3 ≥ 10 numbered)

C1. **KERNEL32/NTDLL/OLE32/OLEAUT32 측 synthetic round-trip — real syscall X**: pe_kernel32_core
    extend + pe_ntdll_core + pe_ole32_core + pe_oleaut32_core 측 self_test() 측 53+8+12+11+11 =
    syscall_invoke 측 (W1 ext 23 record incl ext_event 8 + W2 26 + W3 22 + W4 22 = 93 record) 측
    println("__<DLL>__ PARTIAL <fn>") + r0_emit JSON 만. 실 macOS pthread_mutex_t / sem_open /
    Darwin open(2) / dlopen / NSXPC / NSObject performSelector / NSInvocation 측 syscall 0 —
    production smoke 측 cycle 36+ pending. 의도적 (own1 Wine 0 / hexa-only mandate 정합).

C2. **synthetic CRITICAL_SECTION / OBJECT_ATTRIBUTES / IUnknown vtable / VARIANT — real
    object/handle X**: 4 module 측 byte struct 측 reference shape 만 (windows.h / winnt.h /
    winternl.h / objbase.h / unknwn.h / oaidl.h / oleauto.h 측 standard public header type
    definition 인용, Wine kernel32/ntdll/ole32/oleaut32 source 측 코드 인용 0, ReactOS 측 코드
    인용 0). pseudo-handle range (HKEY_LOCAL_MACHINE 0x80000002 mirror style, IID_IUnknown
    "00000000-0000-0000-C000-000000000046") = MSDN 공개 spec value, Wine source 인용 0.

C3. **D2R-specific synthetic 하드코딩 — D2R single target (L4) reflect**: KERNEL32 측 mutex
    "Global\\D2R_SingleInstance" + NTDLL 측 object_name "\\??\\C:\\Battle.net\\D2R\\Save\\test.d2s"
    + OLEAUT32 측 BSTR_synth "D2R\\0" 측 D2R-specific. D4 측 (Diablo IV.exe / ProductSettings.json
    등) 0 줄 — D4 retired_intentional 측 status enum 측 unchanged 정합 (predecessor self_mk2_tuning
    land 측 명시).

C4. **53 fn / 435 = ~12.18 pp progression 측 cycle 31→35 cumulative baseline 만, full coverage =
    multi-cycle long-horizon**: 본 cycle 측 evidence augment (cond.1 16→21 row + cond.3 7→10 row,
    cond.2 9 row 유지) 만 — cond.* status field promotion (partial → met) 0. full Win32 coverage
    측 production smoke + fn-by-fn real-vs-stub matrix SSOT + ~435 fn 측 multi-cycle work — 본
    cycle 은 53 fn skeleton-tier 측 baseline 만 정착.

C5. **53 fn 측 PARTIAL emit explicit — silent_error_ban (raw 12) 정합**: PARTIAL ≠ PASS. 본 cycle
    측 status emit 53 모두 `__KERNEL32__/__NTDLL__/__OLE32__/__OLEAUT32__ PARTIAL <fn>` 측 명시
    — production smoke 미달 + real-vs-stub 측 stub side 측 honest acknowledgement. PARTIAL swallow
    as PASS = silent_error_ban 위반 = 본 cycle 측 명시적 회피.

C6. **L2 Warden compat = long-horizon, 본 cycle Warden detection vector 0**: Track W 측 KERNEL32
    sync_primitive + NTDLL Nt* + OLE32 COM + OLEAUT32 IDispatch syscall surface 측 baseline =
    Warden detection 측 syscall pattern match 회피 측 indirect contribution (fn name + fn_group
    classification + synthetic struct shape) 만. 실 Warden 측 KERNEL32 EnterCriticalSection
    timing pattern / NTDLL NtQueryInformationProcess heuristic / OLE32 CoCreateInstance CLSID
    table probe / OLEAUT32 IDispatch::Invoke automation injection detection 측 분석 0 — 별도
    Track J (c_bnet_warden_environment_compat) territory (parallel BG, 본 cycle 측 untouched).

C7. **online_expansion_lockin marker block 측 사용자 lock-in 2026-05-03 reflect — verbatim 측 0**:
    user_directive_paraphrase 측 5 항목 (L1 EULA/ToS / L2 Warden long-horizon / L3 multi-year /
    L4 D2R single target / L5 Track J/K/L/O/Q/W parallel) 측 paraphrase 만 — 사용자 원문 인용 0
    (BR-NO-USER-VERBATIM raw 175 정합). marker 측 `online_expansion_lockin` block 5 field 모두
    true + lockin_iso "2026-05-03" + 6 redundant boolean ack field 측 explicit reflection.

C8. **Track W scope 측 J / K / L / M / N / O / P / Q 분리 — 본 cycle 측 cross-link untouched**:
    본 cycle 은 W (KERNEL32 extend + NTDLL + OLE32 + OLEAUT32 bundled) 만 — Track J (lib/perf/
    c_bnet_warden_environment_compat) + Track K (lib/loader/pe_winsock_ws2_32 + pe_wininet_https
    + pe_schannel_tls) + Track L (.roadmap.diablo2_resurrected D2R realm protocol cross-link) +
    Track M (tool/*) + Track N (lib/perf/) + Track O (cycle 28 baseline, W1 EXTEND 측 directly
    extends) + Track P (lib/loader/battlenet_bypass.hexa) + Track Q (pe_user32_core + pe_gdi32_core
    + pe_advapi32_core, predecessor) 측 territory 0 라인 touch (W1 만 Track O 모듈 EXTEND).
    9 track parallel BG land 측 lock-in (L5) — cross-cycle merge audit 별도 cycle.

C9. **runtime exec smoke 측 미수행 — cap_minutes 90 + skeleton-tier mandate 정합**: 본 cycle 측
    pe_kernel32_core extend + pe_ntdll_core + pe_ole32_core + pe_oleaut32_core 측 self_test() 실측
    stdout 캡처 미수행 — predecessor pe_user32_core/pe_gdi32_core/pe_advapi32_core (Track Q cycle
    29-31) + pe_kernel32_core (Track O cycle 28) + pe_winsock_ws2_32/WININET/SCHANNEL (Track K) 측
    hexa runtime smoke 측 docker route fall-through caveat 동급 posture. emit count 53 + 8 ext +
    11 + 10 + 11 = 93 record + LOC 1 EXTEND (689) + 3 NEW (492+478+472) 합산 + grep PARTIAL 337
    (loader 합산) + grep __KERNEL32__/NTDLL/OLE32/OLEAUT32 PARTIAL 75 측 static-validate 만.

C10. **fn_kind range 측 module 별 다른 max — assertion 측 module-local max enforced**: pe_kernel32_core
     fn_kind 0..32 (33 entries, max 32, W1 extension), pe_ntdll_core fn_kind 0..14 (15 entries,
     max 14), pe_ole32_core fn_kind 0..11 (12 entries, max 11), pe_oleaut32_core fn_kind 0..10
     (11 entries, max 10). 각 module 측 record_syscall_invoke 측 fn_kind <= module_max assertion
     별도 enforcement (module 간 fn_kind namespace 분리 — global fn_kind 통합 SSOT 측 cycle 36+
     candidate).

C11. **Track W = bundled 4-cycle land in single Track session — cycle 32 + 33 + 34 + 35 sequential
     numbering preserved**: cycle 32 (KERNEL32 W1 extend), cycle 33 (NTDLL W2 new), cycle 34 (OLE32
     W3 new), cycle 35 (OLEAUT32 W4 new) 측 sequential cycle ID 보존 — single Track W session 내
     4 increment 합산 land. coverage progression metric 측 cycle 별 breakdown explicit (cycle 31
     79 → cycle 32 94 → cycle 33 109 → cycle 34 121 → cycle 35 132). marker
     `win32_coverage_progression` field 측 cycle_27/28/29/30/31/32/33/34/35 reflect.

C12. **W1 KERNEL32 extend additive_only — 기존 18 fn 0 라인 변경**: pe_kernel32_core.hexa 측 cycle
     28 baseline 18 fn (file_io 5 + memory 5 + process 5 + system 3) + fn_kind 0..17 + 4 TSV table
     (syscall_invoke / file_io_round_trip / memory_lifecycle / proc_sys_event) 0 라인 변경. W1
     extension 측 fn_kind 18..32 (sync_primitive 5 + locale 3 + file_extras 4 + thread 3) + 5번째
     TSV table ext_event 추가 + assertion `fn_kind <= 17 → fn_kind <= 32` 단일 line update +
     stats len 4 → 5 + syscall_invoke_count 18 → 33 single-byte assertion adjust (additive_only
     spirit preserved — 기존 18 fn record_syscall_invoke 호출 18 line 0 라인 삭제 / 0 라인 변경,
     기존 4 group fn_group_name 분기 0 라인 변경).

C13. **cond.2 evidence unchanged per spec (KERNEL32+OLE32+OLEAUT32 evidence routed to cond.1
     cluster)**: spec 측 "cond.2/cond.3 status STAYS at current value" + cond.2 evidence array
     unchanged (9 rows preserved from Track Q1+Q2 land). KERNEL32 W1 + OLE32 W3 + OLEAUT32 W4 측
     evidence 는 본 cycle 측 cond.1 (kernel32 + 광의 Win32 syscall surface) cluster append 처리
     — cond.2 (60% threshold per-DLL render coverage, user32 + gdi32 specific) 측 scope 별도. NTDLL
     W2 측 evidence 는 cond.3 (ntdll Nt* syscall bridge) cluster append (Nt* core syscall bridge =
     cond.3 직접 영역).

C14. **OLE32 + OLEAUT32 = D3D11/12 device creation prerequisite**: cross_link section 측
     "loader_dx_dependency: win32 ole32/oleaut32 (COM) = D3D11/12 device creation prerequisite"
     명시 — Track W3 + W4 측 COM apartment + IUnknown vtable + IDispatch + Variant baseline 측
     loader_dx (D3D11/12) 측 dependency unblock partial (실 D3D11CreateDevice = NTDevice 측
     IUnknown::QueryInterface 측 IDXGIFactory probe pattern, automation_layer_dependency 측
     OLEAUT32 IDispatch::Invoke 측 ITypeInfo lookup pattern). loader_dx cond promotion 측 본
     cycle X — multi-cycle long-horizon 정합.

## §8 next-cycle hooks

- **cycle 36+ KERNEL32 추가 expansion** — DLL load (LoadLibraryW/GetProcAddress/FreeLibrary) +
  TLS (TlsAlloc/TlsGetValue/TlsSetValue/TlsFree) + file mapping (CreateFileMappingW/MapViewOfFile)
  + pipe (CreatePipe/PeekNamedPipe) + etc 수십 cycle 측 incremental expansion — Track O+W1 측
  self-similar pattern.
- **cycle 38+ NTDLL 추가 expansion** — NtMapViewOfSection + NtCreateSection + NtUnmapViewOfSection
  + RtlNtStatusToDosError + LdrEnumerateLoadedModules + RtlAcquireSRWLockExclusive — cond.3 직접
  영역 추가 fn, Track W2 측 self-similar pattern.
- **cycle 40+ OLE32 + OLEAUT32 추가 expansion** — CoTaskMemAlloc/CoTaskMemFree + CoGetClassObject
  + CoCreateGuid + IClassFactory + IStream vtable simulation (OLE32) + SysAllocString/SysFreeString
  + LoadTypeLib + RegisterTypeLib + ITypeInfo vtable simulation (OLEAUT32) — Track W3+W4 측
  self-similar pattern.
- **cycle 50+ WINTRUST + WINHTTP + USERENV + SHLWAPI** — signature verify (WinVerifyTrust) + HTTP
  client (WinHttpOpen) + user environment (CreateEnvironmentBlock) + shell helper
  (PathCombineW/PathFileExistsW) — separate Track territory.
- **production smoke cycle (multi-cycle, eta 1-2 year+)** — 실 EnterCriticalSection =
  pthread_mutex_t lock; 실 CreateMutexW = sem_open + mach_port_t; 실 NtCreateFile = Darwin open(2)
  + namespace mapping; 실 RtlAllocateHeap = malloc bridge; 실 LdrLoadDll = dlopen; 실 CoInitializeEx
  = NSXPCConnection bridge; 실 CoCreateInstance = NSObject alloc + IUnknown vtable rosetta; 실
  VariantInit = NSValue init; 실 SafeArrayCreate = NSMutableArray + CFArrayRef; 실 IDispatch::Invoke
  = NSObject performSelector + NSInvocation.
- **loader_win32.cond.1 + cond.2 + cond.3 status promotion partial → met 측 production smoke matrix
  + 60% threshold matrix** — 132 + 추가 fn 측 production smoke 합산 후 fn-by-fn real-vs-stub
  matrix SSOT 측 land + per-DLL real fn count vs stub count ratio matrix + Warden compat syscall
  pattern align cycle 측 합산 후 status 승격 (multi-cycle long-horizon 합산 결과).
- **Track J cross-link audit cycle** — c_bnet_warden_environment_compat 측 KERNEL32 sync timing
  pattern / NTDLL NtQueryInformationProcess heuristic / OLE32 CLSID table probe / OLEAUT32
  IDispatch automation injection detection vector 측 본 Track W 측 fn coverage SSOT 측
  cross-reference (parallel BG agent territory merge audit).
- **online_expansion_lockin L2 long-horizon Warden compat baseline** — KERNEL32 + USER32 + GDI32 +
  ADVAPI32 + NTDLL + OLE32 + OLEAUT32 syscall sequence match + heap pattern match + TEB/PEB shape
  match + sync primitive pattern + COM apartment pattern + IDispatch automation pattern 측
  multi-year roadmap 측 milestone breakdown (cycle 100+ 합산 결과).
- **loader_dx unblock partial cross-link** — Track W3 OLE32 COM apartment + IUnknown vtable +
  W4 OLEAUT32 IDispatch + Variant + SafeArray = D3D11CreateDevice + IDXGIFactory probe baseline
  prerequisite (multi-cycle, loader_dx cond promotion separate cycle).
