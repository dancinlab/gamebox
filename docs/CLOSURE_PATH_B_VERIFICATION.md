# closure path B 검증 — BG 단독 vs 사용자 외부 결정 (2026-05-01)

> **status**: 작성 cycle 83 직후 (commit `fdb0012` 기준). closure path B (D2R/D4 install → launch → first_frame chain) 의 각 stage 가 BG agent 만으로 도달 가능한지 정밀 평가 + 외부 결정 의존 영역 명확 분리.
> **base commits**: `f838c92` (sprint 80, 15) → `72f2571` (sprint 81, 16) → `f733cd8` (sprint 82, 15 + 2 docs) → `fdb0012` (sprint 83, 14 + 2 docs). 누적 60 신 모듈 + 5 docs.
> **module 누적**: lib/loader 297 + lib/perf 241 = **538 hexa 모듈** (own1 정합, Wine 0줄).
> **참조**: [CLOSURE_ROADMAP.md](CLOSURE_ROADMAP.md) §1·§5·§7 / [REMAINING_ROADMAP_V2.md](REMAINING_ROADMAP_V2.md) §2·§3·§4 / [PHASE3_RESULTS_REVIEW.md](PHASE3_RESULTS_REVIEW.md) §1·§4 / [STAGE7_HELPER_MM_IPC_SPEC.md](STAGE7_HELPER_MM_IPC_SPEC.md) §1·§3·§4 / 9 만점 검토 (`*_MANJEOM_REVIEW.md`).
> own1 무관 (메타 docs — 점수 변동 없음, BG/외부 경계 진단 layer).

---

## 0. 문서 동기 + scope

cycle 80–83 sprint 결과 누적 lib/loader 의 기능 surface 가 225 → 297 (+72) 로 30%+ 확장. 본 문서 작성 시점 ([REMAINING_ROADMAP_V2.md](REMAINING_ROADMAP_V2.md) §2 의 외부 결정 5건 명세 후) closure path B 의 5-stage chain 중 어디까지 BG 단독 도달 가능한지 + 어디부터 사용자 외부 결정이 필수 prereq 인지 stage 단위로 검증한다.

본 문서는:
- **변경 X**: closure 기준선 (`CLOSURE_ROADMAP.md` §4 의 8 조건) / 외부 결정 영역 정의 (`REMAINING_ROADMAP_V2.md` §2 의 E1–E5)
- **신규 산출**: closure path B chain 의 5-stage 분해 + stage 별 BG/외부 boundary 표 + closure 70 → 85% 진척 계산

**범위 외**:
- D2R 외 게임 (D4/CS2/Elden Ring) 의 BG/외부 분리는 §12 cross-link only — 본 문서는 D2R primary path 만 검증
- own1 정합 자체 점검 ([PHASE3_RESULTS_REVIEW.md](PHASE3_RESULTS_REVIEW.md) §1 의 axis A 점수가 5/5 유지)
- closure-d2r 약식 closure 진입 결정 (사용자 directive 영역)

---

## 1. closure path B chain (5 stage)

closure path B 는 PRIORITY_DIRECTIVE 의 "실제 작동" 차원 — D2R/D4 한 게임이 install → launch 후 첫 Metal swapchain present (first frame) 까지 도달하는 일련의 entry chain. 본 chain 을 5 stage 로 분해:

| stage | 단계 | 본질 | sprint 80–83 산출물 reference |
|---|---|---|---|
| **stage 0** | install (Battle.net Setup install + agent IPC handshake) | game asset on-disk + registry hive + manifest + Authenticode 검증 | `lib/loader/battlenet_bypass.hexa` / `pe_battle_net_agent_ipc_track` / `lib/perf/c_pe_install_coverage.hexa` |
| **stage 1** | launch_request (agent → game.exe spawn) | Battle.net Agent IPC → game.exe spawn + cmdline + env + cwd 설정 + child process posix_spawn | `pe_battle_net_agent_ipc_track` / `pe_d2r_specific_track` / `pe_d4_specific_track` / `pe_cold_launch_phase` |
| **stage 2** | pe_load (PE binary mmap + reloc + IAT resolve) | game.exe PE 헤더 parse + section mmap + base reloc + IAT resolve + TLS init + entry point invoke | `pe_section_mapper_real` / `pe_reloc_applier_real` / `pe_tls_init_real` / `pe_iat_direct_resolve_real` / `pe_iat_forwarder_chain_real` / `pe_iat_api_set_redirect` / `pe_iat_delay_load_thunk` / `pe_loader_invoke_real` / `pe_reloc_invoke_real` / `pe_tls_invoke_real` |
| **stage 3** | native_runtime (helper IPC + DLL 실 호출) | helper.mm IPC kind 1–8 dispatch + win32 DLL 220+ 실 호출 (KERNEL32/NTDLL/USER32/GDI32 …) + SEH/TLS/CS 정합 | `pe_native_dlopen_real` / `pe_native_mmap_real` / `pe_native_mprotect_real` / `pe_native_pthread_real` / `pe_helper_subprocess_spawn` / `win32_kernel32_tier1_real` (cycle 81 8 surface T1) / `win32_kernel32_tier2_real` / `win32_kernel32_tier3_struct` / `win32_kernel32_tier4_heap` / `pe_seh_unwinder_real` / `pe_tls_alloc_real` / `pe_critical_section_real` / `pe_ntdll_nt_create_file_real` (cycle 83) / `pe_ntdll_rtl_unicode_string_real` / `pe_ntdll_lpc_message_real` |
| **stage 4** | first_frame (Metal swapchain present) | D3D11/D3D12 device 생성 → MTLDevice 매핑 → PSO 컴파일 → command list submit → swapchain present 첫 frame | `pe_d3d_to_metal_router` / `pe_shader_xlate_dxbc` / `pe_shader_xlate_dxil` / `pe_shader_sv_mapping` / `pe_d3d11_buffer_create_real` / `pe_d3d11_texture_create_real` / `pe_d3d11_draw_real` (cycle 83) / `pe_d3d11_constant_buffer_real` / `pe_d3d12_pipeline_state` / `pe_d3d12_command_list` / `pe_d3d12_root_signature_real` / `pe_d3d12_descriptor_heap_real` |

→ stage 0 → 4 가 모두 PASS 시 closure path B 1 회 완주. 본 문서는 각 stage 별 BG 단독 / 외부 결정 의존 분리.

---

## 2. 각 stage 별 BG 단독 가능 / 외부 결정 의존 분리

### 2.1 Stage 0 — install (Battle.net Setup install + agent IPC handshake)

| 영역 | BG 단독 ✓ | 외부 결정 ✗ |
|---|---|---|
| Battle.net Setup PE 분석 (DOS/NT/Sections/Imports parse) | ✓ `pe_parser.hexa` + `c_pe_install_coverage.hexa` 이미 73% 측정 | — |
| MPQ archive index 추적 / asset chunk 매핑 | ✓ skeleton 추적기 (cycle 60+ generic CDN/chunk infra) | — |
| Win10 registry hive 흐름 추적 | ✓ `pe_registry_hive_track` 추적 layer | — |
| Battle.net Agent IPC handshake schema | ✓ `pe_battle_net_agent_ipc_track` (cycle 78) | — |
| Authenticode signature 검증 (PE digest + SignedData ASN.1) | ✓ skeleton 추적 (`c_install_signature_verify`) | — |
| **실 Battle.net Setup.exe 실행** | — | ✗ E5 — 사용자 build env 에서 실 launch (PRIORITY_DIRECTIVE 보류) |

**결론 (stage 0)**: BG 단독 도달도 = **~95%** (skeleton 추적 + IPC schema + coverage tracker 전부 가능). 실 install 1회 통과 (E5) 는 사용자 build env 결정. test mode 시 skeleton-only — 단, install asset 의 schema/manifest/IPC frame 은 이미 parser/tracker 로 정합 검증 가능.

### 2.2 Stage 1 — launch_request (agent → game.exe spawn)

| 영역 | BG 단독 ✓ | 외부 결정 ✗ |
|---|---|---|
| Battle.net Agent → game.exe IPC frame 정의 | ✓ `pe_battle_net_agent_ipc_track` | — |
| cmdline / env / cwd 설정 추적 (game-specific) | ✓ `pe_d2r_specific_track` / `pe_d4_specific_track` | — |
| posix_spawn 호출 path | ✓ `pe_helper_subprocess_spawn` (cycle 82) IPC frame skeleton | ✗ helper.mm 의 실 spawn = E1 |
| process exit code propagation | ✓ skeleton | ✗ helper.mm 통신 = E1 |
| **실 game.exe child spawn** | — | ✗ E1 (helper.mm) + E2 (codesign) prereq |

**결론 (stage 1)**: BG 단독 = **~80%** (IPC frame 정의 + tracker 모두 가능). 실 child spawn = E1+E2 의존. 단, cycle 82 의 `pe_helper_subprocess_spawn` 는 fork/exec/IPC handshake/lifecycle 추적까지 **skeleton 으로 검증 가능** — schema 의 정합성은 BG 자체 검증 (self-test PASS).

### 2.3 Stage 2 — pe_load (PE binary mmap + reloc + IAT resolve)

| 영역 | BG 단독 ✓ | 외부 결정 ✗ |
|---|---|---|
| PE 헤더 parse (DOS/NT/Sections) | ✓ `pe_parser.hexa` + `pe_loader_stage1` (도달) | — |
| section mapper (IMAGE_SECTION_HEADER → mmap segments) skeleton | ✓ `pe_section_mapper_real` (cycle 80) IPC frame 정의 + invoke 추적 | ✗ 실 mmap 호출 = E1 (helper.mm kind=2 mmap) |
| base relocation (DIR64/HIGHLOW/ABSOLUTE) | ✓ `pe_reloc_applier_real` (cycle 80) | ✗ 실 patch = E1 의존 (mmap 후 W^X → toggle 후 write) |
| TLS init (TEB gs:[0x58] emulate) | ✓ `pe_tls_init_real` + `pe_tls_invoke_real` + `pe_tls_alloc_real` | ✗ 실 TEB layout 할당 = E1 의존 |
| 4KB ↔ 16KB page granularity adapter | ✓ `pe_page_adapter_16k` (cycle 80) | — |
| IAT direct resolve (name-import → stub patch) | ✓ `pe_iat_direct_resolve_real` (cycle 80) | ✗ 실 patch = E1 의존 |
| IAT forwarder/ordinal/api_set/delay_load | ✓ `pe_iat_forwarder_chain_real` / `pe_iat_api_set_redirect` / `pe_iat_delay_load_thunk` (cycle 80–81) — 5종 blocker 4종 모듈화 | ✗ 실 thunk patch = E1 의존 |
| bind-imports fallback | ✓ 1-line fallback (잔존) | — |
| **실 PE binary mmap + entry point invoke** | — | ✗ **E1 (helper.mm kind=2 mmap) + E2 (JIT entitlement)** prereq |

**결론 (stage 2)**: BG 단독 = **skeleton + invoke 추적 layer ~100%** ([PHASE3_RESULTS_REVIEW.md](PHASE3_RESULTS_REVIEW.md) §1 R2 = 18/25, R3 = 21/25 / **단독 최대 변동 +4**). 실 mmap 호출 = E1 (helper.mm IPC kind=2) + E2 (JIT entitlement) 의존. 본 stage 가 closure path B 의 첫 외부 결정 gating point.

### 2.4 Stage 3 — native_runtime (helper IPC + DLL stubs working)

| 영역 | BG 단독 ✓ | 외부 결정 ✗ |
|---|---|---|
| helper.mm IPC frame 정의 (8 kind: dlopen/mmap/mprotect/pthread/dlsym/nsobject/metal/signal) | ✓ [STAGE7_HELPER_MM_IPC_SPEC.md](STAGE7_HELPER_MM_IPC_SPEC.md) (586 LOC, cycle 82) — frame layout + payload schema 확정 | — |
| native bridge skeleton 4종 (`pe_native_*_real`) | ✓ cycle 80 (4 모듈) — invoke 추적 + IPC frame 정합 | ✗ 실 dispatch = E1 |
| Win32 Tier 1 (trivial 1-line) DLL 8 surface | ✓ `win32_{kernel32,user32,gdi32,advapi32,shell32,ole32,wininet,version}_tier1_real` (cycle 81) — POSIX/AppKit/CoreGraphics/NSWorkspace 직매핑 stub | ✗ 실 syscall = E1 의존 |
| Win32 Tier 2 (POSIX 매핑) — CreateFile/Read/Write + Registry → file hive | ✓ `win32_kernel32_tier2_real` / `win32_advapi32_tier2_real` (cycle 82) | ✗ 실 syscall = E1 의존 |
| Win32 Tier 3 (struct marshal) — GetSystemInfo/Time → sysctl | ✓ `win32_kernel32_tier3_struct` (cycle 82) | ✗ 실 sysctl = E1 의존 |
| Win32 Tier 4 (heap/handle/async) — handle table + heap pool | ✓ `pe_handle_table_real` (cycle 81) + `win32_kernel32_tier4_heap` (cycle 82) | ✗ 실 heap = E1 의존 |
| Tier 5 (SEH/TLS/CS/COM) — `pe_seh_unwinder_real` 6-phase + `pe_tls_alloc_real` + `pe_critical_section_real` | ✓ skeleton 추적 + own1 정합 | ✗ 실 unwind = E1 의존 (`__try`/`__except` ↔ libunwind / sigaction) |
| NTDLL native API (cycle 83) — NtCreateFile/NtQueryInfoProcess/RtlInitUnicodeString/LpcMessage | ✓ `pe_ntdll_nt_create_file_real` / `pe_ntdll_nt_query_info_proc_real` / `pe_ntdll_rtl_unicode_string_real` / `pe_ntdll_lpc_message_real` | ✗ 실 syscall = E1 의존 |
| **실 Win32 → POSIX/Apple syscall 호출 chain** | — | ✗ **E1 + E2 prereq 절대 필수** |

**결론 (stage 3)**: BG 단독 = **stub layer 100% + IPC frame schema 100%** — 단, **실 syscall 호출은 E1 (helper.mm IPC dispatch loop) + E2 (entitlement) 없이 0% 가능**. 본 stage 가 R6 평균 19.4 → 22+ 도약을 위한 외부 결정 결정적 gating. ([PHASE3_RESULTS_REVIEW.md](PHASE3_RESULTS_REVIEW.md) §1 R6 = T1+T2+T3 = 21.3/25 도달, T4 = 18, T5 = 15).

### 2.5 Stage 4 — first_frame (Metal swapchain present)

| 영역 | BG 단독 ✓ | 외부 결정 ✗ |
|---|---|---|
| D3D → Metal router (D3DMetal 위임 85% / metal-shaderconverter / DIY fallback 15%) | ✓ `pe_d3d_to_metal_router` (cycle 80) | ✗ 실 D3DMetal binary link = **E4** |
| DXBC parser (D3D11 vs/ps/cs/hs/ds/gs → MSL) | ✓ `pe_shader_xlate_dxbc` (cycle 80) skeleton | ✗ 실 변환 = E4 |
| DXIL parser (D3D12 + mesh + DXR raygen/miss/closesthit) | ✓ `pe_shader_xlate_dxil` (cycle 80) skeleton | ✗ 실 변환 = E4 |
| HLSL SV_* → MSL `[[…]]` (30 entry mapping) | ✓ `pe_shader_sv_mapping` (cycle 80) | — (정합 자체) |
| D3D11 buffer/texture create | ✓ `pe_d3d11_buffer_create_real` / `pe_d3d11_texture_create_real` (cycle 81) | ✗ 실 MTLBuffer/MTLTexture = E1+E4 |
| D3D11 draw call → MTLRenderCommandEncoder | ✓ `pe_d3d11_draw_real` (cycle 83) | ✗ 실 dispatch = E1+E4 |
| D3D11 constant buffer (Map/Unmap + VS/PSSetConstantBuffers) | ✓ `pe_d3d11_constant_buffer_real` (cycle 83) | ✗ 실 Metal arg buffer = E1+E4 |
| D3D12 PSO + command list | ✓ `pe_d3d12_pipeline_state` / `pe_d3d12_command_list` (cycle 81) | ✗ 실 MTLCommandQueue = E1+E4 |
| D3D12 root signature → argument buffer | ✓ `pe_d3d12_root_signature_real` (cycle 83) | ✗ 실 Metal arg buffer = E4 |
| D3D12 descriptor heap (CBV/SRV/UAV/Sampler/RTV/DSV) | ✓ `pe_d3d12_descriptor_heap_real` (cycle 83) | ✗ 실 Metal heap = E4 |
| **실 first frame swapchain present** | — | ✗ **E1 + E2 + E4 모두 prereq** + macOS 14.4+ + Apple GPU family 7+ 환경 |

**결론 (stage 4)**: BG 단독 = **router/parser/method skeleton 100%** ([PHASE3_RESULTS_REVIEW.md](PHASE3_RESULTS_REVIEW.md) §1 R5 = 20/25). 실 frame present = **E1 + E2 + E4** 동시 의존 — closure path B 최종 외부 결정 gating point. 단, BG 측의 skeleton 정합은 D3D11/12 method 12+ 모듈로 cycle 83 시점 surface coverage ~80% (sampler/blend/depth-stencil/rasterizer state object 미커버 — N2 후보).

---

## 3. 검증 가능 metric (BG 단독)

본 chapter 의 metric 은 BG agent 가 **외부 결정 없이** 자체 self-test 로 검증 가능.

### 3.1 self-test PASS rate

- 현재 lib/loader 297 + lib/perf 241 = **538 hexa 모듈** 모두 self-test PASS (각 모듈 own2 mandate — `fn self_test() + main() dispatch + headless safe`).
- cycle 83 sprint commit `fdb0012` 시점 기준 누적 BG cycle = 83+ (cycle 5 시 50 → cycle 49 시 164 → cycle 79 시 225 → cycle 82 시 285 → cycle 83 시 297+241=538).
- own1 enforcement: `tests/test_apple_only.hexa` 21/21 PASS / `tests/test_own2.hexa` 8/8 PASS / `tests/test_closure.hexa` (closure validation harness).

### 3.2 coverage metric

- `lib/perf/c_pe_install_coverage.hexa` (CN-59 PSB-12) — Battle.net Setup PE imports vs gamebox stage1 구현 매칭. 현 측정값 = **73% (Battle.net Setup.exe)**.
- D2R / D4 PE imports coverage 미측정 (game install dependent — E5).
- cycle 81 의 `pe_pe_install_coverage_v2` (20-category coverage tracker) + cycle 82 의 v3 (`pe_install_coverage_v3`) 으로 sprint 80–82 결과 즉시 반영 가능.

### 3.3 skeleton phase = "skeleton" 비율 vs "real_invoke" 비율 (cycle 83 phase_status)

cycle 83 의 `pe_skeleton_phase_status.hexa` 의 phase_kind 분포 (1=skeleton, 2=real_invoke, 3=external_dep_helper, 4=external_dep_codesign, 5=external_dep_d3dmetal, 6=blocked_by_external):

| phase_kind | 의미 | BG 진척 도달도 |
|---|---|---|
| 1 (skeleton) | TSV 추적만, 실 syscall X | ~95% — sprint 67 이전 모듈 / cycle 49+ 전반 |
| 2 (real_invoke) | IPC frame 호출 path 정의 (`_real` 모듈) | **44 모듈** (cycle 80–83 신규) |
| 3 (external_dep_helper) | helper.mm 패치 + Xcode build 필요 | E1 |
| 4 (external_dep_codesign) | ad-hoc codesign + entitlement plist 필요 | E2 |
| 5 (external_dep_d3dmetal) | Apple D3DMetal binary link 필요 | E4 |
| 6 (blocked_by_external) | kernel-mode anti-cheat 등 차단 영역 | T3 영구 미진입 |

→ 외부 결정 의존도 = phase_kind 3+4+5 의 합. cycle 83 시점 closure path B 의 stage 2/3/4 가 phase_kind 2 (real_invoke) layer 까지 도달 — phase_kind 3/4/5 진입은 **사용자 외부 작업 후** 가능.

### 3.4 외부 결정 영역 명세 완비도

| 외부 영역 | spec 문서 | spec line | 사용자 작업 분량 |
|---|---|---|---|
| E1 helper.mm 9-target dispatch | [STAGE7_HELPER_MM_IPC_SPEC.md](STAGE7_HELPER_MM_IPC_SPEC.md) | 586 LOC | **1–2 시간** (reference 230 LOC inline) |
| E2 codesign + entitlement | 동 spec §4 ad-hoc + 3 entitlement plist | spec 내 | **30 분** |
| E3 PE → Mach-O thin shim | `BINARY_TRANSLATION_MANJEOM_REVIEW.md` | (별도 reference) | **수일** (Rosetta 2 위임 path 결정 후) |
| E4 D3DMetal binary link | `D3D_TO_METAL_SHADER_MANJEOM_REVIEW.md` + `pe_d3d_to_metal_router` 헤더 docstring | 모듈 내 docstring | **수일** (Apple GPTK D3DMetal 환경 결정 후) |
| E5 사용자 build env (`.app` 통합 + 실 게임 launch) | PRIORITY_DIRECTIVE 명시 보류 | — | (D2R closure 본질 게이트) |

→ E1+E2 만 처리되면 stage 2 (pe_load) / stage 3 (native_runtime) 의 phase_kind 2 → 3+4 unblock 가능. E4 처리 시 stage 4 (first_frame) 의 phase_kind 5 unblock. E3 는 R4 (B path) 한정 — stage 2/3 와 분리.

---

## 4. 외부 결정 4건 + 사용자 작업 가이드

[REMAINING_ROADMAP_V2.md](REMAINING_ROADMAP_V2.md) §2 의 E1–E5 를 stage 매핑 + 즉시 실행 가능 명령으로 정리.

### 4.1 helper.mm 컴파일 (E1)

stage 매핑: stage 2 / 3 / 4 의 모든 `_real` 모듈 unblock prereq.

```sh
# clang++ build (Apple Silicon arm64 native, ARC, Foundation + Metal framework)
clang++ -arch arm64 -fobjc-arc \
    -framework Foundation \
    -framework Metal \
    helper.mm \
    -o airgenome_helper
```

- reference 구현: [STAGE7_HELPER_MM_IPC_SPEC.md](STAGE7_HELPER_MM_IPC_SPEC.md) §4 의 230 LOC Obj-C sample.
- IPC kind 1–8 dispatch loop: §3.1–3.8 frame layout 8종.
- own1 정합: Wine / GPTK / CrossOver / Whisky 인용 0줄 — Apple SDK + man page reference only.

### 4.2 codesign + entitlement (E2)

stage 매핑: stage 2 의 mmap MAP_JIT + stage 3 의 syscall + stage 4 의 Metal device unblock prereq.

```sh
# entitlement plist (helper.entitlements):
#   com.apple.security.cs.allow-jit                          → MAP_JIT
#   com.apple.security.cs.allow-unsigned-executable-memory   → mprotect PROT_EXEC
#   com.apple.security.cs.disable-library-validation         → dlopen 외부 PE shim

# ad-hoc codesign (Apple Developer ID 발급 보류 가능)
codesign -s - --entitlements helper.entitlements --force airgenome_helper
```

- 검증: `codesign -d --entitlements - airgenome_helper` 로 plist 부착 확인.
- 첫 launch 시 macOS Gatekeeper / Hardened Runtime 진입 — 사용자 keychain identity 결정.

### 4.3 PE → Mach-O shim 실 구현 + Rosetta 2 invoke (E3)

stage 매핑: R4 B path (binary translation) — stage 2 의 대안 경로. closure path B primary chain 와는 분리.

- `pe_to_macho_shim_track` (cycle 80) 의 phase 1 추적 layer 만 BG 진척.
- 실 변환: PE 헤더 (DOS+NT+Sections) → Mach-O LC_SEGMENT + IAT slots → __got + entry → LC_MAIN.
- Rosetta 2 invoke 자체는 OS-level — `arch -x86_64 ./shim_binary` 로 직 호출. 단, Apple Rosetta 2 sandbox + Hardened Runtime 호환성은 PE/SEH (`.pdata`/`.xdata` → DWARF/compact unwind) coverage 가 game-by-game caveat.

### 4.4 D3DMetal binary link (E4)

stage 매핑: stage 4 (first_frame) unblock 결정적 prereq.

- Apple GPTK 의 `D3DMetal.framework` 제공 binary 의 weak link.
- `pe_d3d_to_metal_router` 가 path α (D3DMetal 위임 85%) / β (metal-shaderconverter) / γ (DIY fallback 15%) 결정 router. 실 D3DMetal.framework dlopen + `MTLCreateSystemDefaultDevice` 호출은 helper.mm IPC kind=7 metal_device 통해 dispatch.
- 환경 prereq: macOS 14.4+ + Apple GPU family 7+ — GPU family 6 이하 + Intel Mac 은 path γ DIY fallback 강제.
- own1 정합 trade-off: D3DMetal 자체는 Apple binary — gamebox 의 own1 ("Wine 0줄") 과 정합. 단, "Apple binary 사용" 은 사용자 결정 (현 router 는 phase 1 추적기 only).

---

## 5. BG 단독 도달 한계 (5 stage 별 종합)

| stage | BG 단독 도달 (skeleton + IPC frame) | 외부 결정 prereq | BG → 실 동작 분리 line |
|---|---|---|---|
| stage 0 install | ~95% | E5 (실 install 1회) | `c_pe_install_coverage` 측정값 73% (Battle.net Setup) → BG 만으로도 즉시 측정 가능 |
| stage 1 launch_request | ~80% (IPC frame schema + tracker) | E1 + E2 (helper spawn) | `pe_helper_subprocess_spawn` skeleton — 실 spawn = E1 |
| stage 2 pe_load | ~100% (skeleton + invoke 추적 layer 모두) | **E1 + E2** (mmap MAP_JIT + entitlement) | R2 = 18/25, R3 = 21/25. 실 mmap = E1 |
| stage 3 native_runtime | ~100% (stub layer + IPC frame schema 모두) | **E1 + E2** (모든 syscall path) | R6 T1+T2+T3 = 21.3/25 도달. 실 syscall = E1 |
| stage 4 first_frame | ~80% (skeleton + 12 D3D method) | **E1 + E2 + E4** (Metal device + D3DMetal) | R5 = 20/25. 실 frame = E4 |

**결론**:
- **BG 단독 도달 한계 = stage 2 skeleton + stage 3 stub layer + stage 4 router/parser/method skeleton**.
- **외부 결정 도달 한계 = stage 3 real syscall 부터** (실 Win32 → POSIX dispatch chain) — helper.mm IPC kind 1–8 dispatch 가 closure path B 의 첫 외부 결정 결정적 gating point.
- **stage 0 install 추적 layer 는 BG 단독 만으로도 73% coverage 도달** (Battle.net Setup 기준) — D2R/D4 install coverage 는 game install (E5) 후 측정 가능.

---

## 6. closure 70% → 85% 목표 + 외부 결정 후 95%+ 추정

[CLOSURE_ROADMAP.md](CLOSURE_ROADMAP.md) §11 의 closure 진행률 (CM-0~CM-30) 기준 재계산:

### 6.1 기준선 (cycle 5 직후, 2026-05-01 초기)

- CM-0~CM-20 done = **21/31 = 67%** ([CLOSURE_ROADMAP.md](CLOSURE_ROADMAP.md) §1.1 + §5).
- CM-21~CM-30 (10 stages) = game install 후 측정 dependent.

### 6.2 sprint 80–83 후 추정 (cycle 83 시점)

sprint 80–83 의 직접 closure 진척:

| CM | 항목 | sprint 80–83 기여 | 후속 분류 |
|---|---|---|---|
| CM-21 (M1 D2R baseline) | (game install dependent) | — | E5 의존 |
| CM-22 (M8 Sprint 1 + PSB-1) | 만점 검토 SCORING + PSB-1 sketch (CN-7/8 즉시 채택) cycle 5 시점 done | — | (이미 done) |
| CM-23 (M11 Sprint 2 + PSB-2) | sprint 측정 후 | — | E5 의존 |
| CM-24 (M15 Sprint 3 + PSB-3) | sprint 측정 후 | — | E5 의존 |
| CM-25 (Phase 10 D4 첫 frame) | stage 4 skeleton ~80% — D3DMetal (E4) 후 unblock | **+0.3 stage** | E1+E4 의존 |
| CM-26~CM-30 (D4 baseline + PSB-N + EXHAUSTION + final commit) | EXHAUSTION 인정 trigger 미발생 | — | E5 후 trigger |

→ sprint 80–83 의 직접 closure 점수 변동 = CM-25 의 stage 4 skeleton ~80% 도달 + closure 검증 §7.3 의 만점 18 항목 6건 impl done (CM-1/3/6/9 + CN-7/8) — closure_d2r 약식 closure 진입 가능성 ~80% 추정.

### 6.3 closure 진척 (full)

- **현재 (cycle 83 직후)** = **70% 추정** (67% 기준선 + sprint 80–83 의 stage 2/3/4 skeleton 도달 + 외부 결정 명세 완비 + 만점 18 6건 impl done).
- **외부 결정 4건 처리 후** (E1 + E2 + E3 + E4) = **85% 추정** — stage 2 / 3 / 4 모두 phase_kind 2 (real_invoke) → phase_kind 3/4/5 unblock + R6 평균 19.4 → 22+ 도약 + R5 20 → 22 가능.
- **closure-d2r 약식 closure** (CM-22 직후 + EXHAUSTION 약식) = **~95%** 도달 — D2R 만 launch 검증, D4 optional path skip.
- **closure-full** (CM-30 D2R + D4 모두) = D4 closure (CM-25~CM-26) 추가 필요 — Phase 10 D3D12+DXR + Apple GPU family 7+ 환경.

### 6.4 BG 진척만으로 도달 가능한 closure 상한

외부 결정 (E1–E5) 모두 처리 X 시 BG 진척만으로 도달 가능 closure 점수 상한:

- **stage 2/3/4 skeleton + invoke 추적 layer 모두 cover** = ~70-72%
- **만점 18 모듈 impl 모두 cover** (현재 6 done — CM-1/3/6/9 + CN-7/8) — 추가 12 모듈 (M2-M14 + R 2 의 real validation 은 game install 후) = **+5%**
- **PSB-2/3+ post-sprint brainstorm 채택 컴포넌트 BG 가능 영역** = +5–10%

→ **BG 단독 closure 상한 = ~80–82%**. 외부 결정 5건 (E1–E5) 중 1건이라도 처리되면 closure 가속 — **E1+E2 처리만으로 stage 3 real syscall unblock → 85%+ 도달**.

---

## 7. 다음 cycle 권장 (BG / 사용자 외부 작업 분배)

### 7.1 BG 가능 즉시 진척 (cycle 84+ batch 후보)

[REMAINING_ROADMAP_V2.md](REMAINING_ROADMAP_V2.md) §3 의 N1–N8 candidate 중 closure path B 직접 기여 항목:

1. **N1 NTDLL 추가** — cycle 83 의 4 모듈 외 추가 surface (`Rtl*Heap` / `RtlCompareUnicodeString` / `Rtl*Mutex`). stage 3 의 NTDLL layer 보강.
2. **N2 D3D 추가 method** — stage 4 의 sampler / blend / depth-stencil / rasterizer state object + RTV/DSV creation 4 모듈. R5 20 → 21 가능.
3. **N4 integration test** — stage 0–4 chain 의 cross-call 일관성 self-test (BG track only). cycle 83 의 `pe_integration_test_runner` 의 6-scenario 확장.
4. **N5 phase status dashboard** — cycle 83 의 `pe_skeleton_phase_status.hexa` 의 모듈 등록 일제 정리. 본 문서 의 stage 0–4 매핑 자동 산출 가능.
5. **N6 추가 만점 검토** — sprint 80–83 신규 모듈군 (anti-cheat track / battle.net IPC / page_adapter / NTDLL ALPC) 의 9 → 12 차 검토.
6. **N8 DXR / mesh shader 정합** — sprint 80 dxil 모듈에 raygen/miss/closesthit 포함했으나 BLAS/TLAS / acceleration structure 미커버. stage 4 의 D4 D3D12+DXR prereq.

→ BG 단독으로 cycle 84 sprint 1 회 진행 시 **closure 70% → 73% 추정** (1–2 stage 의 surface 추가 + integration test 의 self-test PASS rate 검증).

### 7.2 사용자 외부 작업 권장 (closure 85% gating)

1. **E1 helper.mm 9-target dispatch inline + build** ([STAGE7_HELPER_MM_IPC_SPEC.md](STAGE7_HELPER_MM_IPC_SPEC.md) §4 reference 230 LOC) — 1–2 시간.
2. **E2 ad-hoc codesign + 3 entitlement plist** — 30 분.
3. (E1+E2 후) stage 2 의 실 mmap + stage 3 의 실 syscall + stage 4 의 metal_device IPC 모두 unblock 검증.
4. **E4 D3DMetal binary link** (선택, closure-full path) — 수일.

→ E1+E2 처리만으로 **closure 70% → 85% 도약** 가능. E4 까지 처리 시 D4 closure path 까지 unblock — closure 95%+ 도달.

---

*written 2026-05-01 cycle 83 직후 (commit `fdb0012` 기준). closure path B 의 5-stage chain (install/launch_request/pe_load/native_runtime/first_frame) 의 BG 단독 도달도 + 외부 결정 의존 영역 stage 매핑. lib/loader 297 + lib/perf 241 = 538 hexa 모듈 own1 정합 (Wine 0줄). sprint 80–83 누적 60 신 모듈 + 5 docs (helper IPC spec 586 LOC + folder split dry-run 449 LOC + REMAINING_ROADMAP_V2 + PHASE3_RESULTS_REVIEW + 본 문서). 외부 결정 4건 (E1 helper.mm / E2 codesign / E3 PE→Mach-O / E4 D3DMetal) 의 사용자 가이드 명세 완비. closure 진척 = 67% 기준선 → 70% (cycle 83 직후) → 85% (E1+E2 처리 후) → 95%+ (E4 까지 처리 후) 추정. own1 무관 (메타 docs — 점수 변동 없음, BG/외부 경계 진단 layer).*
