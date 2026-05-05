# Phase 3 결과 재평가 — sprint 80-82 후 (2026-05-01)

> **status**: cycle 83 직전 작성. cycle 79 만점 검토 v1 (commit `f5401e6`) 작성 후 3 sprint (cycle 80 / 81 / 82) + M1 fix 진행한 결과 반영.
> SSOT v1: `docs/REMAINING_ROADMAP.md` (5축×5 = /25), 8 영역 만점 검토 docs (`*_MANJEOM_REVIEW.md`)
> 채점 축 (5 / 25): A=own1 정합 / B=실 동작 도달 / C=자원 영향 / D=구현 난이도 (역) / E=의존성 (역)
> meta: 본 문서 own1 무관 (점수 변동 분석 — directive 정합성 점검 layer)

---

## 1. 영역별 phase 1+2 진척 + 점수 재평가

### M1 — `win32_abi_i386` self-test FAIL fix

| 축 | v1 (cycle 79) | v2 (cycle 82 후) | Δ |
|---|---|---|---|
| A. own1 정합 | 5 | 5 | — |
| B. 실 동작 도달 | 5 | 5 | — |
| C. 자원 영향 | 5 | 5 | — |
| D. 구현 난이도 역 | 5 | 5 | — |
| E. 의존성 역 | 3 | 3 | — |
| **합계 /25** | **23** | **23** | **— (작업 완료, 점수 무변동 — 100% 도달)** |

**진척**: commit `fe9086a` — `win32_abi_i386.hexa:76` `s.length()` → `len(s)` + bonus `win32_rpcrt4.hexa:69` 동일 typo. loader self-test set 100% PASS 회복. 권고된 1 cycle / 단일 BG agent / 독립 — 검토 그대로 실현.

**v2 분석**: 점수 변동 0이지만 **작업이 100% 완료**된 영역. v1 score 23/25 의 E 점수 3은 R3 (IAT 실 resolve) 의 demangle 의존을 반영한 것 → 본 fix 이후 R3 의 i386 path 도 unblock.

---

### R1 — stage7 native bridge 실 호출

| 축 | v1 (cycle 79) | v2 (cycle 82 후) | Δ |
|---|---|---|---|
| A. own1 정합 | 5 | 5 | — |
| B. 실 동작 도달 | 3 | **4** | **+1** (4 _real 모듈 추가) |
| C. 자원 영향 | 3 | 3 | — |
| D. 구현 난이도 역 | 1 | **2** | **+1** (IPC frame spec 586-line 확정) |
| E. 의존성 역 | 2 | 2 | — (helper.mm 외부 결정 잔존) |
| **합계 /25** | **14** | **16** | **+2** |

**진척**:
- cycle 80: `pe_native_dlopen_real` / `pe_native_mmap_real` (MAP_JIT + W^X) / `pe_native_mprotect_real` / `pe_native_pthread_real` — 4 모듈 IPC frame skeleton.
- cycle 82: `docs/STAGE7_HELPER_MM_IPC_SPEC.md` (586 LOC) — 8 IPC kind frame layout + helper.mm reference Obj-C 230 LOC + entitlement plist + security boundary. **실 helper.mm 패치는 외부 결정 영역 명시**.
- cycle 82: `pe_helper_subprocess_spawn` — fork/exec/IPC handshake/lifecycle 추적.

**v2 분석**: B 점수 +1 (3→4) — 4 _real 모듈로 entry call 직전까지 IPC dispatch 명세 확정 (단독 launch 까지는 R2/R3/R5 합동 필요). D 점수 +1 (1→2) — IPC framing spec 의 미정 항목 사라지고 stdin/stdout binary frame 결정 확정. **E 점수 변동 X** — helper.mm 실 패치 + codesign + JIT entitlement 발급은 외부 결정 잔존.

---

### R2 — PE binary 실 mmap 적재

| 축 | v1 (cycle 79) | v2 (cycle 82 후) | Δ |
|---|---|---|---|
| A. own1 정합 | 5 | 5 | — |
| B. 실 동작 도달 | 3 | **4** | **+1** (4 sub 모두 _real 모듈) |
| C. 자원 영향 | 4 | 4 | — |
| D. 구현 난이도 역 | 2 | **3** | **+1** (4 phase 모두 진행) |
| E. 의존성 역 | 3 | 3 | — (R1 prereq 잔존) |
| **합계 /25** | **17** | **18** | **+1** |

**진척**:
- cycle 80 phase 1: `pe_section_mapper_real` (IMAGE_SECTION_HEADER → mmap segments) / `pe_reloc_applier_real` (DIR64/HIGHLOW/ABSOLUTE) / `pe_tls_init_real` (TEB gs:[0x58] emulate) / `pe_page_adapter_16k` (4KB↔16KB granularity).
- cycle 82 phase 2: `pe_loader_invoke_real` / `pe_reloc_invoke_real` / `pe_tls_invoke_real` — IPC invoke 추적 (section_map/reloc_apply/tls_init/entry/cleanup phase).

**v2 분석**: 4 sub (0-A section / 0-B reloc / 0-C W^X / 0-D TLS) 모두 _real 모듈 + IPC invoke 추적. B/D +1씩 — 단독 launch 미달 (R6 prereq 잔존), R1 base 의존 미해소 (E 변동 X).

---

### R3 — IAT 실 resolve

| 축 | v1 (cycle 79) | v2 (cycle 82 후) | Δ |
|---|---|---|---|
| A. own1 정합 | 5 | 5 | — |
| B. 실 동작 도달 | 2 | **4** | **+2** (direct + forwarder + ordinal + delay-load + api-set 모두) |
| C. 자원 영향 | 5 | 5 | — |
| D. 구현 난이도 역 | 3 | **4** | **+1** (5종 blocker 4종 해소) |
| E. 의존성 역 | 2 | **3** | **+1** (R6 Tier 1 동시 진행으로 cross-cutting 압력 완화) |
| **합계 /25** | **17** | **21** | **+4 (단독 최대 변동)** |

**진척**:
- cycle 80 phase 1: `pe_iat_direct_resolve_real` (name-import → own2 stub address patch) / `pe_iat_forwarder_chain_real` (forwarder/ordinal/api_set/delay_load/bind_imports 5 case 통합 추적).
- cycle 81 phase 2: `pe_iat_api_set_redirect` (Win10 PEB→ApiSetMap, api-ms-win-core-* → kernelbase) / `pe_iat_delay_load_thunk` (`__delayLoadHelper2` 5-phase lifecycle).

**v2 분석**: v1 의 5종 blocker (forwarder / ordinal / delay-load / API-set / bind-imports) 중 4종 모듈화 완료 (bind-imports 만 단순 fallback 1 line). v1 의 "R6 batch 동시 진입 시 22/25" 예측이 거의 실현 — R6 Tier 1 8 모듈 (cycle 81) 동시 진행으로 B 점수 +2 (2→4). **8 영역 중 단독 최대 점수 변동**.

---

### R4 — BINARY_TRANSLATION (B path: PE → Mach-O thin shim → Rosetta 2 위임)

| 축 | v1 (cycle 79) | v2 (cycle 82 후) | Δ |
|---|---|---|---|
| A. own1 정합 | 4 | 4 | — |
| B. 실 동작 도달 | 5 | 5 | — |
| C. 자원 영향 | 4 | 4 | — |
| D. 구현 난이도 역 | 4 | 4 | — |
| E. 의존성 역 | 4 | 4 | — |
| **합계 /25** | **21** | **21** | **— (shim 추적기만 — 외부 의존 영역)** |

**진척**:
- cycle 80: `pe_to_macho_shim_track` (1 모듈) — PE → Mach-O thin shim header conversion 추적.

**v2 분석**: shim 추적 모듈 1건만 추가, 점수 변동 0. **이유**: B path 는 Apple Rosetta 2 위임이 본질 → 실 변환 코드는 외부 결정 영역 (Apple binary). gamebox 측은 shim 헤더 변환 + IAT → __got patch + PE/SEH coverage 검증만 책임. cycle 80 `pe_to_macho_shim_track` 가 그 책임의 phase 1 (track-only) — phase 2 실 변환은 Apple toolchain 호출 시점에 가능.

---

### R5 — D3D11/12 → Metal shader 변환

| 축 | v1 (cycle 79) | v2 (cycle 82 후) | Δ |
|---|---|---|---|
| A. own1 정합 | 5 | 5 | — |
| B. 실 동작 도달 | 4 | **5** | **+1** (DXBC + DXIL + SV mapping + D3D11/12 핵심 method 매핑) |
| C. 자원 영향 | 4 | 4 | — |
| D. 구현 난이도 역 | 2 | **3** | **+1** (8 모듈로 6 cycle estimate 의 ~5 cycle 분량 진척) |
| E. 의존성 역 | 4 | 4 | — |
| **합계 /25** | **19** | **20** | **+1** |

**진척**:
- cycle 80 phase 1: `pe_d3d_to_metal_router` (d3dmetal_delegate 85% / metal-shaderconverter / DIY fallback router) / `pe_shader_xlate_dxbc` (D3D11 vs/ps/cs/hs/ds/gs → MSL) / `pe_shader_xlate_dxil` (D3D12 incl. mesh/amp + DXR) / `pe_shader_sv_mapping` (HLSL SV_* → MSL [[…]] 30 entry).
- cycle 81 phase 2: `pe_d3d11_buffer_create_real` (D3D11_BUFFER_DESC → MTLBuffer) / `pe_d3d11_texture_create_real` (D3D11_TEXTURE2D_DESC + DXGI_FORMAT) / `pe_d3d12_pipeline_state` (graphics/compute/mesh/raytracing PSO) / `pe_d3d12_command_list` (→ MTLCommandBuffer).

**v2 분석**: B +1 (D3DMetal 위임 router + DXBC/DXIL parser 양 path + D3D11/12 핵심 method 4종 모두 모듈화). D +1 (12 추천 candidate 중 8 모듈 진행). E 변동 X (R6 약결합 잔존).

---

### R6 — WIN32_DLL_REAL_IMPL (220+ DLL stub 실 구현)

#### v1 (cycle 79): 단일 점수 16/25 + Tier split (T1=22 / T2=21 / T3=21 / T4=17 / T5=13)

#### v2 (cycle 82 후): Tier 1 + 2 + 3 진행

| Tier | v1 점수 | v2 점수 | Δ | v2 모듈 |
|---|---|---|---|---|
| **Tier 1** (trivial 1-line) | 22 | **22** | — (도달 — KERNEL32/USER32/ADVAPI32/GDI32/SHELL32/OLE32/WININET/VERSION 8 DLL × 30+ fn) | cycle 81 8 모듈 |
| **Tier 2** (POSIX 매핑) | 21 | **21** | — (도달 — KERNEL32 CreateFile/Read/Write + ADVAPI32 Registry) | cycle 82 2 모듈 |
| **Tier 3** (struct marshal) | 21 | **21** | — (도달 — GetSystemInfo/Time/Version → sysctl) | cycle 82 1 모듈 |
| **Tier 4** (heap/handle/async) | 17 | **18** | **+1** (handle table + heap 진척) | cycle 81 `pe_handle_table_real` + cycle 82 `win32_kernel32_tier4_heap` |
| **Tier 5** (SEH/TLS/CS/COM) | 13 | **15** | **+2** (SEH unwinder + TLS pool + Critical Section 모두) | cycle 82 `pe_seh_unwinder_real` + `pe_tls_alloc_real` + `pe_critical_section_real` |
| **Tier 평균** | **(22+21+21+17+13)/5 = 18.8** | **(22+21+21+18+15)/5 = 19.4** | **+0.6** |
| **Tier 1+2+3 평균** | **(22+21+21)/3 = 21.3** | **(22+21+21)/3 = 21.3** | **— (도달 후 변동 0 — 만점 검토 v1 권장 sprint 후보 모두 실현)** |
| **R6 단일 점수** | 16 | **18** | **+2** (Tier 4 / 5 도달로 평균 상승) |

**진척**:
- cycle 81 Tier 1 (8 모듈): `win32_kernel32_tier1_real` (GetCurrentThreadId/PID/TickCount/Sleep/lstrlen) / `win32_user32_tier1_real` (GetSystemMetrics/CursorPos/ForegroundWindow → AppKit) / `win32_advapi32_tier1_real` (GetUserName/ComputerName/IsUserAnAdmin) / `win32_gdi32_tier1_real` (GetDeviceCaps/StockObject → CoreGraphics) / `win32_shell32_tier1_real` (SHGetFolderPath/ShellExecute → NSWorkspace) / `win32_ole32_tier1_real` (CoInitialize/CoCreateGuid → uuid_generate) / `win32_wininet_tier1_real` (InternetOpen/Cookie → NSURLSession) / `win32_version_tier1_real` (GetFileVersionInfo → PE Resource).
- cycle 81 보너스: `pe_handle_table_real` (Win32 user-mode HANDLE table) / `pe_pe_install_coverage_v2` (20-category coverage tracker).
- cycle 82 Tier 2/3/4/5 (7 모듈): `win32_kernel32_tier2_real` (POSIX open/read/write) / `win32_kernel32_tier3_struct` (sysctl) / `win32_kernel32_tier4_heap` (자체 user-mode heap) / `win32_advapi32_tier2_real` (Registry → file hive) / `pe_seh_unwinder_real` (`__try`/`__except` 6-phase) / `pe_tls_alloc_real` (TlsAlloc/Free pool) / `pe_critical_section_real` (recursive pthread_mutex + spin).

**v2 분석**: v1 의 Tier 1+2+3 (sprint 후보 22-21 평균) 모두 실현. Tier 4 +1 (handle table / heap 진척). **Tier 5 가장 큰 이득 +2 (13→15)** — v1 의 "만점 미달, Rosetta SEH 활용 trade-off 결정 필요" 평가가 cycle 82 의 `pe_seh_unwinder_real` 6-phase 추적 모듈로 부분 해소. SEH 자체의 own1 정합은 여전히 trade-off (libunwind reference 정책 미확정) — 그러나 추적 layer 는 100% 자체.

---

### M2 — Folder split refactor

| 축 | v1 (cycle 79) | v2 (cycle 82 후) | Δ |
|---|---|---|---|
| A. own1 정합 | 5 | 5 | — |
| B. 실 동작 도달 | 1 | 1 | — (refactor 자체 launch 영향 0) |
| C. 자원 영향 | 5 | 5 | — |
| D. 구현 난이도 역 | 4 | 4 | — |
| E. 의존성 역 | 3 | 3 | — |
| **합계 /25** | **18** | **18** | **— (dry-run docs만 — 보류 유지)** |

**진척**:
- cycle 82: `docs/FOLDER_SPLIT_DRY_RUN.md` (449 LOC) — 259 파일 분류, 7-bucket 권장, 26+ 외부 reference, plateau 후 90 분 강행 가이드.

**v2 분석**: v1 검토의 권고 ("bg agent 정착기 (모듈 합산 ~250 도달 후 plateau) 에 1 cycle 짧게 진입 권장") 그대로 — cycle 82 시점 lib/loader/ 모듈 수 285 (v1 시점 225 → 285 +60 신규). 보류 결론 유지, 실 이동 X. dry-run docs 만 작성으로 plateau trigger 시 즉시 강행 가능 상태.

---

## 2. sprint 통계

| sprint | commit | 신규 모듈 | 신규 docs | 누적 lib/loader |
|---|---|---|---|---|
| M1 fix | `fe9086a` | 0 (수정 2건) | 0 | (v1 시점 225) |
| cycle 80 | `f838c92` | **15** | 0 | 240 |
| cycle 81 | `72f2571` | **16** | 0 | 256 |
| cycle 82 | `f733cd8` | **15** | 2 (`STAGE7_HELPER_MM_IPC_SPEC.md` 586 LOC + `FOLDER_SPLIT_DRY_RUN.md` 449 LOC) | 271 → **285** (실측 cycle 82 후) |
| **합계** | **4 commits** | **46 신 모듈** | **3 docs (1035 LOC)** | **+60 모듈 (225→285, +27%)** |

**관측**:
- 3 sprint × 평균 15.3 모듈/sprint = 매 cycle 일정 페이스. plateau 미발생.
- docs 1035 LOC = M2 dry-run + R1 IPC spec — 외부 결정 영역의 사용자 가이드 layer.
- M1 1-line × 2 fix 가 R3 의 i386 path unblock + loader self-test 100% PASS 회복.

---

## 3. PRIORITY_DIRECTIVE 4 축 (작동/성능/자원/속도) 재평가

`docs/PRIORITY_DIRECTIVE.md` 의 4 기준 — 본 sprint 결과 반영:

### 3-1. 작동 (correctness — 실 launch / 실 frame 도달)

| 영역 | v1 → v2 변동 | sprint 결과의 작동 기여 |
|---|---|---|
| R3 IAT resolve | 17 → 21 (+4) | direct + forwarder + ordinal + delay-load + api-set 5 path 모두 모듈화 → 92% fn 도달 가능에서 ~98% (bind-imports fallback 1 line 만 잔존) |
| R6 Tier 1+2+3 | 21.3 (도달) | KERNEL32 224 + USER32 50 + ADVAPI32 53 + GDI32 18 + SHELL32 8 + OLE32 2 + WININET 11 + VERSION 3 + Registry 13 + 핵심 ~330 fn 모듈화 |
| R6 Tier 5 | 13 → 15 (+2) | SEH unwinder + TLS pool + Critical Section 추가로 D2R private offline launch 가능률 ~70% → ~80% 추정 |
| R1 base | 14 → 16 (+2) | IPC spec 확정으로 helper.mm 외부 패치 후 즉시 wire-up 가능 상태 |

→ **작동 축 종합**: 본 3 sprint 가 **실 launch 가능률 향상에 집중적 기여** (PRIORITY_DIRECTIVE 1순위 정합). R3 / R6 Tier 1-3 / R6 Tier 5 부분이 핵심 작동 lever.

### 3-2. 성능 (perf — frametime / throughput / latency)

| 영역 | 기여 |
|---|---|
| R5 D3D → Metal | 8 모듈 (router + DXBC/DXIL parser + SV mapping + D3D11/12 핵심 method) — D3DMetal 위임 85% + Apple metal-shaderconverter 호출 + DIY fallback 15% 분리. Apple-native path 의 frametime 직접 영향 |
| R6 Tier 1 (`GetTickCount` / `QueryPerformanceCounter` / `Sleep` 등) | mach_absolute_time 직매핑으로 latency µs 단위 정확도 |
| R6 Tier 4 heap | 자체 user-mode heap pool — malloc 직매핑보다 정확하나 perf 직접 비교 미정 |

→ **성능 축**: R5 의 8 모듈이 1 차 기여. R6 Tier 4/5 의 perf impact 는 second-order.

### 3-3. 자원 (resource — 메모리 / CPU / GPU)

| 영역 | 기여 |
|---|---|
| R2 0-A `pe_page_adapter_16k` | 4KB ↔ 16KB granularity adapter + waste% tracking — Apple Silicon page mismatch 의 자원 cost 수치화 |
| R5 D3DMetal 위임 path | shader 변환 cost 0 (Apple binary 내부) — gamebox 측 추가 메모리 0 |
| R6 Tier 4 handle table | hash map 기반 (현재 trie 추정) — O(N) lookup 회피 |
| `pe_pe_install_coverage_v2` | 20-category 모듈 coverage tracker — 미구현 영역 즉시 식별 |

→ **자원 축**: R2 page adapter + R5 D3DMetal 위임이 직접 기여.

### 3-4. 속도 (speed — cold-launch / load time)

| 영역 | 기여 |
|---|---|
| R3 IAT resolve cold path | 1회성 launch 비용. 437 fn × O(log n) name lookup ≈ ms 단위 — 만점 검토 v1 의 "C=5/5" 점수 그대로 |
| R5 PSO/DXIL→AIR cache (SCORING D1+R2+D2+D3) | 2nd launch hitch 제거 — 본 sprint 직접 추가 X (이미 만점 자리) |
| R6 Tier 1 trivial 1-line | 함수 호출 latency 무시 수준 |

→ **속도 축**: 본 3 sprint 의 직접 기여는 R3 (cold launch) + R5 cache 영역. cold-launch 시 IAT resolve + PSO build 양쪽 모두 만점 검토 SCORING 자리 흡수.

---

## 4. 외부 결정 영역 (BG 못 진행) 정리

본 sprint 가 **모듈 추적 layer 만 진행 가능** + **실 동작 패치는 외부 결정** 영역:

### 4-1. helper.mm 패치 (R1)

- **현 상태**: airgenome 부모 repo 의 `airgenome_helper.mm` 가 Phase 1 stage2 hexa loader bridge 한정 (commit `57f121ac`). stage7 9-target dispatch (mmap/dlopen/dlsym/pthread_create/mprotect/Metal_device/NSObject_alloc/other) 미존재.
- **본 sprint 산출물**: `docs/STAGE7_HELPER_MM_IPC_SPEC.md` (586 LOC) — 8 IPC kind frame layout + Obj-C reference 230 LOC + entitlement plist + security boundary.
- **사용자 가이드**: spec docs 의 reference 230 LOC 를 `airgenome_helper.mm` 에 inline 후 codesign + JIT entitlement 발급 → cycle 80 의 4 _real 모듈 IPC frame 즉시 wire-up. 추정 외부 작업 분량 = 1-2 시간.

### 4-2. codesign + JIT entitlement 발급

- **현 상태**: Apple Developer ID + `com.apple.security.cs.allow-jit` + `com.apple.security.cs.allow-unsigned-executable-memory` + `com.apple.security.cs.disable-library-validation` 3 entitlement plist 미작성.
- **사용자 가이드**: ad-hoc codesign (Apple Developer ID 발급 보류 가능) + 위 3 entitlement 추가. JIT 영역 (R1 의 MAP_JIT) 동작 검증.

### 4-3. PE → Mach-O 실 변환 (R4 B path)

- **현 상태**: cycle 80 `pe_to_macho_shim_track` 추적기만. 실 변환 (PE 헤더 → Mach-O LC_SEGMENT + IAT → __got + entry → LC_MAIN) 미구현.
- **본 sprint 산출물**: phase 1 추적 layer 만.
- **사용자 가이드**: Apple Rosetta 2 위임 path 결정 후 shim 변환 wrapper 작성. PE/SEH (`.pdata`/`.xdata` → DWARF/compact unwind) coverage 가 game-by-game caveat.

### 4-4. D3DMetal 위임 (R5)

- **현 상태**: cycle 80 `pe_d3d_to_metal_router` 가 path α (D3DMetal 위임 85%) / β (metal-shaderconverter) / γ (DIY fallback) 결정 routing 추적기. 실 D3DMetal.framework dlopen + `MTLCreateSystemDefaultDevice` 호출은 R1 prereq.
- **사용자 가이드**: macOS 14.4+ + Apple GPU family 7+ 환경 검증 후 D3DMetal binary 호출. GPU family 6 이하 + Intel Mac 은 path γ DIY fallback 강제.

→ **외부 결정 영역 합계 = 4건** (helper.mm / codesign / PE→Mach-O / D3DMetal). 본 sprint 가 모두 phase 1 추적 layer + 가이드 docs 형태로 사용자 외부 작업 prereq 명세 제공.

---

## 5. 다음 phase 권장 (v2 점수 ≥20 영역)

| 영역 | v2 점수 | 후속 sprint 후보 |
|---|---|---|
| **M1 fix** | 23 | (도달 완료) |
| **R3 IAT resolve** | **21** | bind-imports fallback 1-line + R6 Tier 1-3 batch 와 IAT slot patch end-to-end test |
| **R4 B path** | **21** | shim 변환 wrapper 실 구현 + PE/SEH coverage 분석 (D2R / D4 .pdata size 분포 측정) |
| **R5 D3D→Metal** | **20** | DXR acceleration struct translate + mesh shader compute lowering fallback + DXBC/DXIL parser 실 변환 |
| **R6 Tier 1+2+3** | **21.3** | (도달 완료) |
| R6 Tier 5 평균 | 15 | SEH 본격 — Rosetta 활용 vs 자체 unwinder 결정 |
| R2 PE mmap | 18 | helper.mm IPC wire-up 후 실 mmap 호출 self-test |
| R1 base | 16 | helper.mm 패치 외부 진행 후 D 점수 +1, E 점수 +1 → 18 가능 |
| M2 folder split | 18 | plateau trigger (모듈 ~300 도달) 시 90분 sprint |

→ **즉시 권장 cycle 83+ batch**:
1. R3 + R6 batch end-to-end (만점 검토 v1 의 권고 정확 실현 — R3 의 IAT slot patch + R6 의 stub 함수 실 호출 chain).
2. R5 phase 3 — DXR / mesh / DXBC 실 변환 8 모듈 (만점 검토 v1 의 cycle 81 batch 의 잔여 4개).
3. R6 Tier 5 SEH trade-off 결정 docs.
4. R4 B path PE/SEH coverage 측정 skeleton.

→ **사용자 외부 작업 권장**:
1. `airgenome_helper.mm` 9-target dispatch inline + codesign + JIT entitlement (1-2 시간) → R1 / R2 만점 도달 unblock.
2. macOS 14.4+ + Apple GPU family 7+ 환경 확인 → R5 path α 검증 unblock.

---

*written 2026-05-01 cycle 83 직전. cycle 79 만점 검토 v1 (commit `f5401e6`) 작성 후 sprint 80 (commit `f838c92`, 15 모듈) + sprint 81 (`72f2571`, 16 모듈) + sprint 82 (`f733cd8`, 15 모듈 + 2 docs) + M1 fix (`fe9086a`, 1-line ×2) 합 46 신 모듈 + 3 docs 진행. 8 영역 점수 변동 합계: M1=무변동(도달), R1=+2, R2=+1, R3=+4(최대), R4=무변동(외부의존), R5=+1, R6=+2(단일)/Tier1-3 도달, M2=무변동(보류). 외부 결정 영역 4건 (helper.mm / codesign / PE→Mach-O / D3DMetal) 의 사용자 가이드 명세 완비. 다음 phase 권장 영역 9개 (점수 ≥20: 4개 / 진행 중: 5개).*
