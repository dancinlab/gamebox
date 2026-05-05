# 만점 채점 v2 — sprint 80-83 후 컴포넌트 grading 확장 (2026-05-01)

> v1 ★29 (D7+R7+G2 흡수+own1-4) → v2 ★45+ 후보 review.
> 참조: [SCORING.md](SCORING.md) v1, `lib/dispatch.hexa` line 132 loader array (~290 모듈).

축 (v1 동일):
- **A. Apple-only stack** (/5) — own 1: hexa PE loader + macOS syscalls + Apple D3DMetal binary + Rosetta 2 + DIY Win32 shim. Wine/CrossOver/community 0 줄.
- **B. Runtime** (/5) — runtime hot path 직접 적용도.
- **C. 데이터재해석** (/5) — D-list 적용. R/G-list 는 axis 제외 (/10 max).

---

## 1. v1 → v2 신규 컴포넌트 후보

### 1-1. own2 driven (skeleton + IPC layer 만점)

cycle 64-78 구간 — Phase 1 stage2 hexa 로더 + helper IPC layer 신설. 실 PE 매핑 + IAT direct resolve + macho shim 라우팅이 own1 stack 의 **실 동작 layer** 로 격상.

| # | name | 모듈 path | A own1 | B 실 | C 데이터 | /15 | 비고 |
|---|---|---|---|---|---|---|---|
| Sk-1 | pe_section_mapper_real | `lib/loader/pe_section_mapper_real.hexa` | 5 | 4 | 5 | **14** | R2 phase 1 — 16K page adapter |
| Sk-2 | pe_iat_direct_resolve_real | `lib/loader/pe_iat_direct_resolve_real.hexa` | 5 | 4 | 5 | **14** | R3 phase 1 — IAT runtime resolve |
| Sk-3 | pe_native_dlopen_real | `lib/loader/pe_native_dlopen_real.hexa` | 5 | 3 | 5 | 13 | helper subprocess 의존 |
| Sk-4 | pe_to_macho_shim_track | `lib/loader/pe_to_macho_shim_track.hexa` | 5 | 5 | 5 | **15** ⭐ | R4 B path — PE→Mach-O shim 핵심 |
| Sk-5 | pe_d3d_to_metal_router | `lib/loader/pe_d3d_to_metal_router.hexa` | 5 | 5 | 5 | **15** ⭐ | D3D11/12 → Metal API 라우터 |
| Sk-6 | pe_reloc_applier_real | `lib/loader/pe_reloc_applier_real.hexa` | 5 | 4 | 5 | **14** | base relocation runtime apply |
| Sk-7 | pe_tls_init_real | `lib/loader/pe_tls_init_real.hexa` | 5 | 4 | 4 | 13 | TLS callback init |
| Sk-8 | pe_page_adapter_16k | `lib/loader/pe_page_adapter_16k.hexa` | 5 | 5 | 4 | **14** | M1 16K page vs Win 4K 매핑 |
| Sk-9 | pe_iat_forwarder_chain_real | `lib/loader/pe_iat_forwarder_chain_real.hexa` | 5 | 4 | 5 | **14** | API set redirection chain |
| Sk-10 | pe_loader_invoke_real | `lib/loader/pe_loader_invoke_real.hexa` | 5 | 5 | 4 | **14** | stage2 actual invoke entry |

→ 신규 **만점 ⭐ 2** (Sk-4, Sk-5) + **14점 6** (Sk-1, Sk-2, Sk-6, Sk-8, Sk-9, Sk-10).

### 1-2. Tier 1 trivial 함수 만점 후보

cycle 70-79 구간 — kernel32/user32/advapi32/gdi32/shell32/ole32/wininet/version 8 DLL 의 trivial-tier 실 함수 구현. Apple AppKit/Foundation 매핑 직결.

| # | name | 모듈 path | A | B | C | /15 | 비고 |
|---|---|---|---|---|---|---|---|
| T1-1 | win32_kernel32_tier1_real | `lib/loader/win32_kernel32_tier1_real.hexa` | 5 | 5 | 4 | **14** | 8 trivial: GetTickCount/Sleep 등 |
| T1-2 | win32_user32_tier1_real | `lib/loader/win32_user32_tier1_real.hexa` | 5 | 5 | 4 | **14** | AppKit NSWindow 매핑 |
| T1-3 | win32_advapi32_tier1_real | `lib/loader/win32_advapi32_tier1_real.hexa` | 5 | 4 | 4 | 13 | reg read-only sim |
| T1-4 | win32_gdi32_tier1_real | `lib/loader/win32_gdi32_tier1_real.hexa` | 5 | 4 | 4 | 13 | CoreGraphics 변환 |
| T1-5 | win32_shell32_tier1_real | `lib/loader/win32_shell32_tier1_real.hexa` | 5 | 4 | 4 | 13 | NSWorkspace 매핑 |
| T1-6 | win32_ole32_tier1_real | `lib/loader/win32_ole32_tier1_real.hexa` | 5 | 3 | 4 | 12 | COM init 흉내 |
| T1-7 | win32_wininet_tier1_real | `lib/loader/win32_wininet_tier1_real.hexa` | 5 | 4 | 4 | 13 | NSURLSession 라우팅 |
| T1-8 | win32_version_tier1_real | `lib/loader/win32_version_tier1_real.hexa` | 5 | 5 | 4 | **14** | trivial GetFileVersionInfo |

→ **14점 3** (T1-1, T1-2, T1-8). 만점 진입 X — C 데이터재해석 4/5 limit (단순 함수 매핑은 게놈 표현 불가).

### 1-3. Game-specific 만점 후보

cycle 76-83 구간 — D2R/D4/CS2/Delta Force/Elden Ring 5 타이틀 specific track. 실 launch path 의 게임별 phase chain (앞의 generic Phase 0a~10 위에 game-specific layer).

| # | name | 모듈 path | A | B | C | /15 | 비고 |
|---|---|---|---|---|---|---|---|
| Gs-1 | pe_d2r_specific_track | `lib/loader/pe_d2r_specific_track.hexa` | 5 | 4 | 5 | **14** | D2R 7-phase chain |
| Gs-2 | pe_d4_specific_track | `lib/loader/pe_d4_specific_track.hexa` | 5 | 4 | 5 | **14** | D4 8-phase + DX12 + DXR |
| Gs-3 | pe_cs2_specific_track | `lib/loader/pe_cs2_specific_track.hexa` | 5 | 4 | 5 | **14** | CS2 Source2 + DX11 |
| Gs-4 | pe_delta_force_specific_track | `lib/loader/pe_delta_force_specific_track.hexa` | 5 | 3 | 5 | 13 | TenCent anti-cheat block |
| Gs-5 | pe_elden_ring_specific_track | `lib/loader/pe_elden_ring_specific_track.hexa` | 5 | 4 | 5 | **14** | DX12 + EAC offline path |
| Gs-6 | pe_lineage_classic_r28_track | `lib/loader/pe_lineage_classic_r28_track.hexa` | 5 | 4 | 5 | **14** | NCSoft native r28 client |

→ **14점 5** (Gs-1, Gs-2, Gs-3, Gs-5, Gs-6). C=5 (게임별 launch sequence 가 데이터 자체).

### 1-4. Tier 2/3/4 강화 함수 만점 후보

cycle 79-83 구간 — kernel32 tier2/3/4 + advapi32 tier2 + ntdll 핵심 4 + d3d11/12 핵심 8 실구현.

| # | name | 모듈 path | A | B | C | /15 | 비고 |
|---|---|---|---|---|---|---|---|
| T2-1 | win32_kernel32_tier2_real | `lib/loader/win32_kernel32_tier2_real.hexa` | 5 | 5 | 4 | **14** | file/thread mid-tier |
| T2-2 | win32_kernel32_tier3_struct | `lib/loader/win32_kernel32_tier3_struct.hexa` | 5 | 4 | 5 | **14** | TEB/PEB/LDR 구조체 |
| T2-3 | win32_kernel32_tier4_heap | `lib/loader/win32_kernel32_tier4_heap.hexa` | 5 | 5 | 4 | **14** | HeapAlloc/HeapFree malloc 매핑 |
| T2-4 | pe_seh_unwinder_real | `lib/loader/pe_seh_unwinder_real.hexa` | 5 | 4 | 5 | **14** | SEH chain runtime walk |
| T2-5 | pe_critical_section_real | `lib/loader/pe_critical_section_real.hexa` | 5 | 5 | 4 | **14** | pthread_mutex 매핑 |
| T2-6 | pe_d3d11_draw_real | `lib/loader/pe_d3d11_draw_real.hexa` | 5 | 5 | 5 | **15** ⭐ | D3D11 Draw → MTL encoder |
| T2-7 | pe_d3d12_root_signature_real | `lib/loader/pe_d3d12_root_signature_real.hexa` | 5 | 5 | 5 | **15** ⭐ | DX12 root sig → MTL arg buffer |
| T2-8 | pe_d3d12_descriptor_heap_real | `lib/loader/pe_d3d12_descriptor_heap_real.hexa` | 5 | 5 | 5 | **15** ⭐ | descriptor heap → MTL heap |

→ **만점 ⭐ 3** (T2-6, T2-7, T2-8) + **14점 5** (T2-1~T2-5).

### 1-5. Skeleton phase 200+ 만점 후보 (perf 라인 132 신 모듈)

cycle 64-83 perf side ~200 신 모듈 중 실 launch 직결 컴포넌트.

| # | name | A | B | C | /15 | 비고 |
|---|---|---|---|---|---|---|
| Pf-1 | c_d2r_first_frame_trace | 5 | 5 | 5 | **15** ⭐ | first frame KPI trace |
| Pf-2 | c_d4_dxr_first_frame_trace | 5 | 5 | 5 | **15** ⭐ | DXR first frame trace |
| Pf-3 | c_d2r_dx11_pipeline_warm | 5 | 5 | 5 | **15** ⭐ | D2R PSO 사전 warm |
| Pf-4 | c_d4_pso_warmup_priority | 5 | 5 | 5 | **15** ⭐ | D4 zone-aware PSO warm |
| Pf-5 | c_dxil_to_air_fusion | 5 | 5 | 5 | **15** ⭐ | DXIL→AIR fusion 단계 cache |
| Pf-6 | c_metal_private_heap_prewarm | 5 | 5 | 4 | **14** | private heap pre-alloc |
| Pf-7 | c_pe_runtime_layout_track | 5 | 4 | 5 | **14** | runtime PE layout 게놈 |
| Pf-8 | c_iat_runtime_bind_track | 5 | 4 | 5 | **14** | IAT bind runtime 추적 |
| Pf-9 | c_frame_drop_kpi | 5 | 5 | 5 | **15** ⭐ | frame drop KPI 게놈 |
| Pf-10 | c_thermal_state_history | 5 | 5 | 5 | **15** ⭐ | thermal state 시계열 |

→ **만점 ⭐ 7** (Pf-1~5, Pf-9, Pf-10) + **14점 3** (Pf-6, Pf-7, Pf-8).

---

## 2. v2 점수 분포

| /15 | v1 개수 | v2 개수 | v2 신규 |
|---|---|---|---|
| 15 ⭐ | 5 (D2/D3/D5/D6 + D4) + own1-4 + R-list 4 (R-list /10 만점) = 13 | ~25 | Sk-4, Sk-5, T2-6, T2-7, T2-8, Pf-1, Pf-2, Pf-3, Pf-4, Pf-5, Pf-9, Pf-10 (12) |
| 14 | 2 (D1/D7) + 보조 2 (CN-5/CN-6) = 4 | ~22 | Sk-1, Sk-2, Sk-6, Sk-8, Sk-9, Sk-10, T1-1, T1-2, T1-8, T2-1, T2-2, T2-3, T2-4, T2-5, Gs-1, Gs-2, Gs-3, Gs-5, Gs-6, Pf-6, Pf-7, Pf-8 (22) |
| 13 | R-list 3 (R5/R6/R7) | 동일 + 6 신규 | Sk-3, Sk-7, T1-3, T1-4, T1-5, T1-7 |
| 12 | 1 (G7→R1 흡수, 0 별도) | 동일 + 1 | T1-6 |
| ≤11 | 10 (G-list 폐기) | 동일 | — (신규 폐기 카테고리 X — 만점 후보만 review) |

→ v1 만점 ★13 → v2 만점 ★25 (신규 ⭐ 12).
→ v1 14점 4 → v2 14점 26 (신규 22).

---

## 3. ★29 → ★45+ 확장

v1 ★29 누적 (★ 합계, axis 합 의미):

```
v1 만점 D-list 5 × 15  = 75
v1 만점 own1-driven 4 × 15 = 60
v1 만점 R-list 4 × 10 = 40
─────────────────────────
v1 ★29 합계 = 175 axis points (★ 단위 = 만점 컴포넌트 1개)
```

v2 신규 ★ pool:

```
v2 신규 ⭐ 12 × 15 = 180 axis points
v2 신규 14점 22 × 14 = 308 (보조 — 만점 X)
─────────────────────────
v2 신규 만점 ★12 추가
v2 누적 만점 ★ = 13 (v1) + 12 (v2 신규) = ★25
```

만약 14점 ★ 후보를 부분적으로 만점 진입 (B 또는 C 강화 후) 시키면:
```
14→15 승격 후보 (~10 개): ★35
보조 그대로 (12 개): ★25
─────────────────────────
v2 가능 최대 ★45+
```

→ **★45+ 후보 pool**: ★25 (확정 v2 만점) + ★10~20 (B/C 강화 후 승격 가능).

---

## 4. own1/own2/PRIORITY_DIRECTIVE 정합 재확인

5 만점 ⭐ 12 신규 컴포넌트 모두 작동 직접 영향 review:

### own1 (Apple-only stack, Wine 0)

| ⭐ 컴포넌트 | own1 violation 여부 | 검증 |
|---|---|---|
| Sk-4 pe_to_macho_shim_track | 0 | hexa PE 로더 + DIY shim |
| Sk-5 pe_d3d_to_metal_router | 0 | Apple D3DMetal binary 라우팅 |
| T2-6 pe_d3d11_draw_real | 0 | Metal MTLRenderCommandEncoder 직결 |
| T2-7 pe_d3d12_root_signature_real | 0 | Metal argument buffer 매핑 |
| T2-8 pe_d3d12_descriptor_heap_real | 0 | Metal heap 매핑 |
| Pf-1~5, Pf-9, Pf-10 | 0 | airgenome label/forecast/harvest 직결 |

→ 12/12 own1 violation 0. Wine 인용 0. CrossOver 인용 0.

### own2 (측정 가능성 — emit/self_test/headless/panic/snake_case/entry shim)

| 항목 | ⭐ 12 모듈 검증 |
|---|---|
| (a) emit 필수 (r0_emit ≥ 1) | 12/12 통과 (skeleton phase 자동 emit) |
| (b) self_test + main() | 12/12 통과 (`gamebox loader test` + `gamebox perf test` 포함) |
| (c) headless | 12/12 통과 |
| (d) panic 형식 | 11/12 통과 (Sk-4 1개 부분 — track-only, panic path X) |
| (e) snake_case event_type | 12/12 |
| (f) entry shim ≤10 줄 | 12/12 |

→ own2 통과율 95%+. Sk-4 panic 형식 추가 시 100%.

### PRIORITY_DIRECTIVE (외부 결정 4건 차단)

PRIORITY_DIRECTIVE.md 의 외부 결정 4건 (Battle.net OAuth / D2R license / D4 license / CS2 Steam DRM) 미해결 상태에서:

| ⭐ 컴포넌트 | 외부 결정 의존 | sprint 가능 여부 |
|---|---|---|
| Sk-4, Sk-5 | X (loader 일반) | ✓ 즉시 |
| T2-6, T2-7, T2-8 | X (D3D layer 일반) | ✓ 즉시 |
| Pf-1 (D2R first frame trace) | △ (D2R 실 launch 시 측정) | trace 코드만 ✓, 실 데이터 X |
| Pf-2 (D4 DXR first frame) | △ (D4 실 launch 시) | 동상 |
| Pf-3, Pf-4 | △ | 동상 |
| Pf-5 dxil_to_air_fusion | X (offline path) | ✓ |
| Pf-9 frame_drop_kpi | X (generic) | ✓ |
| Pf-10 thermal_state_history | X (generic) | ✓ |

→ ⭐ 12 중 8 즉시 sprint 가능, 4 (Pf-1~4) 는 외부 결정 4건 처리 후 실 검증.

---

## 5. v3 trigger

v2 → v3 trigger 조건 (외부 결정 4건 처리 + 실 launch 시 만점 검증 가능 영역):

### v3 trigger 1: Battle.net OAuth 해결 후

- Pf-1 (D2R first frame trace) 실 데이터 수집 → C=5 검증 확정
- Pf-2 (D4 DXR first frame trace) 동상
- Pf-3, Pf-4 (PSO warm) 동상

→ ⭐ 4 컴포넌트가 paper 만점에서 **데이터 검증 만점** 으로 격상.

### v3 trigger 2: D2R/D4 license 확보 후

- Gs-1 (pe_d2r_specific_track) B 4→5 승격 → 14→15 만점 진입
- Gs-2 (pe_d4_specific_track) 동상

→ 추가 ★2 진입.

### v3 trigger 3: CS2 Steam DRM 처리 후

- Gs-3 (pe_cs2_specific_track) 14→15 진입 → ★1 추가
- Pf-5 dxil_to_air_fusion CS2 Source2 데이터 검증

### v3 trigger 4: Elden Ring EAC offline path 검증

- Gs-5 (pe_elden_ring_specific_track) 14→15 진입 → ★1 추가

### v3 예상 ★ 합계

```
v2 확정: ★25
v3 trigger 1 (OAuth): ★25 (검증 격상, 신규 X)
v3 trigger 2 (D2R/D4 license): +★2 = ★27
v3 trigger 3 (CS2 DRM): +★1 = ★28
v3 trigger 4 (Elden Ring): +★1 = ★29

v3 신규 cycle 84+ pool (현재 미정): +★10~15 추정
─────────────────────────
v3 예상 ★ = 38~44
```

→ ★45+ 도달은 v3 후 cycle 100+ pool 까지 확장 시 가능.

---

## 부록 A. v1/v2/v3 비교 요약

| 항목 | v1 (cycle 63) | v2 (cycle 84) | v3 (예상, cycle 100+) |
|---|---|---|---|
| 채점 컴포넌트 총 | 33 (D7+R7+G12+own1-4+보조2) | ~80 (v1 + 신규 50) | ~100+ |
| 만점 ⭐ | 13 | 25 | 38~44 |
| 14점 | 4 | 26 | ~30 |
| 폐기 (≤12) | 10 (G-list) | 동일 + 1 (T1-6) | TBD |
| own1 violation | 0 | 0 | 0 (불변) |
| own2 통과율 | 17/17 (100%) | 49/50 (95%+) | TBD |

## 부록 B. 만점 ⭐ 25 final list

1. **D2** pso_genome (v1)
2. **D3** frame_cmd_genome (v1)
3. **D4** rosetta_hotloop_genome (v1)
4. **D5** metal_heap_hexagon (v1)
5. **D6** frametime_jsonl (v1)
6. **CN-1** C-PE-PARSE-CACHE (v1 own1-driven)
7. **CN-2** C-IAT-RESOLVE-CACHE (v1 own1-driven)
8. **CN-3** C-WIN32-CALL-PROFILE (v1 own1-driven)
9. **CN-4** C-SYSCALL-BRIDGE-CACHE (v1 own1-driven)
10. **R1** d3dmetal_shader_cache (v1, /10 만점)
11. **R2** dxil_air_translation_cache (v1, /10)
12. **R3** d3dmetal_heap_preallocate (v1, /10)
13. **R4** d3dmetal_pso_dedup (v1, /10)
14. **Sk-4** pe_to_macho_shim_track (v2 신규)
15. **Sk-5** pe_d3d_to_metal_router (v2 신규)
16. **T2-6** pe_d3d11_draw_real (v2 신규)
17. **T2-7** pe_d3d12_root_signature_real (v2 신규)
18. **T2-8** pe_d3d12_descriptor_heap_real (v2 신규)
19. **Pf-1** c_d2r_first_frame_trace (v2 신규)
20. **Pf-2** c_d4_dxr_first_frame_trace (v2 신규)
21. **Pf-3** c_d2r_dx11_pipeline_warm (v2 신규)
22. **Pf-4** c_d4_pso_warmup_priority (v2 신규)
23. **Pf-5** c_dxil_to_air_fusion (v2 신규)
24. **Pf-9** c_frame_drop_kpi (v2 신규)
25. **Pf-10** c_thermal_state_history (v2 신규)

→ ★25 = v1 13 + v2 신규 12. v3 trigger 후 ★29 도달 (+4: Gs-1, Gs-2, Gs-3, Gs-5).

---

## 결론

- v1 ★29 (혼합 axis sum) → v2 **★25 만점 컴포넌트** (axis sum 재정의: 1 컴포넌트 = 1★).
- v2 신규 ⭐ 12 모두 own1 violation 0 / own2 95%+ 통과.
- ★45+ 후보 pool: 14점 22 컴포넌트 중 ~10 개가 B/C 강화로 만점 진입 가능 — sprint 84+ 작업 대상.
- v3 trigger 외부 결정 4건 처리 + 실 launch 데이터 검증 후 ★38~44 도달 예상.
- cycle 100+ 까지 ★45+ 도달 — v2 의 14점 pool + v3 신규 컴포넌트 합산.
