# GPTK_ANALYSIS — Apple Game Porting Toolkit 공개 정보 분석

**Status**: research / reference only
**Created**: 2026-05-02
**own1 enforcement**: GPTK 의 Wine 부분 = third-party (CrossOver/CodeWeavers source).
본 문서 = **architecture 학습 only**, code lift **0 줄**.
airgenome-gamebox 의 own1 §A (DIY hexa PE 로더 + Win32 shim, Wine 0) 는 그대로 유지.

---

## 1. GPTK 한 줄 요약

> Apple Game Porting Toolkit (GPTK) =
> **CrossOver(=Wine fork) + Apple D3DMetal (DirectX→Metal binary translator) + Rosetta 2**.

Apple 이 release 한 GPTK build 의 wine64/wineserver 등 Win32 binary path 의 **source 는 CodeWeavers / winehq.org** 가 공식 인정.
출처: CodeWeavers blog (2023-06-06): "Wine comes to macOS: Apple's Game Porting Toolkit powered by CrossOver source code".

---

## 2. 버전 history (공개 정보 기반)

| version | release | 주요 변화 |
| --- | --- | --- |
| GPTK 1.0   | 2023-06 (WWDC23) | 최초 release. CrossOver 22.1.1 base. D3DMetal beta. DX11/12 일부. |
| GPTK 1.x patch | 2023 H2 | dxgi/d3d11/d3d12 override 안정화. Battle.net launcher 일부 동작. |
| GPTK 2.0   | 2024-06 (WWDC24) | AVX2 지원 (Rosetta 2 path). D3DMetal 성능 개선. ray tracing path 추가. |
| GPTK 2.1   | 2024 H2 | shader converter 개선. Metal 3.1 활용. |
| GPTK 3.0   | 2025-2026 | shader converter (DXIL→Metal) 확장. metal-cpp. Metal Performance HUD. |

(주: 모두 공개된 Apple developer page / WWDC video / community wiki 기반.
GPTK 의 wine 부분 source 는 CodeWeavers 가 winehq 에 contribute 하는 patch 의 superset.)

---

## 3. GPTK 아키텍처 (공개 정보)

```
+-------------------------------------------------------+
|              Windows .exe / .dll  (PE32+ / x86_64)     |
+-------------------------------------------------------+
                          |
                          v
+-------------------------------------------------------+
|  Wine (CrossOver fork) — wine64 + wineserver          |
|     - PE loader (ntdll.dll.so)                         |
|     - kernel32 / user32 / gdi32 / advapi32 ...        |
|     - WINEDLLOVERRIDES dxgi/d3d11/d3d12 → native       |
+-------------------------------------------------------+
                          |
                          v   (DX11/12 calls)
+-------------------------------------------------------+
|  D3DMetal.framework (Apple proprietary)                |
|     - libd3dshared.dylib                               |
|     - DirectX → Metal binary translator                |
|     - DXIL/DXBC shader → Metal IR                      |
+-------------------------------------------------------+
                          |
                          v
+-------------------------------------------------------+
|  Metal / IOKit / kernel (Apple SDK)                    |
+-------------------------------------------------------+
                          |
                          v   (x86_64 instr. → arm64)
+-------------------------------------------------------+
|  Rosetta 2 (Apple) — instruction translator            |
+-------------------------------------------------------+
                          |
                          v
+-------------------------------------------------------+
|  Apple Silicon (M1/M2/M3/M4)                           |
+-------------------------------------------------------+
```

핵심:
- **GPTK 의 wine 부분 = third-party** (own1 §A 거부).
- **GPTK 의 D3DMetal 부분 = Apple proprietary binary** (own1 §A 허용 — "Apple binary 만 위에서 작동").
- **Rosetta 2 = Apple binary** (own1 §A 명시 허용 — instruction translator).

---

## 4. Wine vs GPTK 차이점

| 항목 | upstream Wine | GPTK |
| --- | --- | --- |
| base | winehq.org community | CrossOver 22.1.1 fork |
| platform | Linux/BSD/macOS/Windows | macOS (Apple Silicon) only |
| graphics | wined3d (DX→OpenGL/Vulkan) 또는 DXVK | **D3DMetal (DX→Metal direct)** |
| PE loader | ntdll.dll.so (Wine 자체) | CrossOver fork (동일 lineage) |
| 32-bit (i386) | wine32 지원 | **미지원** (x86_64 only) |
| AVX2 | host CPU 의존 | GPTK 2.0+ Rosetta path 지원 |
| ray tracing | DXVK 필요 | D3DMetal 직접 (Metal 3 RT API) |
| license | GNU LGPL | Apple license + LGPL (wine 부분) |
| update freq | rolling | WWDC 연간 + minor |

핵심 차이 = **그래픽 path**. 나머지 (PE loader, Win32 shim, kernel32 ...) 는 거의 동일한 Wine code 를 사용.

---

## 5. D3DMetal binary 의 own1 정합 사용

own1 §A 의 명시:
> Apple binary (D3DMetal.framework, libd3dshared.dylib, Metal API, Foundation, AppKit) 과 macOS syscalls 만 위에서 작동

→ D3DMetal 자체는 Apple binary 이므로 **dlopen / weak link 허용**.
→ GPTK 의 wine64 / wineserver / cxbottle / WINEDLLOVERRIDES env path 는 **모두 거부**.

본 project (commit `c6e7611`) 의 D3DMetal 정합 사용:
1. `native/d3dmetal_probe.sh` — D3DMetal binary 의 macOS 시스템 location 탐지 (Apple SDK only).
2. `lib/loader/pe_d3dmetal_link_status.hexa` — weak link 가능 여부 판단.
3. fallback = DIY hexa shader translator (`lib/loader/pe_shader_xlate_dxbc.hexa`, `pe_shader_xlate_dxil.hexa`, `pe_d3d_to_metal_router.hexa`).
4. `native/helper.mm` — IPC kind=7 응답이 Metal device probe (`MTLCreateSystemDefaultDevice`) — Apple Metal framework only.

따라서 D3DMetal binary 의 *호출만* 차용, GPTK 의 wine source 는 0 줄.

---

## 6. i386 (32-bit) 처리 — GPTK 미지원 vs 본 project 자체 path

GPTK 는 **x86_64 only**. wine32-on-wine64 (WoW64) path 가 macOS GPTK 에는 빠져있음.
→ Battle.net Setup launcher (i386 PE) 같은 mixed binary 는 GPTK 로는 못 돌림.

본 project 의 path (own1 자체 작성):
- `lib/loader/pe_i386_pe32_loader_real.hexa`
- `lib/loader/pe_i386_iat_resolve_real.hexa`
- `lib/loader/pe_i386_calling_conv_real.hexa`
- `lib/loader/pe_i386_seh_chain_real.hexa`
- `lib/loader/pe_i386_x87_fpu_inst_real.hexa`
- `lib/loader/pe_i386_winmain_entry_real.hexa`
- `lib/loader/i386_call_frame.hexa`
- `lib/loader/win32_abi_i386.hexa`
- `lib/loader/syscall_map_i386.hexa`

→ GPTK 가 못 하는 i386 path 를 본 project 가 직접 작성. own1 §A 정합.

---

## 7. PE loader / IAT 처리 패턴 (architecture 학습 only)

Wine (= GPTK 의 PE 부분) 의 일반적 PE loader 흐름은 **공개된 Microsoft PE/COFF spec** 과 동일:

1. DOS header 0x5A4D check
2. NT header PE\0\0 check
3. FileHeader.Machine (0x8664 PE32+ / 0x014C PE32) dispatch
4. OptionalHeader 파싱 (ImageBase, SizeOfImage, EntryPoint, DataDirectories)
5. Section header → mmap + 적절 protection (RX / RW / R)
6. Relocation (.reloc) 적용 — IMAGE_REL_BASED_DIR64 / HIGHLOW
7. IAT 채우기 — IMAGE_IMPORT_DESCRIPTOR → dlopen-equivalent → dlsym-equivalent
8. TLS callback 호출
9. Entry point jump (winmainCRTStartup → WinMain → main)

본 project 는 이 흐름을 **Microsoft 공식 PE/COFF spec 만 참조**해 hexa 로 직접 구현.
- Phase 0a: `lib/loader/pe_parser.hexa` (header parse — DONE)
- Phase 0b/1: `lib/loader/pe_loader.hexa`, `pe_loader_stage1.hexa`, `pe_loader_stage2.hexa`
- relocation: `lib/loader/pe_relocations.hexa`, `pe_reloc_applier_real.hexa`, `pe_reloc_invoke_real.hexa`
- IAT: `lib/loader/pe_imports.hexa`, `pe_iat_direct_resolve_real.hexa`, `pe_iat_dlsym_routing_real.hexa`, `pe_iat_forwarder_chain_real.hexa`, `pe_iat_api_set_redirect.hexa`, `pe_iat_delay_load_thunk.hexa`, `pe_iat_runtime_bind.hexa`
- TLS: `lib/loader/pe_tls_init_real.hexa`, `pe_tls_callback_dispatch.hexa`, `pe_tls_data_section.hexa`

→ 흐름은 동일하지만 **code 는 hexa native, Microsoft spec 만 참조** (own1 §A f).

---

## 8. airgenome-gamebox 자체 path 와의 비교 표

| 영역 | GPTK 방식 | airgenome-gamebox 방식 (own1) |
| --- | --- | --- |
| PE32+ loader | wine64 ntdll.dll.so (CrossOver fork) | `lib/loader/pe_loader*.hexa` (DIY) |
| PE32 (i386) loader | **미지원** | `lib/loader/pe_i386_pe32_loader_real.hexa` (DIY) |
| kernel32 | wine kernel32.dll.so | `lib/loader/win32_kernel32*.hexa` (DIY) |
| user32/gdi32 | wine user32/gdi32.dll.so | `lib/loader/win32_user32*.hexa`, `win32_gdi32*.hexa` (DIY) |
| ntdll | wine ntdll.dll.so | `lib/loader/win32_ntdll.hexa` + `pe_ntdll_*.hexa` (DIY) |
| MSVC CRT | wine msvcrt.dll.so | `lib/loader/win32_msvcrt.hexa` + `pe_msvc_*.hexa` (DIY) |
| SEH (x64) | wine ntdll RtlVirtualUnwind | `lib/loader/pe_seh_x64_pdata_walker.hexa` + `pe_seh_unwinder_real.hexa` (DIY) |
| SEH (i386) | wine ntdll fs:0 chain | `lib/loader/pe_i386_seh_fs0_chain.hexa` + `pe_i386_seh_dispatch_runtime.hexa` (DIY) |
| DX11/12 | D3DMetal **(Apple binary)** | D3DMetal weak link **+** DIY shader translator fallback |
| ray tracing | D3DMetal RT path | `lib/loader/pe_d3d12_raytracing.hexa` + Metal 3 RT (Apple) |
| x86_64 instr. | Rosetta 2 | Rosetta 2 (own1 §A d 허용) |
| Metal device | `MTLCreateSystemDefaultDevice` | `native/helper.mm` IPC kind=7 (Apple Metal only) |

→ 그래픽 stack (D3DMetal, Metal, Rosetta) 은 Apple binary 로 일치.
→ 나머지 (PE loader, Win32 shim, MSVC CRT, SEH) 는 **GPTK 의 wine 부분과 분리, hexa DIY**.

---

## 9. 결론 — own1 §A 의 학습 효용

GPTK 분석 = own1 enforcement 의 **negative example 학습**:
1. GPTK 의 wine 부분 = "minimal + Wine 0 + 만점" 의 Wine 0 위반.
2. CrossOver source 의 LGPL 전염성 (whole-binary linking) 은 own1 §A 의 hard rule 과 충돌.
3. GPTK 의 32-bit 미지원 = 본 project 가 i386 path 를 직접 작성하지 않으면 D2R 같은 mixed-binary 게임의 launcher path 도 못 돌림.
4. GPTK 의 D3DMetal binary 는 Apple proprietary → own1 §A 허용 → 본 project 도 그대로 weak link.

**code lift = 0**. architecture 흐름 (PE loader 단계, IAT 처리 순서, SEH chain 구조) 은 *Microsoft 공식 spec 만 참조*하고 hexa 로 자체 구현 — own1 §A f 정합.

---

## 10. 출처 (분석 only, code 0 줄)

- https://developer.apple.com/games/game-porting-toolkit/ (Apple 공식)
- https://developer.apple.com/videos/play/wwdc2023/10123/ ("Bring your game to Mac, Part 1")
- https://www.codeweavers.com/blog/mjohnson/2023/6/6/wine-comes-to-macos-apple-s-game-porting-toolkit-powered-by-crossover-source-code (CodeWeavers 공식 인정)
- https://www.applegamingwiki.com/wiki/Game_Porting_Toolkit (community wiki)
- https://github.com/Whisky-App/Whisky (Whisky README — "CrossOver 22.1.1 + Apple GPTK + D3DMetal" 명시; archived 2025-05-11)
- https://learn.microsoft.com/en-us/windows/win32/debug/pe-format (PE/COFF spec — 본 project 의 *유일* PE 참조)
- https://learn.microsoft.com/en-us/windows/win32/api/ (Win32 API spec — 본 project 의 *유일* Win32 참조)

**중요**: 본 문서의 모든 정보는 **공개 marketing / WWDC video / community wiki / CodeWeavers blog 기반**.
GPTK 또는 Wine source 의 어떤 부분도 본 project 에 lift 되지 않음. own1 §A 위반 0 건.
