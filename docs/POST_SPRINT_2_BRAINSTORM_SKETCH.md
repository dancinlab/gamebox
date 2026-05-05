# PSB-2 Sketch — Post-Sprint-2 Brainstorm (pre-emptive paradigm discovery)

> **scope**: closure roadmap (`docs/CLOSURE_ROADMAP.md`) § 5.5 의 PSB-2 sketch.
> **위치**: 실제 PSB-2 round 는 Sprint 2 (R1 C-SHADER-WARM + R2 C-DXIL-PERSIST) 측정 **후** 진행. 본 문서는 *sketch* — Sprint 2 측정 input 을 *가정* 하고 미리 가능한 후보 brainstorm + 만점 채점 + 우선순위.
> **상위**: [CLOSURE_ROADMAP.md](CLOSURE_ROADMAP.md), [BRAINSTORM.md](BRAINSTORM.md), [SCORING.md](SCORING.md), [PERF_PLAN.md](PERF_PLAN.md).
> **이전**: [POST_SPRINT_1_BRAINSTORM_SKETCH.md](POST_SPRINT_1_BRAINSTORM_SKETCH.md) — PSB-1 sketch 4 만점 후보 (C-PSO-CHAIN, C-HEAP-DEFRAG, C-INPUT-LATENCY, C-GPU-HUD-RING).

---

## 0. 채점 기준 (재확인)

| axis | 정의 |
|---|---|
| **A. Apple-only stack** (/5) | hexa PE loader + macOS syscalls + Apple D3DMetal + Rosetta 2 + DIY Win32 shim. Wine/CrossOver/community 0 줄 |
| **B. Runtime** (/5) | 게임 플레이 *중* 효과 (launch/install 만 X) — 단, launch-cold 후속/incremental 도 B=5 가능 (cold launch 직후 첫 frame seq) |
| **C. 데이터재해석** (/5) | raw 데이터를 cache/filter/forecast-ready 로 reproject |

→ 만점 = A+B+C 각 5/5 = **15/15**. 본 sketch 의 PSB-2 후보는 모두 만점 15 또는 14 (A=4) 만 게재.

---

## 1. PSB-2 round 신규 만점 후보 sketch

### 1.1 C-DXIL-DELTA ⭐ (15/15) — 패치 변경 shader 만 re-compile

**한 줄 정의** + **비유**:
게임 패치 후 *바뀐* shader 만 식별 → 재컴파일, 나머지는 기존 cache 그대로 재활용.
*비유*: 책 개정판 — 바뀐 챕터만 인쇄, 나머지 챕터는 1쇄 그대로. cache nuke (전체 재인쇄) 회피.

**시나리오**:
- D4 시즌 패치 (e.g. season 5 → season 6) → 5000 shader 중 200 개만 변경
- 기존 방식: cache 전체 invalidate → 첫 launch 5-10 분 stall (CM-22 측정 baseline)
- DELTA 방식: 200 개만 re-compile → 첫 launch +10-20 sec 만 비용
- D2R: hotfix (skill balance) 후 VFX shader 일부 변경 → DELTA 로 town 진입 즉시 silky

**무기** + **효과**:
- columnar diff (old hash vs new hash, axis = 8 PSO axis + shader bytecode hash) + result_cache (delta plan = "재사용 4800, 재컴파일 200")
- 효과: 패치 후 첫 launch stall **-90%↑**, season-cycle 만족도 max

**만점 채점**:
| axis | 점수 | 근거 |
|---|---|---|
| A | 5 | DXIL bytecode + D3DMetal cache = Apple stack native |
| B | 5 | cold launch 후속 (launch 직후 첫 frame seq), runtime 효과 |
| C | 5 | columnar diff + result_cache 로 reproject |
| **/15** | **15** ⭐ | |

**의존 phase / 모듈**:
- M9 C-DXIL-PERSIST (이미 done) — persist cache 위에 delta layer 추가
- 새 모듈: `lib/perf/c_dxil_delta.hexa`

**작업 시간**: ~3-4 일 (M9 구조 재사용, columnar diff 자산 재사용)

---

### 1.2 C-ZONE-PREFETCH-IO ⭐ (15/15) — zone 진입 직전 asset 파일 미리 읽기

**한 줄 정의** + **비유**:
zone transition 직전 (포털 클릭, 문 통과) 다음 zone 의 asset 파일을 disk 에서 page cache 로 미리 read.
*비유*: 다음 방 들어가기 전 책장에서 책을 1 페이지씩 펼쳐 둠 — 들어갔을 때 즉시 읽기 가능, page fault 0.

**시나리오**:
- D4 Kyovashad → Fractured Peaks 포털 클릭 → 200MB asset (zone mesh + texture + shader) 미리 read
- 포털 이동 (~3 sec animation) 동안 OS page cache 충전 → 도착 즉시 stutter 0
- D2R: act → act 진입 (waypoint), Mephisto Durance 진입 (boss zone) → boss 첫 frame instant
- 기존 방식: zone 진입 시 page fault burst → 1-3 sec stutter (CM-22 R1 측정 baseline)

**무기** + **효과**:
- trie (zone path patterns, e.g. `/kyovashad/portal_to_*` → 다음 zone candidate set) + result_cache (file path → page cache prefetch list)
- 효과: zone transition stutter **-70%↑**, R1 (C-SHADER-WARM) 단독 대비 +30% 효과

**만점 채점**:
| axis | 점수 | 근거 |
|---|---|---|
| A | 5 | macOS `posix_fadvise` / `mmap` + APFS — 100% Apple stack |
| B | 5 | runtime, zone transition 매 발생 효과 |
| C | 5 | trie + result_cache 로 zone path → prefetch list reproject |
| **/15** | **15** ⭐ | |

**의존 phase / 모듈**:
- M12 C-SHADER-SEQ (이미 done) — shader sequence 위에 IO prefetch layer 추가
- 새 모듈: `lib/perf/c_zone_prefetch_io.hexa`

**작업 시간**: ~4-5 일 (zone path trie 학습 + IO prefetch syscall 통합)

---

### 1.3 C-PSO-FAMILY-CLUSTER ⭐ (15/15) — 닮은 PSO 묶어 한꺼번에 컴파일

**한 줄 정의** + **비유**:
8-axis PSO state 가 6 axis 이상 동일한 PSO 들을 *family* 로 묶어 batch 컴파일.
*비유*: 비슷한 옷 5벌 (같은 원단/색깔/사이즈, 단추만 다름) — 1 회 재단으로 5벌 동시 만듦. 1 벌씩 5 회 재단 대비 80% 시간 절약.

**시나리오**:
- D4 마법사 fireball / fireball_critical / fireball_combat / fireball_combo / fireball_finale (8 axis 중 6 axis 같음, blend mode + depth state 만 다름) → family 1 개로 batch
- D4 town 진입 시 30+ NPC shader (humanoid 공통 axis 6 개) → 5 family × 6 PSO = 30 개를 5 batch 로
- D2R: skill VFX (cold/fire/lightning element 만 다른 동일 mesh) → family clustering 으로 PSO compile 횟수 -50%
- 기존 PSO compile 비용: 5000 PSO × 3-10ms = 15-50 sec 누적 → family 화 시 -30~50%

**무기** + **효과**:
- columnar (PSO 8-axis similarity matrix) + cluster (axis match score ≥ 0.75 → same family)
- 효과: PSO compile 횟수 **-30~50%**, 첫 boss 첫 frame stutter 추가 -20%, M4 C-PSO 위 multiplier

**만점 채점**:
| axis | 점수 | 근거 |
|---|---|---|
| A | 5 | D3DMetal PSO 캡처 + Apple stack |
| B | 5 | runtime 매 PSO compile 시 효과 |
| C | 5 | columnar similarity matrix + cluster 로 reproject |
| **/15** | **15** ⭐ | |

**의존 phase / 모듈**:
- M4 C-PSO (이미 done) + CN-7 C-PSO-CHAIN (이미 done) — 8-axis state vector 위에 cluster layer
- 새 모듈: `lib/perf/c_pso_family_cluster.hexa`

**작업 시간**: ~3-4 일 (M4 + CN-7 자산 재사용, similarity matrix 만 신규)

---

### 1.4 C-SHADER-CACHE-SHARED (14/15, A=4) — 사용자간 shader cache 공유 (커뮤니티 캐시)

**한 줄 정의** + **비유**:
같은 게임 / 같은 GPU / 같은 macOS 버전 사용자간 shader cache 를 P2P / CDN 으로 공유.
*비유*: 동네 도서관 — 첫 사용자가 책 1 권 사면 (compile) 모두가 빌려 읽음 (download). 첫 launch instant.

**시나리오**:
- 10K macOS D4 사용자가 zone shader cache 공유 → 새 사용자 첫 진입 (cold cache) 도 instant
- D2R/D4 모두 — Apple Silicon M1/M2/M3 별 + macOS 버전 별 cache pool 분리
- 기존: 신규 사용자 첫 launch 5-10 분 stall → SHARED 시 1 분 (download time only)
- launch ETA -80% (network 충분 가정)

**무기** + **효과**:
- columnar (cache fingerprint = GPU + OS + game build hash) + result_cache (CDN URL → local cache merge) + label rule (verified cache 만 수용)
- 효과: 신규 사용자 launch ETA -80%, community network effect

**만점 채점**:
| axis | 점수 | 근거 |
|---|---|---|
| A | **4** | shader cache 자체는 Apple stack 100%, 단 P2P/CDN 공유 layer (network) 가 Apple 외부 (HTTPS/IPFS 등) — A axis 1 점 감점 |
| B | 5 | launch + 첫 frame seq runtime 효과 |
| C | 5 | columnar + result_cache + label rule |
| **/15** | **14** | A=4 로 만점 미달 — R-list 만점 (10/10) 으로는 통과 |

**의존 phase / 모듈**:
- M9 C-DXIL-PERSIST (이미 done) + 새 network layer (Phase 미정)
- 새 모듈: `lib/perf/c_shader_cache_shared.hexa` + `lib/net/cache_share.hexa`

**작업 시간**: ~5 일+ (network layer + verification + Apple security review)

**verdict**: 본 sketch 만점 후보 3 와 별도 — R-list 흡수 또는 보조 14 그룹.

---

## 2. round 정의 운영 (PSB-2 input)

### 2.1 round 별 입력 → 후보 검증

| 후보 | Sprint 2 측정 input | 검증 항목 | 채택 결정 |
|---|---|---|---|
| **C-DXIL-DELTA** | M11 2nd launch 측정 (incremental delta 비율) | 2nd launch 시 변경 shader % (예: 패치 후 1-5%) | delta 비율 ≤10% 면 채택 (재컴파일 비용 vs 재사용 비용 ratio) |
| **C-ZONE-PREFETCH-IO** | R1 단독 vs R1+ZONE-PREFETCH 비교 | zone transition stutter 수치 차이 (sec) | R1+PREFETCH 가 R1 단독 대비 +20% 이상 개선 시 채택 |
| **C-PSO-FAMILY-CLUSTER** | M4/CN-7 PSO compile 횟수 측정 | 8-axis 6+ match family 빈도 (전체 PSO 의 % | family 화 가능 PSO ≥30% 면 채택 (-30% compile 효과 보장) |

### 2.2 round 운영

| round | 입력 | 출력 | time-cap |
|---|---|---|---|
| **PSB-2 round 1** | Sprint 2 측정 (M11 2nd launch + zone transition + PSO compile 분포) + 본 sketch 3 후보 | 채택된 만점 컴포넌트 list + sprint 매핑 | 3 일 |
| **PSB-2 round 2** | round 1 + Sprint 2 unexplained anomaly | 추가 만점 후보 발견 (가능성 30-50%) | 3 일 |
| **PSB-2 종합** | round 1+2 채택 list | sprint S4-S6 schedule + closure ETA 갱신 | 1 일 |

→ **PSB-2 합계**: 1 주 (7 일).

---

## 3. 종료 조건 추적

### 3.1 PSB-2 누적 만점

| sketch | 신규 만점 | 누적 만점 |
|---|---|---|
| 기존 (BRAINSTORM round 1 + own1-driven + Sprint 1/2 done) | — | **20** |
| **PSB-2 sketch 채택 시** | **+3** (C-DXIL-DELTA, C-ZONE-PREFETCH-IO, C-PSO-FAMILY-CLUSTER) | **23** |
| C-SHADER-CACHE-SHARED (R-list 만점 흡수) | +1 (R-list) | (R-list 별도) |

### 3.2 EXHAUSTION 추적

> **공식 정의**: PSB-N 에서 새 만점 0 × **3 sprint 연속** = EXHAUSTION → docs/EXHAUSTION_<date>.md 작성 + closure.

| sprint | 새 만점 | EXHAUSTION 카운터 |
|---|---|---|
| PSB-1 | 4 (sketch 채택 시) | reset 0 |
| **PSB-2** | **3 (sketch 채택 시)** | reset 0 |
| PSB-3 | ? | 0 일 시 +1 |
| PSB-4 | ? | 0 일 시 +2 |
| PSB-N (3 연속 0) | 0 | **EXHAUSTION 발동** |

→ PSB-2 sketch 채택 시 EXHAUSTION 카운터 reset, +1 sprint 거리 추가.

### 3.3 본 sketch 의 즉시 효과

본 sketch 의 3 만점 후보가 PSB-2 round 직전 발견된 셈 → PSB-2 round 시점에 *최소* 새 만점 3 보장. EXHAUSTION 까지 거리 +1 sprint.

---

## 4. 우선순위 배치

### 4.1 sprint 매핑

| sprint | 컴포넌트 | 시점 | 비고 |
|---|---|---|---|
| **S2** | R1 C-SHADER-WARM + R2 C-DXIL-PERSIST | Sprint 2 (이미 진행 중/완료) | closure roadmap |
| **S3** | PSB-1 sketch 4 (C-PSO-CHAIN 등) | PSB-1 채택 후 | 이미 PSB-1 sketch |
| **S4** | **C-DXIL-DELTA** (15/15) | PSB-2 채택 후 first | M9 즉시 활용, 3-4 일 |
| **S5** | **C-ZONE-PREFETCH-IO** (15/15) | S4 후 | R1 보강, 4-5 일 |
| **S6** | **C-PSO-FAMILY-CLUSTER** (15/15) | S5 후 | M4+CN-7 활용, 3-4 일 |

### 4.2 우선순위 근거

1. **C-DXIL-DELTA first (S4)** — M9 C-DXIL-PERSIST (이미 done) 위 layer 만 추가. 3-4 일로 가장 빠른 ROI. 시즌 패치 cycle 마다 효과 발생 → 사용자 체감 매 시즌. PSB-2 누적 만점 21 즉시 달성.
2. **C-ZONE-PREFETCH-IO second (S5)** — R1 (C-SHADER-WARM) 보강 layer. zone transition 은 D2R/D4 모두 빈번 (분당 1-3 회) → runtime 누적 효과 큼. 4-5 일 다소 길지만 stutter -70%↑ ROI 압도적.
3. **C-PSO-FAMILY-CLUSTER third (S6)** — M4 C-PSO + CN-7 C-PSO-CHAIN 위 multiplier. PSO compile 횟수 -30~50% 는 launch + 첫 boss 모두 효과. 3-4 일.

### 4.3 estimate + closure 영향

| 후보 | 작업일 | closure ETA 영향 | 누적 만점 |
|---|---|---|---|
| C-DXIL-DELTA | 3-4 일 | +0.13 달 | 20 → **21** |
| C-ZONE-PREFETCH-IO | 4-5 일 | +0.17 달 | 21 → **22** |
| C-PSO-FAMILY-CLUSTER | 3-4 일 | +0.13 달 | 22 → **23** |
| **합계** | **10-13 일** | **~+0.43 달** | **만점 20 → 23** |

→ closure 만점 20 → **만점 23** 으로 격상 (+3). closure ETA: 21.4 달 (PSB-1 후) → ~21.83 달 (PSB-2 후).

### 4.4 PSB-2 sketch 채택 안 시 fallback

- 만점 20 (Sprint 1+2 + PSB-1 4 채택 누적) 그대로 closure
- PSB-2 round 1 + round 2 에서 추가 만점 발견 시 EXHAUSTION 카운터 reset 1 회 가능
- PSB-3 으로 넘김

---

## 5. round 2 발견 가능성 (Sprint 2 측정 input 후)

### 5.1 Sprint 2 unexplained anomaly

Sprint 2 (R1 C-SHADER-WARM + R2 C-DXIL-PERSIST) 측정 후, 만점 20 컴포넌트로 *설명 안 되는* anomaly cluster 가 잔재할 가능성:
- shader-warm 후에도 잔존하는 stutter → 새 origin 추정 (e.g. PSO eviction race, descriptor heap pressure)
- DXIL-persist 가 hit 했지만 latency 큰 case → `lib/perf/c_dxil_latency_class.hexa` 후보 (cache hit latency 분포 분류)

→ **PSB-2 round 2 추정 후보 수**: 2-3 개, 만점 가능성 50-70%.

### 5.2 cross-sketch correlation

PSB-1 sketch 4 + PSB-2 sketch 3 의 cross 상관:
- C-PSO-CHAIN ↔ C-PSO-FAMILY-CLUSTER → chain 학습 + family 화 결합 layer 가능
- C-HEAP-DEFRAG ↔ C-ZONE-PREFETCH-IO → defrag idle window 와 prefetch IO window 의 conflict 분석
- C-GPU-HUD-RING ↔ all → observability 통합 layer

→ round 2 에서 cross-component meta-layer 후보 발견 가능성.

---

## 6. 참고 — sketch 단계의 한계 + 정직성

본 sketch 는 *예측*. 실제 PSB-2 는 Sprint 2 측정 후 진행하므로 본 3 후보가:
- (a) 만점 유지 — A axis (Apple-only) 5 점 가정은 변동 가능 (e.g. ZONE-PREFETCH 의 `posix_fadvise` hint 가 macOS 에서 효과 약함 경우 C axis 감점)
- (b) 더 좋은 후보 발견 — round 2 에서 본 3 보다 만점/효과 큰 후보 발견 시 우선순위 재조정
- (c) 의존 phase 변동 — M9/M12 후속 phase 의 일정 변동 시 sketch 의 sprint S4-S6 도 같이 변동
- (d) 측정 input 부정 — 예: family cluster 가능 PSO ≤30% 라면 C-PSO-FAMILY-CLUSTER 채택 보류

→ 본 sketch 는 PSB-2 *진입점* 만 보장. 실제 채택은 측정 후 재검증.

---

## 7. 다음

본 sketch 채택 시:
1. closure roadmap (`docs/CLOSURE_ROADMAP.md`) § 5.5 → § 5.7 로 PSB-2 sketch 3 후보 reflect
2. SCORING.md 의 D-list 만점 표 갱신 (PSB-2 sketch 후보 3 reservation)
3. Sprint 2 측정 후 PSB-2 round 1/2 진행 → POST_SPRINT_2_BRAINSTORM.md 작성 (실측정 reflect)
4. sprint S4-S6 작업표 (별도 PSB_2_FULL.md, Sprint 2 측정 후 작성) reservation

---

*written 2026-05-01 (Sprint 2 측정 전 sketch).*
*post: PSB-2 실측정은 Sprint 2 시점 별도 docs/POST_SPRINT_2_BRAINSTORM.md 로 작성.*
