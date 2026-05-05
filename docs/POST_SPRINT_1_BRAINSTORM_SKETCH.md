# PSB-1 Sketch — Post-Sprint-1 Brainstorm (pre-emptive paradigm discovery)

> **scope**: closure roadmap (`docs/CLOSURE_ROADMAP.md`) § 5.5 의 PSB-1 sketch.
> **위치**: 실제 PSB-1 round 1 은 M8 sprint 측정 (CM-22, +15 달) **후** 진행. 이 문서는 *sketch* — 미리 가능한 후보 brainstorm + 만점 채점 + 우선순위.
> **상위**: [CLOSURE_ROADMAP.md](CLOSURE_ROADMAP.md), [BRAINSTORM.md](BRAINSTORM.md), [SCORING.md](SCORING.md), [PERF_PLAN.md](PERF_PLAN.md).
> **이전**: BRAINSTORM.md round 1 = 만점 16 채택 (D 12 + R 2 + own1-driven 4 중 D 4) — 본 sketch 는 *그 외 추가 후보*.

---

## 0. 채점 기준 (재확인)

| axis | 정의 |
|---|---|
| **A. Apple-only stack** (/5) | hexa PE loader + macOS syscalls + Apple D3DMetal + Rosetta 2 + DIY Win32 shim. Wine/CrossOver/community 0 줄 |
| **B. Runtime** (/5) | 게임 플레이 *중* 효과 (launch/install 만 X) |
| **C. 데이터재해석** (/5) | raw 데이터를 cache/filter/forecast-ready 로 reproject |

→ 만점 = A+B+C 각 5/5 = **15/15**. 본 sketch 의 PSB-1 후보는 모두 만점 15 또는 14 (A=4) 만 게재.

---

## 1. PSB-1 round 1 신규 만점 후보 sketch

### 1.1 C-PSO-CHAIN ⭐ (15/15) — PSO 도미노 예측

**한 줄 정의** + **비유**:
PSO state 가 다음 frame 에 어떤 PSO 로 *전이* 할지를 sequence 학습으로 예측 → pre-warm.
*비유*: 도미노 — 첫 PSO 가 set 되면 다음 N 개 PSO 까지 한 줄로 같이 set 되는 패턴이 게임마다 고정. 첫 도미노가 보이면 뒤 N 개 미리 준비.

**시나리오**:
- D2R: town → act 던전 진입 시 PSO 그룹 (skill VFX + monster shader + boss 도입) 이 한 묶음으로 등장. M8 sprint 1 측정 후 transition 이 sequence 화 가능 보일 것.
- D4: open-world zone boundary 통과 시 PSO chain 이 길게 (~20-50 PSO) 발생 → 첫 boss fight 진입 silky 효과.

**무기** + **효과**:
- AC trie (PSO hash sequence pattern 매칭) + Holt (다음 chain 길이 예측) + result_cache (예측된 PSO pre-compile 결과)
- 효과: zone-entry hitch 제거 + 첫 boss 첫 frame stutter -50%↑

**만점 채점**:
| axis | 점수 | 근거 |
|---|---|---|
| A | 5 | D3DMetal PSO 캡처 + hexa PE 위에서 동작 |
| B | 5 | runtime 매 frame 효과 |
| C | 5 | sequence trie + forecast 로 reproject |
| **/15** | **15** ⭐ | |

**의존 phase / 모듈**:
- Phase 5 (D3DMetal PSO 캡처) + M4 C-PSO 의 8-axis state vector 흡수
- 새 모듈: `lib/perf/c_pso_chain.hexa`

**작업 시간**: ~3 일 (M4 구조 재사용, AC trie 자산 재사용)

---

### 1.2 C-HEAP-DEFRAG ⭐ (15/15) — 메모리 자동 청소부

**한 줄 정의** + **비유**:
heap fragmentation 패턴을 학습 → 빈 슬롯 통합 시점을 예측 + game idle window 에 defrag 자동 trigger.
*비유*: 청소부가 손님 적은 시간에 자리 정리 — 게임이 한가한 frame (cinematic, town 정지) 을 forecast 해서 그 때 defrag.

**시나리오**:
- D2R: town stash 정리 직후 / cutscene 직전 (FPS 60 안정 확인) → defrag 안전 window
- D4: world boss between-wave (8-15 sec idle) → defrag window 자주 발생
- 30+ 분 세션 진행 시 fragmentation 누적으로 mid-game stall (50-500ms) 발생 → 미연 방지

**무기** + **효과**:
- state vector (heap 5-axis) + Holt (idle window forecast) + label rules (defrag trigger 조건)
- 효과: long session stall 제거, P99 frame -20%↑, mid-game stutter -40%↑

**만점 채점**:
| axis | 점수 | 근거 |
|---|---|---|
| A | 5 | D3DMetal heap + macOS allocator |
| B | 5 | runtime, 30+ 분 세션 핵심 |
| C | 5 | state vector + forecast → idle window reproject |
| **/15** | **15** ⭐ | |

**의존 phase / 모듈**:
- Phase 5 (D3DMetal heap 접근) + M3 C-HEAP 의 5-axis state 흡수
- 새 모듈: `lib/perf/c_heap_defrag.hexa`

**작업 시간**: ~4 일 (M3 위에 forecast layer + trigger 로직)

---

### 1.3 C-INPUT-LATENCY ⭐ (15/15) — 입력→화면 반응 지연 추적

**한 줄 정의** + **비유**:
키/마우스 입력 timestamp → 다음 화면 update timestamp 까지의 지연을 ring 으로 누적 + 비정상 spike 분류.
*비유*: 우편배송 추적번호 — 발송 (입력) ~ 도착 (frame display) 모든 구간 별 지연을 수집해서 어디서 지체됐나 식별.

**시나리오**:
- D2R: PvP duel — input lag spike = 패배. 3-frame (~50ms) spike 가 dueling 만족도 핵심.
- D4: PvP world zone (Fields of Hatred) 즉시반응. boss mechanic dodge 도 입력지연 critical (>2 frame = 죽음).
- M8 측정 후 input→present 의 unexplained variance 가 보일 것 (CPU/GPU/Metal driver/HID stack 어디서 손해?)

**무기** + **효과**:
- columnar (input event stream) + label classifier (4 lag class: HID/CPU/GPU/present) + Holt (lag 추세 forecast)
- 효과: D4 PvP 만족도 지표화 + lag 원인 분류 + driver tuning hint

**만점 채점**:
| axis | 점수 | 근거 |
|---|---|---|
| A | 5 | macOS HID + D3DMetal present + DIY shim 입력 hook |
| B | 5 | runtime, PvP 핵심 |
| C | 5 | columnar + classifier + forecast |
| **/15** | **15** ⭐ | |

**의존 phase / 모듈**:
- Phase 4a (user32 input hook) + Phase 5 (D3DMetal present timestamp)
- 새 모듈: `lib/perf/c_input_latency.hexa`

**작업 시간**: ~3 일

---

### 1.4 C-GPU-HUD-RING ⭐ (15/15) — MTL_HUD 모든 metric 통합 ring

**한 줄 정의** + **비유**:
Apple `MTL_HUD_ENABLED=1` 의 모든 metric (FPS, GPU%, memory, command buffer, drawable wait) 을 하나의 통합 ring schema 로 흡수 → harvest 로 흘려보냄.
*비유*: 차량 OBD-II — 별도 게이지로 흩어진 모든 signal 을 표준 schema 로 단일 stream 화.

**시나리오**:
- D2R/D4 모두 — observability infrastructure 개선 (현재 M2/M3/M4 등은 *각자* 자기 ring schema 사용해 cross-component 비교 어려움). 이 컴포넌트가 *통합 layer*.
- M14 C-CORRELATE 의 입력 품질을 즉시 향상.

**무기** + **효과**:
- harvest ring (variable record) + columnar (multi-metric time series) + AC trie (HUD line parser)
- 효과: 모든 만점 컴포넌트의 입력 quality 향상, M14 correlation accuracy +10%↑, 디버그 시간 -50%↑

**만점 채점**:
| axis | 점수 | 근거 |
|---|---|---|
| A | 5 | Apple MTL_HUD = D3DMetal/Metal native |
| B | 5 | runtime, 모든 측정의 기반 |
| C | 5 | multi-metric → 통합 ring schema reproject |
| **/15** | **15** ⭐ | |

**의존 phase / 모듈**:
- Phase 5 (D3DMetal + MTL_HUD 활성)
- 새 모듈: `lib/perf/c_gpu_hud_ring.hexa`

**작업 시간**: ~2 일 (harvest ring 자산 재사용, parser 만 신규)

---

### 1.5 C-AUDIO-DSP (14/15, A=4) — 오디오 처리 비용 추적

**한 줄 정의** + **비유**:
DirectSound/XAudio2 → CoreAudio 변환 + DSP (reverb, mix bus) 의 CPU 비용을 ring 으로 추적.
*비유*: 백그라운드 음악 라디오 — 무대 뒤에서 조용히 돌아가지만 가끔 dial 돌리면 noise spike.

**시나리오**:
- D2R: 100+ 동시 spell sound + monster grunt → DSP CPU spike → frame budget 침해
- D4: spatial audio (Atmos 호환?) 처리 비용 → 특히 dense town hub
- audio glitch 가 frame stutter 와 *상관* 가능 (CPU contention)

**무기** + **효과**:
- columnar + label rules (spike 분류) + Holt (DSP 부하 forecast)
- 효과: audio-induced frame variance 분리 + tuning

**만점 채점**:
| axis | 점수 | 근거 |
|---|---|---|
| A | **4** | XAudio2/DirectSound shim + CoreAudio = Apple-stack 부분 일치이지만, audio 가 D3DMetal/Rosetta 핵심 axis 에서 약간 외곽 |
| B | 5 | runtime |
| C | 5 | ring + classifier + forecast |
| **/15** | **14** | A=4 로 만점 미달 — R-list 만점 (10/10) 으로는 통과 |

**의존 phase / 모듈**:
- Phase 4d (audio shim, 미정 phase, Phase 4 이후)
- 새 모듈: `lib/perf/c_audio_dsp.hexa`

**작업 시간**: ~3 일 (audio shim phase 의존)

**verdict**: 본 sketch 만점 후보 4 와 별도 — R-list 흡수 또는 보조 14 그룹.

---

## 2. round 2 발견 가능성 (M8 측정 input 후)

### 2.1 correlation analysis (M14) 결과 패턴

M14 C-CORRELATE 가 multi-anomaly fusion 을 ≥80% accuracy 로 분류하면, 분류된 anomaly *cluster* 마다 새 컴포넌트 후보 발생:
- "PSO + heap" 동시 spike → C-PSO-HEAP-COREL (둘이 cluster 만드는 frame 분류기)
- "shader compile + texture eviction" 연쇄 → C-SHADER-TEX-CASCADE (eviction 직후 shader 컴파일 chain 패턴)
- "rosetta hot loop + frame variance" 상관 → C-ROSETTA-FRAME-LINK (Rosetta 번역 burst 가 frame variance 만드는 패턴)

→ **PSB-1 round 2 추정 후보 수**: 3-5 개, 만점 가능성 60-80%.

### 2.2 sprint 1 의 unexplained anomaly

M8 sprint 1 (P99 frame -30%↑ + stutter -50%↑) 측정 후, 만점 14 컴포넌트로 *설명 안 되는* anomaly cluster 가 잔재할 것:
- 일부 stutter 가 PSO/HEAP/SHADER 어디에도 attribute 안 됨 → 새 origin 추정 (e.g. swapchain wait, drawable starvation, present queue contention)
- 새 후보: C-DRAWABLE-WAIT (swapchain present 대기 패턴 학습), C-CMDBUF-COMMIT-LATENCY (Metal commit→done 지연 분포)

→ **PSB-1 round 2 추정 후보 수**: 2-4 개.

### 2.3 D4 측정 시 발견될 가능성 큰 영역

CM-25 (D4 첫 frame, +18 달) 후 PSB-4 round 에서 발견 가능성 크지만, sketch 단계에서 미리 후보 zone 식별:
- DXR (ray tracing) 가속 구조 (BLAS/TLAS) 캐시 → C-DXR-AS-CACHE
- D3D12 root signature 변경 cost → C-RS-CHANGE-PROFILE
- ExecuteIndirect command list 패턴 → C-CMDLIST-INDIRECT-PROFILE
- DirectStorage I/O (D4 미지원이지만 향후?) → 후보로 보류

→ **PSB-4 round 1 추정 후보 수**: 4-6 개 (D4-input 강함). PSB-1 sketch 는 D2R 중심이므로 본 절은 PSB-4 reservation.

---

## 3. 종료 조건 추적

### 3.1 PSB-1 종료 조건

| 조건 | 기준 |
|---|---|
| **새 만점 N 개 발견** | round 1 + round 2 합쳐 만점 ≥1 → sprint 진행 |
| **만점 0 + 사양 X** | 기존 만점 14 가 D2R 만족도 100% → PSB-1 종료, PSB-2 로 |
| **time-cap 1 주** | brainstorm 자체 1 주 제한, 만점 채택 시 별도 sprint |

### 3.2 PSB-1+ 누적 EXHAUSTION 조건 (closure 종료)

> **공식 정의**: PSB-N 에서 새 만점 0 × **3 sprint 연속** = EXHAUSTION → docs/EXHAUSTION_<date>.md 작성 + closure.

| sprint | 새 만점 | 누적 |
|---|---|---|
| PSB-1 | ? | reset |
| PSB-2 | ? | 0 일 시 +1 |
| PSB-3 | ? | 0 일 시 +2 |
| PSB-N (3 연속 0) | 0 | **EXHAUSTION 발동** |

### 3.3 본 sketch 의 즉시 효과

본 sketch 의 4 만점 후보 (C-PSO-CHAIN, C-HEAP-DEFRAG, C-INPUT-LATENCY, C-GPU-HUD-RING) 가 PSB-1 round 1 직전 발견된 셈 → PSB-1 round 1 시점에 *최소* 새 만점 4 보장. EXHAUSTION 까지 거리 +1 sprint.

---

## 4. 우선순위 배치

### 4.1 sprint 매핑

| sprint | 컴포넌트 | 시점 | 비고 |
|---|---|---|---|
| **S2** | R1 (C-DXIL-PERSIST) | CM-22 직후 | 이미 closure roadmap |
| **S3** | **C-PSO-CHAIN** (15/15) | PSB-1 채택 후 | first to implement — 첫 boss fight silky |
| **S4** | **C-HEAP-DEFRAG** (15/15) | S3 후 | second — long session stall killer |
| **S5** | **C-INPUT-LATENCY** (15/15) | S4 후 | D4 PvP 만족도 (D4 phase 후 효과 max) |
| **S6** | **C-GPU-HUD-RING** (15/15) | S5 후 또는 병렬 | observability infra — 다른 sprint 의 입력 quality ↑ |

### 4.2 우선순위 근거

1. **C-PSO-CHAIN first** — 첫 boss fight silky 효과는 사용자 *체감* 즉시. M4 C-PSO 위에 sequence layer 만 추가, 기 자산 재사용도. ROI 가장 큼.
2. **C-HEAP-DEFRAG second** — long session (30+분) stall 은 D2R 의 farming/grinding loop 핵심. M3 C-HEAP 위에 forecast + trigger.
3. **C-INPUT-LATENCY third** — D4 PvP 핵심이지만 D4 phase (CM-25, +18 달) 도달 전엔 효과 측정 어려움. D2R dueling 으로 약식 검증 가능.
4. **C-GPU-HUD-RING fourth** (또는 병렬) — observability infrastructure 라 다른 만점 component 의 입력 quality 를 향상. 단독 사용자 체감은 낮으나 누적 효과 큼. S3-S5 와 병렬 진행 가능.

### 4.3 estimate + closure 영향

| 후보 | 작업일 | closure ETA 영향 | 누적 만점 |
|---|---|---|---|
| C-PSO-CHAIN | 3 일 | +0.1 달 | 14 → **15** |
| C-HEAP-DEFRAG | 4 일 | +0.15 달 | 15 → **16** |
| C-INPUT-LATENCY | 3 일 | +0.1 달 | 16 → **17** |
| C-GPU-HUD-RING | 2 일 | +0.07 달 (병렬 시 0) | 17 → **18** |
| **합계** | **12 일** | **~+0.4 달** | **만점 14 → 18** |

→ closure 만점 14 → **만점 18** 로 격상 (+4). closure ETA: 21 달 → ~21.4 달 (D-list 만점 11 → 15 + R-list 만점 2 + own1-driven 4 = 21 — 단, own1-driven 4 는 이미 합계 14 안에 포함이라 net 만점 18 = 8(D 기존) + 2(R) + 4(own1) + 4(PSB-1 신규) ).

### 4.4 PSB-1 sketch 채택 안 시 fallback

- 만점 16 (D 12 + R 2 + own1-driven 4 중 D 4 — 기존 만점 14 가정 + 본 sketch 4 채택 시 18) 그대로 closure
- PSB-1 round 1 + round 2 에서 추가 만점 발견 시 EXHAUSTION 카운터 reset 1 회 가능

---

## 5. round 정의 (운영 약속)

| round | 입력 | 출력 | time-cap |
|---|---|---|---|
| **PSB-1 round 1** | M8 sprint 1 측정 결과 + 본 sketch 4 후보 | 채택된 만점 컴포넌트 list + sprint 매핑 | 3 일 |
| **PSB-1 round 2** | round 1 + correlation analysis (M14) 결과 + sprint 1 unexplained anomaly | 추가 만점 후보 | 4 일 |
| **PSB-1 종합** | round 1+2 채택 list | sprint S3-S6 schedule + closure ETA 갱신 | 1 일 |

→ **PSB-1 합계**: 1 주 (8 일).

---

## 6. 참고 — sketch 단계의 한계 + 정직성

본 sketch 는 *예측*. 실제 PSB-1 은 M8 측정 후 진행하므로 본 4 후보가:
- (a) 만점 유지 — A axis (Apple-only) 5 점 가정은 PE 로더 + Win32 shim phase 진행 중 변동 가능 (e.g. audio phase 가 generic 으로 흐를 시 A=4)
- (b) 더 좋은 후보 발견 — round 2 에서 본 4 보다 만점/효과 큰 후보 발견 시 우선순위 재조정
- (c) 의존 phase 변동 — Phase 5 일정 지연 시 sketch 의 sprint S3-S6 도 같이 지연

→ 본 sketch 는 PSB-1 *진입점* 만 보장. 실제 채택은 측정 후 재검증.

---

## 7. 다음

본 sketch 채택 시:
1. closure roadmap (`docs/CLOSURE_ROADMAP.md`) § 5.5 → § 5.6 로 PSB-1 sketch 4 후보 reflect
2. SCORING.md 의 D-list 만점 표 갱신 (PSB-1 sketch 후보 4 reservation)
3. CM-22 (M8 sprint + PSB-1) 의 deliverable 에 본 sketch 명시적 input 으로 표시
4. sprint S3-S6 작업표 (별도 PSB_1_FULL.md, M8 측정 후 작성) reservation

---

*written 2026-05-01 (commit 0477ae1 기준, M8 측정 +15 달 전 sketch).*
*post: PSB-1 실측정은 CM-22 시점 별도 docs/POST_SPRINT_1_BRAINSTORM.md 로 작성.*
