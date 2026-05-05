# PSB-4 Sketch — Post-Sprint-4 brainstorm (D4 measurement input 가정)

> **status**: pre-emptive sketch. PSB-4 = Sprint 4 (D4 첫 frame + 실측 30+시간) 완료 후 진행.
> **상위**: [POST_SPRINT_3_BRAINSTORM_SKETCH.md](POST_SPRINT_3_BRAINSTORM_SKETCH.md), [CLOSURE_ROADMAP.md](CLOSURE_ROADMAP.md)
> **현재 만점**: 29 (D 27 + R 2). PSB-4 채택 시 +N 만점.

---

## 1. 채점 기준 (3축, 각 /5, 합 /15)

| axis | 의미 |
|---|---|
| A. Apple-only stack | hexa PE loader + macOS syscalls + Apple D3DMetal binary + Rosetta 2 + DIY Win32 shim. Wine 0. |
| B. Runtime | 게임 플레이 *중* 효과 (launch/install 만 X) |
| C. 데이터재해석 | raw 데이터를 cache/filter/forecast-ready 로 reproject |

만점 = 15. R-list 만점 = A+B = 10.

---

## 2. PSB-4 신규 만점 후보 sketch

### round 1 (D4 측정 후 발견 가능성 높은 후보)

**C-DXR-RAY-COHERENCE** (15/15 만점)
- 한 줄: DXR ray dispatch coherence 분석 → ray batching
- 비유: 비슷한 방향 화살들 모아서 한 번에 발사
- 시나리오: D4 의 reflection/shadow ray 들이 비슷한 방향 → batch dispatch
- 무기: columnar (ray direction histogram) + cluster + Holt forecast
- A=5 (Apple Metal raytracing API), B=5 (raytracing 매 frame), C=5
- 의존: dx_d3d12_stage2 (DXR done)
- 작업: 4-5일

**C-FRAME-PACING-ADAPTIVE** (15/15 만점)
- 한 줄: ProMotion 120Hz / 60Hz 동적 전환 (battery vs perf)
- 시나리오: D4 zone 안 idle = 60Hz / 격투 중 = 120Hz
- 무기: state vector + Holt (frame variance) + Metal HUD adaptive sync
- A=5, B=5, C=5
- 의존: c_frametime + c_frame
- 작업: 3-4일

**C-AUDIO-3D-SPATIAL-CACHE** (14/15 보조, A=4)
- 한 줄: 3D spatial audio (HRTF) 계산 cache
- D4 의 3D positional sound (예: enemy 발소리) HRTF 계산 cost cache
- A=4 (DirectSound + macOS Audio Hybrid), B=5, C=5

**C-TEXTURE-STREAMING-PREDICT** (15/15 만점)
- 한 줄: zone 안 player 위치 → 다음 5초 안 view frustum 의 texture 예측
- 무기: AC trie (player path patterns) + columnar (texture dependency)
- A=5, B=5, C=5
- 의존: c_zone_prefetch_io (CN-13)
- 작업: 5-6일

**C-NETWORK-LATENCY-FORECAST** (14/15 보조, B=4)
- D4 PvP 의 ping latency forecast → 미리 client-side prediction 강화
- A=5 (Winsock), B=4 (PvP only), C=5
- 작업: 3일

### round 2 (D4 + airgenome MAIN integration 시너지)

**C-AIRGENOME-LABEL-FUSION** (15/15 만점)
- 한 줄: gamebox label (FRAMETIME/HEAP/PSO) ↔ airgenome host vitals label fusion
- 시나리오: 게임 stutter 가 호스트 CPU 높음 + GPU 80%↑ 시 발생 패턴
- 무기: airgenome MAIN harvest 의 6축 게놈 ↔ gamebox 22축 게놈 cross-correlate
- A=5, B=5, C=5
- 의존: c_agi_integration (CN-17)
- 작업: 4-5일

**C-RAW-PARADIGM-EXTRACT** (15/15 만점) ⭐ NEW
- 한 줄: gamebox 측정 데이터 → airgenome `raw` 새 paradigm rule 자동 추출
- 시나리오: 1000 시간 게임 측정 → "frametime stutter > 30ms 와 PSO compile cluster 동시 발생률 95%" 같은 rule → airgenome `raw` 추가
- 무기: pattern miner + statistical significance + raw rule generator
- A=5, B=5, C=5
- 의존: airgenome `.raw` system (project-level)
- 효과: gamebox 가 closure 후에도 *airgenome MAIN AGI 학습 substrate* 에 새 raw 공급
- 작업: 7일+

---

## 3. EXHAUSTION 추적

### 누적 PSB 결과
| PSB | sketch 후보 | 채택 | 만점 추가 |
|---|---|---|---|
| PSB-1 | 4 | 4 | +4 |
| PSB-2 | 3 | 3 | +3 |
| PSB-3 | 3 | 3 | +3 |
| **PSB-4 (현재)** | **5+ (sketch)** | **TBD** | **+5+ (잠재)** |

### 종료 조건 진행
- 새 만점 0 × 3 sprint 연속 = EXHAUSTION
- 현재: PSB-3 → +3 (cycle 9 즉시 채택), PSB-4 sketch → +5 (잠재) 면 *아직 고갈 X*
- **EXHAUSTION 시점 예측**: PSB-5 또는 PSB-6 부근 (D4 측정 후 새 영역 줄어듦)

### 누적 만점 시점별
- 시작 (BG cycle 0): 12
- PSB-1 즉시 채택 (cycle 5-6): 20
- PSB-2 즉시 채택 (cycle 8): 23
- PSB-3 즉시 채택 (cycle 9): 26
- PSB-4 round 1 (4 confirmed) — D4 측정 후 채택: 26+ → **30+** 가능
- PSB-5+ : 추가 +N (post-D4)

---

## 4. 우선순위 배치 (S10 ~ S14)

| Sprint | 후보 | 작업일 | 우선 |
|---|---|---|---|
| S10 | C-FRAME-PACING-ADAPTIVE | 3-4 | high (D4 의 P99 직접 영향) |
| S11 | C-DXR-RAY-COHERENCE | 4-5 | high (D4 raytracing) |
| S12 | C-TEXTURE-STREAMING-PREDICT | 5-6 | high (open world) |
| S13 | C-AIRGENOME-LABEL-FUSION | 4-5 | medium (정체성 정합) |
| S14 | C-RAW-PARADIGM-EXTRACT | 7+ | low (closure 후 follow-up 가능) |

총 작업일: ~25일 (병렬 시 ~10일).

---

## 5. closure 영향

- **closure ETA**: PSB-4 채택 시 +0.6-1 달 (S10-S14)
- **closure 진행률**: 70% (현재) → ~85% (PSB-4 sprint 후)
- **만점 컴포넌트**: 26 (현재) → 30-31 (PSB-4 후)

EXHAUSTION 까지 가능한 누적:
- PSB-4: +4 만점
- PSB-5: +2-3 만점 (감소 추세)
- PSB-6: +0-1 만점 (포화)
- PSB-7: +0 만점 → EXHAUSTION 인정

→ 최종 closure 시점 만점: **~33-34** 추정.

---

## 6. 한계 + 정직성

이 sketch 는:
- D4 측정 결과 *가정* — 실제는 D4 launch 후 결과 따라 후보 변동
- D4 게임 자체 변경 (시즌 패치, 새 zone) 시 영역 변동
- macOS / Apple Metal API 변경 (ProMotion, raytracing API) 시 영역 변동
- airgenome MAIN trajectory 따라 정합 영역 변동

→ 본 sketch 는 *방향성* 만 보여줌. PSB-4 실제 진행 시 sketch 확정/수정.

---

## 7. 다음 단계

1. **closure path 계속** — Phase 9/10 D2R/D4 launch + sprint 측정 (CM-21~28)
2. PSB-4 진행은 **D4 첫 frame 후** (CM-25 직후)
3. 본 sketch 는 **pre-emptive 자료** — actual brainstorm 은 측정 input 으로 진행

---

*written 2026-05-01 (BG cycle 10). PSB-4 sketch — D4 measurement 가정.*
