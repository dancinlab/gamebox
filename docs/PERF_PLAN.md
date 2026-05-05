# gamebox 성능 계획 — GPTK runtime 개선

> **status**: design (2026-04-30, pre-implementation)
> **scope**: macOS Apple Silicon 에서 D2R/D4 게임 런타임 (perf/resource/speed) 을
>           GPTK 만 사용하면서 stock 대비 개선
> **identity alignment**: airgenome 의 *데이터재해석 + 60-byte 게놈 + filter playbook* 을
>                          GPTK 가 토해내는 런타임 데이터에 직접 적용

---

## 0. 출발점 / 동기

airgenome 은 **OS 게놈 스캐너** — vitals → 6축 60-byte hexagon → anomaly → forecast.
filter 자산 32 종 (SHBF, APBF, AC trie, columnar, result_cache 등) 이미
modules/filters/ 에 구현·검증 (production-validated 10 종, 2-17.9× 개선).

이 DNA 를 GPTK 에 접목하면, **stock GPTK** (Apple wine64 + Rosetta 2 + D3DMetal) 가
하지 않는 일 — runtime 데이터 재해석, 패턴 학습, 캐시 amortization, anomaly+forecast —
을 우리가 위에 얹어서 game runtime perf/resource/speed 를 끌어올린다.

GPTK *내부 코드* (Rosetta 2 binary translator, D3DMetal kernel) 는 closed-source
HW-accel 이라 못 만짐. 그러나 GPTK 가 노출하는 표면 — **shader cache files,
DXIL/AIR translation cache, Metal heap hints, env vars, persistable state** — 은
전부 우리 영역.

---

## 1. 만점 기준 (3축, 각 /5, 합 /15)

> **revision 2026-05-01**: own 1 cleanup 후 axis A 재정의 (Wine 0 mandate). 모든 만점 컴포넌트는 신규 정의에서도 만점 유지.

| axis | 의미 |
|---|---|
| **A. Apple-only stack** | hexa PE loader + macOS syscalls + Apple D3DMetal binary + Rosetta 2 + DIY Win32 shim 위에서만 동작. Wine / CrossOver / community wine 0 줄. (이전 "GPTK-specific (Apple wine64 / D3DMetal / Rosetta 2)" 정의 → own 1 mandate 후 entire stack 이 own source code 로 한정) |
| **B. Runtime** | 실제 게임 플레이 중 효과. launch/setup/install 만 개선되는 건 0~2. |
| **C. 데이터재해석** | airgenome 표현 (게놈 / columnar / SHBF / APBF / AC trie / Holt forecast) 으로 reproject. 단순 env tuning 은 0~2. |

만점 = 15. R-list 만점 = A+B = 10. C 가 5 인 것 = 정체성 정합.

→ 만점 컴포넌트 **14** (D 12 + R 2):
  - 기존 D 8 (C-FRAMETIME / C-HEAP / C-PSO / C-FRAME / C-SHADER-AGING / C-SHADER-SEQ / C-SHADER-WARM++ / C-CORRELATE)
  - 기존 R 2 (C-DXIL-PERSIST / C-SHADER-WARM)
  - **신규 own1-driven D 4** (C-PE-PARSE-CACHE / C-IAT-RESOLVE-CACHE / C-WIN32-CALL-PROFILE / C-SYSCALL-BRIDGE-CACHE)

자세히 → [SCORING.md](SCORING.md), [CLOSURE_ROADMAP.md](CLOSURE_ROADMAP.md).

---

## 2. 경계선 (이길 수 있는 것 / 없는 것)

| Layer | 우리가 이길 수 있나? | 이유 |
|---|---|---|
| Rosetta 2 binary translator (x86→arm64) | ❌ | Apple HW-accel, closed |
| D3DMetal kernel (DX→Metal lowering) | ❌ | Apple proprietary |
| **D3DMetal shader/PSO cache files** | ✅ | filesystem, 우리 쓰기 가능 |
| **DXIL→AIR translation cache** | ✅ | 디스크 cache, persist 가능 |
| **Rosetta 2 translation cache** | ✅ | 디스크 cache, persist 가능 |
| **Metal heap size / queue priority hints** | ✅ | env vars + Metal API hints |
| **Wine prefix DLL overrides (GPTK build)** | ⚠️ | upstream Wine 영역 — 우리 scope 외 |
| **Wineserver lifecycle** | ⚠️ | 같은 이유 — scope 외 |
| **gamebox shell layer** | ✅ | 우리 코드 — 그러나 GPTK-specific 아님 |
| **DNS / network / 운영 layer** | ✅ | GPTK-specific 아님 — 별 sprint |

scope 외 (Wine/shell/network) 는 만점 기준 A=0 또는 B=0 으로 자동 탈락.

---

## 3. G-list (1차 broad 안 — 대부분 폐기)

처음 12개 후보. 만점 기준 적용해 보니 거의 다 fail.

| # | Name | A | B | /10 | verdict |
|---|---|---|---|---|---|
| G1 | prefix_index_SHBF | 0 (shell) | 0 (launch) | 0 | ❌ |
| G2 | proc_trie | 0 | 0 | 0 | ❌ |
| G3 | dns_AC | 0 | 0 (login) | 0 | ❌ |
| G4 | result_cache (detect) | 0 (gamebox) | 0 | 0 | ❌ |
| G5 | stat_cache | 1 (Wine syscall) | 2 | 3 | ❌ |
| G6 | proc_genome | 0 (관찰) | 0 | 0 | ❌ |
| G7 | shader_warm | 5 (D3DMetal) | 2 (cold) | 7 | ⚠️ R-list 로 이동 |
| G8 | prefix_minimal | 1 (Wine config) | 0 (init) | 1 | ❌ |
| G9 | wineserver_warm | **0** (upstream Wine!) | 0 | 0 | ❌ — GPTK 아님 |
| G10 | rosetta_pretrans | 4 (Rosetta) | 1 (install) | 5 | ⚠️ R-list 로 이동 |
| G11 | io_coalesce | 0 (Wine syscall) | 2 | 2 | ❌ |
| G12 | dispatch_fast | 0 (shell) | 0 | 0 | ❌ |

→ G7, G10 만 부분 유효. 나머지 10 개 모두 만점 기준 미달.

**교훈**: "GPTK 와 함께 쓰는 도구" ≠ "GPTK". 만점 채점 안 하면 scope creep.

---

## 4. R-list (만점 GPTK + Runtime, 데이터재해석 X)

GPTK 자원 직접 다루지만 단순 cache/pre-warm/env tuning. 60-byte 게놈/columnar 같은
재해석 없음. /15 은 C 점수 빠져 /10 max.

| # | Name | 무엇 | filter | A | B | /10 |
|---|---|---|---|---|---|---|
| **R1** | `d3dmetal_shader_cache` | 알려진 PSO hash 를 ~/Library/Caches/com.apple.metal/ pre-populate | SHBF | 5 | 5 | **10** |
| **R2** | `dxil_air_translation_cache` | DXIL→AIR 결과 launch 간 persist + RAM-pinned mmap | result_cache | 5 | 5 | **10** |
| **R3** | `d3dmetal_heap_preallocate` | 게임별 MTL heap 사전 사이즈 (D2R 2GB / D4 6GB) | per-game profile | 5 | 5 | **10** |
| **R4** | `d3dmetal_pso_dedup` | redundant PSO set 차단 | AC trie | 5 | 5 | **10** |
| **R5** | `rosetta_xlation_persist` | Rosetta 번역 캐시 영구화 | result_cache | 5 | 4 | **9** |
| **R6** | `metal_queue_priority` | 게임 Metal queue priority hint | hint | 4 | 5 | **9** |
| **R7** | `rosetta_avx_per_scene` | scene/zone 별 Rosetta env swap | per-state | 5 | 4 | **9** |

R-list = **만점 보조 컴포넌트**. D-list 와 함께 쓰일 때 시너지 큼 (R1 = 캐시 source,
D5 = eviction 스마트 — 둘이 짝).

---

## 5. D-list (만점 + 데이터재해석 — airgenome 정체성 정합) ⭐

GPTK runtime 데이터를 airgenome 표현 (60-byte 게놈 / columnar / SHBF / AC trie) 으로
재투영. /15 만점.

| # | 이름 | 원본 데이터 | airgenome 재해석 | A | B | C | /15 |
|---|---|---|---|---|---|---|---|
| **D1** | `dxil_air_columnar` | shader 캐시 = N 파일 | columnar `[hash, dxil_sz, air_sz, compile_ms, last_used]` | 5 | 4 | 5 | 14 |
| **D2** | `pso_genome` | D3DMetal pipeline state object | 6축 hexagon `[vertex, fragment, depth, blend, raster, primitive]` 60-byte → SHBF dedup | 5 | 5 | 5 | **15** ⭐ |
| **D3** | `frame_cmd_genome` | frame command buffer | 6축 `[draw, state_chg, tex_bind, buf_upd, compute, blit]` time-series → Holt forecast → pre-allocate next frame | 5 | 5 | 5 | **15** ⭐ |
| **D4** | `rosetta_hotloop_genome` | Rosetta 2 hot-loop trace | 6축 `[int, fp, simd, mem, branch, sys]` → 알려진 hotspot 매치 → custom hint | 5 | 5 | 5 | **15** |
| **D5** | `metal_heap_hexagon` | Metal heap byte-level usage | 6축 `[tex_active, tex_evicted, buf_active, buf_residual, frag_pct, peak]` → anomaly = OOM 임박 → pre-evict | 5 | 5 | 5 | **15** ⭐ |
| **D6** | `frametime_jsonl` | 프레임 타이밍 JSONL stream | columnar `[frame_id, gpu_ms, cpu_ms, queue_wait, shader_compile_ms]` → stutter 분류 → forecast | 5 | 5 | 5 | **15** ⭐ |
| **D7** | `shader_cache_shbf_trie` | filesystem of cached shaders | SHBF (membership) + AC trie (pipeline stage 분류) | 5 | 5 | 4 | 14 |

**5 개 만점 (15/15)**: D2 / D3 / D4 / D5 / D6.
이 중 D2 / D3 / D5 / D6 가 airgenome 의 harvest / forecast / label 모듈에
**직접 접속** — 정체성 가장 강함.

---

## 6. airgenome 정체성 매핑

| airgenome 컴포넌트 | GPTK 적용 (D-list) |
|---|---|
| `core/core.hexa` Vitals 6축 struct | D2 PSO 6축 / D3 frame 6축 / D4 rosetta 6축 / D5 heap 6축 |
| `modules/harvest.hexa` 60-byte 누적 ring | D2/D3/D4/D5 게놈 → `forge/genomes-gptk.ring` |
| `modules/label.hexa` anomaly 분류 | D5 OOM 임박 라벨, D6 stutter 패턴 라벨 |
| `modules/forecast.hexa` Holt 1h 예측 | D3 next-frame 예측, D6 next-second stutter forecast |
| `modules/filters/columnar` (2-9× 검증) | D1, D6 |
| `modules/filters/SHBF` (17.9× 검증) | D2, D7, R1 |
| `modules/filters/AC trie` (DNS) | D7, R4 |
| `modules/filters/result_cache` (4.8-7.8× 검증) | R2, R5 |

→ **airgenome M4/M5/M6 파이프라인이 게임 런타임 데이터에 직접 접속.**
   이게 진짜 정체성 정합.

---

## 7. 예상 런타임 % 비교 ⚠️ ESTIMATE

> **주의**: 실측 아님. 일반 D3DMetal/Rosetta 패턴 + filter 자산 amortization 기반 추정.
> baseline = stock GPTK (우리 컴포넌트 없음). 단위 = vs baseline Δ%.

| 컴포넌트 | FPS avg | P99 frame | Stutter/min | Memory | Cold launch | Disk |
|---|---|---|---|---|---|---|
| **D2** pso_genome | +3~8% | +20~35% | -70~95% combat-entry | +50~200MB cache | -10~20% | +50~200MB |
| **D3** frame_cmd_genome | +1~3% | +5~15% | -5~15% | +1~3% | 0 | 0 |
| **D5** metal_heap_hex | +2~5% | +15~30% | -50~80% texture stall | -5~10% (smart eviction) | 0 | 0 |
| **D6** frametime_jsonl | 0 (관찰) | 0 | 0 standalone | +1% | 0 | +50MB/h |
| **D4** rosetta_hotloop_genome | +1~3% | +2~5% | -5~10% | -3~5% | -10~20% (2nd+) | +100~300MB |
| **D1** dxil_air_columnar | 0 | 0 | 0 | 0 | -20~40% | -30~50% |
| **D7** shader_cache SHBF+trie | +0~2% | +0~5% | -20~40% | 0 | -10~25% | -10~20% |
| **R1** shader_cache_warm | +0~2% | **+30~60%** | **-85~99% zone** | +500MB~2GB | **-30~60%** | +500MB~2GB |
| **R2** dxil_air_persist | +1~3% | +10~25% | -30~60% | +300MB~1GB | **-50~80% (2nd+)** | +300MB~1GB |
| **R3** heap_preallocate | +0~2% | +1~5% | -10~20% (first only) | +500MB~2GB | +5~15% (slower init) | 0 |

### 게임별 시나리오 체감 (estimate)

| 시나리오 | D2 | D3 | D5 | D6 | R1 | R2 |
|---|---|---|---|---|---|---|
| D2R 캐릭 첫 진입 | +++ | + | + | enable | +++ | ++ |
| D2R 4-player co-op | ++++ | ++ | ++ | enable | + | + |
| D4 zone transition | +++ | + | ++++ | enable | **++++++** ⭐ | +++ |
| D4 open-zone exploration | + | ++ | **+++++** ⭐ | enable | ++ | + |
| D4 boss fight (effects spam) | ++++ | +++ | +++ | enable | ++ | + |
| 2nd launch onwards | + | 0 | 0 | enable | ++ | **+++++** ⭐ |

---

## 8. 조합 시너지 (estimate)

| 조합 | 누적 FPS avg | 누적 P99 | 누적 stutter | 시너지 |
|---|---|---|---|---|
| D5 단독 | +2~5% | +15~30% | -50~80% | — |
| R1 단독 | +0~2% | +30~60% | -85~99% zone | — |
| D5 + R1 | +3~7% | **+40~75%** | **-90~99%** | high (R1=캐시 source / D5=eviction 스마트) |
| **D2 + D5 + R1** | **+5~12%** | **+50~85%** | **-92~99%** | **highest** |
| D2 + D3 + D5 + D6 (D-quad) | +6~13% | +35~65% | -80~95% | high (genome 풀스택, label/forecast 활용) |
| **D2 + D3 + D5 + D6 + R1 + R2** (만점 6) | **+8~16%** | **+55~95%** | **-95~99%** | 거의 sum |

---

## 9. 효율 ranking (임팩트 / 작업시간 / 디스크)

| | 임팩트 score | 작업 | 디스크 | **효율** |
|---|---|---|---|---|
| D5 metal_heap_hex | 9/10 | 2-3d | 0 | ★★★★★ |
| D2 pso_genome | 8/10 | 2-3d | +200MB | ★★★★★ |
| R1 shader_cache_warm | 8/10 | 3-4d | +2GB | ★★★★ |
| D6 frametime_jsonl | 5/10 (enabler) | 1d | +50MB/h | ★★★★ |
| D3 frame_cmd_genome | 5/10 | 2d | 0 | ★★★ |
| R2 dxil_air_persist | 6/10 | 2d | +1GB | ★★★ |
| D7 shader_cache SHBF | 4/10 | 1d | 0 | ★★★ |
| D4 rosetta_hotloop | 5/10 | 4-5d | +300MB | ★★ |

---

## 10. Sprint 추천

### Sprint 1 (1주) — D-quad 정체성 정합

**D5 + D2 + D6** + (필요시 D3)

- 디스크 비용 거의 0
- runtime 기여 큰 셋
- airgenome harvest/label/forecast 직접 접속
- D6 가 D2/D5 의 enabler — 같이 가야 시너지 발현

목표:
- D4 open-zone P99 frame -15~30%
- D4 zone transition stutter -50~80%
- D2R 4-player combat-entry stutter -70~95%
- forge/genomes-gptk.ring 에 게임 게놈 분/시간 단위 누적 시작

### Sprint 2 (1주) — R1 zone hitch killer

**R1** (D4 zone transition 의 game-changer)

- 디스크 +2GB 대신 zone stutter -85~99%
- D5 와 짝으로 동작 (R1 = 캐시 source / D5 = eviction 스마트)

### Sprint 3 (1주, optional) — R2 + D4

**R2 + D4** — 2nd launch 가속 + Rosetta hotloop

### NOT 이 sprint

- G1-G12 (대부분 만점 기준 fail)
- network / shell layer 최적화 (별 프로젝트로)
- Wine upstream 영역 (G9 wineserver, G11 io_coalesce)

---

## 11. 사전 의존성

R0 = **instrumentation 모듈** (D-list 시작 전 필수)

- D2/R1: D2R/D4 첫 실행 시 PSO/shader hash 수집 → SHBF 시드 데이터
- D3: 프레임 단위 command buffer signature 캡처
- D5: Metal heap mtime + size 샘플링
- D6: D3DMetal logging hook 또는 Metal Performance Shaders telemetry

R0 자체는 D2-D6 만점 기준에 직접 안 맞음 (관찰만) — 그러나 **pre-warm cache 시드 만들기** 가
R0 의 본질이라 D2/R1 의 일부로 편입.

---

## 12. 측정 (sprint 종료 시 검증)

### KPI

1. **P99 frame time** (ms) — D5/R1 핵심 지표
2. **Stutter rate per minute** — count of frames > 16.67ms in 60fps target
3. **FPS average** — overall throughput
4. **Memory pressure events / 분** — D5 검증
5. **Cold launch ms** — R2 검증
6. **Combat-entry stutter ms (P99)** — D2 핵심
7. **Zone transition stutter ms** — R1 핵심

### 측정 환경

- Mac mini M2 Pro (target dev box) + 외부 모니터 1440p
- D2R: campaign Act 1 + 4-player co-op 시나리오
- D4: open-world Sanctuary + zone transitions + boss arena
- 30분 세션, 3 회 반복, median 기준

### 비교 baseline

stock GPTK (우리 컴포넌트 0개) — 같은 게임/시나리오에서 미리 측정.

---

## 13. 솔직한 제약 / 리스크

| | |
|---|---|
| Apple D3DMetal/Rosetta 내부 코드 | ❌ 못 만짐 |
| LD_PRELOAD shim | ⚠️ Apple 보안 변동에 민감, 우회 가능성 |
| 게임 EULA | ⚠️ Battle.net 가드가 변형된 cache 거부할 가능성 |
| 추정 % 자체 | ⚠️ R0 instrumentation 후 실측해야 신뢰 |
| GPTK 버전 호환 | ⚠️ Apple GPTK 메이저 업데이트 시 cache 포맷 변경 가능 |
| 디스크 사용량 | R1 (+2GB) / R2 (+1GB) 부담은 user 결정 |

---

## 14. 다음 액션

1. **R0 instrumentation 모듈** scaffolding (lib/perf/r0_instrument.sh)
2. **D5 metal_heap_hex** 구현 (lib/perf/d5_heap_genome.{sh,hexa})
3. **D2 pso_genome** 구현 (lib/perf/d2_pso_genome.{sh,hexa})
4. **D6 frametime_jsonl** 구현 (lib/perf/d6_frametime.{sh,hexa})
5. baseline 측정 스크립트 (tool/bench/baseline_stock_gptk.sh)
6. 검증 스크립트 (tool/bench/measure.sh)

각각 self-test + baseline-vs-ours 벤치 포함.

---

## 부록 A: filter 자산 카탈로그 (airgenome modules/filters/)

본 계획에서 재활용할 filter:

| filter | 검증 | 적용처 |
|---|---|---|
| SHBF (sharded bloom) | 17.9× cold / 2.5× persistent | D2, D7, R1 |
| APBF (adaptive prob bloom) | wave 1 | (예비) |
| AC trie (Aho-Corasick byte) | DNS wave 1 | D7, R4 |
| columnar | 2-9× | D1, D6 |
| result_cache | 4.8-7.8× | R2, R5 |
| trie | wave 1 | (예비) |

---

## 부록 B: 채점 기준 명문화

### A. GPTK-specific (/5)

| 점수 | 기준 |
|---|---|
| 5 | Apple wine64 / D3DMetal / Rosetta 2 의 캐시·env·hint 직접 다룸 |
| 4 | 위 중 일부 + 우리 코드 보조 |
| 3 | GPTK 와 함께 쓰지만 GPTK-only 아님 |
| 2 | generic Wine 동작 |
| 1 | shell / orchestration |
| 0 | network / observability / unrelated |

### B. Runtime (/5)

| 점수 | 기준 |
|---|---|
| 5 | 게임 플레이 중 매 프레임/초 동작 |
| 4 | 게임 플레이 중 가끔 동작 (zone transition, combat entry 등) |
| 3 | 2nd launch 부터 효과 |
| 2 | cold launch 시 1회 효과 |
| 1 | install/setup 시 1회 효과 |
| 0 | runtime 무관 |

### C. 데이터재해석 (/5)

| 점수 | 기준 |
|---|---|
| 5 | raw GPTK 데이터를 airgenome 표현 (60-byte 게놈 / columnar / SHBF / AC trie) 으로 reproject |
| 4 | filter 자산 활용하지만 airgenome native 표현 아님 |
| 3 | 단순 cache / memoize |
| 2 | env tuning |
| 1 | 단순 pre-warm |
| 0 | 재해석 없음 |

---

*written 2026-04-30. live SSOT — 진행하면서 업데이트.*
