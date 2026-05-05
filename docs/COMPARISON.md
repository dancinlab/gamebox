# 컴포넌트 % 비교 (quick reference)

> **estimate** — 실측 아님. baseline = stock GPTK. 단위 = Δ% vs baseline.
> 자세한 채점·근거는 [PERF_PLAN.md](PERF_PLAN.md) 참조.

## 만점 채점 요약

채점: A. GPTK-specific (/5) + B. Runtime (/5) + C. 데이터재해석 (/5) = /15

### 만점 (15/15) — 우선순위 1

| | 이름 | 한 줄 |
|---|---|---|
| D2 | pso_genome | D3DMetal PSO 를 60-byte 게놈으로 → SHBF dedup |
| D3 | frame_cmd_genome | frame 별 command buffer 6축 → Holt forecast |
| D4 | rosetta_hotloop_genome | Rosetta hotloop 6축 → custom hint |
| D5 | metal_heap_hexagon | Metal heap 6축 → anomaly = OOM 임박 → pre-evict |
| D6 | frametime_jsonl | frame timing 컬럼 → stutter classify+forecast |

### 14/15 — 한 끗 부족

| | 이름 | 빠진 점 |
|---|---|---|
| D1 | dxil_air_columnar | runtime (B=4) — 주로 cold launch |
| D7 | shader_cache_shbf_trie | 데이터재해석 (C=4) — SHBF/trie 만, 게놈 X |

### 10/10 (R-list, C 점수 빠짐)

| | 이름 |
|---|---|
| R1 | d3dmetal_shader_cache (pre-populate) |
| R2 | dxil_air_translation_cache (persist) |
| R3 | d3dmetal_heap_preallocate |
| R4 | d3dmetal_pso_dedup |

### 9/10

| | 이름 | 빠진 점 |
|---|---|---|
| R5 | rosetta_xlation_persist | B=4 (2nd launch부터) |
| R6 | metal_queue_priority | A=4 (Metal API generic) |
| R7 | rosetta_avx_per_scene | B=4 |

### Fail (만점 기준 미달, 폐기)

G1, G2, G3, G4, G5, G6, G8, G9, G11, G12 — shell / Wine generic / 관찰 등

---

## 주요 % 비교 표

| 컴포넌트 | FPS avg | P99 frame | Stutter/min | Memory | Cold launch | Disk |
|---|---|---|---|---|---|---|
| D2 pso_genome | +3~8% | +20~35% | -70~95% | +200MB | -10~20% | +200MB |
| D3 frame_cmd_genome | +1~3% | +5~15% | -5~15% | +1~3% | 0 | 0 |
| D5 metal_heap_hex | +2~5% | +15~30% | -50~80% | -5~10% | 0 | 0 |
| D6 frametime_jsonl | 0 | 0 | 0 (enabler) | +1% | 0 | +50MB/h |
| D4 rosetta_hotloop | +1~3% | +2~5% | -5~10% | -3~5% | -10~20% (2nd+) | +300MB |
| D1 dxil_air_columnar | 0 | 0 | 0 | 0 | -20~40% | -30~50% |
| D7 shader SHBF+trie | +0~2% | +0~5% | -20~40% | 0 | -10~25% | -10~20% |
| **R1** shader_cache_warm | +0~2% | **+30~60%** | **-85~99% zone** | +2GB | **-30~60%** | +2GB |
| R2 dxil_air_persist | +1~3% | +10~25% | -30~60% | +1GB | **-50~80% (2nd+)** | +1GB |
| R3 heap_preallocate | +0~2% | +1~5% | -10~20% | +2GB | +5~15% | 0 |

## 시나리오별 (D2R / D4)

| 시나리오 | best 단일 | best 조합 |
|---|---|---|
| D2R 캐릭 첫 진입 | D2 / R1 | D2+R1 |
| D2R 4-player co-op | D2 | D2+D5 |
| **D4 zone transition** ⭐ | **R1** ⭐ | **D5+R1** |
| **D4 open-zone exploration** ⭐ | **D5** ⭐ | D2+D5 |
| D4 boss fight | D2 | D2+D3+D5 |
| 2nd launch onwards | R2 | R2+D4 |

## 조합 누적 (estimate)

| 조합 | FPS avg | P99 | Stutter |
|---|---|---|---|
| D5 단독 | +2~5% | +15~30% | -50~80% |
| R1 단독 | +0~2% | +30~60% | -85~99% zone |
| D5 + R1 | +3~7% | +40~75% | -90~99% |
| **D2 + D5 + R1** | **+5~12%** | **+50~85%** | **-92~99%** |
| D-quad (D2+D3+D5+D6) | +6~13% | +35~65% | -80~95% |
| **만점 6 (D2+D3+D5+D6+R1+R2)** | **+8~16%** | **+55~95%** | **-95~99%** |

## 효율 (★★★★★ 가 best)

| 컴포넌트 | 작업 | 디스크 | 효율 |
|---|---|---|---|
| D5 | 2-3d | 0 | ★★★★★ |
| D2 | 2-3d | +200MB | ★★★★★ |
| R1 | 3-4d | +2GB | ★★★★ |
| D6 | 1d | +50MB/h | ★★★★ |
| D3 | 2d | 0 | ★★★ |
| R2 | 2d | +1GB | ★★★ |
| D7 | 1d | 0 | ★★★ |
| D4 | 4-5d | +300MB | ★★ |

## Sprint 1 추천 (1주)

**D5 + D2 + D6** (디스크 거의 0, runtime 큰 셋, airgenome harvest/label/forecast 직접 접속)

목표 (estimate):
- D4 open-zone P99 frame -15~30%
- D4 zone transition stutter -50~80%
- D2R 4-player combat-entry stutter -70~95%
- forge/genomes-gptk.ring 에 게임 게놈 누적 시작

## Sprint 2 추천

**R1** — D4 zone transition 의 game-changer (디스크 +2GB / stutter -85~99%)
