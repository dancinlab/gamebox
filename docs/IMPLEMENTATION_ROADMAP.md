# Implementation Roadmap — 만점 milestones (자유 schema)

> **scope**: BRAINSTORM 에서 만점 통과한 12 개 컴포넌트 (D 8 + R 4) 의 마일스톤.
> **자유**: axis/byte/schema 컴포넌트별 자연 형태.
> **상위**: [BRAINSTORM.md](BRAINSTORM.md), [PERF_PLAN.md](PERF_PLAN.md), [SCORING.md](SCORING.md).

---

## 1. 마일스톤 (M0 → M16)

| M | 이름 | 종류 | depends-on | ETA | 산출물 | 만점 |
|---|---|---|---|---|---|---|
| **M0** | R0 instrumentation enabler | enabler | — | 3-5d | `lib/perf/r0_*.{sh,hexa}` + JSONL | A=5, B=2 (enabler 인정) |
| **M1** | baseline 측정 (stock GPTK) | measurement | M0 | 2d | `tool/bench/baseline_d2r.jsonl` + `_d4.jsonl` | mandatory pre-sprint |
| **M2** | **C-FRAMETIME** | D-list 15/15 | M0, M1 | 1-2d | `lib/perf/c_frametime.{sh,hexa}` + `forge/genomes-gptk-frametime.ring` | 5+5+5 ✅ |
| **M3** | **C-HEAP** | D-list 15/15 | M0, M2 | 2-3d | `lib/perf/c_heap.{sh,hexa}` + 4 label + `forge/genomes-gptk-heap.ring` | 5+5+5 ✅ |
| **M4** | **C-PSO** | D-list 15/15 | M0, M3 | 3-4d | `lib/perf/c_pso.{sh,hexa}` + SHBF + AC trie + `forge/genomes-gptk-pso.ring` | 5+5+5 ✅ |
| **M5** | **C-FRAME** | D-list 15/15 | M2 | 2d | `lib/perf/c_frame.{sh,hexa}` + Holt forecast + `forge/genomes-gptk-frame.ring` | 5+5+5 ✅ |
| **M6** | **C-SHADER-AGING** ⭐NEW | D-list 15/15 | M0, M4 | 2d | `lib/perf/c_shader_aging.{sh,hexa}` + APBF + `forge/genomes-gptk-shader-aging.ring` | 5+5+5 ✅ |
| **M7** | airgenome harvest 통합 | wiring | M3, M4, M5, M6 | 1-2d | ring 파일 multi-schema 흡수 (header axes/rec_bytes 동적) | invariant 2 유지 |
| **M8** | **Sprint 1 실측** | measurement | M7 | 2d | baseline 대비 표 → `D2R_D4_COMPARISON.md` measured 컬럼 | 만점 게이트 |
| **M9** | **C-DXIL-PERSIST** | R-list 10/10 | M0, M1 | 1-2d | `lib/perf/c_dxil_persist.sh` + columnar index | 5+5 ✅ |
| **M10** | **C-SHADER-WARM** | R-list 10/10 | M4, M9 | 2d | `lib/perf/c_shader_warm.sh` + SHBF seed | 5+5 ✅ |
| **M11** | **Sprint 2 실측** | measurement | M10 | 2d | results 갱신 — R 효과 분리 측정 | 만점 게이트 |
| **M12** | **C-SHADER-SEQ** ⭐NEW | D-list 15/15 | M4, M6, M10 | 3d | `lib/perf/c_shader_seq.{sh,hexa}` + AC trie + Holt + `forge/genomes-gptk-shader-seq.ring` | 5+5+5 ✅ |
| **M13** | **C-SHADER-WARM++** | D-list 15/15 | M10, M12 | 2d | M10 진화 (SHBF + AC trie + columnar + Holt) | 5+5+5 ✅ |
| **M14** | **C-CORRELATE** ⭐NEW | D-list 15/15 | M2, M3, M4 | 2d | `lib/perf/c_correlate.{sh,hexa}` + label fusion + `forge/genomes-gptk-correlate.ring` | 5+5+5 ✅ |
| **M15** | **Sprint 3 실측** | measurement | M14 | 2d | 신규 만점 3개 효과 측정 + correlation analysis 결과 | 만점 게이트 |
| **M16** | **C-ROSETTA-PERSIST** + **C-METAL-QUEUE-PRI** (deferred) | R-list 9/10 | M0 | 3-4d | `lib/perf/c_rosetta_persist.sh` + `c_metal_queue.sh` | 4-5+4-5 |

총 ETA: **~6-7주** (직렬), 병렬 시 **4주** 가능.

---

## 2. 의존성 그래프

```
M0 (R0 instrumentation)
 ├─→ M1 (baseline)
 │    └─→ M8 (S1 실측)  ←── M2/M3/M4/M5/M6/M7
 │    └─→ M11 (S2 실측) ←── M9/M10
 │    └─→ M15 (S3 실측) ←── M12/M13/M14
 │
 ├─→ M2 (C-FRAMETIME) ── enabler
 │    ├─→ M3 (C-HEAP) ── M4 (C-PSO) ── M6 (C-SHADER-AGING)
 │    └─→ M5 (C-FRAME)
 │
 ├─→ M3, M4, M5, M6 → M7 (harvest 통합) → M8 (S1)
 │
 ├─→ M9 (C-DXIL-PERSIST)
 │    └─→ M10 (C-SHADER-WARM, needs M4 PSO seed) → M11 (S2)
 │
 ├─→ M4, M6, M10 → M12 (C-SHADER-SEQ)
 │   M10, M12      → M13 (C-SHADER-WARM++)
 │   M2, M3, M4    → M14 (C-CORRELATE)
 │   M12, M13, M14 → M15 (S3)
 │
 └─→ M16 (C-ROSETTA + C-METAL-QUEUE deferred)
```

---

## 3. Sprint 매핑

### Sprint 1 — D-quad 정체성 정합 + APBF 신규 (M0→M8, ~2.5주)

| 주 | 작업 | 만점 |
|---|---|---|
| W1 | M0 R0 + M1 baseline + M2 C-FRAMETIME | enabler + 1 만점 |
| W2 | M3 C-HEAP + M4 C-PSO + M5 C-FRAME | 3 만점 |
| W3 | M6 C-SHADER-AGING + M7 통합 + M8 측정 | 1 만점 + wiring |

산출 (estimate): D2R/D4 baseline 대비 P99 -30~80%, stutter -50~95%.

### Sprint 2 — R-warm cache (M9→M11, ~1주)

| 주 | 작업 | 만점 |
|---|---|---|
| W4 | M9 C-DXIL-PERSIST + M10 C-SHADER-WARM + M11 측정 | 2 만점 R-list |

산출 (estimate): D4 zone hitch -85~99%, 2nd cold launch -50~80%.

### Sprint 3 — 신규 만점 진화 (M12→M15, ~1.5주)

| 주 | 작업 | 만점 |
|---|---|---|
| W5 | M12 C-SHADER-SEQ + M13 C-SHADER-WARM++ | 2 만점 |
| W6 | M14 C-CORRELATE + M15 측정 | 1 만점 + analysis |

산출:
- C-SHADER-SEQ: zone-transition pre-compile → R1 보다 **+5~15% 추가 stutter 감소**
- C-CORRELATE: 어떤 stutter 가 어떤 원인 (PSO/heap/frame) 인지 분석 → 다음 sprint 우선순위
- C-SHADER-WARM++: 만점 진화로 single-launch 안에 작동

### Sprint 4 — Deferred / Optional (M16, ~1주)

| 주 | 작업 | 만점 |
|---|---|---|
| W7 | M16 C-ROSETTA-PERSIST + C-METAL-QUEUE-PRI | 2 R-list 9/10 |

---

## 4. 마일스톤 acceptance criteria

각 M 의 done 조건. 미달 시 다음 M 시작 X.

### M0 R0 (enabler)

- [ ] `lib/perf/r0_hud_parser.sh` MTL_HUD stdout → JSONL
- [ ] `lib/perf/r0_cache_watch.sh` D3DMetal cache 디렉토리 mtime 모니터
- [ ] `lib/perf/r0_heap_sample.sh` 30Hz heap 샘플
- [ ] `lib/perf/r0_session.sh` 게임당 사이드카 JSONL
- [ ] self-test 4 항목 PASS
- [ ] 출력 → `~/.airgenome/gamebox/r0/<game>_<sid>.jsonl`

### M1 Baseline 측정

- [ ] D2R 시나리오 5개 (PERF_PLAN § 12) 각 30분 × 3회 = JSONL
- [ ] D4 시나리오 5개 동일
- [ ] median + P50/P99 추출 → `tool/bench/baseline.csv`
- [ ] 저장: `docs/BASELINE_<date>.md`

### M2 C-FRAMETIME

- [ ] 20-byte per-row columnar
- [ ] 4 stutter 패턴 분류 (spike/sustained_drop/oscillation/zone_transition)
- [ ] Holt 1-second forecast
- [ ] ring header 16-byte
- [ ] self-test: 합성 입력 100K rows → 분류 정확도 ≥ 90%
- [ ] **만점**: 5+5+5 ✅

### M3 C-HEAP

- [ ] 13-byte/record 5-field schema
- [ ] 4 라벨 (OOM_imminent / frag_critical / eviction_storm / thrashing)
- [ ] pre-evict 신호 채널 (sidecar → game)
- [ ] Holt trajectory forecast
- [ ] self-test: 합성 heap 시나리오 → 라벨 정확도 ≥ 90%
- [ ] **만점**: 5+5+5 ✅

### M4 C-PSO

- [ ] HUD parser + cache reader hybrid
- [ ] 8-field 24-byte/record schema
- [ ] SHBF (1KB, FPR 1%, 100K PSO)
- [ ] AC trie (vs/ps/cs 분류)
- [ ] dedup decision API
- [ ] self-test: 합성 100K PSO → SHBF 정확도, dedup hit rate
- [ ] **만점**: 5+5+5 ✅

### M5 C-FRAME

- [ ] 7-field per-second roll-up (24 byte/record)
- [ ] Holt forecast (1초 ahead)
- [ ] self-test: 합성 frame stream → forecast MAE
- [ ] **만점**: 5+5+5 ✅

### M6 C-SHADER-AGING ⭐NEW

- [ ] APBF 64KB (adaptive prob bloom, time-decay)
- [ ] age_bucket 4-bit (0=hot ~ 15=cold)
- [ ] cold shader auto-evict policy
- [ ] self-test: 합성 shader access pattern → bucket 추이 검증
- [ ] **만점**: 5+5+5 ✅

### M7 harvest 통합

- [ ] ring header 동적 (axes/rec_bytes 자유)
- [ ] `forge/genomes-gptk-*.ring` 5개 작성 (frametime/heap/pso/frame/shader-aging)
- [ ] `airgenome/modules/harvest.hexa` 가 multi-schema 흡수
- [ ] invariant 1 (core 미수정), 2 (모듈 간 import X) 유지
- [ ] self-test: ring round-trip + harvest read

### M8 Sprint 1 실측

- [ ] D2R 시나리오 5개 × 3회 측정
- [ ] D4 시나리오 5개 × 3회 측정
- [ ] baseline 대비 표 → `docs/D2R_D4_COMPARISON.md` 의 estimate → measured 컬럼 추가
- [ ] **만점 게이트**: P99 -30%↑ (D4 open-zone) AND stutter -50%↑ AND FPS 무회귀

### M9 C-DXIL-PERSIST

- [ ] cache directory watcher
- [ ] persist + restore 양방향
- [ ] columnar index (24 byte/row)
- [ ] GPTK version invalidation
- [ ] self-test: persist→ wipe → restore round-trip

### M10 C-SHADER-WARM

- [ ] M4 의 SHBF + PSO seed 사용
- [ ] pre-populate 액션 (background, idle 시)
- [ ] gptk_version mismatch 시 nuke
- [ ] self-test: empty cache → seed → 무결성 검증

### M11 Sprint 2 실측

- [ ] D4 zone transition 측정 (R1 효과)
- [ ] 2nd cold launch 측정 (R2 효과)
- [ ] 표 갱신 — R1 단독 / R1+R2 / S1+S2 통합 3 케이스
- [ ] **만점 게이트**: zone stutter -85%↑ AND 2nd cold -50%↑

### M12 C-SHADER-SEQ ⭐NEW

- [ ] AC trie 학습 (zone 진입 → shader compile sequence)
- [ ] Holt forecast (next-zone 예측)
- [ ] sequence storage `forge/genomes-gptk-shader-seq.ring`
- [ ] self-test: 합성 zone-transition 시퀀스 → next-zone 예측 정확도 ≥ 70%

### M13 C-SHADER-WARM++

- [ ] M10 진화 (SHBF + AC trie + columnar + Holt)
- [ ] M12 forecast 활용한 pre-compile timing
- [ ] self-test: M10 보다 stutter 감소 +5%↑

### M14 C-CORRELATE ⭐NEW

- [ ] label fusion (C-FRAMETIME + C-HEAP + C-PSO + C-FRAME)
- [ ] correlation score 계산
- [ ] 30 byte/row sparse storage
- [ ] self-test: 합성 multi-anomaly 시나리오 → correlation 검출

### M15 Sprint 3 실측

- [ ] M12/M13/M14 효과 측정
- [ ] correlation 분석 결과로 다음 sprint 우선순위 도출
- [ ] **만점 게이트**: M13 으로 M10 대비 +5% stutter 감소 AND M14 정확한 원인 분류

### M16 C-ROSETTA-PERSIST + C-METAL-QUEUE-PRI (deferred)

- [ ] Rosetta cache directory persist
- [ ] Metal queue priority hint (env or LD_PRELOAD)
- [ ] self-test
- [ ] optional — Sprint 1+2+3 결과 만족스러우면 skip

---

## 5. 만점 게이트 (sprint 진행 차단)

각 sprint 끝 미달 시 sprint 종료 X (재작업):

| Sprint | 만점 게이트 |
|---|---|
| **S1** end (M8) | M2/M3/M4/M5/M6 self-test PASS + M7 통합 PASS + M8 측정 P99 -30%↑ (D4 open-zone) |
| **S2** end (M11) | M9/M10 self-test PASS + M11 측정 R1 단독 zone stutter -85%↑ |
| **S3** end (M15) | M12/M13/M14 self-test PASS + M15 측정 M13 > M10 +5% AND M14 correlation 정확 |

미달 시 BRAINSTORM 회귀 — 만점 sub-option 재선정.

---

## 5.5 반복 패러다임 발굴 루프 (만점 고갈까지) ♾️

> **directive**: 각 sprint 완료 후 → 추가 패러다임 brainstorm → 만점 통과 새 컴포넌트 발견 시 다음 sprint → 고갈까지 반복.

### 루프 구조

```
Sprint N 구현 → Sprint N 측정 → Post-Sprint N Brainstorm
                                       ├── 만점 새 후보 있음 → Sprint N+1
                                       └── 고갈 (만점 새 후보 없음) → STOP
```

### 각 post-sprint brainstorm 의 input

| input | 의미 |
|---|---|
| 1. 구현 중 발견한 새 채널 / API / cache 위치 | 새 데이터 source = 새 후보 |
| 2. 측정 결과의 unexplained anomaly | 모르는 원인 = 새 무기 필요 |
| 3. correlation analysis (M14) 결과 | 우선순위 재조정 + 새 컴포넌트 |
| 4. 사용자 보고 (D2R/D4 외 다른 시나리오) | 새 시나리오 = 새 문제 |
| 5. GPTK 메이저 업데이트 | 새 표면 = 새 후보 |
| 6. macOS 메이저 업데이트 (Metal API 변화) | 새 hint/API = 새 후보 |
| 7. airgenome 새 filter 자산 | 새 무기 → 기존 문제 재매칭 |
| 8. 다른 게임 추가 (Wow, Overwatch, ...) | 새 데이터 → 새 패턴 (4-game 검토 결과: [CLOSURE_ROADMAP.md § 12](CLOSURE_ROADMAP.md#12-다른-게임-검토-결과-cs2--delta-force--purplelineage)) |

### 각 post-sprint brainstorm 의 output

| output | 의미 |
|---|---|
| (a) 새 만점 후보 N 개 | A+B+C=15 통과 |
| (b) 만점 미달 후보 (참고용) | 제외, 나중에 변경되면 재평가 |
| (c) 폐기 후보 | 만점 영원히 안 됨 |
| (d) 만점 게이트 갱신 | 다음 sprint 의 % 목표 |

### 루프 종료 조건 (고갈)

- post-sprint brainstorm 3 회 연속 새 만점 후보 0 개
- OR sprint N 의 measured improvement Δ% < 1% (의미 없는 미세 개선)

→ 종료 시: `docs/EXHAUSTION_<date>.md` 작성 (탐색 종료 보고서, 다음 패러다임 변화 시 재시작 트리거 명시).

### 첫 번째 post-sprint brainstorm 가이드 (Post-S1, M8 직후)

S1 측정 결과 가지고 — *지금 시점에 미리 sketch* (M8 시점에 갱신):

| 발견할 가능성 큰 새 만점 후보 | 무기 | 문제 |
|---|---|---|
| **C-PSO-CHAIN** | AC trie + Holt | PSO compile 들이 *연쇄* — 다음 PSO 예측 |
| **C-HEAP-DEFRAG** | columnar + scheduler | frag_critical 라벨 발생 시 자동 defrag 트리거 |
| **C-GPU-HUD-RING** | columnar | MTL_HUD 모든 metric 을 단일 ring 으로 통합 |
| **C-AUDIO-DSP** | state vector | audio decoding overhead (D4 의 ambient sound) — Apple AVAudio 대체 hint |
| **C-INPUT-LATENCY** | state vector | Wine 입력 처리 latency 측정 + Metal queue priority 연동 |

→ S1 측정 후 confirm/reject.

### 두 번째 (Post-S2, M11 직후) — sketch

| 발견할 가능성 | 무기 | 문제 |
|---|---|---|
| **C-DXIL-DELTA** | columnar diff | 게임 패치 시 변경된 shader 만 re-compile (full nuke 회피) |
| **C-ZONE-PREFETCH-IO** | trie + result_cache | zone 진입 직전 asset file 들 pre-read (page cache warm) |
| **C-PSO-FAMILY-CLUSTER** | columnar + clustering | 비슷한 PSO 들 family 로 묶어 한꺼번에 컴파일 |

### 세 번째 (Post-S3, M15 직후) — sketch

M14 correlation 분석 결과가 input — "어떤 anomaly 가 어떤 다른 anomaly 와 자주 동시 발생" 이 새 만점 후보 도출 핵심:

| 발견할 가능성 | 무기 | 문제 |
|---|---|---|
| **C-CASCADE-LABEL** | label fusion + temporal | A → B 인과 추적 (PSO compile → heap pressure → frame stutter chain) |
| **C-REMEDIATE-PLAN** | rule engine | label 발생 시 자동 액션 sequence (pre-evict + pre-warm + queue priority 동시) |

### 4 회차 이후

매 sprint 완료 후 동일 패턴.
**원칙**: 새 만점 후보 1 개라도 있으면 sprint 진행. 0 개 3 회 연속이면 고갈 인정.

---

## 6. 위험 대응 (M 별)

| M | 주요 리스크 | 완화 |
|---|---|---|
| M0 | MTL_HUD 포맷 변경 | parser schema validator + fallback |
| M3 | heap label false positive | self-test 합성 데이터 90% 정확도 게이트 |
| M4 | PSO hash 수집 어려움 | (b) cache reader fallback + (d) LD_PRELOAD 마지막 |
| M6 | APBF parameter tuning | 24h 합성 access pattern 으로 검증 |
| M10 | seed 양 부족 | 사용자별 첫 실행 자동 seed 누적 |
| M12 | zone-transition 검출 어려움 | C-FRAMETIME 의 zone_transition 라벨 재사용 |
| M14 | correlation false positive | sliding window + min support threshold |
| M16 | Rosetta API/cache 변동 | optional 처리, deferred 가능 |

---

## 7. live SSOT — checklist

```
- [x] M0  R0 instrumentation       done (2b7a625, 5/5 PASS)
- [B] M1  baseline                  blocked (Phase 9 D2R 첫 frame 후, ~13.5 달)
- [x] M2  C-FRAMETIME               done synthetic (real validation: Phase 9 후)
- [x] M3  C-HEAP                    done synthetic (9/9 label accuracy)
- [x] M4  C-PSO                     done synthetic (50/50 SHBF dedup)
- [x] M5  C-FRAME                   done synthetic (Holt monotonic + flat 검증)
- [x] M6  C-SHADER-AGING ⭐         done synthetic (APBF 16-bucket)
- [x] M7  harvest 통합              done synthetic (multi-schema ring)
- [B] M8  Sprint 1 측정             blocked (Phase 9 후)
- [x] M9  C-DXIL-PERSIST            done synthetic (snapshot/restore round-trip)
- [x] M10 C-SHADER-WARM             done synthetic (SHBF + populate + nuke)
- [B] M11 Sprint 2 측정             blocked (Phase 9 후)
- [x] M12 C-SHADER-SEQ ⭐           done synthetic (4/4 zone prediction)
- [x] M13 C-SHADER-WARM++           done synthetic (SHBF+trie+col+Holt)
- [x] M14 C-CORRELATE ⭐            done synthetic (3 fusion: scores 80/50/30)
- [B] M15 Sprint 3 측정             blocked (Phase 9 후)
- [ ] M16 C-ROSETTA + C-METAL-QPRI  deferred

### own 1 (DIY hexa PE 로더) — track A
- [x] Phase 0a pe_parser           done (Battle.net-Setup.exe 5 sections)
- [ ] Phase 0b pe_loader           planned — mmap + relocations + IAT
- [ ] Phase 1   x64 ABI shim       planned
- [ ] Phase 2   win32_stubs        planned
- [ ] Phase 3a  kernel32           planned
- [ ] Phase 3b  ntdll              planned
- [ ] Phase 3c  msvcrt             planned
- [ ] Phase 4a  user32             planned
- [ ] Phase 4b  gdi32              planned
- [ ] Phase 4c  advapi32           planned
- [ ] Phase 5a  DirectX 11 binding planned
- [ ] Phase 5b  DirectX 12 binding planned (D4 필수)
- [ ] Phase 6   Winsock            planned
- [ ] Phase 7   OSS Win 게임 검증  planned
- [ ] Phase 8   Battle.net 우회    planned
- [ ] Phase 9   D2R 첫 frame       planned (~13.5 달 누적)
- [ ] Phase 10  D4 첫 frame        planned (~18 달 누적)

### own1-driven 신규 만점 4 (track D, Phase 진행 중 합류)
- [ ] CN-1 C-PE-PARSE-CACHE        ← Phase 0a 후 즉시 (만점 +1)
- [ ] CN-2 C-IAT-RESOLVE-CACHE     ← Phase 0b 후
- [ ] CN-3 C-WIN32-CALL-PROFILE    ← Phase 2 후
- [ ] CN-4 C-SYSCALL-BRIDGE-CACHE  ← Phase 3a 후

### track B — HEXA upstream
- [ ] RFC A1 float arithmetic 안정화  draft → PR
- [ ] RFC A2 bit ops & | ^ << >>      draft → PR
- [ ] RFC A3 dict literal             draft → PR

### PSB (post-sprint paradigm discovery, track C)
- [B] PSB-1 Post-S1 brainstorm      blocked (M8 후)
- [B] PSB-2 Post-S2 brainstorm      blocked (M11 후)
- [B] PSB-3 Post-S3 brainstorm      blocked (M15 후)

> **현재 (commit 0477ae1, 2026-05-01)**:
> M0 + M2-M14 + harvest_integ + Phase 0a (총 17 모듈) hexa-only 완성. self-test 18/18 PASS.
> own 1 enforcement 21/21 PASS (Wine 0 + community 흔적 0).
> 만점 총합 update: **14** (기존 D 8 + R 2 + 신규 own1-driven D 4).
> closure ETA ~21 달 (D2R + D4 모두). [CLOSURE_ROADMAP.md](CLOSURE_ROADMAP.md) § 5 참조.

> **[B]** = blocked. 측정 milestones 는 사용자 액션 (GPTK + Battle.net + D2R/D4 설치) 후
> 진행 가능. 그 외 모든 milestones 는 hexa-only 자율 진행 (skeleton + synthetic self-test).
- [ ] PSB-1  Post-S1 brainstorm (M8 직후)   pending
- [ ] PSB-2  Post-S2 brainstorm (M11 직후)  pending
- [ ] PSB-3  Post-S3 brainstorm (M15 직후)  pending
- [ ] PSB-N  반복... 만점 고갈까지            ♾️
```

각 완료 시 `proof: <path>` 추가, status `planned → active → done`.

각 PSB-N 완료 시 산출물:
- `docs/POST_SPRINT_<N>_BRAINSTORM.md` — 새 만점 후보 + 채점 + 채택
- 채택된 후보 → 다음 sprint M-list 에 추가
- 새 만점 후보 0 개 3 회 연속 → `docs/EXHAUSTION_<date>.md` 작성 + 종료

---

## 8. 다음 액션 (M0 시작)

1. `lib/perf/` 디렉토리 생성
2. `lib/perf/r0_hud_parser.sh` skeleton
3. `lib/perf/r0_cache_watch.sh` skeleton
4. `lib/perf/r0_heap_sample.sh` skeleton
5. `lib/perf/r0_session.sh` skeleton
6. `tests/test_perf.sh` skeleton
7. `entry` 에 `gamebox r0 <sub>` 라우팅 추가

---

*written 2026-04-30. 자유 + 기법-우선 roadmap. 만점 12 컴포넌트 (D 8 + R 4) 채택.*
