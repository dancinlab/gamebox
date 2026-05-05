# 잔여 로드맵 v2 — sprint 80–82 결과 반영 (2026-05-01)

> **status**: 작성 2026-05-01 cycle 83 직전. v1 ([REMAINING_ROADMAP.md](REMAINING_ROADMAP.md), cycle 79) 작성 후 3 sprint (80/81/82) 결과 반영.
> **base commits**: f838c92 (sprint 80, 15 모듈) / 72f2571 (sprint 81, 16 모듈) / f733cd8 (sprint 82, 15 모듈 + 2 docs).
> **scope**: 본 문서는 v1 의 R1–R6 / M1–M2 8 영역을 sprint 결과 기준으로 재평가하고, **외부 결정 영역** 분리 + **v2 신규 후보** + **cycle 83 batch 분배** 제안.

---

## 1. v1 → v2 진척 요약

| 영역 | v1 점수 | v1 상태 | sprint 결과 (80/81/82 기여 모듈) | v2 잔여 |
|---|---|---|---|---|
| **M1** win32_abi_i386 fix | 23/25 | 미수정 (pre-existing FAIL) | 완료 — fe9086a `len(s)` 1-line typo 정정 (sprint 직전 cycle, v1 직후) | **없음** |
| **R1** stage7 native bridge | 14/25 | skeleton (cycle 63 wrapper) | 4 IPC frame skeleton (`pe_native_dlopen_real` / `pe_native_mmap_real` / `pe_native_mprotect_real` / `pe_native_pthread_real`, sprint 80) + helper subprocess spawn (`pe_helper_subprocess_spawn`, sprint 82) + `STAGE7_HELPER_MM_IPC_SPEC.md` (sprint 82) | helper.mm 컴파일 + entitlement codesign (**외부**) |
| **R2** PE 실 mmap | 17/25 | `pe_loader_stage2` skeleton | section/reloc/tls/page_adapter (`pe_section_mapper_real` / `pe_reloc_applier_real` / `pe_tls_init_real` / `pe_page_adapter_16k`, sprint 80) + invoke 추적 phase 2 (`pe_loader_invoke_real` / `pe_reloc_invoke_real` / `pe_tls_invoke_real` / `pe_tls_alloc_real`, sprint 82) | 실 mmap 호출 (R1 helper 의존) |
| **R3** IAT 실 resolve | 17/25 | `pe_iat_resolve_stage2` skeleton | direct + forwarder (`pe_iat_direct_resolve_real` / `pe_iat_forwarder_chain_real`, sprint 80) + api_set + delay_load (`pe_iat_api_set_redirect` / `pe_iat_delay_load_thunk`, sprint 81) | 실 patch (R2 의존) |
| **R4** binary translation B path | 21/25 | 추적기 0 (`pe_jit_codegen_track` 만 cycle 67) | `pe_to_macho_shim_track` 추적기 (sprint 80) | 실 PE → Mach-O thin 변환 + Rosetta 2 invoke 경로 (**외부**) |
| **R5** D3D → Metal | 19/25 | skeleton 0 | router + DXBC/DXIL/SV (`pe_d3d_to_metal_router` / `pe_shader_xlate_dxbc` / `pe_shader_xlate_dxil` / `pe_shader_sv_mapping`, sprint 80) + buffer/tex/PSO/cmdlist (`pe_d3d11_buffer_create_real` / `pe_d3d11_texture_create_real` / `pe_d3d12_pipeline_state` / `pe_d3d12_command_list`, sprint 81) | D3DMetal binary link + metal-shaderconverter pipeline (**외부**) |
| **R6** DLL 실 구현 | 16–22/25 | KERNEL32/USER32/etc. stub만 | T1 8 surface (`win32_{kernel32,user32,gdi32,advapi32,shell32,ole32,wininet,version}_tier1_real`, sprint 81) + T2 POSIX (`win32_kernel32_tier2_real` / `win32_advapi32_tier2_real`) + T3 struct (`win32_kernel32_tier3_struct`) + T4 heap (`win32_kernel32_tier4_heap`) + T5 SEH/TLS/CS (`pe_seh_unwinder_real` / `pe_tls_alloc_real` / `pe_critical_section_real`, sprint 82) | T2 추가 surface / T3 struct 깊이 / T4 heap pool 정합 / NTDLL 미착수 |
| **M2** folder split | 18/25 | 보류 (사용자 결정 대기) | `FOLDER_SPLIT_DRY_RUN.md` (sprint 82, 실 이동 X) | plateau 후 강행 — 사용자 trigger 대기 |

**합계 v2 잔여**: R1 외부 1 + R2 외부 1 + R3 외부 1 + R4 외부 1 + R5 외부 1 + R6 BG 4 + M2 1 = **외부 결정 5 / BG 가능 4 / 사용자 trigger 1**.

---

## 2. 외부 결정 영역 (BG agent 못 진행)

BG agent 의 sandbox / hexa-only 환경으로는 **불가능**한 작업. 사용자 또는 외부 build env 가 처리해야 함.

| # | 영역 | 외부 작업 | 차단 사유 |
|---|---|---|---|
| **E1** | helper.mm Xcode build | `airgenome_helper.mm` → `.app` bundle 컴파일 + signing | BG 에 Xcode/clang/codesign 부재. `STAGE7_HELPER_MM_IPC_SPEC.md` 가 IPC 프레임 정의했으나 실 빌드는 외부 |
| **E2** | entitlement codesign | `com.apple.security.cs.allow-jit` / `allow-unsigned-executable-memory` / `allow-dyld-environment-variables` 부착 | macOS Hardened Runtime — 사용자 keychain identity 필요 |
| **E3** | PE → Mach-O 실 변환 | R4 B path 의 thin shim header rewrite + Rosetta 2 invoke | Rosetta 2 binary translation 호출 자체는 OS-level — 사용자 env 에서 실험 |
| **E4** | D3DMetal binary link | Apple GPTK D3DMetal `.dylib` link + metal-shaderconverter 호출 | own1 정합 — 외부 binary 사용은 사용자 결정 (현 router 는 추적기 only) |
| **E5** | 사용자 build env | airgenome-gamebox `.app` 통합 + 실 게임 launch 실험 | PRIORITY_DIRECTIVE 명시 보류 영역 ("실제 작동실험 외 나머지 모두 구현") |

→ E1–E5 는 v2 BG 작업에서 제외. 본 문서는 **E5 ≠ blocker** 가정 하에 BG 진척 가능 영역만 cycle 83 배정.

---

## 3. v2 신규 작업 후보 (BG 가능)

| # | 영역 | 근거 | 예상 cycle |
|---|---|---|---|
| **N1** | NTDLL 추가 (Nt*/Zw* / RTL_*) | R6 의 미착수 surface — T1/T2/T3 가 Win32 위주, NT layer 미커버. `Rtl*Heap` / `NtCreateFile` / `RtlCompareUnicodeString` 등 | 2 cycle |
| **N2** | D3D 추가 method | R5 phase 3 — sampler state / blend state / depth-stencil / rasterizer / RTV/DSV / D3D12 descriptor heap | 1 cycle |
| **N3** | 게임 specific 확장 | sprint 82 의 D2R/D4 specific 추적기 패턴 — POE2 / Lost Ark / WoW / FFXIV / 기타 sprint 78 RoPRO 후보 | 1 cycle |
| **N4** | integration test | 128 skeleton + 46 sprint 모듈 cross-call 일관성 — 단일 entry chain 시뮬레이션 (track only) | 1 cycle |
| **N5** | phase status dashboard | 모든 R/M 영역의 현 phase / 외부 prereq / BG cycle ETA 단일 docs | 0.5 cycle |
| **N6** | 추가 만점 검토 | sprint 80–82 신규 모듈군에 대한 9 → 12 차 검토 (anti-cheat track / battle.net IPC / page_adapter 등) | 0.5 cycle |
| **N7** | NTDLL syscall stub policy | `KiUserExceptionDispatcher` / `LdrInitializeThunk` / `KiUserApcDispatcher` — Wine 0줄 유지 + 추적기 | 1 cycle |
| **N8** | DXR / mesh shader 정합 | sprint 80 dxil 모듈에 raygen/miss/closesthit 포함했으나 BLAS/TLAS / acceleration structure 미커버 | 1 cycle |

→ N1–N8 합계 ~8 cycle BG 가능. cycle 83 sprint 단일에 16 모듈 분배 가능 (모듈 ≈ 0.5 cycle 기준).

---

## 4. cycle 83 batch 후보 (16 BG)

분배 원칙: NTDLL 4 + D3D 4 + 게임 4 + 인프라 4 = 16. 각 모듈 self-test PASS + `lib/dispatch.hexa` loader array 등록 표준.

### NTDLL 4 (N1 + N7)
1. `win32_ntdll_rtl_heap_real` — `RtlCreateHeap` / `RtlAllocateHeap` / `RtlFreeHeap` / `RtlDestroyHeap`
2. `win32_ntdll_rtl_unicode_real` — `RtlInitUnicodeString` / `RtlCompareUnicodeString` / `RtlUpcaseUnicodeChar`
3. `win32_ntdll_nt_file_real` — `NtCreateFile` / `NtReadFile` / `NtWriteFile` / `NtClose` POSIX bridge
4. `win32_ntdll_ldr_init_track` — `LdrInitializeThunk` / `KiUserExceptionDispatcher` 추적기 (Wine 0줄)

### D3D 4 (N2 + N8)
5. `pe_d3d11_sampler_blend_state` — sampler / blend / depth-stencil / rasterizer state object
6. `pe_d3d12_descriptor_heap_real` — CBV/SRV/UAV/sampler descriptor heap + handle increment
7. `pe_d3d11_rtv_dsv_real` — RTV/DSV creation + bind to OM
8. `pe_d3d12_dxr_accel_struct` — BLAS/TLAS / `BuildRaytracingAccelerationStructure`

### 게임 4 (N3)
9. `pe_poe2_specific_track` — Path of Exile 2 entry chain (Steam DRM / asset bundle)
10. `pe_lost_ark_specific_track` — Lost Ark + EAC fingerprint (track only)
11. `pe_wow_specific_track` — WoW launcher + Battle.net Agent (sprint 82 IPC 재사용)
12. `pe_ffxiv_specific_track` — FFXIV Dawntrail + sqex launcher

### 인프라 4 (N4 + N5 + N6)
13. `pe_integration_chain_sim` — 128+46 모듈 cross-call 단일 entry simulation (track only)
14. `pe_phase_status_dashboard` — 모든 R/M phase + 외부 prereq tracker (모듈 형태로 자기 self-test)
15. `pe_install_coverage_v3` — sprint 80–82 결과 반영 v3 coverage (sprint 81 v2 후속)
16. `pe_remaining_roadmap_v2_anchor` — 본 문서 모듈측 anchor + 만점 검토 cross-ref tracker

→ 16 모듈 PASS + push 시 **사용자 enforcement 충족** (BG 16 default).

---

## 5. PRIORITY_DIRECTIVE 정합 재평가

PRIORITY_DIRECTIVE 4 차원 (작동 / 성능 / 자원 / 속도) 기준 sprint 80–82 의 4 차원 기여 분석.

| 차원 | sprint 80–82 기여 | 평가 | v2 추가 필요 |
|---|---|---|---|
| **작동** | R1 IPC frame + R2/R3/R5 phase 1·2 추적기 — entry point chain 의 **추적 layer** 완성 | 추적 → 실 호출 1 단계 도약. 실 작동은 helper.mm (E1) gating | E1 외부 처리 시 자동 unblock — BG 외 |
| **성능** | R5 D3D buffer/texture/PSO/cmdlist — GPU path 의 추적기 8 모듈. perf instrumentation 은 cycle 67 후속 미진척 | 추적기 단계 — 실 perf 측정 불가 (외부 GPU 의존) | N2/N8 D3D 깊이 + 실 측정은 E1 후 |
| **자원** | R6 T4 heap (`win32_kernel32_tier4_heap`) — Win32 heap 추적 / page_adapter 16k (sprint 80) | 자원 layer 첫 모듈 — heap pool 정합 미진척 | N1 RTL heap 추가로 NTDLL layer 보강 |
| **속도** | R3 phase 2 (api_set / delay_load) — DLL resolve cold-path 추적 / sprint 78 RoPRO 미진척 | resolve 추적은 진척 — 실 속도는 R3 patch (R2 의존) | E2/E3 후 측정 |

**결론**: sprint 80–82 는 **작동 차원**에 집중 기여 (helper IPC + PE/IAT/D3D 추적). 성능/자원/속도 는 추적 layer 만 — 실 측정은 외부 결정 (E1–E5) 후. v2 cycle 83 batch 는 **자원 차원** (NTDLL RTL heap) 보강 + **작동 차원** (D3D 깊이 / 게임 specific) 유지.

---

## 6. v3 trigger 조건

v2 → v3 작성 trigger:

1. **plateau 도달** — BG batch 가 2 sprint 연속 신규 surface 0 + 추적기 중복만 발생 시 (현재까지 매 sprint 신규 16 모듈, plateau 미관측)
2. **외부 결정 처리** — E1 (helper.mm build) 또는 E2 (codesign) 또는 E3 (PE → Mach-O 실 변환) 1개 이상 unblock 시 — R1/R2/R3/R4 점수 재산정 필요
3. **다른 게임 우선순위 변동** — D2R/D4 외 PRIORITY_DIRECTIVE 추가 (POE2 / Lost Ark / WoW 우선 지정) 시 게임 specific 분배 재계획
4. **만점 검토 신규 항목** — sprint 80–82 신규 영역 (anti-cheat track / battle.net IPC / page_adapter) 의 9 → 12 차 검토 결과가 v2 점수 재산정을 요구할 때
5. **M2 folder split 강행** — 사용자 trigger 시 v3 는 분리된 폴더 구조 기준 재작성

→ trigger 미발생 시 v2 가 cycle 83 + 84 + 85 sprint 동안 유효. cycle 86 즈음 plateau 또는 외부 결정 도래 시 v3 재작성.

---

*written 2026-05-01 cycle 83 직전. base: f838c92 / 72f2571 / f733cd8. own1 무관 (메타 docs). 사용자 directive: "잔여 로드맵 v2 — cycle 80-82 sprint 결과 반영 + 외부 결정 영역 분리 + 다음 phase 후보 정리".*
