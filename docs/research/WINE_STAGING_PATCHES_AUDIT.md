# WINE_STAGING_PATCHES_AUDIT — wine-staging 119 patches 전수조사

own1: Wine staging code lift 0 줄. 본 문서는 `/tmp/crossover-research/wine-staging/patches/` 의 119 개 patch directory 의 `definition` file 만 reference 한 algorithm intent 분석. Wine source code/diff hunk 는 본 project 로 lift 하지 않는다.

- 조사 대상: `/tmp/crossover-research/wine-staging/patches/` (research-only mirror, build artifact 아님)
- directory 수: 119 (실 patch 118 + `Staging` meta 1 + `gitapply.sh` 1)
- 조사 일시: 2026-05-02
- 조사 방법: `definition` text 의 `Fixes:`/`Helps:`/`Depends:`/`Disabled:` line read-only

## 1. Top-level summary

- own1 enforcement: 119 entries 의 patch hunk 자체는 본 project 에 단 한 줄도 도입하지 않는다. 본 audit 은 **알고리즘 의도** 만을 기록한다.
- 본 project (airgenome-gamebox) 의 wave 18+ scope 에 직접 reference 가능한 patch 는 **0 개의 lift, 약 12 개의 의도 reference** 로 한정된다. 나머지 ~107 개는 본 project 의 graceful skip 정책 (binary translation 단계에서 직접 dispatch 가 차단되면 skip 으로 closure) 과 무관한 application-level shim, audio backend, 16-bit emulation, GTK theming 등이다.
- 119 entries 안에 **msync / fsync / esync (synchronization primitive)** 관련 patch 는 **없다** (wine-staging mirror 의 다른 sub-tree 또는 별도 repo 책임).
- 119 entries 안에 **Battle.net / Blizzard / Activision** 이름이 박힌 patch 는 **없다** (Battle.net Setup 의 정상 dispatch 는 generic ntdll/kernelbase/winemac path 의 합으로만 가능).
- 119 entries 안에 **cxx exception (msvcp / __cxa) 강화** patch 는 **없다**. SEH chain 강화는 `ntdll-Exception` 1 건 뿐 (DBG_PRINTEXCEPTION_C re-throw, 사실상 debugger 보조).

## 2. Categorized list (전수)

본 list 는 119 entry 를 algorithm intent 기준 9 bucket 으로 분류. `*` 는 `Disabled: true` (wine-staging 자체에서 비활성).

### 2.1 macOS 특화 (1)
- `winemac.drv-no-flicker-patch` — fullscreen flicker (bug 34166), `ForceOpenGLBackingStore` registry knob 추가. 본 project 의 winemac dispatch 와 무관 (본 project 는 wine binary 를 들고 다니지 않음).

### 2.2 synchronization (msync/fsync/esync) — 0
없음. wine-staging 의 sync patch tree 는 별도 sub-directory 에 존재 (본 audit scope 외).

### 2.3 cxx exception 강화 — 0
없음. msvcp / `__CxxFrameHandler*` / `__cxa_throw` 관련 patch 는 119 안에 없다.

### 2.4 SEH / exception dispatch (1)
- `ntdll-Exception` — bug 44819. `OutputDebugString` 호출 시 두 번째 `DBG_PRINTEXCEPTION_C` 를 throw 하여 debugger 가 wide-char message 를 잡을 수 있게 한다. SEH chain 자체 수정이 아니라 debugger 협력 path.

### 2.5 PE loader / virtual memory (3)
- `ntdll-WRITECOPY` — bug 29384. `VirtualProtect` 가 `PAGE_WRITECOPY` 를 `PAGE_READWRITE` 로 정상 보고. Voobly/AoE2/MSYS2 launch 지원. 본 project 의 binary translation pre-flight 단계에서 PE section flag 정합성 점검 시 의도 reference.
- `ntdll-ForceBottomUpAlloc` — bug 48175 / 46568. 64-bit AION crysystem.dll, msxml6.dll high-pointer crash. `try_map_view`/`free_ranges` 알고리즘으로 bottom-up 강제. 본 project 는 직접 mapping 하지 않으므로 reference 만.
- `ntdll-NtQueryVirtualMemory` — `MemorySectionName` class, `K32GetMappedFileName` 구현. (`Depends:` ntdll-NtDevicePath, ntdll-ForceBottomUpAlloc, ntdll-Junction_Points)

### 2.6 thread / server / signal (5)
- `server-Signal_Thread` — `TerminateThread` race fix. 본 project 의 dispatch shim 에서 thread teardown 동기화 의도 reference.
- `server-PeekMessage` — bug 28884. higher-priority message removal.
- `server-File_Permissions` (no def), `server-Inherited_ACLs*` (Silverlight, disabled), `server-Stored_ACLs` — POSIX↔Windows ACL bridge.

### 2.7 ntdll / kernel32 misc (10)
- `ntdll-APC_Performance` (no def), `ntdll-ext4-case-folder` (47099), `ntdll-Hide_Wine_Exports` (38656 — anti-detection knob, DRM bypass 가 아니라 application 의 wine 인지 차단), `ntdll-Junction_Points` (12401 — .NET GAC reparse-point), `ntdll-NtDevicePath` (37487 — `\SystemRoot\` 해결), `ntdll-RtlQueryPackageIdentity` (stub), `ntdll-Serial_Port_Detection` (39793).
- `kernel32-CopyFileEx` (22692/22690), `kernel32-Debugger`, `kernel32-limit_heap_old_exe` (44931 — old exe 3MB heap cap), `krnl386.exe16-GDT_LDT_Emulation` (30237).

### 2.8 graphics / d3d / wined3d (16)
`d2d1_ID2D1GeometryGroup*`, `d3d9-fake_vtbl_init` (59708 — MSVC vtable byte-pattern), `d3drm-starwars` (StarWars Rebellion crash chain), `d3dx9_36_controller` (39876 — Humanity Asset), `d3dx9_36-D3DXStubs`, `d3dx9_animation_TrackDesc`, `d3dx9-setrawvalue` (MudRunner), `d3dx9-sprite-state`, `ddraw-GetPickRecords`, `ddraw-IDirect3DTexture2_Load` (Prezzie Hunt), `dxgi_getFrameStatistics`, `vkd3d-latest`, `wined3d-Indexed_Vertex_Blending*`, `wined3d-rotate-WINED3D_SWAP_EFFECT_DISCARD` (47331 — Steam browser flicker), `wined3d-SWVP-shaders*`, `wined3d-unset-flip-gdi`, `wined3d-zero-inf-shaders` (Halo Online).

### 2.9 input / x11 / mac / display / theming (15)
`comctl32_animate_avi`, `comctl32-rebar-capture`, `dinput-joy-mappings`, `dinput-scancode`, `loader-KeyboardLayouts`, `user32-message-order`, `user32-Mouse_Message_Hwnd` (Word/Photoshop), `user32-rawinput-keyboard*`, `user32-rawinput-mouse` (Overwatch Phantom/Fallout4), `user32-recursive-activation`, `uxtheme-GTK_Theming*`, `winex11-Fixed-scancodes*`, `winex11-ime-check-thread-data` (FFXI EULA), `winex11-Window_Style`, `winex11-WM_WINDOWPOSCHANGING*`, `wintab32-improvements`.

### 2.10 audio / media / mfplat (10)
`dmime_segment_getaudiopath`, `dmscript_enum_routine`, `dsound-EAX*`, `mf_http_support` (The Good Life), `mfplat-streaming-support*`, `mmsystem.dll16-MIDIHDR_Refcount`, `sapi-ISpObjectToken-CreateInstance`, `winepulse-aux_channels` (Proton issue 6100 ref), `winepulse-PulseAudio_Support`, `winmm-mciSendCommandA`, `xactengine3_7-PrepareWave`.

### 2.11 com / oleaut / msxml / wbem / shell / mshtml / cert / wininet (그 외)
- mshtml/msxml: `mshtml-adobe` (Photoshop CC2019 login + Adobe CC installer), `mshtml-TranslateAccelerator`, `msxml3_embedded_cdata*`, `msxml3-FreeThreadedXMLHTTP60`, `msxml3-whitespace`, `msxml3-write_out_doc`.
- oleaut: `oleaut32_typelib_dispatch`, `oleaut32_VarAdd`, `oleaut32-CreateTypeLib`, `oleaut32-default-pic-size`, `stdole32.idl-Typelib`, `version-VerQueryValue`, `vcomp_for_dynamic_init_i8`.
- wbem/wininet/cert: `wbemdisp-ISWbemObject-Invoke`, `wininet-Cleanup`, `wininet-proxy_username`, `wintrust-WTHelperGetProvCertFromChain` (44061), `wine.inf-Dummy_CA_Certificate` (38022).
- shell: `shell32-ACE_Viewer*`, `shell32-IconCache`, `shell32-Progress_Dialog*`, `shell32-Toolbar_Bitmaps`, `shlwapi-AssocGetPerceivedType`.
- 기타: `advapi32-Token_Integrity_Level*` (UAC, disabled), `comdlg32-lpstrFileTitle`, `dbghelp-Debug_Symbols` (no def), `dcomp-DCompositionCreateDevice2` (Clip Studio Paint 4 menu, Corgi Warlock), `explorer-Video_Registry_Key`, `fonts-Missing_Fonts`, `gdiplus-Performance-Improvements`, `inseng-Implementation` (39456 — CIF reader; **installer download path**), `mountmgr-DosDevices`, `nvapi-Stub_DLL` (PhysX/CS:GO/FarCry3), `odbc32-fixes`, `riched20-IText_Interface`, `richedit20-ImportDataObject`, `Staging` meta, `user32-DM_SETDEFID`, `user32-DrawTextExW`, `user32-GetSystemMetrics*`, `user32-msgbox-Support-WM_COPY-mesg`, `win32u-NtGdiExtTextOutW-rotation`, `windows_storagefile`, `windowscodecs-GIF_Encoder`, `windowscodecs-TIFF_Support`, `winecfg-Libraries`, `winecfg-Staging`, `winecfg-Unmounted_Devices*`, `winedbg-Process_Arguments`, `winedevice-Default_Drivers`, `wineps.drv-PostScript_Fixes*`, `ws2_32-af_unix` (52568).

총 119. (118 patch dir + 1 `Staging` meta = 본 목록과 일치, `gitapply.sh` 는 directory 가 아니라 list 외.)

## 3. Battle.net / Blizzard / cxx exception 관련 patches (zoom in)

### 3.1 Battle.net / Blizzard 직접 매칭 — 0
patch name + definition 의 `Fixes:`/`Helps:` text 모두에서 **Battle.net / Blizzard / Diablo / Overwatch (Activision side) / Hearthstone / WoW / SC2** 식별자 0 건. (Overwatch 는 `user32-rawinput-mouse` definition 에 mouse drift 보조 reference 1 회 등장하나, Battle.net Setup launcher path 와는 무관.)

### 3.2 Battle.net Setup 정상 dispatch 관련 *암묵* 의존 (intent 만 reference)
Battle.net Setup.exe 같은 native installer 가 wine 위에서 정상 launch 하려면, 본 119 patch 중 다음의 *알고리즘 의도* 가 필요. 본 project 는 patch 의 hunk 를 lift 하지 않고, 동일한 dispatch invariant 를 자체 binary translation 단계에서 graceful skip 또는 native shim 으로 closure 한다.

| 의도 reference | wine-staging patch | 본 project 의 closure path |
| --- | --- | --- |
| `VirtualProtect` 가 `PAGE_WRITECOPY` 를 정상 보고 | `ntdll-WRITECOPY` | binary translation pre-flight 의 PE section flag 검사에서 WRITECOPY → READWRITE 로 normalize, mismatch 시 graceful skip |
| 64-bit pointer 영역 회피한 PE map | `ntdll-ForceBottomUpAlloc` | 본 project 는 직접 mapping 하지 않음. native loader (Wine 외) 에 위임, allocation 실패 시 skip closure |
| `K32GetMappedFileName` / `\SystemRoot\` 해결 | `ntdll-NtQueryVirtualMemory`, `ntdll-NtDevicePath` | dispatch shim 의 path resolver 에 동일 invariant (resolve → 실패 시 skip) |
| Junction point (.NET GAC) | `ntdll-Junction_Points` | reparse point 가 등장하면 binary translation 단계에서 즉시 graceful skip — 본 project 는 GAC 까지 따라가지 않는다 |
| `wintrust` chain 검증 NULL 방어 | `wintrust-WTHelperGetProvCertFromChain` | dispatch 에서 cert chain NULL → graceful skip (본 project 는 signature verification 을 시도하지 않는다) |
| Dummy CA certificate | `wine.inf-Dummy_CA_Certificate` | Battle.net 같은 launcher 의 HTTPS CA store 가 없을 때, 본 project 는 fetch 자체를 skip 으로 닫는다 (own1: 다른 wine 가공 X) |
| `inseng` CIF reader (installer download) | `inseng-Implementation` | **본 project 는 installer download path 를 시도하지 않는다.** Battle.net Setup 이 inseng 를 호출하면 즉시 graceful skip (binary 가 본 project 의 stage2 hexa loader 의 책임 영역을 벗어남). |
| Wine version hide (anti-detection) | `ntdll-Hide_Wine_Exports` | 본 project 의 dispatch 는 Battle.net 의 anti-detection probe 를 우회하지 않는다 (own1 enforcement: 타사 우회 금지). probe 결과를 그대로 노출하고 application 이 자체 종료하면 정상 closure. |
| Server thread teardown race | `server-Signal_Thread` | 본 project 의 dispatch shim 에서 child thread 의 join 은 deterministic, race 가 발생하면 graceful skip (timeout) closure. |
| `ntdll-Exception` (DBG_PRINTEXCEPTION_C) | bug 44819 | 본 project 는 debugger 협력 path 를 lift 하지 않는다. SEH 자체는 OS 의 native unwind 에 위임. |
| `winemac.drv-no-flicker-patch` | bug 34166 | 본 project 는 winemac 위에서 동작하지 않으므로 무관. |

### 3.3 cxx exception 강화 — 0 (다시 확인)
119 안에 `msvcp*`/`__CxxFrameHandler*`/`__cxa_throw`/`vcruntime`/`unwind` 관련 patch 가 없음을 directory name 및 definition text 에서 재확인. Battle.net Setup 의 cxx exception 경로는 본 project 의 stage2 hexa loader 가 OS-native unwind 에 위임하는 invariant 만 유지하면 된다 (own1: Wine staging code lift 0 줄 유지).

### 3.4 PE loader 강화 — 3 (앞의 2.5 참조)
`ntdll-WRITECOPY` / `ntdll-ForceBottomUpAlloc` / `ntdll-NtQueryVirtualMemory` 의 algorithm intent 만 reference. 본 project 는 PE loader 자체를 lift 하지 않는다.

### 3.5 SEH chain 강화 — 1 (`ntdll-Exception`)
debugger-cooperative re-throw, 본 project 의 graceful skip path 와 의도가 다르다 (skip 은 exception 을 swallow 하지 않고 closure marker 로 propagate). reference 만.

### 3.6 DRM bypass — 0 (정상 dispatch — 부분 reference)
119 안에 명시적 DRM bypass patch 는 없다. anti-detection 영역의 `ntdll-Hide_Wine_Exports` 는 own1 enforcement 상 본 project 가 사용하지 않는다. `nvapi-Stub_DLL` 은 PhysX / CS:GO / FarCry3 의 nvapi 초기화 누락 보호이며 DRM 과 무관, 본 project 의 graceful skip 정책과 같은 결 (call 실패 → null/stub 반환 → application 이 자체 fallback).

### 3.7 Battle.net Setup launch path 의 invariant matrix (zoom-in)

Battle.net Setup.exe (Activision/Blizzard installer) 가 native loader 위에서 정상 dispatch 되는 동안 깨질 수 있는 OS contract invariant 7 종, 본 project 의 closure rule 1:1 mapping.

| # | invariant | wine-staging 의 수정 의도 (definition reference 만) | 본 project 의 closure |
| --- | --- | --- | --- |
| I1 | `VirtualProtect` 의 `old_prot` out-param 은 caller 가 알아볼 수 있는 protection class | `ntdll-WRITECOPY` 의 `PAGE_WRITECOPY` → `PAGE_READWRITE` normalize | wave 18 pre-flight 에서 확인 후 mismatch 면 skip closure (binary 가 strict 한 contract 를 가정하면 본 project 는 그 contract 를 정상화하지 않는다) |
| I2 | 64-bit 영역 mapping pointer 가 high-pointer 에 가지 않도록 | `ntdll-ForceBottomUpAlloc` 의 `try_map_view`/`free_ranges` reorder | 본 project 는 mapping 을 직접 하지 않음. host loader 의 결과를 trust, allocation 실패 시 immediate skip |
| I3 | Path resolver 가 NT prefix (`\SystemRoot\`, `\??\`) 를 정상 expand | `ntdll-NtDevicePath` | wave 19 path resolver 가 NT prefix 인식 못 하면 skip |
| I4 | reparse / junction point 등장 시 정상 follow | `ntdll-Junction_Points` (.NET GAC) | 본 project 는 follow 하지 않음 — 즉시 skip |
| I5 | thread teardown 의 wait 가 thread 가 실제 stop 한 뒤에 unblock | `server-Signal_Thread` (TerminateThread race) | dispatch shim 의 join 은 deterministic, wait 가 timeout 되면 skip |
| I6 | cert chain 검증 호출이 NULL 입력 으로도 crash 안 하고 답해야 | `wintrust-WTHelperGetProvCertFromChain` (44061) | 본 project 의 dispatch 는 cert chain 호출이 등장하면 NULL → skip closure marker |
| I7 | HTTPS fetch 가 동작할 CA store 존재 | `wine.inf-Dummy_CA_Certificate` (38022) | 본 project 는 HTTPS fetch path 자체를 skip closure (own1: CA store 가공 X) |

I1–I7 합 → Battle.net Setup 이 본 project 의 stage2 hexa loader 를 *실제로 통과* 하는 path 는 **존재하지 않는다** (이는 의도된 closure). 본 project 의 책임은 "Battle.net Setup 이 실패함을 deterministic 하게 알리고 hexa loader 가 closure marker 를 남기는 것" 이다.

### 3.8 patch dependency graph (subset, 본 audit 의 scope 만)

```
ntdll-NtQueryVirtualMemory
  ├── Depends: ntdll-NtDevicePath
  ├── Depends: ntdll-ForceBottomUpAlloc
  └── Depends: ntdll-Junction_Points

ntdll-WRITECOPY
  └── #Depends: ntdll-ForceBottomUpAlloc  (commented out — regression risk)

server-Inherited_ACLs
  └── Depends: server-Stored_ACLs
       └── Depends: server-File_Permissions

shell32-Progress_Dialog
  └── Depends: kernel32-CopyFileEx

shell32-ACE_Viewer
  └── Depends: shell32-Progress_Dialog

advapi32-Token_Integrity_Level
  └── Depends: Staging
```

본 project 는 dependency 자체를 lift 하지 않으므로 graph 는 reference 용. wave plan 의 순서만 align 한다 (e.g., wave 19 path resolver 는 wave 18 PE flag 보다 늦게 도입).

## 4. 본 project 의 wave 18+ 에서 reference 가능한 patches

own1 강제: 아래 목록은 **algorithm intent reference 전용**, code lift 0 줄.

1. `ntdll-WRITECOPY` — wave 18 의 PE section flag normalize 단계에서 invariant 확인.
2. `ntdll-NtDevicePath` — wave 19 의 path resolver 의 `\SystemRoot\` graceful skip rule.
3. `ntdll-Junction_Points` — wave 19 의 reparse-point 즉시 skip rule.
4. `ntdll-NtQueryVirtualMemory` — wave 20 의 mapped-file-name query 가 등장하면 skip closure.
5. `server-Signal_Thread` — wave 20 의 dispatch shim thread teardown 의 deterministic join + timeout skip.
6. `ntdll-Exception` — wave 21 의 SEH OS-위임 정책 confirmation (lift 하지 않음, 본 project 는 unwind 를 OS 에 맡긴다).
7. `wintrust-WTHelperGetProvCertFromChain` — wave 21 의 cert chain NULL → skip.
8. `wine.inf-Dummy_CA_Certificate` — wave 22 의 HTTPS fetch 차단 시 skip closure (CA store 직접 가공 안 함).
9. `inseng-Implementation` — wave 22 의 installer download path 즉시 skip rule (본 project 의 stage2 hexa loader 는 installer 의 child download 를 따라가지 않는다).
10. `nvapi-Stub_DLL` — wave 23 의 vendor-specific GPU API stub 정책 (null/stub return + fallback path).
11. `kernel32-limit_heap_old_exe` — wave 23 의 legacy exe heap cap rule reference.
12. `kernel32-CopyFileEx` — wave 24 의 file-op progress callback skip rule.

나머지 ~107 patch 는 본 project 의 scope 외 (audio backend, 16-bit, GTK theming, d3d/wined3d, msxml/oleaut application shim, x11 specific input).

### 4.1 wave 별 reference table (확장)

| wave | reference patch | invariant | 본 project closure rule |
| --- | --- | --- | --- |
| 18 | `ntdll-WRITECOPY` | I1 (VirtualProtect old_prot) | PE flag normalize check, mismatch skip |
| 19 | `ntdll-NtDevicePath`, `ntdll-Junction_Points` | I3 / I4 (NT prefix, reparse) | path resolver skip rule |
| 20 | `ntdll-NtQueryVirtualMemory`, `server-Signal_Thread` | I5 (thread teardown), virtual memory query | dispatch shim deterministic join + timeout skip |
| 21 | `ntdll-Exception`, `wintrust-WTHelperGetProvCertFromChain` | I6 (cert NULL), SEH OS-위임 | exception layer skip closure |
| 22 | `wine.inf-Dummy_CA_Certificate`, `inseng-Implementation`, `msi-cabinet` | I7 (HTTPS CA), installer download | network/installer 즉시 skip |
| 23 | `nvapi-Stub_DLL`, `kernel32-limit_heap_old_exe` | vendor stub, legacy heap | stub return skip 표시 |
| 24 | `kernel32-CopyFileEx` | file-op progress callback | callback skip closure |

### 4.2 wave 외 — 명시적으로 reference 하지 않는 patch (rationale)

- wave 외 (audio): `winepulse-*`, `dsound-EAX`, `mf_http_support`, `mfplat-streaming-support`, `xactengine3_7-PrepareWave`, `winmm-mciSendCommandA`. Battle.net Setup 의 launch path 와 무관, audio 는 closure 시점 이후 영역.
- wave 외 (16-bit): `krnl386.exe16-GDT_LDT_Emulation`, `mmsystem.dll16-MIDIHDR_Refcount`. 본 project 는 16-bit binary 를 다루지 않는다.
- wave 외 (theming/x11): `uxtheme-GTK_Theming`, `winex11-*`. host 가 winex11 이 아니므로 무관.
- wave 외 (d3d/wined3d application-specific): `d3drm-starwars`, `d3dx9_36_controller`, `wined3d-zero-inf-shaders`, `wined3d-rotate-WINED3D_SWAP_EFFECT_DISCARD`. 게임 별 quirk fix, Battle.net Setup launch 와 무관.
- wave 외 (msxml/oleaut application shim): `mshtml-adobe`, `msxml3-*`, `oleaut32-*`, `wbemdisp-*`. 본 project 는 application 의 com vtable 보강을 하지 않는다.
- wave 외 (input/wintab): `dinput-*`, `user32-rawinput-*`, `wintab32-improvements`. installer 단계에서 unused.

## 5. graceful skip 정책 vs wine-staging 의 conservative path

| 축 | wine-staging conservative path | 본 project graceful skip 정책 |
| --- | --- | --- |
| 미구현 API | stub + WARN log + 가능한 한 application 진행 (e.g., `nvapi-Stub_DLL`, `d3dx9_36-D3DXStubs`) | dispatch 단계에서 즉시 skip closure marker, application 이 직접 종료하도록 위임 |
| 잘못된 OS contract | wine 이 normalize (e.g., `ntdll-WRITECOPY` 의 WRITECOPY→READWRITE) | binary translation pre-flight 에서 invariant 확인 → mismatch 시 skip |
| race condition | wine server 가 deterministic 하게 만든다 (`server-Signal_Thread`) | dispatch shim 의 join + timeout, timeout skip 으로 closure |
| anti-detection | application 이 wine 임을 모르게 한다 (`ntdll-Hide_Wine_Exports`) | 본 project 는 anti-detection 우회를 시도하지 않는다 (own1 + 게임 vendor TOS) |
| installer download | inseng / msi cabinet 까지 따라간다 | 즉시 skip — 본 project 의 책임 경계 밖 |
| DRM | wine-staging 119 안에 직접 DRM patch 없음 | 본 project 도 DRM bypass 시도하지 않는다 (legal + own1 enforcement) |

요점: wine-staging 의 119 patch 는 application **진행** 을 maximize 하는 conservative path. 본 project 는 binary translation 단계 에서 dispatch 가 깨지면 **즉시 closure** 로 마무리하는 reverse path. 두 path 의 invariant (어떤 contract 가 깨지는지) 는 같으나, response 가 정반대.

## 6. 본 project 적용 path (algorithm direction only)

own1 enforcement 재확인: 아래는 **direction**. 어떤 wine 코드도 lift 하지 않는다.

1. wave 18 — PE section flag normalize layer: WRITECOPY/CoW invariant 점검 + 실패 시 skip closure.
2. wave 19 — path resolver layer: `\SystemRoot\`, junction point, reparse point 의 즉시 skip rule.
3. wave 20 — dispatch shim thread layer: deterministic join + timeout skip, virtual memory query 는 graceful skip.
4. wave 21 — exception layer: SEH 는 OS-native unwind 에 위임, cert chain NULL 은 skip.
5. wave 22 — network/installer layer: HTTPS fetch / inseng download / msi cabinet 은 즉시 skip closure (project 책임 경계 밖).
6. wave 23 — vendor stub layer: nvapi/legacy heap cap 같은 vendor-specific surface 는 stub return + skip 표시.
7. wave 24 — file ops layer: CopyFileEx progress 같은 callback path 는 skip closure.

각 wave 는 wine-staging patch 의 *intent* 를 학습 reference 로 쓰되, hunk 도, 변수 이름도, 함수 시그니처도 lift 하지 않는다. 본 project 의 stage2 hexa loader 는 자체 dispatch invariant table 을 유지하며, 각 entry 는 wave 별 self-test (`docs/raw_canonical_*` 의 rubric jsonl 형식) 로 검증한다.

## 6.1 bucket count breakdown (전수 119)

- macOS-specific: 1 (`winemac.drv-no-flicker-patch`)
- synchronization (msync/fsync/esync): 0
- cxx exception (msvcp/__cxa): 0
- SEH / exception dispatch: 1 (`ntdll-Exception`)
- PE loader / virtual memory: 3 (`ntdll-WRITECOPY`, `ntdll-ForceBottomUpAlloc`, `ntdll-NtQueryVirtualMemory`)
- thread / server: 5 (`server-Signal_Thread`, `server-PeekMessage`, `server-File_Permissions`, `server-Inherited_ACLs`, `server-Stored_ACLs`)
- ntdll/kernel32 misc: 11
- graphics (d3d/wined3d/d2d/dxgi/d3dx9/ddraw/d3drm/dcomp/vkd3d-latest): 17
- input/x11/winemac/theming: 16 (winemac 1 포함)
- audio/media/mfplat: 11
- com/oleaut/msxml/wbem/mshtml/cert/wininet/shell/installer/사용자 surface: 53 (3.4 의 graphics 외 모든 application shim, certificate, msi, inseng, fonts, gdiplus, msxml, oleaut, mshtml, riched20, sapi, wbem, win32u, windows_storagefile, windowscodecs, version, vcomp, wine.inf, winecfg, winedbg, winedevice, wineps.drv, wininet, winmm, wintab32, wintrust, ws2_32, xactengine3_7, shell32, shlwapi, mountmgr, comctl32, comdlg32, dbghelp, dinput, dmime, dmscript, explorer, fonts, kernel32-Debugger, krnl386 16-bit, loader-KeyboardLayouts, mmsystem 16-bit, msi-cabinet, nvapi 1, odbc32, sapi, stdole32, user32 multiple, advapi32 1)
- meta: 2 (`Staging`, `gitapply.sh` — 후자는 directory 가 아니므로 119 카운트 외)

총 directory 119 (`Staging` 포함). `Disabled: true` 표시: ~16 (wine-staging 자체가 비활성). 본 project 는 `Disabled` 여부와 무관하게 어떤 hunk 도 lift 하지 않는다.

## 6.2 본 project 의 dispatch invariant 와 wine-staging 의 align summary

본 project 의 stage2 hexa loader 는 binary translation 단계에서 다음의 dispatch invariant 를 검사한다 (wave 18+ 의 시점). 각 invariant 는 wine-staging 의 conservative path 와 정반대 response 를 갖는다 — 본 project 는 conservative path 를 implement 하는 게 아니라, 그 invariant 가 깨지는 순간을 graceful skip closure 로 마킹해서 binary 의 launch 가 deterministic 하게 종료되도록 만든다.

invariant 검사 list (audit cross-ref):

1. PE section flag (`SEC_IMAGE`, `SEC_NO_CHANGE`, WRITECOPY) 정합성 — wine `ntdll-WRITECOPY` cross-ref
2. PE relocation directory 가 high-pointer 영역에 가지 않음 — wine `ntdll-ForceBottomUpAlloc` cross-ref
3. NT path prefix (`\??\`, `\SystemRoot\`, `\Device\`) 처리 — wine `ntdll-NtDevicePath` cross-ref
4. reparse / junction point 부재 — wine `ntdll-Junction_Points` cross-ref
5. thread teardown 의 deterministic stop — wine `server-Signal_Thread` cross-ref
6. SEH unwind 가 OS-native — wine `ntdll-Exception` 과 다른 방향
7. cert chain NULL 방어 — wine `wintrust-WTHelperGetProvCertFromChain` cross-ref
8. HTTPS CA store 부재 시 fetch skip — wine `wine.inf-Dummy_CA_Certificate` 와 반대 방향 (본 project 는 CA store 를 만들지 않는다)
9. installer download (inseng/msi) 등장 즉시 skip — wine `inseng-Implementation`, `msi-cabinet` 과 반대 방향
10. vendor stub (nvapi 등) 호출 stub-return + skip 표시 — wine `nvapi-Stub_DLL` 과 같은 방향, 강도가 약함

## 7. own1 enforcement audit

- code lift 0 줄: 본 audit 작업 중 119 patch 의 `.patch` hunk 를 한 줄도 본 project repo (`airgenome-gamebox` 또는 `airgenome`) 로 복사하지 않았음을 self-confirm.
- 변수/함수/매크로 이름 lift 0 건: 본 문서가 reference 한 식별자 (e.g., `force_backing_store`, `exit_poll`, `try_map_view`) 는 wine 의 식별자이며, 본 project source tree 에 동일 이름 도입 금지.
- 본 audit 의 reference 는 *알고리즘 의도* level 에 한정, OS contract invariant level 로 abstract 하여 본 project 의 wave plan 에 통합.

## 8. 부록 — 119 patch 명단 (정렬)

```
advapi32-Token_Integrity_Level         comctl32_animate_avi
comctl32-rebar-capture                 comdlg32-lpstrFileTitle
d2d1_ID2D1GeometryGroup                d3d9-fake_vtbl_init
d3drm-starwars                         d3dx9_36_controller
d3dx9_36-D3DXStubs                     d3dx9_animation_TrackDesc
d3dx9-setrawvalue                      d3dx9-sprite-state
dbghelp-Debug_Symbols                  dcomp-DCompositionCreateDevice2
ddraw-GetPickRecords                   ddraw-IDirect3DTexture2_Load
dinput-joy-mappings                    dinput-scancode
dmime_segment_getaudiopath             dmscript_enum_routine
dsound-EAX                             dxgi_getFrameStatistics
explorer-Video_Registry_Key            fonts-Missing_Fonts
gdiplus-Performance-Improvements       inseng-Implementation
kernel32-CopyFileEx                    kernel32-Debugger
kernel32-limit_heap_old_exe            krnl386.exe16-GDT_LDT_Emulation
loader-KeyboardLayouts                 mf_http_support
mfplat-streaming-support               mmsystem.dll16-MIDIHDR_Refcount
mountmgr-DosDevices                    mshtml-adobe
mshtml-TranslateAccelerator            msi-cabinet
msxml3_embedded_cdata                  msxml3-FreeThreadedXMLHTTP60
msxml3-whitespace                      msxml3-write_out_doc
ntdll-APC_Performance                  ntdll-Exception
ntdll-ext4-case-folder                 ntdll-ForceBottomUpAlloc
ntdll-Hide_Wine_Exports                ntdll-Junction_Points
ntdll-NtDevicePath                     ntdll-NtQueryVirtualMemory
ntdll-RtlQueryPackageIdentity          ntdll-Serial_Port_Detection
ntdll-WRITECOPY                        nvapi-Stub_DLL
odbc32-fixes                           oleaut32_typelib_dispatch
oleaut32_VarAdd                        oleaut32-CreateTypeLib
oleaut32-default-pic-size              riched20-IText_Interface
richedit20-ImportDataObject            sapi-ISpObjectToken-CreateInstance
server-File_Permissions                server-Inherited_ACLs
server-PeekMessage                     server-Signal_Thread
server-Stored_ACLs                     shell32-ACE_Viewer
shell32-IconCache                      shell32-Progress_Dialog
shell32-Toolbar_Bitmaps                shlwapi-AssocGetPerceivedType
Staging                                stdole32.idl-Typelib
user32-DM_SETDEFID                     user32-DrawTextExW
user32-GetSystemMetrics                user32-message-order
user32-Mouse_Message_Hwnd              user32-msgbox-Support-WM_COPY-mesg
user32-rawinput-keyboard               user32-rawinput-mouse
user32-recursive-activation            uxtheme-GTK_Theming
vcomp_for_dynamic_init_i8              version-VerQueryValue
vkd3d-latest                           wbemdisp-ISWbemObject-Invoke
win32u-NtGdiExtTextOutW-rotation       windows_storagefile
windowscodecs-GIF_Encoder              windowscodecs-TIFF_Support
wine.inf-Dummy_CA_Certificate          winecfg-Libraries
winecfg-Staging                        winecfg-Unmounted_Devices
wined3d-Indexed_Vertex_Blending        wined3d-rotate-WINED3D_SWAP_EFFECT_DISCARD
wined3d-SWVP-shaders                   wined3d-unset-flip-gdi
wined3d-zero-inf-shaders               winedbg-Process_Arguments
winedevice-Default_Drivers             winemac.drv-no-flicker-patch
wineps.drv-PostScript_Fixes            winepulse-aux_channels
winepulse-PulseAudio_Support           winex11-Fixed-scancodes
winex11-ime-check-thread-data          winex11-Window_Style
winex11-WM_WINDOWPOSCHANGING           wininet-Cleanup
wininet-proxy_username                 winmm-mciSendCommandA
wintab32-improvements                  wintrust-WTHelperGetProvCertFromChain
ws2_32-af_unix                         xactengine3_7-PrepareWave
```

총 119 entry 확인. own1 enforcement: code lift 0 줄.
