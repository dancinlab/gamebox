---
schema: airgenome-gamebox/handoff/win32_cycle56_59_bundled_increment_landed/1
completion_iso: 2026-05-05
track: BL
domain: loader_win32
cycles: [56, 57, 58, 59]
sub_tracks: [BL1, BL2, BL3, BL4]
modules_new: 4
modules_extend: 0
loc_total: 1809
fn_surface_delta: 46
fn_groups_total: 13
roadmap_evidence_added: 9
caveat_count: 14
threshold_70pct_breached: true
threshold_75pct_breached: true
threshold_80pct_breached: true
threshold_85pct_breached: true
threshold_90pct_breached: true
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

Track BL — Win32 cycle 56 ~ 59 bundled 4 sub-cycle skeleton-tier loader land. 4 NEW
loader module 측 시야 baseline coverage 추가, **90% threshold breach milestone** at
cycle 59 (exceeds spec target cycle 65 ~90% by 6 cycle ahead):

- BL1 cycle 56 — `lib/loader/pe_kernelbase_core.hexa` (KERNELBASE modern split, 12 fn 3 group) —
  KERNEL32 split modern Win32 forward-only API surface family-extension; D2R/D4 savegame +
  d4_install_root scan + d4_launcher CreateProcessAsUserW + warden anti-paging VirtualLock baseline
- BL2 cycle 57 — `lib/loader/pe_dxcore_core.hexa` (DXCORE D3D12 adapter enumeration, 10 fn 3 group) —
  D3D12 modern adapter enumeration prerequisite; D2R/D4 renderer init + dGPU/iGPU enum +
  HighPerformance sort baseline
- BL3 cycle 58 — `lib/loader/pe_dcomp_core.hexa` (DCOMP DirectComposition, 11 fn 3 group) —
  Window composition + animation tree; D2R/D4 launcher overlay + Battle.net blur dialog +
  d4_d3d12_swapchain compose + d2r_overlay_alpha animation baseline
- BL4 cycle 59 — `lib/loader/pe_d2d1_core.hexa` (D2D1 Direct2D rendering, 13 fn 4 group) —
  2D rendering tree + DWrite text-format split family; D2R/D4 launcher renderer +
  Battle.net avatar 256x256 + d2r_health_orb_path + button outline + Segoe UI 12pt label baseline,
  **90% threshold breach milestone**

Win32 coverage progression cycle 55 = 352/435 (80.92%) → cycle 59 = 398/435 (91.49%) 누적
+46 fn skeleton-tier delta across 4 modules (Track BL bundled 4-cycle increment).
**90% threshold breached at cycle 59** (BL4 D2D1, 398/435 91.49%, exceeds spec target ~85%
by 6.49 pp; 70%/80%/90% threshold cascade complete with cycle 52 BH1 + cycle 55 BH4 + cycle 59 BL4).
`.roadmap.loader_win32` cond.1 evidence +3 row + cond.2 evidence +3 row + cond.3 evidence
+3 row = 9 row additive augment, status STAYS at current (partial × 3, full ~435 fn
coverage = multi-cycle long-horizon).

# §2 Modules created (4 NEW)

## §2.1 BL1 — pe_kernelbase_core.hexa (KERNELBASE, cycle 56)

- **Path**: `lib/loader/pe_kernelbase_core.hexa`
- **LOC**: 438 / **bytes**: 30029
- **sha256**: `0483ec15a56d27ef2042b148173a401f31747a0f3daf863a33ec3349b52c230f`
- **DLL target**: KERNELBASE.dll
- **fn surface**: 12 / fn_kind 0..11 / 3 group
  - file_extras (4): GetFileInformationByHandleEx / SetFileInformationByHandle / CopyFile2 / FindFirstFileExW
  - memory (4): VirtualAlloc2 / MapViewOfFile3 / CreateFileMappingFromApp / VirtualLock
  - process_modern (4): CreateRemoteThreadEx / SetProcessMitigationPolicy / GetProcessMitigationPolicy / CreateProcessAsUserW
- **emit format**: `__KERNELBASE__ PARTIAL <fn>` (12 fn × 1 emit = 12 println markers + 13 grep total)
- **TSV tables (4)**: syscall_invoke / file_extras_event / memory_event / process_modern_event
- **Synthetic struct families**: FILE_BASIC_INFO 40B + FILE_END_OF_FILE_INFO 8B +
  COPYFILE2_EXTENDED_PARAMETERS 40B dwCopyFlags 0x1 COPY_FILE_FAIL_IF_EXISTS +
  WIN32_FIND_DATAW 592B d4_install_root + MEM_EXTENDED_PARAMETER 16B Type=2 NumaNode +
  MEM_RESERVE_PLACEHOLDER 0x40000 + PROCESS_MITIGATION_POLICY enum (DEP=0, ASLR=1, CFG=7, Signature=8) +
  STARTUPINFOEX 112B + PROC_THREAD_ATTRIBUTE_LIST 0xC0010100
- **Real syscall**: false / Real kernelbase.dll binding: false / POSIX bridge fstat/mmap/dispatch_io·
  Mach VM vm_allocate/vm_protect·NSFileManager·NSData·NSURL binding: 0
- **D2R/D4 savegame baseline**: true / D4 launcher CreateProcessAsUserW baseline: true /
  Warden anti-paging VirtualLock baseline: true / KERNEL32 split modern family: true

## §2.2 BL2 — pe_dxcore_core.hexa (DXCORE, cycle 57)

- **Path**: `lib/loader/pe_dxcore_core.hexa`
- **LOC**: 433 / **bytes**: 27106
- **sha256**: `f469a8ff7d2f4415d1dcc9d1a61d62297d8c88041b6174b190b332617de9cafd`
- **DLL target**: DXCORE.dll
- **fn surface**: 10 / fn_kind 0..9 / 3 group
  - factory (3): DXCoreCreateAdapterFactory / IDXCoreAdapterFactory::CreateAdapterList / IDXCoreAdapterList::GetAdapter
  - adapter (4): IDXCoreAdapter::GetProperty / IDXCoreAdapter::IsValid / IDXCoreAdapter::IsAttributeSupported / IDXCoreAdapter::GetPropertySize
  - notify (3): IDXCoreAdapterFactory::RegisterEventNotification / IDXCoreAdapterFactory::UnregisterEventNotification / IDXCoreAdapterList::Sort
- **emit format**: `__DXCORE__ PARTIAL <fn>` (10 fn × 1 emit = 10 println markers + 11 grep total)
- **TSV tables (4)**: syscall_invoke / factory_event / adapter_event / notify_event
- **Synthetic struct families**: IDXCoreAdapterFactory 0xE0010100 / IDXCoreAdapterList 0xE0020100 /
  IDXCoreAdapter 0xE0030100 + DXCORE_ADAPTER_ATTRIBUTE_D3D12_GRAPHICS GUID +
  DXCoreAdapterProperty enum (DriverDescription=2, DedicatedAdapterMemory=7, IsHardware=11) +
  DXCoreAdapterPreference enum (Hardware=0, MinimumPower=1, HighPerformance=2) + LUID 8B +
  DXGI_ADAPTER_DESC1 vendor 0x10DE NVIDIA / 0x1002 AMD / 0x8086 Intel
- **Real syscall**: false / Real dxcore.dll binding: false / Metal MTLCopyAllDevices·IOKit
  IOServiceMatching IOPCIDevice·IORegistryEntryCreateCFProperty·CGDirectDisplay binding: 0
- **D2R/D4 renderer init baseline**: true / D4 D3D12 prerequisite baseline: true /
  dGPU first sort baseline: true

## §2.3 BL3 — pe_dcomp_core.hexa (DCOMP, cycle 58)

- **Path**: `lib/loader/pe_dcomp_core.hexa`
- **LOC**: 438 / **bytes**: 27624
- **sha256**: `ffba3902e0764977a149d7be1cd9ea7ea7bd99222540021667599ef534f259ba`
- **DLL target**: DCOMP.dll (DirectComposition)
- **fn surface**: 11 / fn_kind 0..10 / 3 group
  - device (4): DCompositionCreateDevice / IDCompositionDevice::CreateTargetForHwnd / IDCompositionDevice::CreateVisual / IDCompositionDevice::Commit
  - visual (4): IDCompositionVisual::SetContent / IDCompositionVisual::SetTransform / IDCompositionVisual::AddVisual / IDCompositionVisual::SetEffect
  - animation (3): IDCompositionAnimation::AddCubic / IDCompositionAnimation::Reset / IDCompositionDevice::CreateAnimation
- **emit format**: `__DCOMP__ PARTIAL <fn>` (11 fn × 1 emit = 11 println markers + 12 grep total)
- **TSV tables (4)**: syscall_invoke / device_event / visual_event / animation_event
- **Synthetic struct families**: IDCompositionDevice 0xF0010100 / IDCompositionTarget 0xF0020100 /
  IDCompositionVisual 0xF0030100 / IDCompositionAnimation 0xF0040100 +
  D2D_MATRIX_3X2_F 24B (identity / scale 2x / translate 100,50) +
  DCOMPOSITION_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR 0 / LINEAR 1 +
  DCOMPOSITION_FRAME_STATISTICS 80B qpc_synth_60Hz +
  IDCompositionEffect (gaussian blur stdDeviation 8.0 / drop shadow offset 4_4) +
  cubic bezier polynomial (constantC 0.0 linearC 1.0 quadC 0.0 cubicC 0.0 = linear ramp)
- **Real syscall**: false / Real dcomp.dll binding: false / CALayer·CAMetalLayer·CAAnimation·
  CABasicAnimation·CAMediaTimingFunction·CATransaction·NSAnimationContext binding: 0
- **D2R/D4 launcher compositor baseline**: true / Battle.net overlay + blur dialog baseline: true /
  d4_d3d12_swapchain compose baseline: true / d2r_overlay_alpha animation baseline: true

## §2.4 BL4 — pe_d2d1_core.hexa (D2D1, cycle 59)

- **Path**: `lib/loader/pe_d2d1_core.hexa`
- **LOC**: 500 / **bytes**: 34588
- **sha256**: `50526090b9be5663c561829a254871013749f6937d383995434f6ac624afa20c`
- **DLL target**: D2D1.dll (Direct2D + DWrite text-format split)
- **fn surface**: 13 / fn_kind 0..12 / 4 group
  - factory (3): D2D1CreateFactory / ID2D1Factory::CreateHwndRenderTarget / ID2D1Factory::CreateBitmapRenderTarget
  - render_target (4): ID2D1RenderTarget::BeginDraw / ID2D1RenderTarget::EndDraw / ID2D1RenderTarget::Clear / ID2D1RenderTarget::DrawRectangle
  - resources (4): ID2D1RenderTarget::CreateSolidColorBrush / ID2D1RenderTarget::CreateBitmap / ID2D1Factory::CreatePathGeometry / ID2D1Factory::CreateStrokeStyle
  - text (2): ID2D1RenderTarget::DrawText / IDWriteFactory::CreateTextFormat
- **emit format**: `__D2D1__ PARTIAL <fn>` (13 fn × 1 emit = 13 println markers + 14 grep total)
- **TSV tables (5)**: syscall_invoke / factory_event / render_target_event / resource_event / text_event
- **Synthetic struct families**: ID2D1Factory 0xF1010100 / ID2D1HwndRenderTarget 0xF1020100 /
  ID2D1BitmapRenderTarget 0xF1020200 / ID2D1SolidColorBrush 0xF1030100 / ID2D1Bitmap 0xF1040100 /
  ID2D1PathGeometry 0xF1050100 / ID2D1StrokeStyle 0xF1060100 / IDWriteTextFormat 0xF1070100 +
  D2D1_RECT_F 16B 0_0_200_32 button + D2D1_COLOR_F 16B (windowtext 0.94/0.94/0.94/1.0,
  battle_net_dark 0.12/0.12/0.16/1.0) + D2D1_RENDER_TARGET_PROPERTIES 28B DEFAULT
  B8G8R8A8 PREMULTIPLIED + D2D1_HWND_RENDER_TARGET_PROPERTIES 16B 1280x720 +
  D2D1_BRUSH_PROPERTIES 24B + D2D1_BITMAP_PROPERTIES 28B dpi 96/96 +
  D2D1_STROKE_STYLE_PROPERTIES 28B FLAT/FLAT/MITER miterLimit 10.0 +
  IDWriteTextFormat synth Segoe UI 12.0pt FontWeight 400 NORMAL FontStretch 5
- **Real syscall**: false / Real d2d1.dll binding: false / CGContextRef CGContextCreate·
  CGContextStrokeRect·CTLineDraw·CGPathCreateMutable·CGGradientCreate·CTFontCreateWithName·
  NSAttributedString binding: 0
- **D2R/D4 launcher renderer baseline**: true / Battle.net avatar 256x256 baseline: true /
  D2R health_orb path baseline: true / D2R/D4 button outline baseline: true /
  Battle.net Segoe UI 12pt label baseline: true / **90% threshold breach milestone**: true

# §3 Roadmap surgical edit (.roadmap.loader_win32)

## §3.1 cond.1 evidence augment (+3 row)

Status: partial → partial (UNCHANGED — coverage milestone NOT status promotion criterion).
Evidence count: 36 → 39.

- BL1 KERNELBASE 12 fn skeleton-tier (cycle 56, file_extras/memory/process_modern split modern)
- Win32 cycle 56 coverage progression row (Track BL1): 352/435 → 364/435 +12 fn delta —
  KERNELBASE modern split family (pe_kernelbase_core.hexa, 364/435 = 83.68%)
- Win32 cycle 56-57 bundled coverage progression row (Track BL1+BL2): 352/435 → 374/435
  +22 fn delta across 2 modules — Track BL part 1+2/4

blocker_reason refresh: appended Win32 cycle 56-57 bundled increment landed via Track BL1+BL2.

## §3.2 cond.2 evidence augment (+3 row)

Status: partial → partial (UNCHANGED — production smoke matrix multi-cycle long-horizon).
Evidence count: 19 → 22.

- BL2 DXCORE 10 fn skeleton-tier (cycle 57, factory/adapter/notify, D3D12 modern adapter
  enumeration prerequisite family-extension to DXGI+D3D11+D3D12 graphics row)
- BL3 DCOMP 11 fn skeleton-tier (cycle 58, device/visual/animation, DirectComposition window
  composition + animation tree family-extension to DWMAPI compositor row)
- USER32+GDI32+COMCTL32+IMM32+USP10+WTSAPI32+UXTHEME+DWMAPI+DXCORE+DCOMP cumulative coverage
  progression row (Track Q1+Q2+AH3+AM3+AZ4+BD3+BH1+BH2+BL2+BL3 UI/input/text-shaping/session/
  visual_styles/compositor/D3D12_prereq/window_composition surface family): cycle 47 → cycle 58
  +123 fn skeleton-tier across 10 modules

blocker_reason refresh: appended UXTHEME+DWMAPI+DXCORE+DCOMP visual-styles + compositor +
window-composition + animation UI surface family-extension cycle 56-58 increment.

## §3.3 cond.3 evidence augment (+3 row)

Status: partial → partial (UNCHANGED — 90% threshold = coverage milestone NOT status promotion criterion;
3 syscall hexa-native bridge 실측 미수행).
Evidence count: 24 → 27.

- BL4 D2D1 13 fn skeleton-tier (cycle 59, factory/render_target/resources/text, Direct2D
  rendering family-extension + DWrite text-format split family) + **90% threshold breach
  milestone** at cycle 59 via BL4 D2D1 (398/435 91.49%, exceeds spec target ~85%)
- Track BL bundled cycle 56-59 cumulative coverage progression row (BL1+BL2+BL3+BL4):
  352/435 → 398/435 +46 fn delta across 4 modules
- Win32 cycle 59 90% threshold breach milestone (cycle 52 BH1 70% + cycle 55 BH4 80% + cycle 59
  BL4 90% threshold cascade complete; 6 cycle ahead of spec target ~85%); 90% threshold
  breach = coverage milestone NOT status promotion criterion

blocker_reason refresh: appended Win32 cycle 56-59 bundled increment landed via Track BL +
**90% threshold breach milestone** at cycle 59 via BL4 D2D1.

## §3.4 Preserved unchanged

- Header line 1-4 comments
- blockers array (loader_win32.blk.1 kernel-AC class, status=open)
- cross_link section (sister_domains, mk1_narrative_refs, plugin_contract)
- status: active / since: 2026-05-03

# §4 Win32 coverage progression (cycle 27..59)

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
| 48 | 272/435 | 62.53% | pe_powrprof_core.hexa NEW (Track BD1) |
| 49 | 282/435 | 64.83% | pe_wbemuuid_core.hexa NEW (Track BD2) |
| 50 | 292/435 | 67.13% | pe_wtsapi32_core.hexa NEW (Track BD3) **67% breach** |
| 51 | 304/435 | 69.89% | pe_iphlpapi_core.hexa NEW (Track BD4) |
| 52 | 314/435 | 72.18% | pe_uxtheme_core.hexa NEW (Track BH1) **70% breach** |
| 53 | 325/435 | 74.71% | pe_dwmapi_core.hexa NEW (Track BH2) |
| 54 | 338/435 | 77.70% | pe_wininet_extras.hexa NEW (Track BH3) |
| 55 | 352/435 | 80.92% | pe_rpcrt4_core.hexa NEW (Track BH4) **80% breach** |
| **56** | **364/435** | **83.68%** | **pe_kernelbase_core.hexa NEW (Track BL1)** |
| **57** | **374/435** | **85.98%** | **pe_dxcore_core.hexa NEW (Track BL2) ≥85%** |
| **58** | **385/435** | **88.51%** | **pe_dcomp_core.hexa NEW (Track BL3)** |
| **59** | **398/435** | **91.49%** | **pe_d2d1_core.hexa NEW (Track BL4) 90% breach** |

Track BL bundled delta cycle 55→59: +46 fn skeleton-tier (+10.57 pp).
Cumulative delta cycle 28→59: +372 fn (+85.51 pp).

# §5 Verifier emit log samples (2026-05-05)

`__KERNELBASE__ PARTIAL GetFileInformationByHandleEx` / `SetFileInformationByHandle` /
`CopyFile2` / `FindFirstFileExW` / `VirtualAlloc2` / `MapViewOfFile3` /
`CreateFileMappingFromApp` / `VirtualLock` / `CreateRemoteThreadEx` /
`SetProcessMitigationPolicy` / `GetProcessMitigationPolicy` / `CreateProcessAsUserW` (12)

`__DXCORE__ PARTIAL DXCoreCreateAdapterFactory` /
`IDXCoreAdapterFactory_CreateAdapterList` / `IDXCoreAdapterList_GetAdapter` /
`IDXCoreAdapter_GetProperty` / `IDXCoreAdapter_IsValid` /
`IDXCoreAdapter_IsAttributeSupported` / `IDXCoreAdapter_GetPropertySize` /
`IDXCoreAdapterFactory_RegisterEventNotification` /
`IDXCoreAdapterFactory_UnregisterEventNotification` / `IDXCoreAdapterList_Sort` (10)

`__DCOMP__ PARTIAL DCompositionCreateDevice` /
`IDCompositionDevice_CreateTargetForHwnd` / `IDCompositionDevice_CreateVisual` /
`IDCompositionDevice_Commit` / `IDCompositionVisual_SetContent` /
`IDCompositionVisual_SetTransform` / `IDCompositionVisual_AddVisual` /
`IDCompositionVisual_SetEffect` / `IDCompositionAnimation_AddCubic` /
`IDCompositionAnimation_Reset` / `IDCompositionDevice_CreateAnimation` (11)

`__D2D1__ PARTIAL D2D1CreateFactory` / `ID2D1Factory_CreateHwndRenderTarget` /
`ID2D1Factory_CreateBitmapRenderTarget` / `ID2D1RenderTarget_BeginDraw` /
`ID2D1RenderTarget_EndDraw` / `ID2D1RenderTarget_Clear` /
`ID2D1RenderTarget_DrawRectangle` / `ID2D1RenderTarget_CreateSolidColorBrush` /
`ID2D1RenderTarget_CreateBitmap` / `ID2D1Factory_CreatePathGeometry` /
`ID2D1Factory_CreateStrokeStyle` / `ID2D1RenderTarget_DrawText` /
`IDWriteFactory_CreateTextFormat` (13)

Smoke status: static_validate_only_no_runtime_exec_in_this_cycle.

# §6 Natural environment alignment posture

- **Goal**: Win32 syscall perfection rosetta toward L2 Warden detection compat —
  KERNEL32 + USER32 + GDI32 + ADVAPI32 + NTDLL + OLE32 + OLEAUT32 + MSVCRT + SHELL32 + COMCTL32 +
  DXGI + WINMM + XAudio2 + IMM32 + SETUPAPI + CRYPT32 + WS2_32(extras) + DBGHELP + USP10 +
  POWRPROF + WBEMUUID + WTSAPI32 + IPHLPAPI + UXTHEME + DWMAPI + WININET(extras) + RPCRT4 +
  KERNELBASE + DXCORE + DCOMP + D2D1 direct contributor (real Windows binary 측 가장 빈번
  호출 surface 31 family — KERNEL32 split modern + D3D12 prerequisite + window composition +
  Direct2D rendering 추가).
- **Horizon**: long_horizon_multi_year_L2_L3
- **cycle_55_baseline**: 352/435 RPCRT4 BH4 (**80% threshold breach milestone**)
- **cycle_56_baseline**: 364/435 (+12 KERNELBASE skeleton-tier via BL1, KERNEL32 split modern
  forward-only Win32 API surface family, D2R/D4 savegame + d4_install_root scan + d4_launcher
  CreateProcessAsUserW + warden anti-paging VirtualLock baseline)
- **cycle_57_baseline**: 374/435 (+10 DXCORE skeleton-tier via BL2, D3D12 modern adapter
  enumeration prerequisite, D2R/D4 renderer init + dGPU/iGPU enum + HighPerformance sort baseline)
- **cycle_58_baseline**: 385/435 (+11 DCOMP skeleton-tier via BL3, DirectComposition window
  composition + animation tree, D2R/D4 launcher overlay + Battle.net blur dialog +
  d4_d3d12_swapchain compose + d2r_overlay_alpha animation baseline)
- **cycle_59_baseline**: 398/435 (+13 D2D1 skeleton-tier via BL4, Direct2D rendering + DWrite
  text-format split, D2R/D4 launcher renderer + Battle.net avatar 256x256 + d2r_health_orb_path +
  button outline + Segoe UI 12pt label baseline; **90% threshold breach milestone** —
  exceeds spec target ~85%)
- **cycle_n_target**: production smoke (real GetFileInformationByHandleEx/VirtualAlloc2/
  CreateRemoteThreadEx = POSIX fstat/mmap_named/dispatch_io + Mach VM vm_allocate/vm_protect +
  pthread_create_in_target_task / real DXCoreCreateAdapterFactory/GetProperty/IsAttributeSupported =
  Metal MTLCopyAllDevices + IOKit IOPCIDevice + IORegistryEntryCreateCFProperty + CGDirectDisplay /
  real DCompositionCreateDevice/SetContent/AddCubic = CALayer + CAMetalLayer + CAAnimation +
  CABasicAnimation + CAMediaTimingFunction + CATransaction / real D2D1CreateFactory/DrawRectangle/
  DrawText/CreatePathGeometry = CGContextRef + CGContextStrokeRect + CTLineDraw + CGPathCreateMutable +
  CGGradientCreate + CTFontCreateWithName + NSAttributedString) + Warden detection compat
- **direct_warden_bypass_vector**: none_in_this_cycle
- **indirect_contribution**: fn_naming_baseline 46 fn delta + 372 fn cumulative cycle 28→59;
  fn_group_classification 13 group delta + 88 total; synthetic struct shape FILE_BASIC_INFO/
  COPYFILE2_EXTENDED_PARAMETERS/MEM_EXTENDED_PARAMETER/PROCESS_MITIGATION_POLICY/STARTUPINFOEX +
  IDXCoreAdapterFactory/IDXCoreAdapterList/IDXCoreAdapter/DXCORE_ADAPTER_ATTRIBUTE_D3D12_GRAPHICS/
  LUID + IDCompositionDevice/IDCompositionTarget/IDCompositionVisual/IDCompositionAnimation/
  D2D_MATRIX_3X2_F/cubic_bezier_polynomial + ID2D1Factory/ID2D1HwndRenderTarget/ID2D1SolidColorBrush/
  ID2D1Bitmap/ID2D1PathGeometry/ID2D1StrokeStyle/IDWriteTextFormat/D2D1_RECT_F/D2D1_COLOR_F/
  D2D1_RENDER_TARGET_PROPERTIES/D2D1_STROKE_STYLE_PROPERTIES synth_only;
  d4_install_root/d2r_savegame_d2s/battle_net_avatar/d2r_health_orb_path/Segoe_UI_12pt synth_only

# §7 Caveats (C1..C14)

- **C1**: Production smoke 측 미수행. Real GetFileInformationByHandleEx / VirtualAlloc2 /
  CreateRemoteThreadEx / DXCoreCreateAdapterFactory / DCompositionCreateDevice / D2D1CreateFactory
  call 0 — POSIX bridge / Mach VM / Metal MTLDevice / IOKit / CALayer / CAMetalLayer /
  CGContextRef / CTFontRef bridge production smoke 별도 cycle (multi-cycle long-horizon).
- **C2**: Warden compat horizon long_horizon_L2 — direct bypass vector 0 in this cycle.
- **C3**: 사용자 verbatim quote 0 — paraphrase only across handoff doc + marker (BR-NO-USER-VERBATIM).
- **C4**: L4 D2R single target 측 path 하드코딩 (d2r_savegame_d2s + d4_install_root + d2r_health_orb).
- **C5**: L5 J/K/L/M/N/O/P/Q/W/AH/AM/AZ/BC/BD/BH parallel BG cross-link untouched (Track BH 측
  read-only, Track BL 측 KERNELBASE/DXCORE/DCOMP/D2D1 territory distinct).
- **C6**: Runtime exec smoke 측 미수행 (skeleton-tier mandate 정합) — static validate only.
- **C7**: fn_kind range module 별 다른 max (BL1=11, BL2=9, BL3=10, BL4=12).
- **C8**: Track BL bundled 4-cycle land in single Track session.
- **C9**: cond.1 + cond.2 + cond.3 status partial → partial 유지 (90% threshold breach =
  coverage milestone NOT status promotion criterion).
- **C10**: BL4 D2D1 = **90% threshold breach milestone** module (398/435 91.49%, exceeds spec
  target ~85% by 6.49 pp).
- **C11**: 70%/80%/90% threshold breach cascade complete (cycle 52 BH1 + cycle 55 BH4 + cycle 59 BL4).
- **C12**: BL2 DXCORE = D3D12 modern adapter enumeration prerequisite, distinct from Track AH4
  pe_dxgi_core.hexa (DXGI baseline).
- **C13**: BL3 DCOMP + BL4 D2D1 family-extension complementary to Track BH2 DWMAPI compositor —
  full visual stack now spans HTHEME → DWM → DXGI → DXCORE → DCOMP → D2D1.
- **C14**: BL1 KERNELBASE = KERNEL32 split modern forward-only API surface — distinct from
  Track O+W1 pe_kernel32_core.hexa baseline (KERNELBASE 측 modern Win32 split-DLL pattern).

# §8 Online expansion lockin (L1..L5)

L1 (EULA/ToS) + L2 (Warden long-horizon) + L3 (multi-year) + L4 (D2R single target /
D4 retired_intentional unchanged) + L5 (Track J/K/L/M/N/O/P/Q/W/AH/AM/AZ/BC/BD/BH parallel BG)
ALL OK 사용자 lock-in 2026-05-03.

# §9 Next cycle actions (signal)

- cycle 60+ KERNELBASE extend — GetTempPath2W / OpenFileById / SetFileCompletionNotificationModes /
  DeviceIoControl / FlushProcessWriteBuffers — Track BL1 self-similar pattern
- cycle 61+ DXCORE extend — DXCoreCreateAdapterFactory v2 / IDXCoreAdapterList::IsStale /
  DXCoreFreeAdapterListMemory / DXCoreNotificationType::AdapterNoLongerValid — Track BL2 self-similar
- cycle 62+ DCOMP extend — IDCompositionVisual2::SetOpacityMode / IDCompositionVisual::SetClip /
  IDCompositionDevice2::CreateScaleTransform / CreateRotateTransform — Track BL3 self-similar
- cycle 63+ D2D1 extend — ID2D1RenderTarget::DrawBitmap / DrawGeometry / FillRectangle /
  ID2D1DeviceContext::BeginDraw + IDWriteTextLayout — Track BL4 self-similar
- cycle 65+ 95% threshold-breach completion — D2D1 cycle 59 = 398/435 (~91.49%) trending toward
  cycle 64 (~95%) via D3D11/D3D9/XInput/DirectInput/DSOUND/MSCTF cumulative
- production smoke cycle (multi-cycle, eta 1-2 year+) — 실 GetFileInformationByHandleEx =
  POSIX fstat + dispatch_io / 실 VirtualAlloc2 = Mach VM vm_allocate + vm_protect / 실
  CreateRemoteThreadEx = pthread_create_in_target_task / 실 DXCoreCreateAdapterFactory =
  Metal MTLCopyAllDevices + IOKit / 실 DCompositionCreateDevice = CALayer + CAMetalLayer +
  CATransaction / 실 D2D1CreateFactory = CGContextRef + CGPathRef + CTFontRef +
  NSAttributedString
- loader_win32.cond.1 + cond.2 + cond.3 status promotion partial → met 측 production smoke matrix +
  Nt* syscall bridge 실측 + per-DLL real fn count vs stub count ratio + Warden compat syscall
  pattern align cycle 합산 후 status 승격 (multi-cycle long-horizon). 70%/80%/90% threshold
  breach = coverage milestone, NOT status promotion criterion.

# §10 Compliance posture (raw 9/11/12/15/175)

- **raw 9 (hexa-only)**: verifier emit native (println __KERNELBASE__/__DXCORE__/__DCOMP__/__D2D1__
  PARTIAL <fn> ×46 delta + r0_emit JSONL ×116). Wine 0 / CrossOver 0 / Whisky 0 / GPTK 0.
  ReactOS / Wine kernelbase/dxcore/dcomp/d2d1 source citation 0. POSIX bridge / Mach VM / Metal
  MTLDevice / IOKit / CALayer / CAMetalLayer / CGContextRef / CTFontRef binding 0.
- **raw 11 (snake_case)**: all field names snake_case throughout JSONL roadmap edit + JSON marker +
  r0_emit event_type + handoff doc front-matter.
- **raw 12 (silent_error_ban)**: every status_emit explicit (4 module × 46 println + 116 r0_emit
  record + 14 stats/summary/kpi/coverage/round_trip emit per module = 116+ PARTIAL emit total).
  PARTIAL ≠ PASS — silent_error_ban 측 명시적 회피.
- **raw 15 (env_lazy)**: all paths repo-relative.
- **raw 175 (br_no_user_verbatim)**: 0 verbatim user quotes in any of 7 created/modified files.

# §11 Cross-Track audit (Track BH territory read-only)

- Track BH (cycle 52-55, UXTHEME+DWMAPI+WININET_EX+RPCRT4) read-only verification: 4 BH module
  files unchanged (sha pinned in cycle 52-55 marker).
- Track BL (cycle 56-59, KERNELBASE+DXCORE+DCOMP+D2D1) territory distinct — no overlap with
  BH UI/IPC family.
- Track BL3 DCOMP + Track BH2 DWMAPI = complementary compositor stack (DWMAPI = window-level
  compositor state + blur-behind, DCOMP = visual tree + animation, distinct surface).
- Track BL4 D2D1 + Track AH4 DXGI = complementary 2D + 3D rendering stack (DXGI = swap chain +
  surface, D2D1 = 2D primitive draw, distinct surface).
