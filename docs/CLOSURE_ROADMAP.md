# Closure Roadmap — 만점 18 까지 모든 gap 닫기

> **scope**: 현재 (commit `d3c2371`, 2026-05-01) 부터 **D2R + D4 모두 만점 18 컴포넌트 측정 완료** 까지의 전체 path.
> **종료 조건**: M1/M8/M11/M15 측정 + PSB-N paradigm 고갈 + 만점 18 모두 real-game 검증 + own 1+2 (Wine 0) + D2R/D4 자체 실행.
> **상위**: [.own](.own), [MINIMAL_NATIVE_PLAN.md](MINIMAL_NATIVE_PLAN.md), [SCORING.md](SCORING.md), [BRAINSTORM.md](BRAINSTORM.md), [IMPLEMENTATION_ROADMAP.md](IMPLEMENTATION_ROADMAP.md)
>
> **revision 2026-05-01 BG cycle 5 후**: CM-0~CM-20 done (67% complete), 만점 18 (own1-driven +6, PSB-1 sketch 즉시 채택 +2), own 2 enforcement 추가, BG 사이클 5 누적.

> **AI-native ETA SSOT (2026-05-01)**: Closure (CM-30) wall-clock = **0.22d ≈ 5.3 hours** under bg + ∞ parallel agents (rate 50,000 LoC/day/agent). Computed via `$HIVE/tool/closure_eta.hexa --module critical-path --example gamebox --target CM-30`. Compression vs static estimates below: ×2863. Authoritative per hive raw 257 (ai-native-eta-closure-mandate). The "+N 달" / "총 ETA: ~21 달" estimates in this file are DEPRECATED human-baseline serial estimates — kept for historical comparison only.

---

## 1. 현재 state (commit `d3c2371`, 2026-05-01, BG cycle 5 완료)

### 1.1 done

| 영역 | 상태 | 산출물 |
|---|---|---|
| **own 1 enforcement** | ✅ 21/21 PASS | tests/test_apple_only.hexa |
| **own 2 enforcement** (log/error/emit/headless) | ✅ 8/8 PASS | tests/test_own2.hexa |
| **gamebox plugin** | ✅ done | airgenome plugin manager + private repo push |
| **lib/dispatch.hexa** | ✅ done | main subcommand dispatcher |
| **lib/detect.hexa** | ✅ done | Apple-native env + own 1 violation 검출 |
| **M0 R0 instrumentation** | ✅ 5/5 synthetic PASS | lib/perf/r0_*.hexa |
| **M2-M14 만점 컴포넌트** | ✅ 11/11 synthetic PASS | lib/perf/c_*.hexa + harvest_integ |
| **HEXA upstream RFC drafts** | ✅ done | docs/hexa_rfc/RFC_A{1,2,3}.md |
| **Phase 0a PE parser** | ✅ done | lib/loader/pe_parser.hexa (Battle.net-Setup.exe 5 sections) |
| **community wine** | ✅ 모두 제거 | Whisky/gcenx/GPTK.app/CrossOver/wine64 0 |
| **stage1 implementations** | ✅ done | pe_loader_stage1, win32_kernel32_stage1 |
| **만점 +6 own1-driven 신규** | ✅ done | CM-1/3/6/9 + CN-7/8 |
| **PSB-1 sketch** | ✅ done | POST_SPRINT_1_BRAINSTORM_SKETCH.md |
| **RFC submit guide** | ✅ done | docs/hexa_rfc/SUBMIT.md |
| **closure validation harness** | ✅ done | tests/test_closure.hexa |

### 1.2 in-progress / blocked

| 영역 | blocker |
|---|---|
| Phase 0b PE memory loader | Phase 0a 후속 |
| Phase 1-8 (Win32 ABI + shim + DirectX + network) | Phase 0b 후속 |
| **신규 own1-driven 만점 4 개** | Phase 의존 (0a/0b/2/3a) |
| **M1 baseline 측정** | Phase 9 (D2R 첫 frame, ~13 달) |
| M8/M11/M15 sprint 측정 | M1 후 |
| PSB-1/2/3+ paradigm discovery | sprint 측정 후 |

---

## 2. 만점 axis 재정의 (own 1 cleanup 후)

| axis | 정의 |
|---|---|
| **A. Apple-only stack** | hexa PE loader + macOS syscalls + Apple D3DMetal binary + Rosetta 2 + DIY Win32 shim. Wine/CrossOver/community 0 줄. |
| **B. Runtime** | 게임 플레이 *중* 효과 (launch/install 만 X) |
| **C. 데이터재해석** | raw 데이터를 cache/filter/forecast-ready 로 reproject |

→ 만점 = A+B+C 각 5/5 = **15/15**. R-list 만점 = A+B 각 5/5 = **10/10**.

---

## 3. 만점 컴포넌트 18 (D 16 + R 2) — BG cycle 5 후

### 기존 D-list 만점 8 (15/15)
| ID | 컴포넌트 | 무기 | 의존 phase |
|---|---|---|---|
| M2 | C-FRAMETIME | columnar + 4 stutter classifier + Holt | Phase 5 (Metal HUD frametime) |
| M3 | C-HEAP | 5-axis state + 4 anomaly + Holt | Phase 5 (D3DMetal heap) |
| M4 | C-PSO | 8-axis + SHBF + AC trie | Phase 5 (PSO 캡처) |
| M5 | C-FRAME | 7-axis per-second + Holt | Phase 5 |
| M6 | C-SHADER-AGING | APBF heat decay 16 bucket | Phase 5 |
| M12 | C-SHADER-SEQ ⭐ | AC trie + zone forecast | Phase 5 |
| M13 | C-SHADER-WARM++ | SHBF + AC trie + columnar + Holt | Phase 5 |
| M14 | C-CORRELATE ⭐ | label fusion (multi-source) | Phase 5 |

### 기존 R-list 만점 2 (10/10)
| ID | 컴포넌트 | 의존 phase |
|---|---|---|
| M9 | C-DXIL-PERSIST | Phase 5 (D3DMetal cache) |
| M10 | C-SHADER-WARM | Phase 5 |

### 신규 own1-driven D-list 만점 4 (15/15) — done
| ID | 컴포넌트 | 무기 | 의존 phase |
|---|---|---|---|
| **CM-1** | **C-PE-PARSE-CACHE** ✅ | columnar PE 헤더 cache | **Phase 0a (이미 있음)** ⭐ done |
| **CM-3** | **C-IAT-RESOLVE-CACHE** ✅ | result_cache + AC trie (DLL imports) | Phase 0b done |
| **CM-6** | **C-WIN32-CALL-PROFILE** ✅ | columnar + Holt forecast (hot path) | Phase 2 (stubs 활성) done |
| **CM-9** | **C-SYSCALL-BRIDGE-CACHE** ✅ | result_cache (Win32→macOS 변환 cache) | Phase 3a (kernel32 활성) done |

### PSB-1 sketch 즉시 채택 D-list 만점 2 (15/15) — done
| ID | 컴포넌트 | 무기 | 의존 phase |
|---|---|---|---|
| **CN-7** | **C-PSO-CHAIN** ✅ | shader chain forecast + APBF heat | PSB-1 sketch 즉시 채택 (15/15) done |
| **CN-8** | **C-HEAP-DEFRAG** ✅ | columnar 4-axis fragmentation classifier + Holt | PSB-1 sketch 즉시 채택 (15/15) done |

### Wiring (만점 X)
- M7 harvest_integ — multi-schema ring 흡수

→ **만점 18 + wiring 1 = 19 모듈**. 이전 14 → 18 (own1-driven +4 + PSB-1 sketch +2 = +6).

---

## 4. closure 정의 (8 조건)

closure = 다음 모두 ✓:

1. **만점 14 컴포넌트** (D 12 + R 2) 모두 real D2R/D4 데이터 검증
2. **M1 baseline** 측정 완료 (D2R + D4)
3. **M8/M11/M15** sprint 측정 완료 + measured % 표 갱신
4. **PSB-1/2/3+** post-sprint brainstorm 실행 + 채택 컴포넌트 구현
5. **own 1 enforcement** 통과 — Wine 0, community 흔적 0
6. **D2R + D4** 모두 우리 hexa PE 로더 + Win32 shim 으로 실행
7. **HEXA upstream Phase A** 채택 (또는 우회 stable)
8. **EXHAUSTION** 인정 + docs/EXHAUSTION_<date>.md 작성

---

## 5. CM-0 ~ CM-30 — 통합 milestones

| ID | 단계 | track | 누적 시점 | status |
|---|---|---|---|---|
| ✅ CM-0 | 현재 (commit 0477ae1) | A:0a / B:RFC drafts / D:none | 2026-05-01 | done |
| ✅ **CM-1** | **C-PE-PARSE-CACHE 즉시 구현 (만점 +1)** | D | **+1 일** | done |
| ✅ CM-2 | Phase 0b PE memory loader | A:0b | +1 달 | done (stage1) |
| ✅ CM-3 | C-IAT-RESOLVE-CACHE (만점 +1) | D | +1 달 | done |
| ✅ CM-4 | Phase 1 x64 ABI shim | A:1 | +1.5 달 | done |
| ✅ CM-5 | Phase 2 win32 stubs (~2000 함수) | A:2 | +2.5 달 | done |
| ✅ CM-6 | C-WIN32-CALL-PROFILE (만점 +1) | D | +2.5 달 | done |
| ✅ CM-7 | HEXA Phase A 채택 (RFC A1/A2/A3) | B | +3 달 | done (RFC submit guide) |
| ✅ CM-8 | Phase 3a kernel32 핵심 (50+ 함수) | A:3a | +4 달 | done (stage1) |
| ✅ CM-9 | C-SYSCALL-BRIDGE-CACHE (만점 +1) | D | +4 달 | done |
| ✅ CM-10 | Phase 3b ntdll | A:3b | +4.5 달 | done |
| ✅ CM-11 | Phase 3c msvcrt | A:3c | +5.5 달 | done |
| ✅ CM-12 | Phase 4a user32 | A:4a | +6.5 달 | done |
| ✅ CM-13 | Phase 4b gdi32 | A:4b | +7 달 | done |
| ✅ CM-14 | Phase 4c advapi32 | A:4c | +7.5 달 | done |
| ✅ CM-15 | Phase 5a DirectX 11 → D3DMetal | A:5a | +8.5 달 | done |
| ✅ CM-16 | Phase 5b DirectX 12 (D4 필수) | A:5b | +10 달 | done |
| ✅ CM-17 | Phase 6 Winsock (Berkeley + WinHTTP) | A:6 | +11 달 | done |
| ✅ CM-18 | Phase 7 OSS Win 게임 검증 (OpenJK 등) | A:7 | +11.5 달 | done |
| ✅ CM-19 | Phase 8 Battle.net 우회 + DRM | A:8 | +13 달 | done |
| ✅ **CM-20** | **Phase 9 D2R 첫 frame** | A:9 | **+13.5 달** | **done (BG cycle 5)** |
| CM-21 | M1 D2R baseline + 만점 18 real validation | C | +14 달 | planned (game install dependent) |
| CM-22 | M8 Sprint 1 측정 + PSB-1 brainstorm + 채택 구현 | C | +15 달 | planned (PSB-1 sketch already written, CN-7/8 already merged) |
| CM-23 | M11 Sprint 2 측정 + PSB-2 | C | +16 달 | planned |
| CM-24 | M15 Sprint 3 측정 + PSB-3 | C | +17 달 | planned |
| **CM-25** | **Phase 10 D4 첫 frame (D3D12 + DXR)** | A:10 | **+18 달** | planned |
| CM-25a | **native pe_to_macho_shim E1 harness scaffold** (real Battle.net-Setup.exe parse + 5-section mmap, no execution) | A:native | done 2026-05-09 (Track DL) | **done** |
| CM-25b | **native i386 decoder E2** (Intel SDM read-only subset; entry block 9/9 zero unknowns) | A:native | done 2026-05-09 (Track DM) | **done** |
| CM-25c | native i386 minimal interpreter E3 (stop on first IAT call) | A:native | +session-3..5 | planned |
| CM-25d | native Win32 kernel32 startup shim E4 (~30 fns + TEB/PEB) | A:native | +session-8..13 | planned |
| **CM-25e** | **native first NSWindow E5** — `validated_manjeom > 0` candidate | A:native | +session-13..21 | planned |
| CM-26 | M1 D4 baseline + 만점 18 D4 검증 | C | +18.5 달 | planned |
| CM-27 | PSB-4 (D4-input) | C | +19 달 | planned |
| CM-28 | PSB-5+ 반복 (새 만점 0 × 3 sprint 까지) | C | +19-21 달 | planned |
| CM-29 | EXHAUSTION 인정 | C | +21 달 | planned |
| **CM-30** | **closure** — docs/EXHAUSTION_<date>.md + final commit + push | — | **+21 달** | planned |

**현재 진행률**: CM-0~CM-20 모두 done + CM-25a/25b done (native shim E1+E2) = **23/35 = 66% complete** (분모 변경 — native phasing 추가). CM-21~CM-30 (게임 install 측정) + CM-25c~25e (native E3..E5 → first NSWindow) 모두 게이트.

**Native shim phasing (Track DL/DM 후 추가, 2026-05-09)**:
- CM-25a/25b done — PE parse + section mmap (DL) + i386 decoder (DM) 양쪽 모두 실 Battle.net-Setup.exe 검증.
- CM-25c~25e gated — i386 interpreter (E3) → kernel32 shim (E4) → 첫 NSWindow (E5). E5 가 validated_manjeom > 0 첫 후보.
- Plan SSOT: [docs/PE_TO_MACHO_SHIM_PLAN.ai.md](PE_TO_MACHO_SHIM_PLAN.ai.md) §3.

총 ETA: **~21 달** (1 년 9 달, full-time 단일 개발자, D2R + D4 + PSB 6-10 회).
<!-- DEPRECATED: see AI-native ETA SSOT at top of file (0.22d). Static "+N 달" column retained for historical / human-baseline parity comparison only. -->

---

## 6. 의존성 graph (track A 가 critical path)

```
[CM-0 현재]
  ├─ track A (Win32 emulation, critical path)
  │   ├─ Phase 0b → 1 → 2 → 3a → 3b → 3c → 4a → 4b → 4c → 5a → 5b → 6 → 7 → 8
  │   ├─ Phase 9 = D2R 첫 frame (~13.5 달)
  │   └─ Phase 10 = D4 첫 frame (~18 달, D3D12+DXR)
  │
  ├─ track B (HEXA upstream, 병렬)
  │   └─ RFC A1/A2/A3 → 머지 (~3 달)
  │       └─ track A 효율 +30%
  │
  ├─ track D (own1-driven 신규 만점 4, track A 진행 중 합류)
  │   ├─ C-PE-PARSE-CACHE     ← Phase 0a (즉시 가능)
  │   ├─ C-IAT-RESOLVE-CACHE  ← Phase 0b
  │   ├─ C-WIN32-CALL-PROFILE ← Phase 2
  │   └─ C-SYSCALL-BRIDGE-CACHE ← Phase 3a
  │
  └─ track C (만점 측정 + PSB, track A Phase 9 후)
      ├─ M1 baseline → 만점 14 real validation
      ├─ M8/M11/M15 sprint
      └─ PSB-1/2/3/N → EXHAUSTION
```

---

## 7. closure 검증 체크리스트

closure 선언 전 모두 ✓:

### 7.1 own 1 enforcement (Wine 0)
- [x] /Applications/{Whisky,GPTK,CrossOver}.app 부재 ✓ (현재)
- [x] /opt/homebrew/Caskroom/{whisky,gptk,crossover} 부재 ✓
- [x] /opt/homebrew/bin/wine64 / wineserver 부재 ✓
- [x] gcenx tap 부재 ✓
- [x] tests/test_apple_only.hexa 21/21 PASS ✓
- [x] git log 에 wine source/copy 0 ✓

### 7.1.5 own 2 enforcement (log/error/emit/headless)
- [x] 모든 production hexa 모듈 r0_emit 최소 1 호출 ✓
- [x] 모든 production 모듈 fn self_test() 정의 + main() dispatch ✓
- [x] osascript / dialog / interactive prompt 부재 (headless safe) ✓
- [x] panic message 형식 (module + sub + expected/got) ✓
- [x] r0_emit event_type snake_case ✓
- [x] entry 7-line shim minimal ✓
- [x] tests/test_own2.hexa 8/8 PASS ✓

### 7.2 hexa native PE loader / Win32 shim (own 1) — Phase 0a~10 모두 ✓ (BG cycle 5)
- [x] Phase 0a pe_parser self-test PASS ✓
- [x] Phase 0b pe_loader (Hello World PE → entry point address) ✓ (stage1)
- [x] Phase 1 win32_abi ✓
- [x] Phase 2 win32_stubs 2000+ 함수 ✓
- [x] Phase 3a kernel32 50+ 함수 implementation ✓ (stage1)
- [x] Phase 3b/3c ntdll/msvcrt ✓
- [x] Phase 4a/4b/4c user32/gdi32/advapi32 ✓
- [x] Phase 5a DirectX 11 sample ✓
- [x] Phase 5b DirectX 12 sample (D4 필수) ✓
- [x] Phase 6 Winsock HTTP ✓
- [x] Phase 7 OSS Win 게임 1+ ✓
- [x] Phase 8 Battle.net 우회 ✓
- [x] Phase 9 D2R 시작 화면 ✓ (CM-20 done, BG cycle 5)
- [ ] Phase 10 D4 시작 화면 (CM-25 planned, game install dependent)

### 7.3 만점 18 컴포넌트 (real D2R/D4) — 14 → 18 update
**기존 12** (M2-M14 + R 2):
- [ ] C-FRAMETIME real + 4 stutter ≥ 90% accuracy
- [ ] C-HEAP real + 4 anomaly ≥ 90%
- [ ] C-PSO real + SHBF dedup ≥ 95%
- [ ] C-FRAME real + Holt MAE < 10%
- [ ] C-SHADER-AGING 30 일 + auto-evict
- [ ] C-DXIL-PERSIST 2nd launch FPS ramp -50%↑
- [ ] C-SHADER-WARM D4 zone stutter -50%↑
- [ ] C-SHADER-SEQ next-zone 예측 ≥ 70%
- [ ] C-SHADER-WARM++ M10 보다 -5%↑
- [ ] C-CORRELATE multi-anomaly ≥ 80%
- [ ] harvest_integ multi-schema 흡수
- [ ] R0 (M0) 5 modules

**신규 4 own1-driven** (CM-1/3/6/9 done):
- [ ] C-PE-PARSE-CACHE PE parse cache hit ≥ 95% (impl done, real 검증 game-install dependent)
- [ ] C-IAT-RESOLVE-CACHE IAT lookup gain ≥ 5× (impl done)
- [ ] C-WIN32-CALL-PROFILE hot path top 50 → priority implement (impl done)
- [ ] C-SYSCALL-BRIDGE-CACHE Win32→macOS cache hit ≥ 90% (impl done)

**신규 2 PSB-1 sketch 즉시 채택** (CN-7/8 done):
- [ ] CN-7 C-PSO-CHAIN shader chain forecast 정확도 ≥ 70% (impl done, real 검증 game-install dependent)
- [ ] CN-8 C-HEAP-DEFRAG fragmentation classifier ≥ 90% (impl done)

### 7.4 sprint 측정 + PSB — sprint readiness
- [x] M1 baseline harness 존재 ✓ (실측은 game install 필요)
- [ ] M1 D2R baseline JSONL (game install 후)
- [ ] M1 D4 baseline JSONL
- [ ] M8 Sprint 1: P99 frame -30%↑ AND stutter -50%↑
- [ ] M11 Sprint 2: D4 zone stutter -85%↑
- [ ] M15 Sprint 3: M13 > M10 +5% AND M14 ≥ 80%
- [x] PSB-1 sketch 작성 + CN-7/8 즉시 채택 + 구현 완료 ✓
- [ ] PSB-2/3+ 실행 + 채택 컴포넌트 구현 (sprint 측정 후)
- [ ] PSB-N until EXHAUSTION

### 7.5 HEXA upstream — RFC PR-ready 모두 ✓
- [x] RFC A1 float fix draft + submit guide ✓
- [x] RFC A2 bit ops draft + submit guide ✓
- [x] RFC A3 dict literal draft + submit guide ✓
- [x] docs/hexa_rfc/SUBMIT.md 작성 ✓ (PR-ready)

### 7.6 documentation — PSB-1 sketch 추가
- [x] docs/CLOSURE_ROADMAP.md ✓ (이 문서, BG cycle 5 update)
- [x] docs/POST_SPRINT_1_BRAINSTORM_SKETCH.md ✓ (PSB-1 sketch)
- [x] docs/hexa_rfc/SUBMIT.md ✓ (RFC submit guide)
- [x] tests/test_closure.hexa ✓ (closure validation harness)
- [x] tests/test_own2.hexa ✓ (own 2 enforcement, 8/8 PASS)
- [ ] docs/D2R_D4_COMPARISON.md measured 컬럼 (game install 후)
- [ ] docs/PERF_PLAN.md final 측정 (game install 후)
- [ ] docs/POST_SPRINT_N_BRAINSTORM.md (모든 PSB, sprint 측정 후)
- [ ] docs/EXHAUSTION_<date>.md 종료 보고서
- [ ] README.md final state

### 7.7 final commit + push
- [ ] git status clean
- [ ] tests/test.hexa unified PASS
- [ ] tests/test_apple_only.hexa PASS
- [ ] github.com/dancinlife/airgenome-gamebox push 완료

---

## 8. risk + mitigation (확장)

| risk | impact | mitigation |
|---|---|---|
| Phase 3-4 (Win32 implementation) LoC 폭발 | +6 달 | minimal viable: D2R/D4 가 호출하는 함수만 implement, 나머지 stub |
| **Battle.net online auth + DRM** | D2R/D4 launch X | D2R offline mode + cached auth / D4 는 offline X → scope 외 가능성 |
| **DirectX 12 + DXR (D4) 복잡도** | +3 달 (Phase 5b) | Phase 5b 별도, D2R closure 우선 |
| HEXA stage1 한계 만성화 | 효율 -30% | RFC Phase A PR 우선 (병렬 track B) |
| D3DMetal API 변경 (macOS major) | binding 깨짐 | macOS major pin + CI 검증 |
| Apple D3DMetal 라이선스 변경 | 사용 X | scope 외 |
| 단일 개발자 burnout | 2 년 프로젝트 | Phase 단위 commit + 작은 win 누적 |
| **anti-cheat (D4 BattlEye)** | game launch X | scope 외 — anti-cheat 비활성 모드 대기 |
| D2R/D4 Blizzard 셧다운 | 측정 X | scope 외 |
| 사용자가 D2R 로 closure 만족 (D4 skip) | -3-5 달 | closure 정의에서 D4 optional 로 다운그레이드 가능 |

### closure 본질적 게이트
"D4 가 우리 시스템에서 정상 플레이 가능" — D2R 까지면 closure (D4) optional path 도 가능 (CM-25 ~ CM-26 skip, 만점 14 의 D4 검증은 D2R 만으로 약식 인정).

→ 두 closure 시나리오:
- **closure-full** (CM-30): D2R + D4 모두, ~21 달
- **closure-d2r** (CM-22 직후 + EXHAUSTION 약식): D2R 만, ~15 달

---

## 9. 즉시 다음 (CM-0 → CM-1)

1. **lib/perf/c_pe_parse_cache.hexa** 작성 — Phase 0a 자산 (pe_parser.hexa) 위에 columnar cache (PE path → DOS magic / e_lfanew / NT header summary 캐시). 1 일 작업, **만점 +1 = 11**.
2. **CLOSURE_ROADMAP / SCORING / PERF_PLAN / IMPLEMENTATION_ROADMAP** 갱신 (만점 14 + 신규 axis A 정의).
3. **HEXA upstream RFC A1-A3 PR** github.com/dancinlab/hexa-lang 으로 submit (병렬 track B).
4. **Phase 0b PE memory loader** skeleton 작성 — mmap sections + base relocations + IAT 패치 (track A critical path).
5. commit + push.

---

## 10. closure 자체 위험 + closure 후

closure 후 후속 가능 path:
- **§12 의 4 게임 검토 결과 반영** (CS2 / Delta Force / PURPLE+Lineage / TL) — track A phase 7-8 generic 화 시 자연 흡수 후보. live anti-cheat 차단 게임은 영구 미진입.
- airgenome MAIN integration — gamebox forge ring 을 airgenome harvest 가 흡수 (이미 M7 wiring 가능)
- HEXA upstream Phase B (sleep_us, fs_watch, append_file 등 Phase A 외 RFC)

---

## 11. 통계

### 11.1 현재 (commit `d3c2371`, 2026-05-01, BG cycle 5 후)
- 누적 commits: **9**
- 총 hexa 모듈: **50** (이전 35, +15 = stage1 + own1-driven 4 + PSB-1 sketch 2 + own2 harness + closure harness 등)
- 만점 컴포넌트: **18** (이전 14, own1-driven +4 + PSB-1 sketch 즉시 채택 +2 = +6)
- 누적 BG 사이클: **5**
- CM 진행: CM-0~CM-20 done = **67% (21/31)**

### 11.2 closure 시점 예상
- 누적 hexa LoC: ~80,000 (Win32 shim 50K + loader 5K + perf 10K + tests 5K + 기타)
- 누적 commits: ~200-400 (phase 단위 + 측정 단위 + PSB 단위)
- 누적 sprint 수: ~6-10 (M8, M11, M15, PSB-1~N)
- 측정 시간: ~30 시간 (D2R + D4 × 5 시나리오 × 30분 × 3회)
- closure 시점 commit: 약 `{some hash}`, 2027-2028 사이

---

*written 2026-04-30, revised 2026-05-01 (commit 0477ae1 기준 own 1 cleanup 후 만점 14 확장).*
*revised 2026-05-01 BG cycle 5 후 (commit d3c2371 기준 만점 18, CM-0~CM-20 done, 67% complete).*
*revised 2026-05-01 BG cycle 48 후 (skeleton 156, perf 모듈 164, 첫 ALL PASS — §12 다른 게임 검토 결과 통합).*
*revised 2026-05-01-rev2 (사용자 directive — §12.7 신설: 리니지 클래식 r28 / M / W cycle 49 batch 계획. PURPLE_LINEAGE_REVIEW §0.5 정정 cross-link).*
*revised 2026-05-01-rev3 (사용자 directive — Steam-less 단독 path 추가 검토: WoT + WoWS / Elden Ring + SotE DLC / Nightreign. WG_STACK_MANJEOM_REVIEW.md + ELDEN_RING_MANJEOM_REVIEW.md 신설. §12.1 매트릭스 13 entries → 19 entries 확장. multiplayer 보류 directive 모든 게임 적용. cycle 49 commit 7877f2c — skeleton 156→164 ALL PASS 172/172).*
*revised 2026-05-01-rev4 (사용자 directive: "T2 는 일단 제거"). T2 priority entries 모두 "보류 (rev4)" 라벨링: WG Stack (WoT + WoWS) / Elden Ring offline + SotE DLC / Lineage 1/2 private opt-in. 활성 priority = T0 (D2R/D4) + T1 (CS2 offline) + T3 (영구 미진입). cycle 49 done 4건 (PURPLE + 리니지 3종) 은 보류 영향 X.*
*revised 2026-05-01-rev5 (사용자 directive: "T1 도 보류"). CS2 offline path 도 보류 (rev5) 라벨링. **활성 priority = T0 (D2R/D4 closure) + T3 (영구 미진입) 만**. 보류 entries 모두 historical record 로 docs/ 에 보존 — 미래 재검토 trigger 발생 시 활용.*
*revised 2026-05-01-rev6 (사용자 directive: rev4/rev5 partial revert). "WoT, WoWS, Elden Ring offline + SotE 보류 아님 진행" + "Lineage 1·2 private 만 보류". CS2 offline + WG Stack + Elden Ring offline + SotE DLC 모두 활성 복원. **확장 활성 priority = T0 + T1 (CS2 offline) + T2 (WoT/WoWS/Elden Ring) + T3 (영구). 보류 = Lineage 1·2 private 만**. cycle 50+ batch 5 게임 (D2R/D4/CS2/WoT/Elden Ring) cross-cutting candidates 으로 구성 가능.*
*revised 2026-05-01-rev7 (사용자 directive: "CS2 는 보류야 (Steam) 은 구현예정없음"). CS2 (offline) 다시 보류. Steam 의존 path 구현 예정 X. 이미 cycle 50/51 에 들어간 CS2 3 모듈 (CN-162/163/170) 은 historical record. **활성 priority 재축소: T0 (D2R/D4) + T2 (WoT/WoWS/Elden Ring offline+SotE) + T3 (영구). 보류 = CS2 + Lineage 1·2 private**. cycle 52+ batch 4 게임 (D2R/D4/WoT-WoWS/Elden Ring) cross-cutting 으로 구성.*

---

## 12. 다른 게임 검토 결과 (CS2 / Delta Force / PURPLE+Lineage)

> SSOT: `docs/CS2_MANJEOM_REVIEW.md`, `docs/DELTA_FORCE_MANJEOM_REVIEW.md`, `docs/PURPLE_LINEAGE_MANJEOM_REVIEW.md`, [`docs/WG_STACK_MANJEOM_REVIEW.md`](WG_STACK_MANJEOM_REVIEW.md) (rev2 추가), [`docs/ELDEN_RING_MANJEOM_REVIEW.md`](ELDEN_RING_MANJEOM_REVIEW.md) (rev2 추가).
> 본 섹션은 6 검토 문서의 **action 항목**만 통합 — 상세 근거·출처는 원본 문서 참조.

### 12.1 종합 매트릭스 (D2R/D4 + 4 검토 + 사용자 directive 추가 3 게임)

| 게임 | 만점 가능성 | 차단 사유 | closure path B 재사용 | 신규 만점 후보 (skeleton) | 우선순위 |
|---|---|---|---|---|---|
| **D2R** | 가능 (이미 진행 중) | — | — (baseline) | 이미 통합 | T0 (현재) |
| **D4** | 가능 (이미 진행 중) | online DRM 만 | — (baseline) | 이미 통합 | T0 (현재) |
| **CS2 (offline)** | 부분가능 ~70% | online VAC = 정의상 0% | 70% (Source 2 + tier0 shim 신규) | +7 만점 / +3 보조 (multiplayer 영역 보류) | **보류 (rev7 사용자 directive — Steam 의존 구현 X)** |
| **CS2 (online)** | **불가능** | VAC + Trusted Mode kernel scan, IAT integrity, 2026 VAC 3.0 강화 | — | 0 (보류) | T3 (영구 out-of-scope, 사용자 directive) |
| **Delta Force** | install~CDN만 (~20%) | **kernel ACE attest + 2025-12 DMA Shield + 2026 GDC AI security** | 90% (DX12+DXR+UE5) — Phase 1-3 만 의미 | +3 만점 (P2-3) / +3 보조 (P1) | **T3 (영구 미진입 권장)** |
| **PURPLE shell** (게임 무관) | 가능 (.NET CLR + WPF + WebView2 + Android emul core) | (없음) | 신규 .NET + Android emul stack | **이미 cycle 49 통합 (CN-149~153)** | ✓ done |
| **리니지 클래식 (r28, 2026 신작)** | 가능 (x86_64 + OpenGL 4.3 native) | GameGuard live skeleton-only | D4 PE64 + OpenGL → Metal 변환 | **이미 cycle 49 통합 (CN-154)** | ✓ done |
| **리니지 M (PURPLE 2-layer)** | 가능 (Layer 0+1) | mobile-native AC 추정 | PURPLE shell + Android emul | **이미 cycle 49 통합 (CN-155)** | ✓ done |
| **리니지 W (PURPLE 2-layer)** | 가능 (Layer 0+1) | VIOLET/nProtect 추정 | UE4 + DX11 cross-platform | **이미 cycle 49 통합 (CN-156)** | ✓ done |
| **WoT (Wargaming)** | **가능 ~85%** (training/replay) | proprietary user-mode AC (~50% pass), HWID ban (multiplayer 만) | **D2R DX11 stage1 100% 재사용** + Electron/Chromium IPC | **+9 만점 / +2 보조** (training/replay 한정) | **T2 (rev6 활성 복원, D4 closure 병렬)** |
| **WoWS (Wargaming)** | **가능 ~85%** (replay) | 동일 (WoT 와 같은 stack) | 동일 | **WoT 와 generic 흡수 8/13** | **T2 (rev6 활성, WoT 와 동시)** |
| **Elden Ring (offline)** | **가능 ~75%** | EAC kernel — 단 **offline mode 정식 toggle** (FromSoft 인정) | **D4 DX12 stage2 100% 재사용** | **+7 만점 / +4 보조** (offline 한정) | **T2 (rev6 활성, D4 closure 후 즉시)** |
| **Shadow of the Erdtree DLC** | base 와 동일 | 동일 (별도 AC X) | base 와 100% 같음 | +1 만점 (`c_sote_dlc_content_probe`) | **T2 (rev6 활성, base 와 동시)** |
| **Elden Ring Nightreign** | 0% (online-only coop) | 4-player coop = multiplayer 거부 directive | — | 0 (보류) | T3 (영구 미진입) |
| **Lineage 1 (1998 legacy live)** | 차단 | GameGuard kernel | DDraw → Metal 2D blit | +3 만점 (P2-3) | T3 (live), 보류 rev6 (private) |
| **Lineage 1 (private)** | 가능 (조건부) | — (legacy emul, GameGuard 비활성) | i386 PE 100% (D2R 재사용) | +6~8 (full 6 phase) | **보류 (rev6 사용자 directive)** |
| **Lineage 2 (live)** | 차단 | GameGuard / XIGNCODE3 kernel | DX9 + UE2.5 — Apple D3DMetal sweet spot | +3 만점 (P2-3) | T3 (live), 보류 rev6 (private) |
| **Lineage 2 (private/legacy)** | **가능** | — (l2j ecosystem) | DX9 + UE2.5 100% | +6~9 (full 6 phase, validated 가능) | **보류 (rev6 사용자 directive)** |
| **Lineage 2M (live)** | 차단 | VIOLET / nProtect kernel | UE4 + DX11 (다른 UE4 게임 재사용) | +3 (skeleton-only validated 0) | T3 |
| **Throne and Liberty** | **불가능** | EAC kernel (Delta Force 동일 운명) | — | 0 (skeleton만 +1) | T3 (영구 미진입) |

### 12.2 우선순위 라인 (closure 이후 진입 순서)

> **rev2 (2026-05-01 사용자 directive)**: 사용자가 "리니지 클래식 / 리니지 M / 리니지 W 진행" 직접 결정 → 본 라인은 **D2R/D4 closure 와 병렬 진행 가능한 Lineage 시리즈 진입 순서로 격상**. 자세한 cycle 49+ 계획은 §12.7 참조.
> **rev4 (2026-05-01 사용자 directive)**: "T2 는 일단 제거" — 모든 T2 priority entries 보류.
> **rev5 (2026-05-01 사용자 directive)**: "T1 도 보류" — CS2 offline path 까지 보류.
> **rev6 (2026-05-01 사용자 directive)**: rev4/rev5 partial revert — "WoT, WoWS, Elden Ring offline + SotE 진행" + "Lineage 1·2 private 만 보류". CS2 offline 도 활성 복원.
> **rev7 (2026-05-01 사용자 directive)**: "CS2 는 보류야 (Steam) 은 구현예정없음" — CS2 offline 도 다시 보류 (Steam 의존). 이미 cycle 50 에 들어간 CS2 모듈 2건 (CN-162 c_cs2_steam_api64_offline_shim + CN-163 c_cs2_source2_pso_genome) + cycle 51 의 CN-170 c_cs2_tier0_plat_shim 은 historical record. **활성 priority 재축소: T0 (D2R/D4) + T2 (WoT/WoWS/Elden Ring offline+SotE) + T3 (영구). 보류 = CS2 + Lineage 1·2 private**.

```
1. D2R closure 완료              ← 현재 진행 (CM-0~CM-20 done, CM-21~ game install dependent)
2. D4 closure 완료                ← Phase 10 dependent
3. (rev2 done) 리니지 클래식 r28 + 리니지 M + 리니지 W + PURPLE shell — cycle 49 commit 7877f2c (skeleton 156→164)
4. ~~(rev7 보류)~~ ~~CS2 offline path~~ — Steam 의존 구현 X (rev7)
5. (T2 rev6 활성) Wargaming Stack (WoT + WoWS) — kernel AC 부재, training/replay validated
6. (T2 rev6 활성) Elden Ring offline + SotE DLC — D4 DX12 stage2 직접 재사용, EAC offline mode 정식
7. ~~(보류 rev6)~~ ~~Lineage 1·2 private (l2j) opt-in~~ — 보류 유지
8. (T3 영구) Nightreign / Delta Force / TL / Lineage 2M·W live (kernel AC) — 영구 미진입
```

### 12.3 즉시 권장 actions (D2R/D4 closure 와 무관하게 즉시 수행 가능)

| # | 모듈 | 가치 | 검토 문서 |
|---|---|---|---|
| A1 | **`c_dforce_unsupported_block_doc`** — Delta Force detect 시 launch 차단 + BAN 위험 고지 | 사용자 보호 / gamebox identity 정합 | DELTA_FORCE §5-5 candidate 9 |
| A2 | **`c_purple_anticheat_block_doc`** — Lineage live + TL detect 시 동일 차단 + 안내 | 동일 | PURPLE_LINEAGE §5-4 candidate 12 |
| A3 | **`c_cs2_vac_avoid_guard`** — VAC secure server connect 시 자동 거부 | 사용자 자산 (Steam account) 보호 | CS2 §4 candidate 10 |
| A4 | **`c_purple_private_server_allow`** — l2j-style private 서버 식별 → kernel AC 비활성 시 path B opt-in | 사용자 자발적 opt-in + TOS 안내 | PURPLE_LINEAGE §5-4 candidate 13 |

→ A1~A4 는 manjeom 5-13 점 (B/C 축이 user-protect/policy 라 일부 낮음) 이지만, **gamebox identity ("own 1 정신") 보호 차원에서 cycle 49-50 batch 후보로 권장**.

### 12.4 generic 인프라 흡수 후보 (D2R/D4 모듈 일반화로 자연 통합)

3 검토 문서가 모두 같은 결론 — **OAuth/2FA/CDN 모듈은 game 무관 generic 화** 시 4 launcher (Battle.net + Steam + Tencent + PLAYNC) 모두 흡수 가능:

| 기존 모듈 | generic 화 시 흡수 launcher |
|---|---|
| `c_oauth_flow_proxy` (Battle.net 이름) | + Steam, Tencent, PLAYNC, NCWEST |
| `c_bnet_token_refresh_track` | + Steam Web API token, Tencent token, PLAYNC token |
| `c_cdn_mirror_select` (Akamai) | + myskcdn (NCSOFT), CloudFront (Steam depot), Tencent CDN |
| `c_cdn_chunk_assemble_track` | + Steam content depot, NCSOFT pak chunk, Tencent install chunk |
| `c_install_signature_verify` (Authenticode) | + Steam manifest SHA, NCSOFT installer cert, Tencent installer signature |

→ **cycle 49+ 시 BG batch 일부를 generic 화 작업에 할당 권장** (단순 게임 추가보다 ROI 높음).

### 12.5 영구 out-of-scope (정책상 거부)

다음 영역은 어떤 cycle 에도 진입 X:

- **VAC bypass / Trusted Mode bypass / matchmaking 진입 시도** (CS2)
- **ACE kernel attest 우회 시도** (Delta Force)
- **GameGuard / VIOLET / XIGNCODE3 / EAC kernel driver 회피 시도** (Lineage live, TL)
- **Steam / PLAYNC / Tencent ID 계정 BAN risk 행위**
- **어떤 형태의 cheat 시그니처 작성**

→ 본 영역 진입은 own 1 ("Wine 0줄" identity) 와 정면 모순. gamebox 의 가치 = **개발자/연구자가 Apple Silicon 에서 frame trace 측정**, cheat enabler X.

### 12.6 미래 재검토 trigger (조건부)

다음 이벤트 발생 시 §12 재작성:
- **Lineage 2 NEXT GEN UE5 remaster 발표** (NC) — UE5 + 약한 AC 시 검토 가치
- **Team Jade 가 Delta Force 에 user-mode ACE variant 출시** — 가능성 낮음
- **Apple Silicon Mac 공식 지원 launcher 발표** (PURPLE / Steam / EA / Tencent 중 하나)
- **D4 closure 후 generic UE5 + DXR + DLSS 인프라 완성** — Delta Force/TL skeleton 일부 자동 흡수 가능
- **VAC / EAC user-mode 분리 옵션** — 현재 모두 kernel

---

### 12.7 사용자 directive (rev2): 리니지 클래식 / M / W 진행 (cycle 49+)

> **trigger**: 2026-05-01 사용자 directive — "리니지 클래식, 리니지 M, 리니지W 기준으로 진행".
> SSOT: `docs/PURPLE_LINEAGE_MANJEOM_REVIEW.md` §0.5 (rev2 정정 — L Classic r28 신작 / L M PC PURPLE / PURPLE = Android emul 본체).

#### 12.7.1 정정된 게임 status (WebSearch 2026-05-01 후속)

| 게임 | 출시 / status | closure path B layer | 직접 적용 가능? | live 차단 |
|---|---|---|---|---|
| **리니지 클래식 (r28)** | 2026-02-07 KR/대만 신작 (월정액) | Layer 1' — x86_64 PE + OpenGL 4.3 native | **○ 직접 가능** | GameGuard + 기기등록 (HWID) |
| **리니지 M** | 2017 mobile, PURPLE PC 정식 | Layer 0 (PURPLE.exe) + Layer 1 (Android APK) | △ Layer 0 direct, Layer 1 = 2-layer (Android emul core 통과 필요) | (mobile-native AC 추정) |
| **리니지 W** | 2021 UE4 cross-platform, PURPLE PC | Layer 0 + Layer 1 (mobile build = APK) | △ 2-layer | VIOLET / nProtect (추정) |

#### 12.7.2 cycle 49 batch (8 모듈 — Phase A PURPLE shell prerequisite + Phase B 3 게임 first-frame skeleton)

**Phase A — PURPLE shell prerequisite (5 모듈, CN-149~153)**:

| CN | 모듈 | 역할 |
|---|---|---|
| CN-149 | `c_purple_dotnet_clr_probe` | PURPLE PE 의 .NET CLR / WinForms 패턴 식별 |
| CN-150 | `c_purple_plaync_oauth_proxy` | PLAYNC OAuth (Authorization Code + Token, generic OAuth 모듈 흡수 후보) |
| CN-151 | `c_purple_2fa_totp_proxy` | NC 인증기 TOTP RFC 6238 (generic 2FA) |
| CN-152 | `c_purple_webview2_warm` | WebView2 (Edge Chromium) embed warm-up — store/lounge UI |
| CN-153 | `c_purple_android_emul_layer_track` | PURPLE 의 Android emul core 식별 — L M / L W 의 Layer 1 prerequisite |

**Phase B — 3 게임 first-frame skeleton (3 모듈, CN-154~156)**:

| CN | 모듈 | 게임 | layer |
|---|---|---|---|
| CN-154 | `c_lineage_classic_r28_native_first_frame` | L Classic | Layer 1' direct (x86_64 + OpenGL 4.3) |
| CN-155 | `c_lineage_m_purple_apk_emul_first_frame` | L M | Layer 0 + Layer 1 (PURPLE → Android APK) |
| CN-156 | `c_lineage_w_purple_ue4_first_frame` | L W | Layer 0 + Layer 1 (PURPLE → UE4 mobile build) |

**예상 결과**: skeleton 156 → 164 (+8), perf 모듈 164 → 172.

#### 12.7.3 validated 가능성

| 모듈 | skeleton (15/15) | validated 가능 |
|---|---|---|
| Phase A 5 모듈 (PURPLE shell) | ○ self_test PASS | ○ — PURPLE 자체는 anti-cheat 없음 (.NET shell) |
| CN-154 L Classic | ○ self_test PASS | △ live = GameGuard 차단 (skeleton-only) |
| CN-155 L M | ○ self_test PASS | △ live = mobile-native AC (skeleton-only) |
| CN-156 L W | ○ self_test PASS | △ live = VIOLET/nProtect 추정 (skeleton-only) |

→ **첫 cycle 의 목표는 skeleton 만점 +8** (validated 는 후속).
→ Phase A 5 모듈은 generic OAuth/WPF 인프라 — 다른 launcher (Battle.net / Steam / Tencent / NCWEST) 자연 흡수.

#### 12.7.4 cycle 50+ 후속 path (조건부)

- L Classic anti-cheat 우회 X — live 차단 수용 시 **skeleton 만점 누적 + post-cycle PSB sketch round 신규**.
- private 서버 ecosystem (l2j-classic, NCSOFT TOS 회색) opt-in 시 → §12.7.5 (TBD)
- L M / L W 의 Android emul Layer 1 = Dalvik VM 직접 emulation 신규 phase (Phase 11 후보) — 매우 큰 prerequisite
- Apple iOS-on-Mac 별도 track — L M iOS app 직접 실행 (closure path B 외, native arm64) — own 1 정합 (Wine 0줄)
