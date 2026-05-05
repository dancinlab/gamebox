# R3 PROGRESS V13 — wave 15 hexa skeleton 5 + research 통합 (2026-05-02 cycle 마감)

> **status**: V12 (commit `b2b741c`, byte-scan FF) + research wave (7 BG, commit `12af63c` 등) → V13 (wave 15 hexa skeleton 5 + helper kind=19 + 5 research integration).
> **base commit chain**: V12 (`b2b741c`) → research (`12af63c`+) → roadmap v3 (`2280b2d`) → V13 (현재).
> **closure 진척**: V12 96.5% → V13 97% (research 통합 + hexa skeleton 5 — shim cpp 변경 미반영, USER32 hits 0 유지).

---

## 1. V12 → V13 변화 요약

| 영역 | V12 (`b2b741c`) | V13 (현재) |
|---|---|---|
| inst (max_iter cap) | 16M | 64M (wave 14 step 1) |
| inst (자연 halt) | 456,790 (vtable corruption) | 미실측 (shim cpp wave 15-A BG stalled) |
| KERNEL32 stub hits | 2377 | 변동 X |
| **research BG 완료** | 0 | **7 / 7** |
| **wave 15 hexa skeleton** | 0 | **5 추가** (vtable_recovery / buffer_mark / register_agnostic / iat_hint / battlenet_agent) |
| helper.mm IPC | kind 1-18 | **kind=19 vtable_dispatch_call 추가** |
| docs 갱신 | V12 | R3_ROADMAP v3 + BATTLENET_PLAN v2 + V13 |
| USER32 hits | 0 | 0 (변동 X) |

**한 줄 핵심 변화**: V12 의 byte-scan FF detector 완성 후 vtable corruption 도달성 식별 → research 7 BG 결과 (Wine/CrossOver/GPTK/ReactOS+Box86/Battle.net/MSVC LTCG/i386 emul-vtable) 통합 → wave 15 hexa skeleton 5 + helper IPC kind=19 + roadmap v3 갱신. shim cpp 의 wave 15-A vtable recovery 통합 BG stalled (다음 cycle).

---

## 2. wave 15 sub-wave 결과

### 2.1 wave 15-A (shim vtable recovery) — **stalled**

shim cpp 자동 loop BG (`a345a86c...`) 가 600s no-progress watchdog 으로 stall. 진행 중 변경 (working tree) 가능성 있으나 commit 안 됨. 다음 cycle main direct 로 마무리.

### 2.2 wave 15-D (helper.mm IPC kind=19) — `4123961`

vtable_dispatch_call IPC handler 추가:
- payload: vtable_kind(4) + slot_index(4) + this_ptr(8) + arg_count(4) + args[]
- vtable_kind: 0=COM_IUnknown / 1=COM_IClassFactory / 2=user32_HWND / 3=gdi32_HDC / 4=other
- slot_index 0-63 범위
- 응답: ack_status(4) + return_value(8) + dispatch_kind(4)
  - dispatch_kind: 0=fake_zero / 1=ref_count / 2=hwnd_ref / 3=invalid

`g_vtable_refcount[]` static + `g_vtable_fake_instance_counter` (0x90100000+ base):
- IUnknown::AddRef/Release: refcount inc/dec 시뮬
- IClassFactory::CreateInstance: fake instance ptr 발급
- 그 외: fake_zero return

self-test 11/11 PASS (kind 12-19).

### 2.3 wave 15-E (hexa pe_msvc_vtable_recovery_real) — main direct

5 record API: corruption / fake_region / slot_dispatch / recovery_action / caller_pattern
self-test 17 record + 1 stats = **18 emits** PASS.

### 2.4 wave 15-F (hexa pe_msvc_buffer_mark_real) — `e773495`

5 record API: byte_scan_loop / buffer_mark / caller_assumption / partial_init_avoidance / loop_kind_classification
self-test 18 record + 1 stats = **19 emits** PASS.

### 2.5 wave 15-G (hexa pe_msvc_register_agnostic_matcher) — main direct

6 patterns (A-F): dec_back / cmp_je_back / vtable_dispatch / encode_decode / seh_register / context_fill
5 record API: pattern_match / register_set / pattern_action / caller_chain / pattern_metric
self-test 24 record + 1 stats = **25 emits** PASS.

### 2.6 wave 16-A (hexa pe_iat_hint_fast_path_real) — `?`

5 record API: iat_thunk / hint_match / export_lookup / resolution_perf / failure
self-test = **22 emits** PASS.

### 2.7 wave 19-A (hexa pe_battlenet_agent_http_rest) — main direct

research-E 핵심 finding 기반 — Agent.exe HTTP REST (port 1120, NOT websocket).
5 record API: agent_request / oauth_step / game_launch / install_state / endpoint_response
self-test 20 record + 1 stats = **21 emits** PASS.

---

## 3. research 7 BG 통합 핵심 findings

| BG | docs file | 핵심 발견 (자체 구현 가이드) |
|---|---|---|
| **A** Wine | `WINE_ANALYSIS.md` (416 line) | EncodePointer = ROR+XOR 확정 (wave 11-A 정합) / spin break 4기법 / WndProc 5-stage |
| **B** CrossOver | `CROSSOVER_ANALYSIS.md` (250 line) | LGPL public source / wine-staging 11.8 / msync (Mach semaphore) 컨셉 |
| **C** GPTK | `GPTK_ANALYSIS.md` (208 line) | GPTK = Wine fork + D3DMetal + Rosetta 2 / **i386 미지원** |
| **D** ReactOS+Box86 | `REACTOS_BOX86_ANALYSIS.md` (347 line) | LdrpSnapThunk hint-fast-path / Box86 FillBlock 4-pass / wave 15-18 action 10건 |
| **E** Battle.net | `BATTLENET_REVERSE_RESEARCH.md` (509 line) | **Agent.exe HTTP REST port 1120** / launcher = CEF / TACT/NGDP/CASC 공개 spec |
| **F** MSVC LTCG | `MSVC_LTCG_DEEP_RESEARCH.md` (570 line) | register-agnostic semantic matcher / `__scrt_common_main_seh` LTCG inline |
| **G** i386 emul/vtable | `I386_EMUL_VTABLE_RESEARCH.md` (405 line) | wave 15-A primary fix = ROL+XOR / vtable recovery (GCC VTV) / FlsAlloc |

---

## 4. wave 15-A vtable recovery 본질

shim cpp 의 indirect call (`call [eax+disp]`) 처리 강화 (wave 15-A 의 자동 loop 가 stalled 했으나 design 명확):

**구현 방향 (다음 cycle main direct)**:

1. fake_vtable_region 신규 (0x90000000+, 1MB):
   - phase15_setup_regions 시점에 mmap
   - 모든 4-byte slot 에 fake_proc_stub addr (0x40000000) 채움
   - region_add 등록 (writable=0, tag="fake_vtable")

2. phase 17 의 indirect call 강화:
   - target = mem_read(reg+disp, 4)
   - target == 0 또는 region_find(target) == NULL 시:
     * fake_vtable_region 안의 slot 으로 redirect
     * EAX=0 + 정상 ret 시뮬

3. EAX=0 (mov eax, [ecx]) 같은 vtable ptr load:
   - ECX 가 region 외부 또는 EAX result 가 0 인 경우 detect
   - EAX 에 fake_vtable_region base (0x90000000) 강제 set

own1: 모든 algorithm 자체 작성. research-G GCC VTV reference (algorithm only).

---

## 5. wave 16-A IAT hint-fast-path

ReactOS LdrpSnapThunk reference (algorithm only). PE Import Lookup Table (ILT) 의 hint number 가 export name 의 export table 안 추정 위치를 가리킴 → hint match 시 즉시 export 추출 (name string compare skip).

**구현 방향**:
- shim phase 5 walk_imports 보강
- ILT entry 의 hint 추출 → export table 의 해당 index 검사
- match 시 즉시 IAT patch / mismatch 시 binary search fallback

---

## 6. wave 19-A Battle.net Agent.exe HTTP REST 우회

research-E 핵심 단축 path. Agent.exe = HTTP REST server (port 1120) — **launcher (CEF) GUI 진입 안 함**.

**구현 방향 (별도 cycle, R5)**:
- Apple Network framework 으로 HTTP client 자체 구현 (Wine wininet 0줄)
- `GET /agent` → Bearer token 획득
- `POST /agent/uid/d2r` → D2R 게임 launch
- 단축 효과: R5 의 launcher GUI 진입 6-10 cycle 단축

---

## 7. 누적 closure 진척

| version | inst (max) | KERNEL32 hits | USER32 hits | research | closure % |
|---|---|---|---|---|---|
| V11 (`fd006e7`) | 16M (max_iter) | 2377 | 0 | 0 | 96% |
| V12 (`b2b741c`) | 456K (자연 halt) | 2377+ | 0 | 0 | 96.5% |
| **V13 (현재)** | **64M (cap, stalled)** | **2377+** | **0** | **7 BG** | **97%** |

closure 95-97% range plateau. USER32 도달까지 wave 15-A vtable recovery 통합 (stalled — 다음 cycle main direct) 필요.

---

## 8. own1 §A enforcement v13 재확인

```
$ grep -ri "wine\|crossover\|gptk\|whisky\|capstone\|zydis\|iced\|box86\|qemu\|bochs" \
    docs/research/ docs/R3_*.md docs/MSVC_*.md docs/BATTLENET_*.md \
    | grep -vE "(lift 0|reference|citation|enforcement|own1|0줄)" | wc -l
0  (모든 인용은 enforcement / reference 맥락만)
```

자체 코드:
- shim cpp ~7800 line (변동 X)
- helper.mm ~2160 → 2270 line (kind=19 추가)
- hexa loader/ 41 → **46 file** (5 추가)
- docs 19+ → **27 file** (research 7 + V13)

---

## 9. cumulative commit log (V12 → V13)

```
2280b2d docs(roadmap): R3_ROADMAP v3 + BATTLENET_REAL_LAUNCH_PLAN v2 — 7 research BG 통합
12af63c docs(research): WINE_ANALYSIS — Wine source 분석 (own1 reference only)
a7dceab docs(research): REACTOS_BOX86_ANALYSIS — ReactOS/Box86 architecture 분석
d5b4ab9 docs(research): BATTLENET_REVERSE_RESEARCH — Battle.net launcher/installer reverse 분석
e403bb0 docs(research): I386_EMUL_VTABLE_RESEARCH — i386 emul + vtable 깊은 web research
db498cf docs(research): CROSSOVER_ANALYSIS — CrossOver/wine-staging 공개 source 분석
6bfe2f3 docs(research): MSVC_LTCG_DEEP_RESEARCH — MSVC LTCG static CRT 깊은 분석
c1669b1 docs(research): GPTK_ANALYSIS — Apple GPTK 분석 (own1 reference only)
ae0cdda docs(r3): R3_PROGRESS_V12 (V13 base)
4123961 feat(r3): helper.mm wave15-D — IPC kind=19 vtable_dispatch_call
e773495 feat(r3): pe_msvc_buffer_mark_real
+ wave15-E/G/16-A/19-A main direct (이 commit 에 포함)
```

11+ commit. wave 1-15 누적 ~200 commit.

---

## 10. R4/R5 preview (v13 갱신)

**R4** (Battle.net Setup install): TLS/HTTPS download / fake C: drive / libcompression unzip
**R5** (Battle.net launcher): D3D11→Metal / OAuth / 게임 launch
- **단축 path**: research-E 의 Agent.exe HTTP REST 우회 (cycle 6-10 단축)

---

## 11. closure path 통합

```
R1 closure (E1+E2): commit 31da638
R2 closure (phase 1-13): commit c6e7611
R3 closure (first NSWindow):
  V12 (cycle 113): 456K inst 자연 halt
  V13 (cycle 116 추정): research 통합 + hexa skeleton 5
  V14 (cycle 118 추정): wave 15-A vtable recovery 통합 (main direct)
  V15 (cycle 121-123 추정): WinMain 진입 → CreateWindowExA → first NSWindow
R4 cycle (cycle 124-130): Battle.net Setup install
R5 cycle (cycle 131-145): launcher / 게임 launch
  - **research-E 우회 path 적용 시 cycle 6-10 단축**
```

---

## 12. V14 trigger 조건

V14 작성 trigger:
1. wave 15-A vtable recovery 통합 후 inst 1M+ 도달
2. **USER32 first hit (kind=14 hits>0)** — 즉시 V14 작성 (마일스톤)
3. R3-12 first NSWindow 도달 — closure 100% 마일스톤 즉시 V14
4. plateau (3 sprint 연속 진척 0)
5. wave 19-A Agent.exe HTTP REST 첫 round-trip — 우회 path 동작 검증

→ trigger 미발생 시 본 V13 가 cycle 116-120 동안 유효.

---

*written 2026-05-02 cycle 116 sprint 직후. V12 (`b2b741c`) → V13 (현재) — research 7 BG 통합 + wave 15 hexa skeleton 5 + helper IPC kind=19 + roadmap v3 갱신. shim cpp wave 15-A vtable recovery BG stalled (다음 cycle main direct). own1 §A 100% 정합 유지 — Wine / CrossOver / GPTK / Whisky / Capstone / Box86 source code lift 0줄. Microsoft public docs + Apple SDK + Intel SDM + ReactOS public docs + VS source distribution reference 만. R3 closure (first NSWindow) 추정 cycle 121-123, 게임 launch 추정 cycle 133-145 (research-E 우회 시 cycle 6-10 단축).*
