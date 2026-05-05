# PSB-5 Sketch — Post-Sprint-5 brainstorm (cycle 11 직후, EXHAUSTION 근접)

> **status**: pre-emptive sketch. PSB-5 = cycle 11 (PSB-4 5/5 즉시 채택) 완료 후 진행.
> **상위**: [POST_SPRINT_4_BRAINSTORM_SKETCH.md](POST_SPRINT_4_BRAINSTORM_SKETCH.md), [CLOSURE_ROADMAP.md](CLOSURE_ROADMAP.md)
> **현재 만점**: **34** (D 32 + R 2). PSB-5 채택 시 +N 만점 (포화 진입).

---

## 1. 채점 기준 (3축, 각 /5, 합 /15) — 동일

| axis | 의미 |
|---|---|
| A. Apple-only stack | hexa PE loader + macOS syscalls + Apple D3DMetal binary + Rosetta 2 + DIY Win32 shim. Wine 0. |
| B. Runtime | 게임 플레이 *중* 효과 (launch/install 만 X) |
| C. 데이터재해석 | raw 데이터를 cache/filter/forecast-ready 로 reproject |

만점 = 15. R-list 만점 = A+B = 10.

---

## 2. PSB-5 신규 만점 후보 sketch (포화 영역)

### round 1 (cycle 11 채택 직후 잔존 영역)

**C-METAL-RESIDENCY-COALESCE** (15/15 만점, 후보)
- 한 줄: Metal residency set 의 frame 단위 coalesce — texture/buffer set 변동 최소화
- 시나리오: D4 zone transition 시 residency set 100+ delta → coalesce 5-10 으로 압축
- 무기: delta encoding (c_dxil_delta 와 유사) + columnar (residency state vector)
- A=5 (Apple Metal API), B=5 (frame 단위), C=5
- 의존: dx_d3d12_stage2 + Metal residency
- 작업: 4-5일

**C-COMPUTE-DISPATCH-MERGE** (15/15 만점)
- 한 줄: 비슷한 compute shader dispatch 들 묶어 한 번에 발사 (DX12 indirect-style)
- 시나리오: D4 particle/cloth simulation 의 작은 dispatch 들 → merged dispatch
- 무기: cluster (dispatch arg similarity) + APBF (dedup)
- A=5, B=5, C=5
- 의존: c_dxr_ray_coherence (CN-23) 의 batching pattern 재사용
- 작업: 4-5일

**C-VRAM-PRESSURE-PRE-EVICT** (15/15 만점)
- 한 줄: VRAM pressure 임박 *전* texture LRU eviction → swap thrash 방지
- 시나리오: D4 raid 진입 직전 VRAM 90%+ → 미리 oldest 20% evict
- 무기: c_swap_pressure_forecast (CN-19) 의 pressure forecast 재사용 + LRU
- A=5, B=5, C=5
- 의존: c_swap_pressure_forecast
- 작업: 3-4일

### round 2 (감소 추세 — 부분 만점)

**C-ROSETTA-CACHE-AHEAD-OF-TIME** (14/15 보조, A=4)
- 한 줄: Rosetta 2 AOT cache 를 게임 update 시 미리 빌드
- A=4 (Rosetta + DIY hint), B=5 (cold start 단축), C=5
- 작업: 3일

**C-MACOS-METAL-HUD-OVERLAY** (13/15 보조, B=3)
- 한 줄: macOS Metal HUD 의 frame stall counter 를 game-side hint 로 노출
- A=5, B=3 (hint only, runtime 영향 부분), C=5
- 작업: 2일

**C-WINSOCK-SYNTHETIC-DELAY-SHIELD** (12/15 보조, A=4, C=3)
- 한 줄: Winsock recv 의 burst 직전 synthetic delay 추가 → game-side jitter buffer 안정
- A=4, B=5, C=3
- 작업: 2일

### round 3 (영역 거의 소진 — 0~1점 가능)

**C-CPU-AFFINITY-NEON-PIN** (11/15)
- D4 ARM64-translated thread 들의 NEON instruction 비율 측정 → core affinity 힌트
- A=4 (Apple Silicon P-core/E-core 직접), B=4, C=3

**C-DISK-IO-SCHED-SUMMARY** (10/15)
- macOS disk I/O scheduler 와 D4 zone load IO pattern correlate
- A=4, B=3, C=3

---

## 3. EXHAUSTION 추적 (업데이트)

### 누적 PSB 결과
| PSB | sketch 후보 | 채택 | 만점 추가 |
|---|---|---|---|
| PSB-1 | 4 | 4 | +4 |
| PSB-2 | 3 | 3 | +3 |
| PSB-3 | 3 | 3 | +3 |
| PSB-4 | 5+ | **5** | **+5** |
| **PSB-5 (현재)** | **3 round1 / 6 total** | **TBD** | **+3 (잠재)** |

### 종료 조건 진행
- 새 만점 0 × 3 sprint 연속 = EXHAUSTION
- 현재까지: PSB-1~4 모두 채택 → 0 streak = 0
- **PSB-5 round 1 잠재 +3** → 채택 시 만점 **37**, EXHAUSTION 미도달
- **PSB-6 예상 +1~2** (감소)
- **PSB-7 예상 +0~1** (포화)
- **PSB-8 예상 +0** → **EXHAUSTION 인정**

### 누적 만점 시점별 (확정 + 예측)
- 시작 (BG cycle 0): 12
- PSB-1 (cycle 5-6): 20
- PSB-2 (cycle 8): 23
- PSB-3 (cycle 9): 26
- PSB-4 (cycle 10): 29
- **PSB-4 sketch 5 채택 (cycle 11): 34** ← 현재 ✅
- PSB-5 round 1 (cycle 12): **37** (예상)
- PSB-6 (cycle 13): **38-39** (감소)
- PSB-7 (cycle 14): **39** (포화)
- PSB-8 (cycle 15): **39** (EXHAUSTION 선언)

---

## 4. 우선순위 배치 (S15 ~ S17)

| Sprint | 후보 | 작업일 | 우선 |
|---|---|---|---|
| S15 | C-METAL-RESIDENCY-COALESCE | 4-5 | high (D4 frame 단위) |
| S16 | C-COMPUTE-DISPATCH-MERGE | 4-5 | high (particle dispatch) |
| S17 | C-VRAM-PRESSURE-PRE-EVICT | 3-4 | high (raid VRAM) |

총 작업일: ~12일 (병렬 시 ~5일).

---

## 5. closure 영향

- **closure ETA**: PSB-5 채택 시 +0.4 달
- **closure 진행률**: 70% (현재) → ~88% (PSB-5 sprint 후)
- **만점 컴포넌트**: 34 (현재) → 37 (PSB-5 후)

EXHAUSTION 도달 후 final closure 시점 만점: **~39** 추정.

---

## 6. 한계 + 정직성

이 sketch 의 모든 후보는:
- D4 / D2R 실제 측정 *전* 이론적 가정 — 실제 launch 후 검증 필요
- macOS / Apple Metal / Rosetta API 변경 시 영역 변동
- airgenome MAIN trajectory 따라 정합 영역 변동

→ 본 sketch 는 *방향성* 만 보여줌. PSB-5 실제 진행 시 sketch 확정/수정.

EXHAUSTION 추정도 추측 — 실제 PSB-N 결과 따라 +/- 1-2 cycle 변동.

---

## 7. 다음 단계

1. **closure path 계속** — Phase 9/10 D2R/D4 launch + sprint 측정 (CM-21~28)
2. PSB-5 진행은 **D4 첫 frame 후 + cycle 11 commit 후** (CM-25 직후)
3. 본 sketch 는 **pre-emptive 자료** — actual brainstorm 은 측정 input 으로 진행
4. EXHAUSTION 도달 시 closure path 마지막 단계 (CM-30) 진입

---

*written 2026-05-01 (BG cycle 11 종료 후). PSB-5 sketch — cycle 11 채택 5/5 직후, 포화 영역 진입.*
