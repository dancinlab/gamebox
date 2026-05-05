# PSB-3 Sketch — Post-Sprint-3 Brainstorm (M14 correlation 결과 input 가정)

> **scope**: closure roadmap (`docs/CLOSURE_ROADMAP.md`) § 5.5 의 PSB-3 sketch.
> **위치**: 실제 PSB-3 round 1 은 Sprint 3 (M13 C-SHADER-WARM++ + M14 C-CORRELATE) 측정 **후** 진행. 이 문서는 *sketch* — 미리 가능한 후보 brainstorm + 만점 채점 + 우선순위.
> **상위**: [CLOSURE_ROADMAP.md](CLOSURE_ROADMAP.md), [BRAINSTORM.md](BRAINSTORM.md), [SCORING.md](SCORING.md), [PERF_PLAN.md](PERF_PLAN.md).
> **이전**: [POST_SPRINT_1_BRAINSTORM_SKETCH.md](POST_SPRINT_1_BRAINSTORM_SKETCH.md) (만점 14 → 18), [POST_SPRINT_2_BRAINSTORM_SKETCH.md](POST_SPRINT_2_BRAINSTORM_SKETCH.md) (만점 18 → 23 가정).
> **input 가정**: M14 C-CORRELATE 가 "동시 발생" label 상관 관계 (2 라벨 동시 발생 빈도) 산출 → 본 sketch 는 그것을 *순서/인과* + *자동 행동* 으로 한 단계 전진.

---

## 0. 채점 기준 (재확인)

| axis | 정의 |
|---|---|
| **A. Apple-only stack** (/5) | hexa PE loader + macOS syscalls + Apple D3DMetal + Rosetta 2 + DIY Win32 shim. Wine/CrossOver/community 0 줄 |
| **B. Runtime** (/5) | 게임 플레이 *중* 효과 (launch/install 만 X) |
| **C. 데이터재해석** (/5) | raw 데이터를 cache/filter/forecast-ready 로 reproject |

→ 만점 = A+B+C 각 5/5 = **15/15**. 본 sketch 의 PSB-3 후보는 만점 15 또는 14 (B=4 launch 위주) 만 게재.

---

## 1. PSB-3 round 3 신규 만점 후보 sketch

### 1.1 C-CASCADE-LABEL ⭐ (15/15) — 원인-결과 사슬 추적

**한 줄 정의** + **비유**:
2 라벨 *동시* 발생 (M14) 을 넘어, *순서* 까지 추적 — 시간 거꾸로 인과 사슬 복원.
*비유*: 사고 발생 → 에어백 → 충돌 → 브레이크 늦음 → 시야 가림 — 시간 추적. 사고 직후 *역방향* 으로 원인 사슬을 따라 올라간다.

**시나리오**:
- D4 boss fight stutter 측정 → 100ms 전 PSO compile spike → 50ms 전 heap pressure → 0ms stutter 발생
- M14 가 (PSO compile, heap pressure, stutter) 셋이 *동시* 라고 알려준 후, CASCADE-LABEL 은 *순서* 와 *지연 시간* 까지 부여 → 사슬 ID = `cascade_pso_heap_stutter_v1`
- D2R 던전 진입 → 200ms 전 zone load → 100ms 전 PSO chain → 50ms 전 monster spawn → 0ms 첫 frame stall

**무기** + **효과**:
- label fusion (M14 결과 재사용) + temporal sliding window (W=200ms) + causal chain detection (lag 분포 검사)
- 효과: M14 가 "동시 발생" 만 알려준다면, 본 컴포넌트는 *순서까지* 추적 → 진짜 *근원* 라벨 식별 가능

**만점 채점**:
| axis | 점수 | 근거 |
|---|---|---|
| A | 5 | hexa PE 위 D3DMetal label + 자체 시계 |
| B | 5 | runtime — 매 stutter event 마다 사슬 복원 |
| C | 5 | label time-series → causal graph 로 reproject |
| **/15** | **15** ⭐ | |

**의존 phase / 모듈**:
- M14 C-CORRELATE (이미 완료 — Sprint 3 결과)
- 새 모듈: `lib/perf/c_cascade_label.hexa`

**작업 시간**: ~4-5 일 (M14 자산 재사용 + temporal lag detection 신규 구현).

---

### 1.2 C-REMEDIATE-PLAN ⭐ (15/15) — 라벨 보면 자동 행동

**한 줄 정의** + **비유**:
CASCADE-LABEL 이 사슬을 식별하면 *자동* 으로 다중 컴포넌트를 orchestrate — airgenome 이 게임을 자율 운영.
*비유*: 화재경보 발동 → 자동으로 (1) 스프링클러 (2) 엘리베이터 정지 (3) 비상문 개방 — 사람 개입 없음, rule engine 이 동시 다발 행동.

**시나리오**:
- C-CASCADE-LABEL 이 "PSO compile cascade" 사슬 detect → 자동 행동:
  1. **C-PSO-CHAIN 활성화** — 다음 PSO 미리 컴파일 (도미노 예측 강제 발동)
  2. **C-HEAP-DEFRAG 트리거** — 메모리 압박 대비 (선행 청소)
  3. **Metal queue priority 일시 상승** — 5초 limited boost
  4. **5초 후 normal mode 복귀** — auto-revert
- D2R: "zone-entry stall cascade" detect → C-PSO-CHAIN + C-DXIL-PERSIST 우선순위 boost + C-INPUT-LATENCY 일시 deprioritize

**무기** + **효과**:
- rule engine (label trigger 매핑) + label trigger watcher + multi-component orchestration RPC
- 효과: airgenome 이 게임을 *자율 운영*. 사람이 profile 보고 손으로 끌 필요 없이, 라벨이 행동을 *부른다*.

**만점 채점**:
| axis | 점수 | 근거 |
|---|---|---|
| A | 5 | 모든 trigger 가 hexa+D3DMetal 자체 자산 |
| B | 5 | runtime — stutter 발생 5초 안 자동 행동 완료 |
| C | 5 | label → action plan 으로 reproject |
| **/15** | **15** ⭐ | |

**의존 phase / 모듈**:
- D-list 18 만점 (C-PSO-CHAIN, C-HEAP-DEFRAG, C-DXIL-PERSIST, ...) 모두 완료
- CN-7/8 (label fusion infra)
- C-CASCADE-LABEL (선행 1.1)
- 새 모듈: `lib/perf/c_remediate_plan.hexa` + `lib/perf/rule_engine.hexa`

**작업 시간**: ~5-7 일 (rule engine 신규 + 모든 D-list 컴포넌트 RPC 통합).

---

### 1.3 C-AGI-INTEGRATION ⭐ (15/15) NEW — gamebox forge ring → airgenome MAIN

**한 줄 정의** + **비유**:
gamebox 의 forge ring (D3DMetal 측정 데이터) 을 airgenome MAIN harvest 트랙에 직접 합류 — 게임이 곧 AGI ops 의 새 워크로드.
*비유*: 게임 플레이 데이터 = AGI 학습 cost telemetry 의 한 워크로드. gamebox closure 안에 갇히지 않고, airgenome 의 60-byte 게놈 본체에 기여.

**시나리오**:
- D4 측정 데이터 (PSO compile time, heap pressure, stutter event, label cascade) → airgenome `modules/harvest.hexa` 수집
- airgenome `modules/forecast.hexa` 가 *AGI 학습 cost* 와 *게임 stutter* 를 비교 분석:
  - "AGI 학습 batch 256 일 때 cost spike pattern" 과 "D4 boss fight PSO cascade" 가 *동형* (구조 동일) → 두 도메인 cross-learning
- gamebox 와 airgenome MAIN 이 60-byte 게놈 통합 — gamebox label 이 airgenome label space 에 정합 등록

**무기** + **효과**:
- `harvest_integ` 확장 (D3DMetal binary 직접 capture)
- airgenome MAIN 트랙 RPC 연결 (cross-process channel)
- 60-byte 게놈 통합 (gamebox label → airgenome genome slot 할당)
- 효과: gamebox 가 "gamebox 안 closure" 외에도 "airgenome MAIN closure" 에도 기여. 정체성 정합 — gamebox 는 airgenome 의 한 sub-genome.

**만점 채점**:
| axis | 점수 | 근거 |
|---|---|---|
| A | 5 | D3DMetal binary 직접 capture, hexa PE 위 |
| B | 5 | 게임 플레이 *중* 데이터 harvest (offline X) |
| C | 5 | airgenome 정체성 정합 — gamebox label → genome reproject |
| **/15** | **15** ⭐ | |

**의존 phase / 모듈**:
- airgenome `modules/harvest.hexa` (외부 의존 — airgenome repo)
- airgenome `modules/forecast.hexa` (외부 의존)
- gamebox 측 `lib/perf/c_agi_integration.hexa` 신규
- M14 C-CORRELATE label 정합 (선행 done)

**작업 시간**: ~3-5 일 (airgenome RPC channel + label space mapping; airgenome 측 repo 변경 필요 시 +2 일).

---

### 1.4 C-USER-SOLO-PROFILE (보조 14/15 — B=4) — 개인 PSO/heap 학습

**한 줄 정의** + **비유**:
사용자 *개인* 의 PSO/heap 패턴 학습 → personal optimization profile 자동 생성.
*비유*: Spotify 의 "올해의 음악" 같은 personal recap — 단, 매일 PSO/heap 패턴을 학습해서 다음 launch 시 *그 사람* 만의 pre-warm 사용.

**시나리오**:
- 사용자 A: D2R Sorceress + Frozen Orb 메타 → 특정 PSO sub-cluster 만 hot
- 사용자 B: D2R Barbarian + Whirlwind → 다른 PSO sub-cluster
- 두 사용자 같은 게임 같은 zone 이라도 launch 후 첫 30초 pre-warm 셋이 다름 → personal profile

**만점 채점**:
| axis | 점수 | 근거 |
|---|---|---|
| A | 5 | 자체 자산 |
| B | 4 | mostly launch 시점 효과 (runtime 영향은 부차) |
| C | 5 | personal histogram → profile reproject |
| **/15** | **14** | B=4 |

**작업 시간**: ~3 일.

---

## 2. 종료 조건 추적

### 2.1 PSB-3 종료 조건

| 조건 | 기준 |
|---|---|
| **새 만점 N 개 발견** | round 1 + round 2 + round 3 합쳐 만점 ≥1 → sprint 진행 |
| **만점 0** | EXHAUSTION 카운터 +1 |
| **time-cap 1 주** | brainstorm 자체 1 주 제한 |

### 2.2 PSB-N 누적 EXHAUSTION 추적

| sprint | 새 만점 | EXHAUSTION 카운터 | 비고 |
|---|---|---|---|
| PSB-1 | 4 (sketch) | reset = 0 | C-PSO-CHAIN, C-HEAP-DEFRAG, C-INPUT-LATENCY, C-GPU-HUD-RING |
| PSB-2 | 5 (가정) | reset = 0 | (PSB-2 sketch 결과 가정) |
| **PSB-3** | **3** (본 sketch — 만점 확정) | reset = 0 | C-CASCADE-LABEL, C-REMEDIATE-PLAN, C-AGI-INTEGRATION |
| PSB-4 | ? | 0 일 시 +1 | D4 첫 frame 측정 input |
| PSB-5 | ? | 0 일 시 +2 | |
| PSB-6 (3 연속 0) | 0 | **EXHAUSTION 발동** | docs/EXHAUSTION_<date>.md |

### 2.3 누적 만점 추적

```
초기:        14 (closure roadmap)
BG cycle 6:  18 (own1-driven 4)
PSB-1:       18 → 22 (+4 sketch)        ※ 이전 sketch
PSB-2:       22 → 23 (+5 가정 — 1 중복 제외)  ※ 이전 sketch (sketch 가정)
PSB-3:       23 → 26 (+3 본 sketch 만점 확정)
PSB-4:       26 → ?+ (D4 input)
…
EXHAUSTION:  PSB-N 에서 새 만점 0 × 3 sprint 연속
```

### 2.4 본 sketch 의 즉시 효과

본 sketch 의 3 만점 후보 (C-CASCADE-LABEL, C-REMEDIATE-PLAN, C-AGI-INTEGRATION) + 1 보조 14 (C-USER-SOLO-PROFILE) → PSB-3 round 시점 *최소* 새 만점 3 보장. EXHAUSTION 까지 거리 +1 sprint 추가 확보.

---

## 3. 우선순위 배치

### 3.1 sprint 매핑

| sprint | 컴포넌트 | 시점 | 비고 |
|---|---|---|---|
| **S7** | **C-CASCADE-LABEL** (15/15) | PSB-3 채택 후 | M14 위에 temporal layer — 자산 재사용 |
| **S8** | **C-REMEDIATE-PLAN** (15/15) | S7 후 | CASCADE 후속 — rule engine 신규 |
| **S9** | **C-AGI-INTEGRATION** (15/15) | S8 후 (또는 병렬) | airgenome MAIN 합류 — gamebox 정체성 정합 |
| **S9.5** | C-USER-SOLO-PROFILE (14/15) | S9 후 또는 backlog | 보조 — B=4 |

### 3.2 우선순위 근거

1. **C-CASCADE-LABEL first** — M14 자산 가장 직접 재사용. 4-5 일 작업으로 가장 빠른 만점 추가. 후속 C-REMEDIATE-PLAN 의 *전제* 라 우선 배치.
2. **C-REMEDIATE-PLAN second** — CASCADE 결과를 *행동* 으로 연결 — airgenome 자율 운영 정체성 핵심. 단, 모든 D-list 18 만점 + CN-7/8 + CASCADE-LABEL 의존 → 마지막에 배치 자연.
3. **C-AGI-INTEGRATION third (또는 S8 와 병렬)** — gamebox closure 외부 의존 (airgenome repo) 이라 schedule 조정 필요. S8 와 병렬 가능 (rule engine ↔ airgenome harvest channel 독립).
4. **C-USER-SOLO-PROFILE backlog** — 14/15 라 만점 우선. EXHAUSTION 카운터 reset 용 보조 후보.

### 3.3 estimate + closure 영향

| 후보 | 작업일 | closure ETA 영향 | 누적 만점 |
|---|---|---|---|
| C-CASCADE-LABEL | 4-5 일 | +0.15 달 | 23 → **24** |
| C-REMEDIATE-PLAN | 5-7 일 | +0.2 달 | 24 → **25** |
| C-AGI-INTEGRATION | 3-5 일 | +0.15 달 (S8 병렬 시 0.05) | 25 → **26** |
| C-USER-SOLO-PROFILE | 3 일 | +0.1 달 | 26 → 27 (선택) |
| **합계 (만점 3)** | **12-17 일** | **~+0.4-0.5 달** | **만점 23 → 26** |

→ closure 만점 23 → **만점 26** 으로 격상 (+3). closure ETA: ~21.4 + 0.5 ≈ **~21.9 달** (PSB-1 0.4 + PSB-2 가정 0.4 + PSB-3 0.5).

### 3.4 PSB-3 sketch 채택 안 시 fallback

- 만점 23 (PSB-2 까지 누적) 그대로 closure 가능
- PSB-3 round 1+2+3 에서 추가 만점 발견 시 EXHAUSTION 카운터 reset

---

## 4. round 정의 (운영 약속)

| round | 입력 | 출력 | time-cap |
|---|---|---|---|
| **PSB-3 round 1** | M13 C-SHADER-WARM++ 측정 결과 + 본 sketch 후보 | 채택된 만점 컴포넌트 list | 3 일 |
| **PSB-3 round 2** | round 1 + M14 C-CORRELATE label fusion 결과 | 추가 만점 후보 (sketch 의 CASCADE/REMEDIATE 검증) | 3 일 |
| **PSB-3 round 3** | round 1+2 + airgenome MAIN repo 동기 (AGI-INTEGRATION 검증) | 최종 만점 채택 + sprint S7-S9 schedule | 2 일 |

→ **PSB-3 합계**: 1 주 (8 일).

---

## 5. closure 영향

### 5.1 진행률

```
closure 진행률: ~70% (PSB-2 후) → ~85% (PSB-3 후, 본 sketch 채택 가정)
남은:           D4 첫 frame 측정 (CM-25, +18 달) + EXHAUSTION 인정 (PSB-N 3 연속 0) + airgenome MAIN integration 정합
```

### 5.2 만점 추가 trajectory

```
18 (현재 docs / BG cycle 6 후)
→ 20 (BG cycle 6 본격 운영)
→ 23 (PSB-2 sketch 채택 후)
→ 26 (PSB-3 sketch 채택 후)
→ ?+ (PSB-4 D4 first frame input)
→ ?+ (PSB-N until 고갈 → EXHAUSTION)
```

### 5.3 정체성 정합

본 sketch 의 C-AGI-INTEGRATION (S9) 채택 시, gamebox 는 더 이상 "독립 closure" 가 아니라 *airgenome MAIN closure 의 sub-genome* 으로 재정의 — 이는 closure roadmap 의 § 1 IDENTITY 정합과 일치. PSB-3 가 정체성 합류 시점.

---

## 6. 참고 — sketch 단계의 한계 + 정직성

본 sketch 는 *예측*. 실제 PSB-3 은 Sprint 3 (M13 + M14) 측정 후 진행하므로 본 3 후보가:
- (a) **만점 유지** — A=5 가정은 D3DMetal binary capture + airgenome RPC 가 모두 자체 자산일 때만. airgenome MAIN repo 가 외부 community 의존을 끌어오면 A=4 로 떨어질 위험
- (b) **더 좋은 후보 발견** — M14 결과의 실측 label correlation 분포가 sketch 가정과 다르면 round 2 에서 우선순위 재조정
- (c) **의존 sprint 변동** — Sprint 3 일정 지연 시 본 sketch 의 S7-S9 도 같이 지연
- (d) **AGI-INTEGRATION 외부 의존 위험** — airgenome repo 측 변경이 필요할 경우, gamebox closure 단독으로는 닫히지 않음 → 정체성 합류는 이점이지만 closure 종료 시점은 airgenome MAIN 정합에 종속

→ 본 sketch 는 PSB-3 *진입점* 만 보장. 실제 채택은 측정 후 재검증.

---

## 7. 다음

- Sprint 3 (M13 + M14) 완료 → 본 sketch 의 가정 데이터 (M14 correlation 결과) 검증
- PSB-3 round 1 진입 → 본 sketch 의 3 만점 후보 + round 2/3 신규 후보 통합 채택
- S7-S9 sprint 진행 → 누적 만점 26 도달
- PSB-4 sketch 작성 (D4 첫 frame 측정 input 가정)
