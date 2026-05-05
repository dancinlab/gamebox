---
schema: airgenome-gamebox/handoff/win32_cycle36_39_bundled_increment_landed/1
completion_iso: 2026-05-04
track: AH
domain: loader_win32
cycles: [36, 37, 38, 39]
sub_tracks: [AH1, AH2, AH3, AH4]
modules_new: 4
modules_extend: 0
loc_total: 1891
fn_surface_delta: 44
fn_groups_total: 14
roadmap_evidence_added: 7
caveat_count: 14
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

Track AH — Win32 cycle 36 ~ 39 bundled 4 sub-cycle skeleton-tier loader land. 4 NEW
loader module 측 시야 baseline coverage 추가:

- AH1 cycle 36 — `lib/loader/pe_msvcrt_core.hexa` (MSVCRT C runtime, 12 fn 3 group)
- AH2 cycle 37 — `lib/loader/pe_shell32_core.hexa` (SHELL32 Shell APIs, 12 fn 4 group)
- AH3 cycle 38 — `lib/loader/pe_comctl32_core.hexa` (COMCTL32 Common Controls, 10 fn 4 group)
- AH4 cycle 39 — `lib/loader/pe_dxgi_core.hexa` (DXGI swap chain D3D11 prerequisite, 10 fn 3 group)

Win32 coverage progression cycle 35 = 132/435 (30.34%) → cycle 39 = 176/435 (40.46%) 누적
+44 fn skeleton-tier delta across 4 modules (Track AH bundled 4-cycle increment).
`.roadmap.loader_win32` cond.1 evidence +3 row + cond.2 evidence +2 row + cond.3 evidence
+2 row = 7 row additive augment, status STAYS at current (partial × 3, full ~435 fn
coverage = multi-cycle long-horizon).

# §2 Bundled 4-cycle table

| cycle | track | module | LOC | bytes | fn | groups | sha256 |
|-------|-------|--------|-----|-------|----|--------|--------|
| 36 | AH1 | lib/loader/pe_msvcrt_core.hexa | 470 | 25339 | 12 | 3 (file_io 5 + memory 3 + string 4) | `4bf2a79ad2484a8263aba1616136893b6686cf0ed12524809aeb9ddb47201626` |
| 37 | AH2 | lib/loader/pe_shell32_core.hexa | 480 | 27696 | 12 | 4 (path 4 + process 3 + drag_drop 3 + misc 2) | `b1a91a23af042779293ff109d135f7601cb111ede991701610a09c855e82c14d` |
| 38 | AH3 | lib/loader/pe_comctl32_core.hexa | 464 | 25432 | 10 | 4 (init 2 + imagelist 3 + listview 3 + treeview 2) | `8492228175707191e6b49fcafd6f1b2642f21cab35be93993d61bfae544c1609` |
| 39 | AH4 | lib/loader/pe_dxgi_core.hexa | 477 | 28570 | 10 | 3 (factory 3 + swapchain 4 + adapter 3) | `f03ad0f7738d963176edb68185f1b040e001ef6e77898e4feb1185ec79dc8119` |
| **total** | **AH bundled** | **4 modules** | **1891** | **107037** | **44** | **14 groups** | — |

# §3 Coverage progression

cycle 27 = 8/435 (1.84%) → cycle 28 = 26/435 (5.98%) +18 KERNEL32 (Track O) → cycle 29 =
46/435 (10.57%) +20 USER32 (Q1) → cycle 30 = 61/435 (14.02%) +15 GDI32 (Q2) → cycle 31 =
79/435 (18.16%) +18 ADVAPI32 (Q3) → cycle 32 = 94/435 (21.61%) +15 KERNEL32 EXTEND (W1) →
cycle 33 = 109/435 (25.06%) +15 NTDLL (W2) → cycle 34 = 121/435 (27.82%) +12 OLE32 (W3) →
cycle 35 = 132/435 (30.34%) +11 OLEAUT32 (W4) → cycle 36 = 144/435 (33.10%) +12 MSVCRT
(AH1) → cycle 37 = 156/435 (35.86%) +12 SHELL32 (AH2) → cycle 38 = 166/435 (38.16%) +10
COMCTL32 (AH3) → cycle 39 = 176/435 (40.46%) +10 DXGI (AH4).

Track AH 4-sub-cycle delta = +44 fn (+10.12 pp). Track AH4 측 cycle 39 coverage 40.46% =
40% threshold 도달 (cumulative since Track O cycle 28 = 168 fn delta).

# §4 fn surface table per module

## AH1 — MSVCRT (12 fn)
- file_io (5): fopen / fread / fwrite / fclose / fprintf
- memory (3): malloc / free / realloc
- string (4): strlen / strcpy / strcmp / sprintf

## AH2 — SHELL32 (12 fn)
- path (4): SHGetFolderPathW / SHGetSpecialFolderPathW / PathCombineW / PathAppendW
- process (3): ShellExecuteW / ShellExecuteExW / FindExecutableW
- drag_drop (3): DragAcceptFiles / DragQueryFileW / DragFinish
- misc (2): SHCreateDirectoryExW / SHGetFileInfoW

## AH3 — COMCTL32 (10 fn)
- init (2): InitCommonControls / InitCommonControlsEx
- imagelist (3): ImageList_Create / ImageList_Add / ImageList_Destroy
- listview (3): ListView_InsertItem / ListView_SetItemText / ListView_GetItemCount
- treeview (2): TreeView_InsertItem / TreeView_DeleteItem

## AH4 — DXGI (10 fn) — D3D11 prerequisite
- factory (3): CreateDXGIFactory / CreateDXGIFactory2 / IDXGIFactory_EnumAdapters
- swapchain (4): IDXGIFactory_CreateSwapChain / IDXGISwapChain_Present /
  IDXGISwapChain_ResizeBuffers / IDXGISwapChain_GetBuffer
- adapter (3): IDXGIAdapter_EnumOutputs / IDXGIAdapter_GetDesc /
  IDXGIAdapter_CheckInterfaceSupport

# §5 .roadmap.loader_win32 evidence augment

`.roadmap.loader_win32` JSONL header surgical edit (additive_only, evidence array append
only, status field preserved):

| cond_id | status_before | status_after | evidence_before | evidence_after | delta |
|---------|---------------|--------------|-----------------|----------------|-------|
| loader_win32.cond.1 | partial | partial (unchanged) | 21 | 24 | +3 (AH1 MSVCRT + AH2 SHELL32 + cycle 35→37 progression) |
| loader_win32.cond.2 | partial | partial (unchanged) | 9 | 11 | +2 (AH3 COMCTL32 + USER32+GDI32+COMCTL32 cumulative) |
| loader_win32.cond.3 | partial | partial (unchanged) | 10 | 12 | +2 (AH4 DXGI + Track AH cumulative cycle 36-39) |

Total evidence augment = +7 row. blocker_reason refresh on cond.1/2/3 acknowledging Win32
cycle 36-39 bundled increment landed (MSVCRT + SHELL32 + COMCTL32 + DXGI). Existing
evidence rows + blocker_reason text preserved (additive append only). header line 1-4
comments + blockers array (loader_win32.blk.1 kernel-AC class) + cross_link section +
status active + since 2026-05-03 모두 보존.

# §6 Synthetic struct families

| module | family | byte size | example value |
|--------|--------|-----------|---------------|
| MSVCRT | FILE* synth | 8 | 0x90010100 (fd_synth 4) |
| MSVCRT | size_t LE | 8 | 0x40 (64) / 0x100 (256) |
| MSVCRT | heap addr | n/a | malloc 0x90100000 / realloc 0x90100400 |
| MSVCRT | strlen result | size_t | "D2R" → 3, "airgenome-gamebox" → 17 |
| MSVCRT | strcmp result | int LE | abc/abd = -1 (0xFFFFFFFF) |
| SHELL32 | CSIDL constants | 4 | 0x05 PERSONAL / 0x1A APPDATA / 0x1C LOCAL_APPDATA / 0x26 PROGRAM_FILES |
| SHELL32 | SHFILEINFO | 696 | dwAttributes 0x80 (SFGAO_FILESYSTEM), szTypeName "D2R Save File" |
| SHELL32 | HDROP synth | n/a | 0x91030000 (count 2 filenames) |
| SHELL32 | HINSTANCE return | n/a | 0x21 (33 = success > 32) |
| COMCTL32 | INITCOMMONCONTROLSEX | 8 | dwSize 0x08, dwICC 0xFF (all classes) |
| COMCTL32 | LVITEM | 60 | mask 0x0D (LVIF_TEXT|LVIF_IMAGE|LVIF_PARAM), iItem 5, "Item 5" |
| COMCTL32 | TVITEM | 40 | mask TVIF_TEXT, hItem 0x92030400, "Tree Node" |
| COMCTL32 | HIMAGELIST | n/a | 0x92010000 (32x32 ILC_COLOR32) |
| DXGI | IID GUID LE | 16 | IDXGIFactory 7B7166EC / IDXGIFactory2 50C83A1C / IDXGISwapChain 310D36A0 / IDXGIAdapter 2411E7E1 |
| DXGI | DXGI_SWAP_CHAIN_DESC | 104 | 1920x1080 R8G8B8A8_UNORM 2-buf DISCARD windowed |
| DXGI | DXGI_ADAPTER_DESC | 308 | "Apple M-series Synthetic Adapter" VendorId 0x10DE DeviceId 0x2684 dedicated 2GB |

# §7 Caveats inline (C1-C14)

- C1. PARTIAL ≠ PASS — own2 silent_error_ban 측 명시적 회피, 4 module 모두 PARTIAL emit
  status 만 (production smoke 측 미수행).
- C2. real fopen / 실 malloc / 실 strlen / 실 sprintf = libSystem fopen(3) / malloc(3) /
  strlen(3) / snprintf(3) bridge 미수행 (long-horizon CRT runtime production smoke pending
  separate cycle).
- C3. real SHGetFolderPathW / 실 ShellExecuteW / 실 DragQueryFileW = NSWorkspace /
  LSOpenURLsWithRole / NSFileManager bridge 미수행 (long-horizon Shell APIs production
  smoke pending separate cycle).
- C4. real ImageList_Create / 실 ListView_InsertItem / 실 TreeView_InsertItem = NSImage /
  NSTableView / NSOutlineView bridge 미수행 (long-horizon Common Controls production smoke
  pending separate cycle).
- C5. real CreateDXGIFactory / 실 CreateSwapChain / 실 Present = D3DMetal binary licence
  dependent (long-horizon, multi-year horizon) + Metal CAMetalLayer bridge 미수행 (D3D11
  prerequisite, D2R cond.3 partial → met path 측 indirect dependent).
- C6. Wine source 인용 0 / ReactOS source 인용 0 / OSS Win32 emulator import 0 / D3DMetal
  binary licence 0 / DirectX SDK binding 0 — synthetic byte arrays inline ONLY (own1 정합).
- C7. macOS libSystem fopen / malloc / strlen / NSObject / NSWorkspace / NSImage /
  NSTableView / NSOutlineView / NSXPC / CAMetalLayer / NSWindow / CGContext 직접 binding
  0 (own1 (c) macOS Apple binary boundary 정합).
- C8. fn_kind canonical map module 별 다른 max (AH1 0..11 12-fn / AH2 0..11 12-fn / AH3
  0..9 10-fn / AH4 0..9 10-fn) — 본 module 내 sanity assertion only.
- C9. Track AH bundled 4-cycle land in single Track session — handoff doc + marker = single
  pair, 4 cycle entry 별 separate marker 미생성 (per-cycle marker fan-out = 별도 cycle 측
  separate Track territory).
- C10. cond.1 cond.2 cond.3 status partial → partial 유지 (status promotion 미수행, evidence
  array append 만) — production smoke matrix + 60% threshold matrix + Nt* syscall bridge
  실측 = 별도 cycle.
- C11. L4 D2R single target 측 path 하드코딩 ("C:\\Users\\D2R\\Documents" + "C:\\Program
  Files\\Diablo II Resurrected\\D2R.exe") synth_only — D4 retired_intentional unchanged.
- C12. L5 J/K/L/M/N/O/P/Q/W parallel BG cross-link untouched — Track AH 측 별도 BG agent
  territory (out-of-scope merge audit).
- C13. AH4 DXGI = D3D11 prerequisite — IDXGIFactory + IDXGISwapChain + IDXGIAdapter 측
  D3D11CreateDeviceAndSwapChain dependency baseline; loader_dx cond promotion separate cycle
  (D3DMetal binary licence multi-year horizon dependent).
- C14. raw 9 (hexa-only) + raw 11 (snake_case) + raw 12 (silent_error_ban) + raw 15 (env
  lazy / repo-relative) + raw 175 (BR-NO-USER-VERBATIM) 모두 enforce — 4 module + 1
  roadmap edit + 1 handoff doc + 1 marker 측 0 verbatim user quote, 0 absolute /Users/...
  leak, 0 silent fail.

# §8 Natural environment alignment posture

Track AH continues L2 long-horizon Warden natural-environment-alignment via Win32 syscall
expansion. real Windows binary 측 가장 빈번 호출 surface +4 family 추가:

- MSVCRT C runtime — fopen/malloc/strlen baseline = nearly_every_PE_binary
  prerequisite (D2R + D2R installer + Battle.net + 다수 PE binary).
- SHELL32 Shell APIs — D2R install path resolution + save folder discovery + launcher
  invocation natural environment baseline (CSIDL_PERSONAL + CSIDL_APPDATA + CSIDL_PROGRAM_FILES
  + ShellExecuteW open-verb).
- COMCTL32 Common Controls — D2R installer/launcher UI + D2R config dialog + 다수 PE
  binary common controls baseline (LVITEM list + TVITEM tree + ImageList icon).
- DXGI swap chain — D3D11/12 device creation prerequisite (IDXGIFactory + IDXGISwapChain +
  IDXGIAdapter 측 1920x1080 R8G8B8A8_UNORM 2-buffer baseline + Apple M-series synthetic
  adapter desc).

direct_warden_bypass_vector: none_in_this_cycle. indirect_contribution: fn_naming
baseline +44 fn delta / +168 cumulative delta since cycle 28 / fn_group_classification +14
group / synthetic_struct_shape FILE+SHFILEINFO+LVITEM+TVITEM+IID+SWAP_CHAIN_DESC+ADAPTER_DESC.

# §9 Predecessors

- `state/markers/airgenome_gamebox_win32_cycle32_35_bundled_increment_landed.marker` (Track
  W marker, coverage progression schema reference)
- `state/markers/airgenome_gamebox_user32_gdi32_advapi32_cycle29_31_increment_landed.marker`
  (Track Q marker)
- `state/markers/airgenome_gamebox_kernel32_core_cycle28_increment_landed.marker` (Track O
  marker)
- `lib/loader/pe_oleaut32_core.hexa` (Track W4, mirror style reference)
- `lib/loader/pe_kernel32_core.hexa` (Track O+W1 EXTEND, 689 LOC reference)

# §10 Files created/modified

| path | kind | LOC | bytes | sha256 |
|------|------|-----|-------|--------|
| lib/loader/pe_msvcrt_core.hexa | NEW loader | 470 | 25339 | `4bf2a79ad2484a8263aba1616136893b6686cf0ed12524809aeb9ddb47201626` |
| lib/loader/pe_shell32_core.hexa | NEW loader | 480 | 27696 | `b1a91a23af042779293ff109d135f7601cb111ede991701610a09c855e82c14d` |
| lib/loader/pe_comctl32_core.hexa | NEW loader | 464 | 25432 | `8492228175707191e6b49fcafd6f1b2642f21cab35be93993d61bfae544c1609` |
| lib/loader/pe_dxgi_core.hexa | NEW loader | 477 | 28570 | `f03ad0f7738d963176edb68185f1b040e001ef6e77898e4feb1185ec79dc8119` |
| .roadmap.loader_win32 | MODIFIED | 5 | 21548 | `d05af39c43bc5e091d075c7e44ea96aa291706dcbf6db31dfe8cf2b03d22382b` |
| docs/airgenome_gamebox_win32_cycle36_39_bundled_increment_landed_2026_05_04.ai.md | NEW handoff | self | self | self |
| state/markers/airgenome_gamebox_win32_cycle36_39_bundled_increment_landed.marker | NEW marker | self | self | self |

(self-pinned files: handoff + marker — sha256 not pinned in own body)

# §11 Next cycle actions (long-horizon)

- cycle 40+ MSVCRT extend — string extra (strncpy / strncmp / strchr / strstr) + memory
  extra (calloc / memcpy / memmove / memset) + format extra (vsprintf / wprintf / fscanf) —
  Track AH1 self-similar pattern (multi-cycle, separate Track session).
- cycle 41+ SHELL32 extend — IShellLink / SHBrowseForFolder / ShellExecuteEx UNICODE flags
  matrix — Track AH2 self-similar pattern.
- cycle 42+ COMCTL32 extend — ToolBar / StatusBar / TabControl / ProgressBar / PropertySheet
  — Track AH3 self-similar pattern.
- cycle 43+ DXGI extend — DXGI 1.2 / 1.3 / 1.4 (IDXGIFactory3 / IDXGISwapChain1 /
  IDXGIOutput EnumDisplayModes / IDXGIDevice / IDXGIDeviceSubObject) + WARP adapter probe
  — Track AH4 self-similar pattern, D3D11/12 cross-link.
- cycle 50+ D3D11 — D3D11CreateDeviceAndSwapChain + ID3D11Device VFTable + ID3D11DeviceContext
  + ID3D11Texture2D — DXGI prerequisite consumed (D3DMetal binary licence multi-year
  horizon dependent).
- production smoke cycle (multi-cycle, eta 1-2 year+) — 실 fopen = libSystem fopen(3) / 실
  malloc = libSystem malloc(3) / 실 SHGetFolderPathW = NSFileManager URLsForDirectory:
  inDomains: / 실 ShellExecuteW = LSOpenURLsWithRole / 실 ImageList_Create = NSImage /
  실 ListView_InsertItem = NSTableView / 실 TreeView_InsertItem = NSOutlineView / 실
  CreateDXGIFactory = CAMetalLayer / 실 CreateSwapChain = MTKView / 실 Present =
  CADisplayLink + CAMetalLayer nextDrawable.
- loader_win32.cond.1 + cond.2 + cond.3 status promotion partial → met = production smoke
  matrix + 60% threshold matrix + Nt* syscall bridge 실측 합산 후 fn-by-fn real-vs-stub
  matrix SSOT 측 land (multi-cycle long-horizon 합산 결과).
- Track AH cross-link audit cycle — c_bnet_warden_environment_compat 측 MSVCRT heap pattern
  / SHELL32 path resolution heuristic / COMCTL32 UI control class probe / DXGI swapchain
  Present timing pattern 측 본 Track AH 측 fn coverage SSOT 측 cross-reference (parallel
  BG agent territory merge audit).
- online_expansion_lockin L2 long-horizon Warden compat baseline — KERNEL32 + USER32 +
  GDI32 + ADVAPI32 + NTDLL + OLE32 + OLEAUT32 + MSVCRT + SHELL32 + COMCTL32 + DXGI syscall
  sequence match + heap pattern match + TEB/PEB shape match + sync primitive pattern + COM
  apartment pattern + IDispatch automation pattern + CRT runtime pattern + Shell APIs
  pattern + Common Controls pattern + DXGI swapchain pattern 측 multi-year roadmap 측
  milestone breakdown (cycle 100+ 합산 결과).
