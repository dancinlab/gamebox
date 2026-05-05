# airgenome 정체성 ↔ GPTK 매핑

> airgenome 의 핵심 자산 (60-byte 게놈 + filter playbook + label/forecast 파이프라인)
> 을 GPTK runtime 데이터에 어떻게 접속시키는지의 1:1 매핑.

## 1. airgenome 핵심 자산

| 자산 | 위치 | 검증 |
|---|---|---|
| 6축 60-byte hexagon (Vitals) | `core/core.hexa` | M0 done |
| sample / assess / AdaptiveThrottle | `core/core.hexa` | M0 done |
| harvest (60-byte 누적 ring) | `modules/harvest.hexa` | M4 done |
| label (anomaly → behavior) | `modules/label.hexa` | M5 done |
| forecast (Holt 1h 예측) | `modules/forecast.hexa` | M6 done |
| filter — SHBF | `modules/filters/` | wave 1, 17.9× cold / 2.5× persistent |
| filter — APBF | `modules/filters/` | wave 1 |
| filter — AC trie (DNS) | `modules/filters/` | wave 1, F7 |
| filter — columnar | `modules/filters/` | own ledger, 2-9× |
| filter — result_cache | `modules/filters/` | own ledger, 4.8-7.8× |

## 2. GPTK runtime 데이터 → airgenome 표현 매핑

| GPTK 데이터 원본 | airgenome 재해석 | 컴포넌트 |
|---|---|---|
| D3DMetal pipeline state objects (다필드 set) | 6축 hexagon `[vertex, fragment, depth, blend, raster, primitive]` 60-byte | **D2** pso_genome |
| frame command buffer (per-frame draw N개) | 6축 `[draw, state_chg, tex_bind, buf_upd, compute, blit]` time-series | **D3** frame_cmd_genome |
| Rosetta 2 hot-loop instruction trace | 6축 `[int, fp, simd, mem, branch, sys]` | **D4** rosetta_hotloop_genome |
| Metal heap byte-level usage | 6축 `[tex_active, tex_evicted, buf_active, buf_residual, frag_pct, peak]` | **D5** metal_heap_hexagon |
| 프레임 타이밍 stream | columnar `[frame_id, gpu_ms, cpu_ms, queue_wait, shader_compile_ms]` | **D6** frametime_jsonl |
| shader cache filesystem | SHBF + AC trie (pipeline stage 분류) | D7 |
| DXIL/AIR translation 결과 | columnar table `[hash, dxil_sz, air_sz, compile_ms, last_used]` | D1 |

## 3. airgenome 모듈 → GPTK 컴포넌트 1:1

| airgenome 모듈 | GPTK 적용 |
|---|---|
| **harvest** (60-byte 누적) | D2/D3/D4/D5 게놈 → `forge/genomes-gptk.ring` (per-host) |
| **label** (anomaly 분류) | D5 → "OOM 임박" 라벨, D6 → "stutter spike" 라벨 |
| **forecast** (Holt 1h) | D3 → "next-frame draw count 예측", D6 → "next-second stutter 예측" |
| **filter SHBF** | D2 (PSO hash dedup), D7 (shader membership), R1 (known shader pre-populate) |
| **filter AC trie** | D7 (pipeline stage 분류), R4 (PSO state 분류) |
| **filter columnar** | D1 (shader table), D6 (frametime table) |
| **filter result_cache** | R2 (DXIL/AIR persist), R5 (Rosetta xlation persist) |

## 4. 정체성 정합 점수

| 컴포넌트 | airgenome 자산 사용 | 자체 표현 | 정합 점수 |
|---|---|---|---|
| D2 | harvest + SHBF + 6축 게놈 | 60-byte | **5/5** |
| D3 | harvest + forecast + 6축 게놈 | 60-byte time-series | **5/5** |
| D4 | harvest + 6축 게놈 | 60-byte | **5/5** |
| D5 | harvest + label + 6축 게놈 | 60-byte | **5/5** |
| D6 | columnar + label + forecast | columnar | **5/5** |
| D1 | columnar | columnar | 5/5 |
| D7 | SHBF + AC trie | filter | 4/5 (게놈 X) |
| R1 | SHBF | pre-warm | 3/5 (재해석 X) |
| R2 | result_cache | persist | 3/5 |
| R3-R7 | per-game profile | env / hint | 1-2/5 |

## 5. 흐름 다이어그램

```
[게임 실행 D2R/D4]
       │
       ↓  (R0 instrumentation hook)
[GPTK runtime 데이터 stream]
       │
       ├── D2 PSO     ──┐
       ├── D3 frame   ──┼─→ 60-byte 게놈 ──→ harvest
       ├── D4 rosetta ──┤                       │
       └── D5 heap    ──┘                       ↓
                                            forge/genomes-gptk.ring
                                                │
       ┌── D6 frametime (columnar) ────────────┤
       │                                        ↓
       │                                   ┌──label───→ anomaly:
       │                                   │           - OOM 임박
       │                                   │           - stutter spike
       │                                   │           - PSO compile peak
       │                                   ↓
       │                              forecast (Holt) ──→ 1h 예측:
       │                                                  - next-second stutter
       │                                                  - next-frame draw
       │                                                  - heap eviction 시점
       │                                        │
       │                                        ↓
       │                                  ┌─[액션]─┐
       │                                  ↓       ↓
       │                              [pre-evict] [pre-warm]
       │                              (D5)        (D2/R1)
       │
       └─→ R1 shader_cache_warm (SHBF seed)
       └─→ R2 dxil_air_persist
       └─→ R5 rosetta_xlation_persist
```

## 6. 다른 표현 자산도 흡수

airgenome 의 추후 자산 (M7-M25 ops convergence, raw 156 placement-axis 등) 에
gamebox 게놈 ring 이 합류 가능. 즉:

> **AGI ops infrastructure** (airgenome MAIN) + **게임 워크로드** (gamebox D-list) =
> 동일 60-byte 게놈 schema, 동일 ring, 동일 label/forecast 파이프라인.
> gamebox 가 airgenome 의 **새로운 호스트** 처럼 동작.

drop-in. invariants 1-4 그대로 만족 (게놈 schema 동일, core/core.hexa 미수정,
modules 끼리 import 안 함, archive read-only).

## 7. 결론

D2 / D3 / D4 / D5 / D6 가 airgenome 정체성에 **drop-in**.
이게 다른 어떤 GPTK 튜닝 도구도 못 하는 점이다 — 단순 cache/env tuning 이 아니라
게임 런타임을 airgenome 게놈으로 reproject 해서 anomaly+forecast 파이프라인에 직접 접속.

따라서 sprint 1 은 **D5 + D2 + D6** (정체성 정합 최대 + 디스크 비용 최소).
