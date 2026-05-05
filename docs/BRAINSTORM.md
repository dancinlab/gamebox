# Brainstorm — 만점 기준, 자유에서 시작

> **방식**: airgenome 의 filter 자산을 *무기* 로, GPTK runtime 의 문제를 *대상* 으로
>           보고 매칭. 만점 (A+B+C=15 또는 R 의 A+B=10) 통과한 것만 채택.
> **자유**: axis 수, byte 수, schema, ring 파일 모두 컴포넌트별 자연 형태.
> **상위**: [PERF_PLAN.md](PERF_PLAN.md), [SCORING.md](SCORING.md), [IDENTITY.md](IDENTITY.md).

---

## 0. 만점 기준 + 자유

### 만점 (A+B+C, 각 /5)

| axis | 의미 |
|---|---|
| **A. GPTK-specific** | Apple wine64 / D3DMetal / Rosetta 2 만 (generic Wine/shell 0) |
| **B. Runtime** | 게임 플레이 *중* 효과 (launch/install 만 X) |
| **C. 데이터재해석** | raw GPTK 데이터를 cache/filter/forecast-ready 로 reproject |

### 자유

- airgenome 60-byte hexagon (6-axis) = vitals 의 *우연한 자연 결과*. 강제 X.
- 컴포넌트별로 axis 수, byte/record, ring 파일 schema 자기 형태로 결정.
- C 만점 = 60-byte 모방 X, **재해석 *원리* 적용** = filter/cache/forecast 가능 구조로 변환.

---

## 1. 우리 무기 (airgenome filter 자산)

`airgenome/modules/filters/` 에 production-validated:

| 무기 | 검증 | 강점 |
|---|---|---|
| **SHBF** (sharded bloom) | 17.9× cold / 2.5× persistent | 정적 set membership |
| **APBF** (adaptive prob bloom) | wave 1 | 시간 변화 set (heat decay) |
| **AC trie** (Aho-Corasick byte) | DNS wave 1 | 패턴 매칭, sequence |
| **columnar** | 2-9× | 대량 분석, 시계열 |
| **result_cache** | 4.8-7.8× | 메모이제이션, persist |
| **trie** | wave 1 | prefix/path lookup |
| **Holt 이중 지수평활** | M6 done | 1h forecast |
| **harvest ring** (variable record) | M4 done | 누적 store |
| **label rules** | M5 done | anomaly classifier |
| **compact state vector** (any axis) | M0/M4 done | 다차원 상태 |

---

## 2. GPTK runtime 문제 카탈로그

| 문제 | 영향 | 빈도 |
|---|---|---|
| PSO compile hitch (첫 사용 시 200-2000ms) | P99 frame ↑↑↑ | 게임 진행 곳곳 |
| Zone-transition shader compile | 200ms-2초 hitch | 매 zone 진입 |
| Memory pressure / texture eviction storm | 50-500ms stall | 메모리 압박 시 |
| Heap fragmentation | 미세 stall 누적 | 30분+ 세션 |
| Frame time variance (stutter) | P99 ↑ | 항시 |
| 2nd launch redundant shader compile | cold launch ↑ | 매 launch |
| 2nd launch redundant DXIL→AIR translation | cold launch ↑ | 매 launch |
| Rosetta hot-loop translation cost | 첫 frame 들 ↓ FPS | 매 launch |
| Pipeline state redundant set (게임이 매 frame 같은 PSO set) | per-frame CPU ↑ | 매 frame |
| Texture streaming overhead | mid-game stall | 텍스처 변경 시 |
| Shader cache aging (안 쓰이는 shader 가 cache 차지) | 디스크 압박 | 30+일 누적 |

---

## 3. 무기 × 문제 매칭 → 후보 (만점 채점 포함)

각 후보 = (무기 1+) × (문제 1) → 컴포넌트.

| 후보 | 무기 | 문제 | A | B | C | /15 |
|---|---|---|---|---|---|---|
| **C-PSO** PSO state cache | SHBF + state vector + trie | PSO compile hitch + 매 frame redundant set | 5 | 5 | 5 | **15** ⭐ |
| **C-FRAME** frame command summary | state vector + Holt + label | frame variance | 5 | 5 | 5 | **15** ⭐ |
| **C-HEAP** Metal heap state | state vector + 4 label + Holt | memory pressure + frag | 5 | 5 | 5 | **15** ⭐ |
| **C-FRAMETIME** frametime stream | columnar + classifier + Holt | stutter | 5 | 5 | 5 | **15** ⭐ |
| **C-SHADER-AGING** shader heat decay | **APBF** | shader cache aging + LRU eviction | 5 | 5 | 5 | **15** ⭐ |
| **C-SHADER-SEQ** shader compile sequence | **AC trie + Holt** | next-shader prediction (zone 진입) | 5 | 5 | 5 | **15** ⭐ |
| **C-CORRELATE** multi-source anomaly fusion | label fusion (PSO + HEAP + FRAMETIME) | 어떤 stutter 가 어떤 원인? | 5 | 5 | 5 | **15** ⭐ |
| C-DXIL-INDEX | columnar | DXIL→AIR cache lookup | 5 | 4 | 5 | 14 |
| C-DXIL-PERSIST | result_cache + columnar index | 2nd launch DXIL redundancy | 5 | 5 | 4 (cache 위주) | 14 |
| C-SHADER-WARM | SHBF + trie + persist | zone-transition compile hitch | 5 | 5 | 4 (재해석은 SHBF 만) | 14 |
| **C-SHADER-WARM++** | SHBF + AC trie + columnar index + Holt forecast | 동상 + zone forecast 적용 | 5 | 5 | 5 | **15** ⭐ |
| C-ROSETTA-PERSIST | result_cache | Rosetta cache 영구화 | 5 | 4 | 4 | 13 |
| C-ROSETTA-VEC | state vector + result_cache | Rosetta hot-loop pattern | 5 | 4 | 5 | 14 |
| C-BNET-PRE | result_cache | Battle.net launcher latency | 4 (BN 자체 GPTK 아님) | 4 | 3 | 11 ❌ |
| C-VERSION-MIGRATE | result_cache | GPTK 업그레이드 시 cache 변환 | 5 | 2 (one-time) | 4 | 11 ❌ |
| C-SESSION-GENOME | state vector | 세션간 비교 | 5 | 2 (post-session) | 5 | 12 ❌ |

→ **만점 (15/15) 통과**: 8 개 (C-PSO, C-FRAME, C-HEAP, C-FRAMETIME, C-SHADER-AGING, C-SHADER-SEQ, C-CORRELATE, C-SHADER-WARM++)
→ **R-list (10/10) 통과**: C-DXIL-PERSIST (14), C-SHADER-WARM (14), C-ROSETTA-VEC (14) — D-list 정의로 14, R-list 로 보면 10 만점
→ **폐기**: C-BNET-PRE, C-VERSION-MIGRATE, C-SESSION-GENOME

---

## 4. 만점 컴포넌트 8 — schema 자유 설계

### 4.1 C-PSO (PSO state cache)

**무엇**: D3DMetal 이 매번 set 하는 PSO (pipeline state object) 의 *상태 정체* 를 capture.
컴파일 hitch 제거 + redundant set dedup.

| field | bits | 비고 |
|---|---|---|
| vertex_hash | 16 | shader hash 상위 16-bit |
| fragment_hash | 16 | |
| compute_hash | 16 | (없으면 0) |
| depth_stencil | 16 | DSV state encoded |
| blend | 16 | |
| raster | 16 | |
| primitive | 16 | topology + IA |
| resource_sig | 16 | bind layout hash |
| ts_ms | 32 | relative |
| hit_count | 16 | dedup hit |
| compile_ms | 16 | 첫 컴파일 |

= **24 byte/record**. 100K PSO = 2.4MB.
**SHBF**: 1KB FPR 1% — "이 PSO 가 이미 cache 에 있나?".
**AC trie**: stage 분류 (vs/ps/cs).

### 4.2 C-FRAME (frame command summary)

**무엇**: per-frame command buffer 의 *카테고리별 합* 을 per-second 로 roll-up.
다음 second 의 draw count 예측 → pre-allocate.

| field | bits |
|---|---|
| draw_count | 16 |
| state_change_count | 16 |
| tex_bind_count | 16 |
| buf_update_bytes | 32 |
| compute_dispatch_count | 16 |
| blit_count | 16 |
| present_wait_us | 32 |
| ts_sec | 32 |
| frame_count | 16 |

= **24 byte/record/sec**. 30분 세션 = 43KB.
**Holt forecast**: 1초 ahead.

### 4.3 C-HEAP (Metal heap state)

**무엇**: Metal heap 의 사용 / evict / fragmentation 30Hz 샘플.
OOM/fragmentation 임박 라벨 + 사전 evict.

| field | bits |
|---|---|
| tex_active_mb | 16 |
| tex_evicted_mb | 16 |
| buf_active_mb | 16 |
| frag_pct | 8 |
| peak_mb | 16 |
| ts_ms | 32 |

= **13 byte/record**. 30Hz × 30분 = 1.4MB.

**4 라벨**:
- OOM_imminent: (tex+buf) / heap > 0.92
- frag_critical: frag_pct > 35
- eviction_storm: tex_evicted Δ > 100MB/sec
- thrashing: tex_active_volatility > 2σ

**Holt**: heap usage trajectory → pre-evict 시점 예측.

### 4.4 C-FRAMETIME (frametime stream)

**무엇**: MTL_HUD 의 frame timing 을 columnar 로 누적, 4 stutter 패턴 분류.

| 열 | 타입 |
|---|---|
| frame_id | u32 |
| ts_us | u64 |
| gpu_ms | u16 (0.01ms fixed-point) |
| cpu_ms | u16 |
| queue_wait_ms | u16 |
| shader_compile_ms | u16 |
| present_ms | u16 |

= **20 byte/row**. 1시간 60fps = 4.3MB.

**4 stutter 패턴**:
- spike: single frame > 2× P50
- sustained_drop: 30+ frames < 0.5× P50
- oscillation: std dev > 1.5× mean
- zone_transition: gap > 100ms (frame skip)

**Holt**: 1초 ahead stutter rate → D2/R1 pre-warm signal.

### 4.5 C-SHADER-AGING (APBF heat decay) ⭐ NEW

**무엇**: shader cache 의 각 shader 가 시간이 지나면서 hot → cold 로 변하는 추세 추적.
오래된 cold shader 자동 evict.

**APBF** = Adaptive Prob Bloom Filter — set membership 가 시간에 따라 자연스럽게 decay.

| field | bits |
|---|---|
| shader_hash | 32 |
| last_used_ts | 32 |
| age_bucket | 4 (0=hot, 15=cold) |
| size_kb | 16 |

= **~10 byte/record** + APBF 64KB.

**액션**: age_bucket 14+ 인 shader 자동 evict (디스크 회수).

### 4.6 C-SHADER-SEQ (shader compile sequence) ⭐ NEW

**무엇**: zone 진입 시 컴파일되는 shader 의 *순서 패턴* 을 AC trie 로 학습.
다음 zone 예측 → pre-compile.

| node | 의미 |
|---|---|
| trie root | "Sanctuary" (게임 시작) |
| level 1 | "Kyovashad" (도시) |
| level 2 | "Fractured Peaks" (zone) |
| pattern | shader hash 시퀀스 [h1, h2, h3, ...] |

**예측**: 현재 zone="Kyovashad" → AC trie 가 다음 zone="Fractured Peaks" 시퀀스 알려줌
→ 해당 shader hash 들 미리 컴파일.

= AC trie ~ 1MB / 100 zones / 1000 shader patterns.

### 4.7 C-CORRELATE (multi-source anomaly fusion) ⭐ NEW

**무엇**: stutter 발생 시 동시에 일어난 다른 anomaly 를 매칭. 원인 분석.
"stutter spike at frame 12345 == PSO compile + heap eviction storm".

| 열 | 출처 |
|---|---|
| ts_us | 공통 |
| stutter_label | C-FRAMETIME |
| pso_event | C-PSO (컴파일 발생?) |
| heap_label | C-HEAP (OOM 임박?) |
| draw_burst | C-FRAME (high draw count?) |
| correlation_score | computed |

= **30 byte/row** (sparse).
**액션**: 가장 빈번한 correlation pattern 으로 sprint 우선순위 재조정.

### 4.8 C-SHADER-WARM++ (만점 SHADER 캐시 사전 populate)

**무엇**: R1 의 만점 버전. 단순 SHBF 만이 아니라 columnar index + AC trie + Holt forecast.

| 무기 | 적용 |
|---|---|
| SHBF | "이 shader 가 이미 cache 에 있나?" |
| AC trie | shader path 분류 (vs/ps/cs) |
| columnar index | shader hash → file path 매핑 |
| Holt forecast | next-zone shader 예측 (C-SHADER-SEQ 와 짝) |

**seed source**: C-PSO 가 수집 + 사용자간 공유 + GPTK 버전 매트릭스.

---

## 5. R-list (만점 10/10) — D-list 만점 미달 대신

| 컴포넌트 | A | B | /10 | 비고 |
|---|---|---|---|---|
| **C-DXIL-PERSIST** | 5 | 5 | **10** | DXIL→AIR cache 영구화 (14/15 D-list) |
| **C-SHADER-WARM** | 5 | 5 | **10** | shader cache pre-populate (단순) |
| **C-ROSETTA-PERSIST** | 5 | 4 | 9 | Rosetta cache 영구화 |
| **C-METAL-QUEUE-PRI** | 4 | 5 | 9 | Metal queue priority hint |

→ Sprint 2 후보.

---

## 6. 횡단 (cross-cutting)

### 6.1 데이터 캡처 채널

| 채널 | A | 가용 |
|---|---|---|
| **MTL_HUD stdout 파싱** | 5 | ✅ 즉시 (선호) |
| MetalCapture API (offline) | 5 | ✅ B↓ |
| Metal HUD 내부 plist | 5 | ⚠️ undocumented |
| D3DMetal cache 파일 | 5 | ✅ |
| Rosetta cache (`/var/db/oah/`) | 5 | ⚠️ root |
| LD_PRELOAD shim | 5 | ⚠️ TOS, SIP |
| dtrace | 4 | ⚠️ entitlement |

→ **HUD stdout → cache 파일 → MetalCapture** 우선. LD_PRELOAD = 마지막 수단.

### 6.2 사이드카 + ring 파일

- **외부 사이드카 프로세스** (game 안에 안 들어감) — airgenome probe/harvest 패턴 정합
- 각 컴포넌트별 ring 파일 (자유 schema):
  ```
  forge/genomes-gptk-pso.ring        24B/record
  forge/genomes-gptk-frame.ring      24B/record (per-second)
  forge/genomes-gptk-heap.ring       13B/record (30Hz)
  forge/genomes-gptk-frametime.ring  20B/row
  forge/genomes-gptk-shader-aging.ring 10B/record + 64KB APBF
  forge/genomes-gptk-shader-seq.ring   AC trie ~1MB
  forge/genomes-gptk-correlate.ring    30B/row
  ```
- ring header (16 byte fixed):
  ```
  magic       u32  "AGGB"
  schema_ver  u16  1
  axes        u8   N (자유)
  rec_bytes   u8   M (자유)
  component   u32  ("PSO ", "FRAM", "HEAP", "FTIM", "SAGE", "SSEQ", "CORR")
  gptk_ver    u32  encoded
  record_cnt  u64
  ```

### 6.3 baseline 측정 의무

stock GPTK = 우리 컴포넌트 0개. **baseline 없이 sprint 시작 = 만점 게이트 fail**.

### 6.4 GPTK 버전 호환

- ring header `gptk_ver`
- mismatch → 자동 nuke + re-warm
- `compatibility_matrix.jsonl` 유지

---

## 7. 만점 유지 체크리스트

각 sprint 시작 전:
- [ ] GPTK-specific (Apple wine64 / D3DMetal / Rosetta)? → A 5?
- [ ] 게임 플레이 *중* 효과? → B 5?
- [ ] cache/filter/forecast-ready 로 reproject? → C 5?
- [ ] airgenome filter 자산 재활용? → bonus
- [ ] baseline 측정 가능? → mandatory
- [ ] GPTK 버전 호환 표? → mandatory

미달 sub-task 는 sprint 분리.

---

## 8. 리스크

| 항목 | 영향 | 완화 |
|---|---|---|
| MTL_HUD 출력 포맷 변경 (macOS 업데이트) | 모든 D | parser schema validator + fallback |
| D3DMetal cache 위치/포맷 변경 | shader-warm/dxil-persist | version-keyed cache + 자동 nuke |
| Battle.net anti-cheat 가 cache 변형 거부 | shader-warm | 읽기 전용 cache, 추가만 |
| Apple SIP 강화 시 LD_PRELOAD 봉쇄 | rosetta-vec, metal-queue-pri | dtrace 또는 cache 분석 우회 |
| GPTK 메이저 업데이트 (2.x → 3.0) | 모두 | invalidation matrix |
| D2R/D4 패치로 PSO 세트 변경 | shader-warm seed | 자동 re-seed 트리거 |

---

## 9. 채택 — 만점 8 + R 4

### 만점 D-list (15/15) — Sprint 1 + Sprint 2

1. **C-PSO** PSO state cache (SHBF + state vector + AC trie)
2. **C-FRAME** frame command summary (state vector + Holt forecast)
3. **C-HEAP** Metal heap state (state vector + 4 label + Holt)
4. **C-FRAMETIME** frametime stream (columnar + 4 classifier + Holt)
5. **C-SHADER-AGING** shader heat decay (**APBF** — 신규)
6. **C-SHADER-SEQ** shader compile sequence (**AC trie + Holt** — 신규)
7. **C-CORRELATE** multi-source anomaly fusion (label fusion — 신규)
8. **C-SHADER-WARM++** SHBF + AC trie + columnar + Holt (R1 의 만점 진화)

### R-list (10/10) — Sprint 3

9. **C-DXIL-PERSIST** DXIL→AIR persist (result_cache + columnar)
10. **C-SHADER-WARM** 단순 pre-populate (SHADER-WARM++ 의 시작 단계)
11. **C-ROSETTA-PERSIST** Rosetta cache 영구화 (9/10)
12. **C-METAL-QUEUE-PRI** Metal queue priority hint (9/10)

→ **로드맵 → IMPLEMENTATION_ROADMAP.md**.

---

## 10. 반복 brainstorm (만점 고갈까지) ♾️

본 BRAINSTORM = round 1. 각 sprint 완료 후 새 brainstorm round.

| 라운드 | 시점 | input | 산출 |
|---|---|---|---|
| **round 1** (이 문서) | pre-sprint | filter 자산 + GPTK 문제 카탈로그 | 만점 12 채택 |
| **round 2** | M8 직후 (Post-S1) | S1 측정 + 구현 중 발견한 새 채널 | `docs/POST_SPRINT_1_BRAINSTORM.md` |
| **round 3** | M11 직후 (Post-S2) | S2 측정 + R-list 효과 분석 | `docs/POST_SPRINT_2_BRAINSTORM.md` |
| **round 4** | M15 직후 (Post-S3) | S3 측정 + correlation 결과 | `docs/POST_SPRINT_3_BRAINSTORM.md` |
| **round N** | sprint 완료 시마다 | 직전 sprint output | `docs/POST_SPRINT_N_BRAINSTORM.md` |
| **종료** | round N 새 만점 0 × 3 회 연속 | 고갈 | `docs/EXHAUSTION_<date>.md` |

### 각 라운드 동일 절차

1. *우리 무기* 카탈로그 갱신 (새 filter 추가됐나?)
2. *GPTK 문제* 카탈로그 갱신 (새 표면 발견? 새 게임 추가? 새 macOS 버전?)
3. *무기 × 문제* 매칭 → 새 후보
4. 만점 채점 (A+B+C=15 또는 R 의 A+B=10)
5. 만점 통과 → 다음 sprint 에 추가
6. 만점 미달 → 참고만 (조건 변하면 재평가)
7. 폐기 → 영구 제외

### 종료 조건 (고갈 인정)

- 새 만점 후보 0 개 × 3 sprint 연속
- OR sprint N measured improvement Δ% < 1% (의미 없는 미세 개선)

종료 시: 도구가 스스로 "더 이상 만점 영역 없음" 인정 — `docs/EXHAUSTION_<date>.md` 에 기록 (다음 패러다임 변화 — GPTK 3.0, macOS major, 새 게임 — 시 재시작 트리거 명시).

---

*written 2026-04-30. round 1. 자유 + 기법-우선. 만점 고갈까지 반복.*
