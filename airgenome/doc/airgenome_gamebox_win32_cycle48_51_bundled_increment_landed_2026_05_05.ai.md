---
schema: airgenome-gamebox/handoff/win32_cycle48_51_bundled_increment_landed/1
completion_iso: 2026-05-05
track: BD
domain: loader_win32
cycles: [48, 49, 50, 51]
sub_tracks: [BD1, BD2, BD3, BD4]
modules_new: 4
modules_extend: 0
loc_total: 1774
fn_surface_delta: 42
fn_groups_total: 13
roadmap_evidence_added: 8
caveat_count: 14
threshold_60pct_breached: true
threshold_67pct_breached: true
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

Track BD — Win32 cycle 48 ~ 51 bundled 4 sub-cycle skeleton-tier loader land. 4 NEW
loader module 측 시야 baseline coverage 추가, **67% threshold breach milestone**:

- BD1 cycle 48 — `lib/loader/pe_powrprof_core.hexa` (POWRPROF power management, 10 fn 3 group) —
  D2R/D4 idle suspend → resume frame timing recover + Battle.net Agent power-state
  notification baseline
- BD2 cycle 49 — `lib/loader/pe_wbemuuid_core.hexa` (WBEMUUID/WMI COM, 10 fn 3 group) —
  Battle.net Agent system inventory query (CPU/GPU/RAM/MAC) + D2R/D4 anti-cheat baseline
  system fingerprint + Warden compat hardware inventory baseline
- BD3 cycle 50 — `lib/loader/pe_wtsapi32_core.hexa` (WTSAPI32 terminal services, 10 fn 3 group) —
  D2R/D4 single-session check + Battle.net Agent fast-user-switching detect + console
  session id baseline
- BD4 cycle 51 — `lib/loader/pe_iphlpapi_core.hexa` (IPHLPAPI IP helper, 12 fn 4 group) —
  D2R/D4 멀티-NIC routing + Battle.net REST endpoint 24.105.62.129 p2p hole-punching
  adapter discovery + DNS resolver baseline

Win32 coverage progression cycle 47 = 262/435 (60.23%) → cycle 51 = 304/435 (69.89%) 누적
+42 fn skeleton-tier delta across 4 modules (Track BD bundled 4-cycle increment).
**67% threshold breached at cycle 50** — first crossing of 67% Win32 fn coverage since
Track AZ4 60% threshold breach at cycle 47, trending toward 70% threshold (cycle 51 = 69.89%).
`.roadmap.loader_win32` cond.1 evidence +3 row + cond.2 evidence +2 row + cond.3 evidence
+3 row = 8 row additive augment, status STAYS at current (partial × 3, full ~435 fn
coverage = multi-cycle long-horizon).

# §2 Modules created (4 NEW)

## §2.1 BD1 — pe_powrprof_core.hexa (POWRPROF, cycle 48)

- **Path**: `lib/loader/pe_powrprof_core.hexa`
- **LOC**: 429 / **bytes**: 26565
- **sha256**: `a6de50de66068b8822077e98f95d0e27372d0e4b475b0a8e527300f3e090347f`
- **DLL target**: POWRPROF.dll
- **fn surface**: 10 / fn_kind 0..9 / 3 group
  - power_state (4): GetSystemPowerStatus / SetSystemPowerState / SetSuspendState / PowerEnumerate
  - schemes (3): PowerGetActiveScheme / PowerSetActiveScheme / PowerWriteACValueIndex
  - notifications (3): PowerRegisterSuspendResumeNotification / PowerUnregisterSuspendResumeNotification / RegisterPowerSettingNotification
- **emit format**: `__POWRPROF__ PARTIAL <fn>` (10 fn × 1 emit = 10 println markers + 11 grep total)
- **TSV tables (4)**: syscall_invoke / power_state_event / scheme_event / notify_event
- **Synthetic struct families**: SYSTEM_POWER_STATUS 12B AC 1 BattFlag 0x08 BattLife 0x42 66pct +
  GUID_TYPICAL_POWER_SAVINGS 381b4222 / GUID_HIGH_PERFORMANCE 8c5e7fda / GUID_MIN_POWER_SAVINGS
  a1841308 16B LE + HPOWERNOTIFY 0x9C010100 + POWERBROADCAST_SETTING 12B + DEVICE_NOTIFY_CALLBACK_ROUTINE 8B
- **Real syscall**: false / Real powrprof.dll binding: false / IOPMrootDomain·IOPMAssertion·pmset·NSWorkspace
  willSleep/didWake binding: 0
- **D2R/D4 idle resume baseline**: true / Battle.net Agent power-state notification baseline: true

## §2.2 BD2 — pe_wbemuuid_core.hexa (WBEMUUID/WMI COM, cycle 49)

- **Path**: `lib/loader/pe_wbemuuid_core.hexa`
- **LOC**: 432 / **bytes**: 26957
- **sha256**: `a5ae94620e037306d51684cc40ea345beae68e45fb81302d42c8b20a2d3c07be`
- **DLL target**: WBEMUUID/wbemprox.dll
- **fn surface**: 10 / fn_kind 0..9 / 3 group
  - locator (3): IWbemLocator::ConnectServer / CoCreateInstance(CLSID_WbemLocator) / IWbemContext init
  - service (4): IWbemServices::ExecQuery / GetObject / CreateInstanceEnum / ExecMethod
  - object_enum (3): IEnumWbemClassObject::Next / Reset / Skip
- **emit format**: `__WBEMUUID__ PARTIAL <fn>` (10 fn × 1 emit = 10 println markers + 11 grep total)
- **TSV tables (4)**: syscall_invoke / locator_event / service_event / enum_event
- **Synthetic struct families**: IID_IWbemLocator dc12a687 / CLSID_WbemLocator 4590f811 /
  IID_IWbemServices 9556dc99 / IID_IEnumWbemClassObject 027947e1 / IID_IWbemClassObject dc12a681 /
  IID_IWbemContext 44aca674 16B LE + IWbemLocator vtable 32B + IWbemServices vtable 88B +
  IWbemClassObject vtable 152B + BSTR Namespace ROOT__CIMV2 24B WCHAR LE + WQL
  SELECT__FROM_Win32_Processor / Win32_OperatingSystem / Win32_VideoController
- **Real syscall**: false / Real wbemprox.dll binding: false / IOKit IORegistryEntry·IOServiceMatching·
  system_profiler·ioreg·sysctl(3) binding: 0
- **Battle.net Agent inventory baseline**: true / D2R/D4 anti-cheat inventory baseline: true /
  Warden compat hardware inventory baseline: true

## §2.3 BD3 — pe_wtsapi32_core.hexa (WTSAPI32 terminal services, cycle 50)

- **Path**: `lib/loader/pe_wtsapi32_core.hexa`
- **LOC**: 428 / **bytes**: 26068
- **sha256**: `d1634210571091402160b3e0d6996fda9dd9dd78aa30136aeb82730101f61006`
- **DLL target**: WTSAPI32.dll
- **fn surface**: 10 / fn_kind 0..9 / 3 group
  - sessions (4): WTSEnumerateSessions / WTSQuerySessionInformation / WTSDisconnectSession / WTSLogoffSession
  - connections (3): WTSOpenServer / WTSCloseServer / WTSGetActiveConsoleSessionId
  - notifications (3): WTSRegisterSessionNotification / WTSUnRegisterSessionNotification / WTSWaitSystemEvent
- **emit format**: `__WTSAPI32__ PARTIAL <fn>` (10 fn × 1 emit = 10 println markers + 11 grep total)
- **TSV tables (4)**: syscall_invoke / session_event / connection_event / notification_event
- **Synthetic struct families**: WTS_SESSION_INFO 24B SessionId 1 Console WTSActive 0 +
  WTS_INFO_CLASS WTSUserName 5 / WTSWinStationName 6 / WTSDomainName 7 / WTSConnectState 8 +
  HSERVER 0x9E010000 + WTS_CURRENT_SERVER_HANDLE 0 + HWND notify 0x9E020100 + wait_event_mask DWORD
  WTS_EVENT_LOGON 0x20 / LOGOFF 0x40 / CONNECT 0x08
- **Real syscall**: false / Real wtsapi32.dll binding: false / loginwindow·SCDynamicStore·
  SystemConfiguration·CGSession·NSWorkspace sessionDidBecomeActive binding: 0
- **D2R/D4 single-session baseline**: true / Battle.net Agent fast-user-switching baseline: true

## §2.4 BD4 — pe_iphlpapi_core.hexa (IPHLPAPI IP helper, cycle 51)

- **Path**: `lib/loader/pe_iphlpapi_core.hexa`
- **LOC**: 485 / **bytes**: 30332
- **sha256**: `32629e731d9cf03618ef08816d287d3c0421c3eef932e2d7ea854397d8f9b16e`
- **DLL target**: IPHLPAPI.dll
- **fn surface**: 12 / fn_kind 0..11 / 4 group
  - adapter (4): GetAdaptersInfo / GetAdaptersAddresses / GetIpAddrTable / GetInterfaceInfo
  - routing (4): GetIpForwardTable / CreateIpForwardEntry / DeleteIpForwardEntry / GetBestRoute
  - arp (2): GetIpNetTable / SendARP
  - dns (2): GetNetworkParams / NotifyAddrChange
- **emit format**: `__IPHLPAPI__ PARTIAL <fn>` (12 fn × 1 emit = 12 println markers + 13 grep total)
- **TSV tables (5)**: syscall_invoke / adapter_event / routing_event / arp_event / dns_event
- **Synthetic struct families**: IP_ADAPTER_INFO 640B Apple M Synthetic Ethernet
  02-50-41-50-50-4C 192.168.1.42 mask 24 gw 192.168.1.1 + IP_ADAPTER_ADDRESSES_LH dual stack v6/v4 +
  MIB_IPADDRROW 24B + IP_INTERFACE_INFO 260B + MIB_IPFORWARDTABLE 8B header + MIB_IPFORWARDROW 56B
  dest 24.105.62.129 Battle.net REST via gw metric 25 + MIB_IPNETTABLE 8B header + MIB_IPNETROW 24B
  ARP cache dynamic + FIXED_INFO 524B HostName DESKTOP-D2R DomainName blizzard.com DnsServerList
  8.8.8.8 1.1.1.1 + OVERLAPPED 32B hEvent 0x9F010100 NotifyAddrChange async
- **Real syscall**: false / Real iphlpapi.dll binding: false / BSD getifaddrs(3)·sysctl(NET_RT_DUMP/IFLIST)·
  route(4)·SystemConfiguration SCDynamicStore·Network.framework nw_path_monitor_t binding: 0
- **D2R/D4 멀티-NIC routing baseline**: true / Battle.net REST 24.105.62.129 p2p baseline: true /
  ntdll-adjacent network helper family-extension to Nt*/Rtl*/WS2_32 core syscall bridge: true

# §3 Roadmap surgical edit (.roadmap.loader_win32)

## §3.1 cond.1 evidence augment (+3 row)

Status: partial → partial (UNCHANGED — coverage milestone NOT status promotion criterion).
Evidence count: 30 → 33.

- BD1 POWRPROF 10 fn skeleton-tier (cycle 48, power_state/schemes/notifications)
- BD2 WBEMUUID/WMI COM 10 fn skeleton-tier (cycle 49, locator/service/object_enum)
- Win32 cycle 48-49 bundled coverage progression row (Track BD1+BD2): 262/435 → 282/435 +20 fn delta

blocker_reason refresh: appended Win32 cycle 48-49 bundled increment landed via Track BD1+BD2
(POWRPROF + WBEMUUID = 20 fn skeleton-tier across 2 modules, 262/435→282/435 ~64.83%) — Track BD
part 1+2/4 of 4 cycle bundled cycle 48-51.

## §3.2 cond.2 evidence augment (+2 row)

Status: partial → partial (UNCHANGED — production smoke matrix + per-DLL real fn count vs stub
count ratio = multi-cycle long-horizon).
Evidence count: 15 → 17.

- BD3 WTSAPI32 10 fn skeleton-tier (cycle 50, sessions/connections/notifications, terminal services
  session surface family-extension to UI/IME/text-shaping family)
- USER32+GDI32+COMCTL32+IMM32+USP10+WTSAPI32 cumulative coverage progression row (Track Q1+Q2+AH3+AM3+AZ4+BD3
  UI/input/text-shaping/session surface family): cycle 47 → cycle 50 +75 fn skeleton-tier across 6 modules

blocker_reason refresh: appended WTSAPI32 cycle 50 increment landed via Track BD3 (terminal services
session surface family-extension to UI+IME+text-shaping surface family, 282/435→292/435 ~67.13%).

## §3.3 cond.3 evidence augment (+3 row)

Status: partial → partial (UNCHANGED — 67% threshold = coverage milestone NOT status promotion criterion;
3 syscall hexa-native bridge 실측 미수행).
Evidence count: 18 → 21.

- BD4 IPHLPAPI 12 fn skeleton-tier (cycle 51, adapter/routing/arp/dns, ntdll-adjacent network helper
  family-extension to Nt*/Rtl*/WS2_32 core syscall bridge)
- Track BD bundled cycle 48-51 cumulative coverage progression row (BD1+BD2+BD3+BD4): 262/435 → 304/435
  +42 fn delta across 4 modules
- 67% threshold breach milestone row + Track BD bundled complete + cumulative since cycle 28: 296 fn
  skeleton-tier across 24 module-cycles

blocker_reason refresh: appended IPHLPAPI cycle 51 increment landed via Track BD4 (lib/loader/
pe_iphlpapi_core.hexa, +12 fn skeleton-tier adapter/routing/arp/dns, ntdll-adjacent network helper
surface family-extension, 292/435→304/435 ~69.89%) + Win32 cycle 48-51 bundled increment landed via
Track BD (4 modules total +42 fn) + **67% threshold breach milestone**.

## §3.4 Preserved unchanged

- Header line 1-4 comments
- blockers array (loader_win32.blk.1 kernel-AC class, status=open)
- cross_link section (sister_domains, mk1_narrative_refs, plugin_contract)
- status: active / since: 2026-05-03

# §4 Win32 coverage progression (cycle 27..51)

| Cycle | Count | Percent | Module / Track |
|------:|------:|--------:|----------------|
| 27 | 8/435 | 1.84% | KERNEL32 mixed (baseline) |
| 28 | 26/435 | 5.98% | pe_kernel32_core.hexa NEW (Track O) |
| 29 | 46/435 | 10.57% | pe_user32_core.hexa NEW (Track Q1) |
| 30 | 61/435 | 14.02% | pe_gdi32_core.hexa NEW (Track Q2) |
| 31 | 79/435 | 18.16% | pe_advapi32_core.hexa NEW (Track Q3) |
| 32 | 94/435 | 21.61% | pe_kernel32_core.hexa EXTEND (Track W1) |
| 33 | 109/435 | 25.06% | pe_ntdll_core.hexa NEW (Track W2) |
| 34 | 121/435 | 27.82% | pe_ole32_core.hexa NEW (Track W3) |
| 35 | 132/435 | 30.34% | pe_oleaut32_core.hexa NEW (Track W4) |
| 36 | 144/435 | 33.10% | pe_msvcrt_core.hexa NEW (Track AH1) |
| 37 | 156/435 | 35.86% | pe_shell32_core.hexa NEW (Track AH2) |
| 38 | 166/435 | 38.16% | pe_comctl32_core.hexa NEW (Track AH3) |
| 39 | 176/435 | 40.46% | pe_dxgi_core.hexa NEW (Track AH4) |
| 40 | 188/435 | 43.22% | pe_winmm_core.hexa NEW (Track AM1) |
| 41 | 198/435 | 45.52% | pe_xaudio2_core.hexa NEW (Track AM2) |
| 42 | 208/435 | 47.82% | pe_imm32_core.hexa NEW (Track AM3) |
| 43 | 218/435 | 50.11% | pe_setupapi_core.hexa NEW (Track AM4) **50% breach** |
| 44 | 230/435 | 52.87% | pe_crypt32_core.hexa NEW (Track AZ1) |
| 45 | 242/435 | 55.63% | pe_ws2_32_extras.hexa NEW (Track AZ2) |
| 46 | 252/435 | 57.93% | pe_dbghelp_core.hexa NEW (Track AZ3) |
| 47 | 262/435 | 60.23% | pe_usp10_core.hexa NEW (Track AZ4) **60% breach** |
| **48** | **272/435** | **62.53%** | **pe_powrprof_core.hexa NEW (Track BD1)** |
| **49** | **282/435** | **64.83%** | **pe_wbemuuid_core.hexa NEW (Track BD2)** |
| **50** | **292/435** | **67.13%** | **pe_wtsapi32_core.hexa NEW (Track BD3) 67% breach** |
| **51** | **304/435** | **69.89%** | **pe_iphlpapi_core.hexa NEW (Track BD4) trending 70%** |

Track BD bundled delta cycle 47→51: +42 fn skeleton-tier (+9.66 pp).
Cumulative delta cycle 28→51: +278 fn (+63.91 pp).

# §5 Verifier emit log samples (2026-05-05)

`__POWRPROF__ PARTIAL GetSystemPowerStatus` / `SetSystemPowerState` / `SetSuspendState` / `PowerEnumerate`
/ `PowerGetActiveScheme` / `PowerSetActiveScheme` / `PowerWriteACValueIndex` /
`PowerRegisterSuspendResumeNotification` / `PowerUnregisterSuspendResumeNotification` /
`RegisterPowerSettingNotification` (10)

`__WBEMUUID__ PARTIAL IWbemLocator_ConnectServer` / `CoCreateInstance_CLSID_WbemLocator` /
`IWbemContext_init` / `IWbemServices_ExecQuery` / `GetObject` / `CreateInstanceEnum` / `ExecMethod` /
`IEnumWbemClassObject_Next` / `Reset` / `Skip` (10)

`__WTSAPI32__ PARTIAL WTSEnumerateSessions` / `WTSQuerySessionInformation` / `WTSDisconnectSession` /
`WTSLogoffSession` / `WTSOpenServer` / `WTSCloseServer` / `WTSGetActiveConsoleSessionId` /
`WTSRegisterSessionNotification` / `WTSUnRegisterSessionNotification` / `WTSWaitSystemEvent` (10)

`__IPHLPAPI__ PARTIAL GetAdaptersInfo` / `GetAdaptersAddresses` / `GetIpAddrTable` / `GetInterfaceInfo` /
`GetIpForwardTable` / `CreateIpForwardEntry` / `DeleteIpForwardEntry` / `GetBestRoute` / `GetIpNetTable` /
`SendARP` / `GetNetworkParams` / `NotifyAddrChange` (12)

Smoke status: static_validate_only_no_runtime_exec_in_this_cycle.

# §6 Natural environment alignment posture

- **Goal**: Win32 syscall perfection rosetta toward L2 Warden detection compat —
  KERNEL32 + USER32 + GDI32 + ADVAPI32 + NTDLL + OLE32 + OLEAUT32 + MSVCRT + SHELL32 + COMCTL32 +
  DXGI + WINMM + XAudio2 + IMM32 + SETUPAPI + CRYPT32 + WS2_32(extras) + DBGHELP + USP10 +
  POWRPROF + WBEMUUID + WTSAPI32 + IPHLPAPI direct contributor (real Windows binary 측 가장 빈번 호출
  surface 23 family).
- **Horizon**: long_horizon_multi_year_L2_L3
- **cycle_47_baseline**: 262/435 USP10 60% threshold breach
- **cycle_48_baseline**: 272/435 (+10 POWRPROF skeleton-tier via BD1, D2R/D4 idle resume + Battle.net
  Agent power-state notification baseline)
- **cycle_49_baseline**: 282/435 (+10 WBEMUUID/WMI COM skeleton-tier via BD2, Battle.net Agent system
  inventory query CPU/GPU/RAM/MAC + D2R/D4 anti-cheat baseline system fingerprint baseline)
- **cycle_50_baseline**: 292/435 (+10 WTSAPI32 skeleton-tier via BD3, D2R/D4 single-session check +
  Battle.net Agent fast-user-switching detect baseline; **67% threshold breach milestone**)
- **cycle_51_baseline**: 304/435 (+12 IPHLPAPI skeleton-tier via BD4, D2R/D4 멀티-NIC routing +
  Battle.net REST 24.105.62.129 p2p hole-punching adapter discovery + DNS resolver baseline; trending
  toward 70% threshold)
- **cycle_n_target**: production smoke (real GetSystemPowerStatus = IOPMrootDomain + IOPMAssertion +
  pmset / real ExecQuery = IOKit IORegistryEntry + system_profiler + sysctl(3) / real
  WTSEnumerateSessions = loginwindow + SCDynamicStore + CGSession + NSWorkspace sessionDidBecomeActive /
  real GetAdaptersInfo = BSD getifaddrs(3) + sysctl(NET_RT_DUMP/IFLIST) + route(4) +
  SystemConfiguration SCDynamicStore + Network.framework nw_path_monitor_t) + Warden detection compat
- **direct_warden_bypass_vector**: none_in_this_cycle
- **indirect_contribution**: fn_naming_baseline_42_fn_delta_296_cumulative + fn_group_classification_
  13_group_delta_61_total + synthetic_struct_shape (SYSTEM_POWER_STATUS_12B + GUID_HIGH_PERFORMANCE_16B +
  IID_IWbemLocator_16B + IWbemServices_vtable_88B + WTS_SESSION_INFO_24B + IP_ADAPTER_INFO_640B +
  MIB_IPFORWARDROW_56B + FIXED_INFO_524B synth_only)
- **Wine source citation**: 0 / ReactOS source citation: 0 / Real DLL binding: 0 /
  IOPMrootDomain·IOPMAssertion·pmset binding: 0 / IOKit IORegistryEntry·system_profiler·sysctl(3) binding: 0 /
  loginwindow·SCDynamicStore·CGSession·NSWorkspace binding: 0 / BSD getifaddrs(3)·sysctl·
  Network.framework binding: 0 / OSS Win32 emulator import: 0

# §7 Caveats inline (14)

- **C1**: Production smoke 측 미수행 — 4 module 모두 PARTIAL emit only, runtime exec smoke separate cycle.
- **C2**: Warden compat 측 long-horizon (multi-year L2 horizon) — 본 cycle direct bypass vector 0.
- **C3**: 사용자 verbatim 0 in any of 7 created/modified files (BR-NO-USER-VERBATIM 정합).
- **C4**: L4 D2R single target 측 path 하드코딩 (24.105.62.129 Battle.net REST endpoint synth only).
- **C5**: L5 J/K/L/M/N/O/P/Q/W/AH/AM/AZ/BC parallel BG cross-link untouched (out-of-scope).
- **C6**: Runtime exec smoke 측 미수행 (skeleton-tier mandate 정합).
- **C7**: fn_kind range module 별 다른 max — BD1/BD2/BD3 = 0..9 (10 fn) / BD4 = 0..11 (12 fn).
- **C8**: Track BD bundled 4-cycle land in single Track session.
- **C9**: cond.1 + cond.2 + cond.3 status partial → partial 유지 (no status promotion).
- **C10**: BD3 WTSAPI32 = 67% threshold breach milestone module (cycle 50 = 292/435 67.13%).
- **C11**: 67% threshold breach = coverage milestone NOT status promotion criterion.
- **C12**: BD2 WBEMUUID = COM-based WMI inventory surface (CoCreateInstance + IUnknown vtable pattern,
  loader_dx OLE32 prerequisite cross-link consumed).
- **C13**: BD4 IPHLPAPI = ntdll-adjacent network helper family-extension to Nt*/Rtl*/WS2_32 core
  syscall bridge — distinct from Track K1 pe_winsock_ws2_32.hexa basic + Track AZ2 pe_ws2_32_extras.hexa
  advanced; iphlpapi → BSD network sysctl/getifaddrs pass-through pattern (cond.3 직접 영역
  evidence augment).
- **C14**: docs/HEXA_UPSTREAM.md untouched (parallel BG track BC scope, out-of-scope merge audit).

# §8 Raw compliance

- **raw_9 hexa_only**: verifier emit native (println __POWRPROF__/__WBEMUUID__/__WTSAPI32__/__IPHLPAPI__
  PARTIAL <fn> ×42 delta + r0_emit JSONL ×112) — Wine 0 / CrossOver 0 / Whisky 0 / GPTK 0 줄.
  ReactOS / Wine powrprof/wbem/wtsapi32/iphlpapi source 측 코드 인용 0. IOKit / SystemConfiguration /
  Network.framework / IOPMrootDomain / loginwindow / BSD socket(2) / getifaddrs(3) 0 줄.
- **raw_10 honest_c3**: 14 caveats inline (C1-C14) in handoff doc §7.
- **raw_11 snake_case**: all field names snake_case throughout JSONL roadmap edit + JSON marker +
  r0_emit event_type.
- **raw_12 silent_error_ban**: every status_emit explicit (4 module × 42 println markers + 112 r0_emit
  records). PARTIAL ≠ PASS.
- **raw_15 env_lazy**: all paths repo-relative (lib/loader/* + docs/* + state/markers/* +
  .roadmap.loader_win32) — no absolute leaks.
- **raw_175 br_no_user_verbatim**: 0 verbatim user quotes in any of 7 created/modified files.

# §9 Next cycle actions

- cycle 52+ POWRPROF extend — PowerReadACValueIndex / PowerEnumerateValuesIndices /
  CallNtPowerInformation / SetThreadExecutionState — Track BD1 self-similar pattern
- cycle 53+ WBEMUUID extend — IWbemClassObject::Get / Put / GetMethodOrigin / SpawnDerivedClass +
  IWbemQualifierSet — Track BD2 self-similar pattern
- cycle 54+ WTSAPI32 extend — WTSQueryUserToken / WTSSendMessage / WTSStartRemoteControlSession +
  WTSFreeMemory — Track BD3 self-similar pattern
- cycle 55+ IPHLPAPI extend — GetExtendedTcpTable / GetExtendedUdpTable / GetTcpStatistics /
  GetUdpStatistics / IcmpSendEcho / IcmpCreateFile — Track BD4 self-similar pattern
- cycle 60+ 70% threshold-breach completion — IPHLPAPI cycle 51 = 304/435 (~69.89%) trending toward
  cycle 52 (~71%) via BD4 extend + cycle 53+ D3D11/D3D9/XInput/DirectInput/DSOUND/MSCTF cumulative
- production smoke cycle (multi-cycle, eta 1-2 year+) — real GetSystemPowerStatus =
  IOPMrootDomain+IOPMAssertion+pmset / real ExecQuery = IOKit IORegistryEntry+sysctl(3) / real
  WTSEnumerateSessions = loginwindow+SCDynamicStore+CGSession / real GetAdaptersInfo = BSD
  getifaddrs(3)+sysctl(NET_RT_DUMP/IFLIST)+SystemConfiguration SCDynamicStore+
  Network.framework nw_path_monitor_t
- loader_win32.cond.1 + cond.2 + cond.3 status promotion partial → met 측 production smoke matrix +
  Nt* syscall bridge 실측 합산 후 fn-by-fn real-vs-stub matrix SSOT 측 land + per-DLL real fn count vs
  stub count ratio matrix + Warden compat syscall pattern align cycle 측 합산 후 status 승격
  (multi-cycle long-horizon 합산 결과). 67% threshold breach = coverage milestone, NOT status promotion
  criterion.

# §10 Files created / modified

## §10.1 Files created (6)

| Path | Type | LOC | Bytes | sha256 |
|------|------|----:|------:|--------|
| `lib/loader/pe_powrprof_core.hexa` | loader_module | 429 | 26565 | a6de50de66068b8822077e98f95d0e27372d0e4b475b0a8e527300f3e090347f |
| `lib/loader/pe_wbemuuid_core.hexa` | loader_module | 432 | 26957 | a5ae94620e037306d51684cc40ea345beae68e45fb81302d42c8b20a2d3c07be |
| `lib/loader/pe_wtsapi32_core.hexa` | loader_module | 428 | 26068 | d1634210571091402160b3e0d6996fda9dd9dd78aa30136aeb82730101f61006 |
| `lib/loader/pe_iphlpapi_core.hexa` | loader_module | 485 | 30332 | 32629e731d9cf03618ef08816d287d3c0421c3eef932e2d7ea854397d8f9b16e |
| `airgenome/doc/airgenome_gamebox_win32_cycle48_51_bundled_increment_landed_2026_05_05.ai.md` | handoff_doc | (this) | (this) | (self) |
| `state/markers/airgenome_gamebox_win32_cycle48_51_bundled_increment_landed.marker` | marker | (small) | (small) | (self) |

## §10.2 Files modified (1)

| Path | Type | Edit kind |
|------|------|-----------|
| `.roadmap.loader_win32` | roadmap_jsonl_header | surgical_jsonl_field_edit_evidence_augment_only (+8 evidence row + 3 blocker_reason refresh; 0 cond.N row deleted, 0 status field promoted) |

# §11 Online expansion lockin (carry from 2026-05-03)

- L1 (EULA/ToS): true
- L2 (Warden long-horizon): true
- L3 (multi-year): true
- L4 (D2R single target / D4 retired_intentional unchanged): true
- L5 (Track J/K/L/O/Q/W/AH/AM/AZ/BC parallel BG): true
- lockin_iso: 2026-05-03

eula_violation_risk_acknowledged: true
warden_detection_risk_acknowledged: true
multi_year_horizon_acknowledged: true
d2r_single_target_lockin: true
d4_retired_intentional_unchanged: true
track_jklo_parallel_lockin: true
track_q_bundled_cycle_29_31: true
track_w_bundled_cycle_32_35: true
track_ah_bundled_cycle_36_39: true
track_am_bundled_cycle_40_43: true
track_az_bundled_cycle_44_47: true
track_bd_bundled_cycle_48_51: true
threshold_50pct_breached: true
threshold_60pct_breached: true
threshold_67pct_breached: true
