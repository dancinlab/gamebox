# D2R / D4 — stock GPTK vs hexa-GPTK 성능 비교

> ⚠️ **estimate** — 실측 아님. baseline = stock GPTK (우리 컴포넌트 0개).
> hexa-GPTK = D2 + D3 + D5 + D6 + R1 + R2 (sprint S1+S2 만점 6개 모두 활성).
> 자세한 채점·근거는 [PERF_PLAN.md](PERF_PLAN.md), [SCORING.md](SCORING.md) 참조.
>
> **하드웨어 가정**: M2 Pro / 16GB / 1440p.
> 보정: M1 base **-10~20%**, M3 Max **+10~20%**, M2 base **-5~10%**.

---

## 1. D2R (Diablo II: Resurrected, DX11)

| 지표 | stock GPTK | hexa-GPTK | Δ% |
|---|---|---|---|
| **FPS avg** (캠페인) | 80~100 | 85~110 | **+5~12%** |
| **FPS avg** (4-player co-op 격투) | 50~70 | 60~80 | **+10~18%** |
| **P99 frame time** | ~14ms (간헐 25-50ms) | ~10-12ms | **+30~60%** 개선 |
| **stutter/min** (co-op 피크) | 5~10 | 0.5~1 | **-85~95%** |
| **combat-entry stutter** (P99) | 50~150ms | 5~15ms | **-85~95%** |
| **시네마틱 hitch** | 30~80ms | 5~10ms | **-80~90%** |
| **memory peak** | 3~4GB | 2.8~3.8GB | **-5~10%** |
| **cold launch (1st)** | 30~45s | 25~38s | -10~20% |
| **cold launch (2nd+)** | 28~42s | 8~18s | **-50~70%** |
| **disk overhead** | 0 | +250MB~1GB | (cache) |

> D2R 는 stock 도 비교적 안정. 큰 win 은 **co-op 격투 stutter** 와 **2nd launch**.

---

## 2. D4 (Diablo IV, DX12) — 최대 임팩트

| 지표 | stock GPTK | hexa-GPTK | Δ% |
|---|---|---|---|
| **FPS avg** (open-zone) | 45~60 | 50~70 | **+8~16%** |
| **FPS avg** (boss fight) | 35~50 | 42~60 | **+15~25%** |
| **FPS avg** (도시/허브) | 55~75 | 60~85 | +8~14% |
| **P99 frame time** | ~30~50ms (peak 200ms+) | ~12~25ms | **+55~95%** 개선 |
| **stutter/min** (open zone) | 15~30 | 0.5~2 | **-92~99%** ⭐ |
| **zone transition stutter** (P99) | 200~2000ms (첫 진입) | 10~50ms | **-90~99%** ⭐⭐ |
| **boss-fight effects spam stutter** | 300~800ms | 10~30ms | **-90~99%** |
| **PSO compile peak (combat entry)** | 100~400ms | 5~20ms | **-85~95%** |
| **memory peak** | 8~12GB | 7.5~11GB | **-5~10%** |
| **cold launch (1st, shader compile)** | 120~180s | 80~140s | -25~40% |
| **cold launch (2nd+)** | 90~150s | 25~60s | **-65~80%** |
| **thermal throttle 발생률** | 중~높음 | 낮~중 (FPS↑ 안정) | -20~40% |
| **disk overhead** | 0 | +1.5~3.5GB | (R1 +2GB / R2 +1GB / D2 +200MB) |

> D4 는 hexa-GPTK 가 **game-changer**.
> zone transition + boss spam 의 stutter 가 거의 사라짐 (-90~99%).
> R1 shader_cache_warm 단독으로도 zone hitch 가 사라짐.

---

## 3. 시나리오별 직접 비교 (P99 frame time, ms)

| 시나리오 | stock GPTK | hexa-GPTK | Δ |
|---|---|---|---|
| D2R 일반 진행 | 14ms | 11ms | -21% |
| D2R 캐릭터 진입 | 80ms | 12ms | **-85%** |
| D2R 4-player 격투 | 35ms | 14ms | **-60%** |
| D4 도시 (Kyovashad) | 22ms | 15ms | -32% |
| D4 open-zone 탐험 | 40ms | 18ms | **-55%** |
| **D4 zone transition (첫 진입)** | **2000ms** | **40ms** | **-98%** ⭐ |
| D4 boss fight 시작 | 600ms | 25ms | **-96%** ⭐ |
| D4 PvP (Fields of Hatred) | 45ms | 22ms | -51% |

---

## 4. 컴포넌트별 게임 기여 (estimate, P99 개선 분배)

| 컴포넌트 | D2R 기여 | D4 기여 |
|---|---|---|
| D2 pso_genome | 35% | 20% |
| D3 frame_cmd | 10% | 15% |
| D5 metal_heap | 15% | **30%** |
| D6 frametime | enabler (D2/D5 가속) | enabler |
| R1 shader_cache_warm | 25% | **40%** ⭐ |
| R2 dxil_air_persist | 15% (2nd+) | 20% (2nd+) |

→ **D2R 는 D2 + R1**, **D4 는 R1 + D5** 가 핵심.

---

## 5. 자원 비용 vs 이득

| | stock GPTK | hexa-GPTK | 트레이드오프 |
|---|---|---|---|
| disk (cache 포함) | 30~90GB (게임만) | +1.5~3.5GB | +3~5% disk vs **stutter -90% (D4)** |
| RAM peak | 동일 | -5~10% smart | 손해 없음 |
| CPU per frame | 동일 | -2~5% (PSO dedup) | 약간 이득 |
| GPU per frame | 동일 | 동일 (D3DMetal 자체는 못 만짐) | — |
| 첫 cold launch | 동일~10% 빠름 | shader 수집 cost | seed 필요 |
| 2nd+ cold launch | — | **-50~80%** | 큰 이득 |

---

## 6. 하드웨어 tier 별 보정 (estimate)

| tier | FPS 보정 | stutter 보정 | 메모리 보정 |
|---|---|---|---|
| M1 base (8GB) | **-15~20%** | +20~40% (메모리 압박 더 큼) | 고압 |
| M1 Pro/Max (16GB+) | -5~10% | +5~15% | 보통 |
| **M2 Pro (16GB)** ⭐ baseline | 0% | 0% | 보통 |
| M2 Max (32GB) | +5~10% | -10~20% | 여유 |
| M3 Pro/Max | +10~15% | -15~25% | 여유 |
| M3 Max (48GB+) | **+15~25%** | -25~40% | 매우 여유 |
| M4 Pro/Max | +20~30% | -30~50% | 매우 여유 |

---

## 7. 한 줄 요약

| 게임 | 스토리 |
|---|---|
| **D2R** | stock 도 무난 → hexa 로 **co-op 격투 stutter ~0** + **2nd launch 절반** |
| **D4** | stock 은 zone transition / boss spam 에서 **수백 ms~2초 hitch** → hexa 로 거의 **silky (-90~99% stutter)**. *zone hitch killer* |

> **D4 에서 hexa-GPTK 는 stock 대비 체감 차이가 매우 큼** — 본 sprint 의 핵심 가치.

---

## 8. 측정 protocol (실측 시 사용할 baseline)

### 측정 환경 (고정)

- **Mac**: Mac mini M2 Pro / 16GB / macOS 26.4+
- **외부 모니터**: 1440p 144Hz
- **GPTK**: 최신 stable
- **Battle.net**: 최신 stable
- **세션**: 게임당 30분, 3회 반복, **median**

### 시나리오 (per game)

#### D2R
1. 캠페인 Act 1 — Cold Plains 진행 (15분, FPS+stutter 측정)
2. 4-player co-op (Pandemonium 또는 Cow Level, 격투 5분)
3. 시네마틱 (Act 1 intro)
4. 캐릭터 진입 (lobby → game, 5회 반복)
5. cold launch (앱 종료 후 재시작, 3회)

#### D4
1. Sanctuary open-world (Fractured Peaks, 자유 이동 15분)
2. zone transition (5개 다른 zone 진입, 첫 진입 + 두번째 진입 분리 기록)
3. boss fight (Tier 1 Helltide boss 또는 World Boss)
4. 도시 (Kyovashad, 군중 5분)
5. cold launch (앱 종료 후 재시작, 3회)

### 측정 도구

| 메트릭 | 도구 |
|---|---|
| FPS avg / P99 | `MTL_HUD_ENABLED=1` 또는 D6 frametime_jsonl |
| stutter rate | D6 columnar — `gpu_ms > 16.67` count/min |
| memory | `vmmap` 샘플링 (10s 주기) |
| cold launch | 앱 launch → "playable" wall-clock |
| zone transition stutter | D6 + zone-detect hook |
| thermal throttle | `pmset -g thermlog` |

### 결과 형식

`tool/bench/results/<date>_<game>_<sprint>.jsonl`:
```json
{"ts":"...", "game":"d4", "sprint":"S1+S2", "scenario":"open_zone", "fps_avg":58, "fps_p99":52, "frame_p99_ms":18, "stutter_per_min":1.2, "mem_peak_mb":9100}
```

---

## 9. 비교 표 reproducibility

표의 모든 % 는 향후 R0 instrumentation + sprint 1/2 구현 후 실측 갱신.
**현재 표는 estimate** — 다음 표시 표기:

- ⭐ = 핵심 (게임 체감 변화 매우 큼)
- ⭐⭐ = game-changer (이전에 불가능 → 가능)
- bold = 의미 있는 임팩트 (사용자가 인지 가능한 수준)
- 일반 = 측정에 잡히지만 체감 미묘

---

*written 2026-04-30. estimate. live SSOT — 실측 후 같은 표 형식으로 갱신.*
