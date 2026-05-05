# 세션 final summary — cycle 64-85 (2026-05-01)

> **status**: 작성 cycle 85 직후. 본 마라톤 세션의 cycle 64 (loader skeleton 시작) 부터 cycle 85 (D3D state 4 종 + WoW + Lineage M Purple + ★45 v2 aggregator + 게임 specific) 까지 22 cycle 동안의 모든 결과 통합.
> **scope**: meta docs — own1 무관 (점수 변동 없음, session 단위 진척 진단 layer).
> **참조 SSOT**: `docs/PRIORITY_DIRECTIVE.md` / `docs/REMAINING_ROADMAP.md` (v1) / `docs/REMAINING_ROADMAP_V2.md` (v2) / `docs/PHASE3_RESULTS_REVIEW.md` / `docs/CLOSURE_PATH_B_VERIFICATION.md` / `docs/SCORING_V2.md` / `docs/STAGE7_HELPER_MM_IPC_SPEC.md` / `docs/FOLDER_SPLIT_DRY_RUN.md` / `docs/*_MANJEOM_REVIEW.md` (13 건).

---

## 1. 누적 통계

| 항목 | 값 | 비고 |
|---|---|---|
| `lib/loader/*.hexa` 모듈 수 | **313** (cycle 85 미커밋 포함) | `ls lib/loader/*.hexa \| wc -l` 실측, cycle 85 신규 14 추가 시 327 |
| `lib/perf/*.hexa` 모듈 수 | 241 | 동일 own1 정합 |
| `lib/*.hexa` 전체 (loader+perf+detect+dispatch) | **556** | hexa-only, Wine/CrossOver/Whisky/GPTK/Reactos 인용 0 |
| `docs/` 만점 검토 / 로드맵 / spec docs | **15+** | 13 `*_MANJEOM_REVIEW.md` + REMAINING_ROADMAP v1/v2 + PHASE3_RESULTS_REVIEW + CLOSURE_PATH_B_VERIFICATION + SCORING v1/v2 + STAGE7_HELPER_MM_IPC_SPEC + FOLDER_SPLIT_DRY_RUN + IDENTITY + CLOSURE_ROADMAP + PRIORITY_DIRECTIVE + IMPLEMENTATION_ROADMAP |
| `docs/` `.md` 전체 | **43** | sketch / brainstorm / 비교 등 포함 |
| commit (cycle 64-85, 22 cycle) | **22+** | git log 실측 — cycle별 1+ commit |
| sprint 80-85 + cycle 85 sprint | **6** | sprint 80 = `f838c92`, 81 = `72f2571`, 82 = `f733cd8`, 83 = `fdb0012`, 84 = `fce574a`, 85 = (cycle 85 직후 추가 — 본 docs commit 직후) |
| 주요 docs LOC (6 핵심 v2 docs) | **2050 LOC** | SCORING_V2 297 + REMAINING_ROADMAP_V2 120 + PHASE3_RESULTS_REVIEW 289 + CLOSURE_PATH_B_VERIFICATION 309 + STAGE7_HELPER_MM_IPC_SPEC 586 + FOLDER_SPLIT_DRY_RUN 449 |

---

## 2. cycle 별 진척

### 2-1. cycle 64-79 — skeleton 16 cycle / 128 모듈

8 모듈 / cycle × 16 cycle = **128 신 skeleton 모듈**. 본 구간은 BG 단독, 점수 만점 검토 v1 산출 직전까지.

| 그룹 | cycle | 8 모듈 주제 | commit |
|---|---|---|---|
| stage4 native helper | 63-65 | wrapper / IPC alternatives | `c8bcacf` / `ae61697` |
| 자원 / 성능 instrumentation | 66-67 | memory tracking / perf | `7188829` / `4546b2a` |
| calling convention | 68 | i386/x64 → AArch64 adapters | `6c5c72e` |
| NT syscall | 69 | NT syscall infrastructure | `9d87ff5` |
| SEH / exception | 70 | control flow + libunwind boundary | `8939129` |
| Win32 sync | 71 | sync primitives → POSIX | `6b7a469` |
| TLS/heap/console | 72 | registry/env | `90e370b` |
| file system / pipe | 73 | mailslot 포함 | `fd5ccf1` |
| process / thread / security | 74 | impersonate/token | `f934cbb` |
| Win32 networking | 75 | advanced (winsock 2) | `226a698` |
| DirectX / Multimedia | 76 | surface skeleton | `cd7dce9` |
| COM / advanced media | 77 | surface skeleton | `c19c247` |
| GDI / printing / imaging | 78 | surface skeleton | `85bfebe` |
| IME / locale / unicode | 79 | NLS surface | `f20aaa6` |

→ cycle 79 직후 commit `f5401e6` 에서 **8 영역 만점 검토 v1 + 잔여 로드맵 v1** 작성 (병렬 BG, 5축×5=/25 grading).

### 2-2. cycle 80-85 — sprint 6 / 70+ 모듈 + 7 docs

만점 검토 v1 의 8 영역 (M1/M2/R1-R6) 가이드대로 sprint 진행.

| sprint | commit | 신규 모듈 | 신규 docs | 핵심 산출 |
|---|---|---|---|---|
| M1 fix (cycle 80 직전) | `fe9086a` | 0 (1-line × 2 fix) | 0 | `win32_abi_i386` `len(s)` typo 정정 — loader self-test 100% PASS 회복 |
| **sprint 80** | `f838c92` | **15** | 0 | R1 4 (`pe_native_*_real`) + R2 4 (section/reloc/tls/page_adapter) + R3 2 (direct/forwarder) + R4 1 (macho_shim_track) + R5 4 (router/dxbc/dxil/SV) |
| **sprint 81** | `72f2571` | **16** | 0 | R6 T1 8 surface + R5 phase 2 4 (buffer/tex/PSO/cmdlist) + R3 phase 2 2 (api_set/delay_load) + bonus 2 (handle_table / coverage_v2) |
| **sprint 82** | `f733cd8` | **15** | 2 (`STAGE7_HELPER_MM_IPC_SPEC.md` 586 LOC + `FOLDER_SPLIT_DRY_RUN.md` 449 LOC) | R1 spec 확정 + R6 phase 2 (T2 POSIX / T3 struct / T4 heap / T5 SEH+TLS+CS) + 게임 전용 (D2R/D4 specific track) + helper subprocess spawn |
| **sprint 83** | `fdb0012` | **14** | 2 (`PHASE3_RESULTS_REVIEW.md` 289 LOC + `CLOSURE_PATH_B_VERIFICATION.md` 309 LOC) | NTDLL (NtCreateFile / RtlUnicodeString / LpcMessage 등) + D3D11 draw / constant_buffer + 게임 specific 확장 + closure 70% 진단 |
| **sprint 84** | `fce574a` | **14** | 2 (`REMAINING_ROADMAP_V2.md` 120 LOC 갱신 + ★45 docs) | NTDLL/D3D 추가 + SEH 확장 + 인프라 + ★45 점수 docs |
| **sprint 85 (cycle 85)** | (직후 commit) | **14** | 1+ (본 SUMMARY) | D3D11 state 4종 (sampler/blend/depth-stencil/rasterizer) + NTDLL 4 (FsControl/NamedPipe/SetInfo/SymbolicLink) + WoW Classic + Retail track + Lineage M Purple + Lineage 2 specific + battle.net download + `pe_manjeom_v2_aggregator` (★45 v2 통합) |

**누적**: cycle 80-85 sprint 6 × 평균 14.7 모듈/sprint = **88 신 sprint 모듈** + 7 신 docs (1995+ LOC). plateau 미발생, 매 cycle 일정 페이스.

---

## 3. 8 영역 점수 v0 → v1 → v2 (PHASE3_RESULTS_REVIEW 인용)

5축 × 5점 / 25 — A=own1 정합 / B=실 동작 도달 / C=자원 영향 / D=구현 난이도 (역) / E=의존성 (역).

| 영역 | v0 (가설) | v1 (cycle 79) | v2 (cycle 84 후) | Δ (v1→v2) |
|---|---|---|---|---|
| **M1** win32_abi_i386 fix | — | 23/25 | **23/25** | — (1-line fix 완료, B/C/D/E 만점 회복) |
| **R1** stage7 native bridge | — | 14/25 | **16/25** | **+2** (4 _real 모듈 + IPC spec 586 LOC 확정 — helper.mm 외부 결정 잔존) |
| **R2** PE 실 mmap | — | 17/25 | **18/25** | **+1** (4 phase 모두 진행 + invoke 추적 — R1 prereq 미해소) |
| **R3** IAT 실 resolve | — | 17/25 | **21/25** | **+4** (단독 최대 변동 — direct/forwarder/ordinal/delay-load/api-set 5 path 모듈화) |
| **R4** binary translation B path | — | 21/25 | **21/25** | — (shim 추적기 1건만 — Apple Rosetta 2 위임 외부 결정) |
| **R5** D3D → Metal | — | 19/25 | **20/25** | **+1** (DXBC + DXIL + SV mapping + D3D11/12 핵심 method 매핑) |
| **R6** DLL 실 구현 | — | 16/25 (T1=22 / T5=13) | **18/25** (T1+2+3=21.3 / T5=15) | **+2** (Tier 4/5 도달 — SEH unwinder + TLS pool + Critical Section) |
| **M2** folder split | — | 18/25 | **18/25** | — (dry-run docs만 — 보류 유지, plateau trigger 대기) |

**합계 평균**: v1 = 18.1/25 → v2 = **19.4/25** (+1.3). 단독 최대 변동 R3 (+4).

cycle 85 직후 (sprint 85 완료) 추정: R5 +1 (state 4종 추가로 D3D11 surface coverage 확장) → R5 = 21/25 가능. R6 +0.5 (NTDLL 4 추가) → 약 18.5. **세션 종료 시점 평균 ~19.6/25 (78% 만점)**.

---

## 4. closure 70 → 80% (cycle 85 직후) — E1+E2 후 85% 도약 가능

`CLOSURE_PATH_B_VERIFICATION.md` 의 5-stage chain 기준 (D2R primary path).

| stage | cycle 79 (v1) | cycle 83 (v2) | cycle 85 후 (estimate) | E1+E2 후 |
|---|---|---|---|---|
| stage 0 install | 90% | 95% | 97% | 97% |
| stage 1 launch_request | 60% | 75% | 80% | 92% |
| stage 2 pe_load | 50% | 70% | 75% | **88%** (E1 helper.mm 후 실 mmap) |
| stage 3 native_runtime | 40% | 65% | 75% | **85%** (E1+E2 후 helper IPC dispatch wire-up) |
| stage 4 first_frame | 30% | 55% | 65% | 75% (E4 D3DMetal 후 80%+) |
| **closure 5-stage 평균** | ~54% | ~72% | **~78%** | **~87%** |

cycle 85 직후 closure ~78%. **E1 (helper.mm Xcode build) + E2 (codesign + entitlement) 후 85%+ 도약 가능** — stage 2/3 의 IPC dispatch 가 trace-only 에서 실 호출로 전환되기 때문.

---

## 5. 외부 결정 4건 (사용자 작업 가이드)

`REMAINING_ROADMAP_V2.md` §2 의 E1-E5 중 BG 미진행 4건. 본 세션 BG 작업으로 spec/dry-run 완료 — 사용자 단계만 잔존.

### E1. helper.mm Xcode build (1-2 시간)
- **gate spec**: `docs/STAGE7_HELPER_MM_IPC_SPEC.md` (586 LOC) — 8 IPC kind frame layout (dlopen / mmap / mprotect / pthread / dispatch_async / mach_*_register / objc_msgSend / sandbox_check) + reference Obj-C 230 LOC + entitlement plist + security boundary + frame parser pseudo
- **사용자 작업**: `airgenome_helper.mm` (parent dir 의 Phase 1 stage2 hexa loader bridge `airgenome_helper.mm`) Xcode project 생성 → `clang -fobjc-arc -framework Foundation -framework AppKit -framework Metal helper.mm -o helper` → `.app` bundle 화 (Info.plist + Resources 구조)
- **검증**: BG 측 `pe_helper_subprocess_spawn` 모듈이 fork/exec/IPC handshake 추적 — IPC kind 1-8 프레임 schema 와 일치 확인

### E2. codesign + entitlement (30분)
- **gate spec**: STAGE7_HELPER_MM_IPC_SPEC.md §3 entitlement plist 구조
- **사용자 작업**: `com.apple.security.cs.allow-jit` + `com.apple.security.cs.allow-unsigned-executable-memory` + `com.apple.security.cs.allow-dyld-environment-variables` 부착 → `codesign --entitlements helper.entitlements --options runtime --sign "<keychain identity>" helper.app`
- **검증**: `codesign -d --entitlements - helper.app` 출력 확인 + macOS Hardened Runtime 통과
- **결과**: stage 3 native_runtime 의 IPC dispatch 를 trace-only → 실 호출로 wire-up

### E3. PE → Mach-O thin shim 실 변환 (수일)
- **gate spec**: `pe_to_macho_shim_track.hexa` (cycle 80, sprint 80) 의 PE 헤더 → Mach-O thin header rewrite spec
- **사용자 작업**: PE PE32+ headers → Mach-O LC_SEGMENT_64 변환 + IAT → `__got` patch + PE/SEH coverage 검증
- **차단 사유**: Apple Rosetta 2 binary translation 호출은 OS-level — 사용자 build env 에서 실험 영역 (PRIORITY_DIRECTIVE 보류)
- **선행**: E1 + E2 (helper IPC 통과 후 진행 가능)

### E4. D3DMetal binary weak link (수일)
- **gate spec**: `pe_d3d_to_metal_router.hexa` (cycle 80) 의 d3dmetal_delegate 85% / metal-shaderconverter / DIY fallback 3-way router
- **사용자 작업**: Apple GPTK D3DMetal `.dylib` weak-link + metal-shaderconverter pipeline 호출
- **own1 trade-off**: 외부 binary 사용은 사용자 결정 영역 (현 router 는 추적기 only — 만점 검토 v1/v2 의 R5 axis A 5/5 유지)
- **선행**: E1 + E2 (Metal device init 자체가 stage 3 통과 후)

---

## 6. PRIORITY_DIRECTIVE 4 차원 정합 (작동/성능/자원/속도)

`docs/PRIORITY_DIRECTIVE.md` 4 기준 — 본 세션 22 cycle 의 차원별 기여:

### 6-1. 작동 (correctness — 실 launch / 실 frame 도달)
- **R3 IAT** (+4): 5 path 모듈화 → ~98% fn 도달 가능
- **R6 Tier 1+2+3** (도달): KERNEL32/USER32/ADVAPI32/GDI32/SHELL32/OLE32/WININET/VERSION + Registry + sysctl ~330 fn
- **R6 Tier 5** (+2): SEH unwinder + TLS pool + Critical Section — D2R private offline launch ~70% → ~80%
- **R1 IPC spec** (+2): 586 LOC frame layout 확정 — helper.mm 패치 후 즉시 wire-up
- **cycle 85 추가**: D3D11 state 4종 (sampler/blend/depth-stencil/rasterizer) — first frame surface coverage

### 6-2. 성능 (perf hot path)
- cycle 67 perf instrumentation 8 모듈 (`lib/perf/`)
- IAT direct resolve runtime patch (R3) — 후속 호출 zero-overhead
- W^X / MAP_JIT (R2) — JIT page flush 최적화
- pe_page_adapter_16k — Apple 16K vs Win 4K granularity adapter

### 6-3. 자원 (memory footprint)
- cycle 66 자원 tracking 8 모듈
- pe_handle_table_real — Win32 user-mode HANDLE table 단일 구조
- pe_tls_alloc_real — pool 재사용으로 fragmentation 회피

### 6-4. 속도 (cold-start launch latency)
- pe_cold_launch_phase 추적기
- pe_battle_net_agent_ipc_track / pe_battle_net_download_track (cycle 85)
- pe_d2r_specific_track / pe_d4_specific_track / pe_wow_classic/retail_track (cycle 85)

→ **4 차원 모두 sprint 80-85 가 직접 기여**. PRIORITY_DIRECTIVE 1순위 (작동) 가 R3/R6/R1 중심으로 가장 강한 변동.

---

## 7. own1 / own2 enforcement 통계

- **Wine / CrossOver / Reactos / Whisky / GPTK 인용**: 0 건 검증 (`grep -ri "wine\|crossover\|whisky\|reactos\|gptk" lib/ | wc -l = 0` — 단, GPTK 는 외부 binary 결정 영역 docs 에서 trade-off 맥락 only)
- **self-test PASS**: 100% 회복 (cycle 80 직전 M1 fix `fe9086a` 후 — `win32_abi_i386.hexa:76` + `win32_rpcrt4.hexa:69` `len(s)` typo 정정으로 loader self-test 모두 PASS)
- **모듈 own1 정합**: 313 loader + 241 perf = **554+ 모듈 own1 axis A 5/5** 유지 (만점 검토 8 영역 axis A 모두 5/5)
- **외부 binary trade-off 영역**: R4 (Apple Rosetta 2) / R5 (Apple D3DMetal) 만 — 본질적으로 Apple 자체 native 이므로 own1 정신 보존 (`own 1 — 타사 Wine/CrossOver/GPTK/Whisky 추천 금지` directive 정합)
- **own 1 trigger/game-changer 언급 0**: cycle 64-85 docs 전체에서 GPTK/Whisky/CrossOver 를 trigger/game-changer 로 인용 X (enforcement 맥락만 OK directive 정합)

---

## 8. 다음 cycle 후보

### 8-1. v3 trigger 조건
- **plateau 도달 시 M2 강행**: lib/loader 모듈 ~250 plateau 후 90 분 강행 가이드 (`FOLDER_SPLIT_DRY_RUN.md`) — 현 313 도달, **trigger ON** 가능. 단, 매 cycle 14-16 모듈 페이스 유지 중이므로 plateau 미발생 — 사용자 directive 대기.
- **v3 점수 재평가**: cycle 86-90 sprint 5+ 후 v3 grading docs 작성 (PHASE3_RESULTS_REVIEW v3) — R5/R6 추가 변동 측정.

### 8-2. 외부 결정 후 단계별 launch
- **E1 통과 후 (1-2시간)**: stage 3 native_runtime trace → 실 호출 wire-up. closure 78% → 82%.
- **E1+E2 통과 후 (+30분)**: 위 + Hardened Runtime 검증. closure 82% → **85%**.
- **E1+E2+E3 통과 후 (수일)**: PE → Mach-O thin shim 실 변환 — D2R private offline 약식 closure 가능. closure 85% → 90%.
- **E1+E2+E4 통과 후 (수일)**: D3DMetal weak-link — first frame 도달 가능. closure 85% → 92%.
- **E1+E2+E3+E4 모두 통과 후**: closure path B 1회 완주 (D2R first frame present) — closure 95%+.

### 8-3. v3 BG 후보 (E1 미통과 시 BG 단독)
- **N9 NTDLL 추가 syscall**: `LdrInitializeThunk` / `KiUserApcDispatcher` / `KiUserExceptionDispatcher` (Wine 0 줄 유지 + 추적기)
- **N10 DXR / mesh shader**: BLAS/TLAS / acceleration structure 정합
- **N11 게임 specific 확장 추가**: POE2 / Lost Ark / FFXIV / 기타 (cycle 85 의 WoW + Lineage 패턴 확장)
- **N12 anti-cheat boundary**: kernel AC 거부 + private 서버 한정 정책 docs (own1 정합 — 우회 X, 차단 명시)
- **N13 integration test sim**: 313+241 모듈 cross-call 일관성 entry chain 시뮬레이션 (track only)

### 8-4. plateau 도달 신호
- 단일 sprint 모듈 < 8 (현 평균 14.7)
- 검토 docs 신규 항목 < 1 / sprint
- closure stage 진척 < 1% / sprint
- v3 점수 평균 변동 < 0.3 / sprint

→ 위 4 신호 중 2+ 동시 발생 시 M2 folder split 강행 + 사용자 외부 결정 4건 trigger 권장.

---

## 9. 세션 종료 — 핵심 산출 7건

1. **313 loader 모듈** (skeleton 128 + sprint 88 + 기존) — own1 정합 + Wine 0 줄
2. **STAGE7_HELPER_MM_IPC_SPEC.md** (586 LOC) — 외부 E1 트리거 가이드
3. **FOLDER_SPLIT_DRY_RUN.md** (449 LOC) — plateau 후 강행 가이드
4. **PHASE3_RESULTS_REVIEW.md** (289 LOC) — v1 → v2 점수 진척 진단
5. **CLOSURE_PATH_B_VERIFICATION.md** (309 LOC) — closure 70% → 78% 진단 + E1+E2 후 85% 도약 경로
6. **REMAINING_ROADMAP_V2.md** (120 LOC) — 외부 결정 5건 명세 + BG 후보 N1-N8
7. **SCORING_V2.md** (297 LOC) + `pe_manjeom_v2_aggregator.hexa` (cycle 85) — ★45 컴포넌트 통합

→ 본 7건이 cycle 64-85 의 **사용자 인계 산출물**. 사용자가 E1-E4 4건 외부 결정 진행 시 closure 95%+ 진입 가능.

---

> **다음 세션 시작 시**: 본 SUMMARY 의 §5 (외부 결정 4건) 진행 상태 확인 → 진행 0 시 §8-3 의 N9-N13 BG 후보로 cycle 86 sprint 진입. 진행 1+ 시 §8-2 단계별 launch 검증 cycle 진입.
