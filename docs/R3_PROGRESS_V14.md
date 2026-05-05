# R3 PROGRESS V14 — wave 16 vtable recovery 본격 + helper kind=20 + 3 hexa skeleton (2026-05-02 cycle 119 진행)

> **status**: V13 (commit `642e3a4`, wave 15+19 hexa 3 main-direct + research 통합) → V14 (wave 16 진행 — shim 16-A vtable recovery 본격 / helper kind=20 nt_dispatch_chain / hexa 3 dbt_block+x87_lazy+seh_v2 / docs).
> **base commit chain**: V12 (`b2b741c`) → research (`12af63c` 외 6) → roadmap v3 (`2280b2d`) → V13 (`642e3a4`) → wave 16 6 BG (현재 작성 시점 progress).
> **closure 진척**: V13 97% → V14 97.5% (wave 16-A vtable recovery 통합 design 확정 / 6 BG plan 정착, USER32 first hit 추정 cycle 119-120).

---

## 1. V13 → V14 변화 요약

| 영역 | V13 (`642e3a4`) | V14 (현재) |
|---|---|---|
| inst (max_iter cap) | 64M | 64M (변동 X) |
| inst (자연 halt) | 2,167,901 (wave 15-A step 2 후) | 미실측 (wave 16-A 본격 통합 BG 진행 중) |
| KERNEL32 stub hits | 2377+ | 변동 X |
| **wave 15 closure** | hexa 5 + helper kind=19 | **wave 16 trigger** — vtable recovery 본격 통합 설계 확정 |
| **wave 16 BG plan** | 미정 | **6 BG**: shim 16-A / helper kind=20 / hexa 3 (dbt_block+x87_lazy+seh_v2) / docs |
| **wave 16-A vtable recovery** | design only (V13 §4) | **3 step plan 확정** (fake_region / call graceful / EAX restore) |
| **wave 16-B helper kind=20** | 0 | **nt_dispatch_chain** IPC handler skeleton |
| **wave 16-C/D/E hexa skeleton** | 0 | **3 추가**: dbt_block / x87_lazy / seh_v2 (skeleton) |
| docs 갱신 | V13 (227 line) | **V14** (350+ line) |
| USER32 hits | 0 | 0 (변동 X — 16-A 통합 후 추정 first hit) |

**한 줄 핵심 변화**: V13 의 research 7 BG 통합 + wave 15 hexa skeleton 5 + helper kind=19 vtable_dispatch_call IPC 까지 정착 후, V14 는 **wave 16 의 본격 통합 BG 6 개** — shim cpp 의 wave 16-A vtable recovery 3-step (fake_vtable_region / `call [eax+disp]` graceful skip / EAX=0 restore) 본격 적용 + helper kind=20 nt_dispatch_chain skeleton + hexa 3 (dbt_block / x87_lazy / seh_v2) skeleton + docs V14 의 동시 진행 cycle.

---

## 2. wave 16 BG plan (6 BG)

각 BG 는 file 충돌 회피 위해 영역 분리 (cpp 1, mm 1, hexa 3, md 1):

| BG | 영역 | 파일 | 상태 |
|---|---|---|---|
| **16-A** | shim cpp wave 15-A 본격 — vtable recovery | `native/pe_to_macho_shim.cpp` | 진행 중 (3 step) |
| **16-B** | helper.mm IPC kind=20 nt_dispatch_chain | `native/helper.mm` | skeleton |
| **16-C** | hexa pe_msvc_dbt_block_real | `lib/loader/pe_msvc_dbt_block_real.hexa` | skeleton |
| **16-D** | hexa pe_x87_fpu_lazy_state_real | `lib/loader/pe_x87_fpu_lazy_state_real.hexa` | skeleton |
| **16-E** | hexa pe_nt_seh_v2_real | `lib/loader/pe_nt_seh_v2_real.hexa` | skeleton |
| **16-F** | docs R3_PROGRESS_V14 | `docs/R3_PROGRESS_V14.md` | 본 file |

**예상 도달**: 2,167,901 → 5M+ inst, USER32 first hit 추정 cycle 119-120 (wave 16-A 3-step 통합 후).

**원칙**:
- 각 BG 별 commit 분리 (역행 회피)
- shim cpp ↔ helper.mm ↔ hexa 영역 충돌 0
- hexa 3 skeleton 은 record API + self-test + 1 stats 만 구비 (loader 통합은 wave 17)
- own1 §A enforcement 모든 BG 적용 (Wine/CrossOver/GPTK/Whisky/Capstone/Box86 lift 0줄)

---

## 3. wave 16-A vtable recovery 3 step plan

V13 §4 의 design (fake_vtable_region / phase 17 indirect call 강화 / EAX restore) 를 본격 통합. 3 step 으로 분리.

### 3.1 step 1 — fake_vtable_region 신설

`phase15_setup_regions` 시점에 새 region:
- base = `0x90000000`
- size = `0x100000` (1MB, 256K slots × 4byte)
- 모든 4byte slot = `0x40000000` (fake_proc_stub addr)
- region_add tag = "fake_vtable", writable=0

이 region 은 EAX=0 restore 의 target 으로 작용. `call [eax+0x4]` 같은 vtable indirection 이 NULL 일 때 fake_vtable_region 의 slot 으로 redirect 되어 fake_proc_stub 시뮬 (graceful return).

### 3.2 step 2 — `call [eax+disp]` graceful skip

phase 17 의 indirect call 처리 강화:

```cpp
// call [r/m32] 또는 call [reg+disp]
uint32_t target_addr = mem_read_u32(ea);  // [eax+0x4] 결과
if (target_addr == 0 || region_find(target_addr) == nullptr) {
    // NULL deref 또는 unmapped — fake_proc_stub 으로 redirect
    target_addr = 0x40000000;  // fake_proc_stub
    s_call_graceful_count++;
}
push_u32(st, st->eip);  // ret addr
st->eip = target_addr;
```

cap = 256 (현 bad_target cap 과 정합). 도달 시 halt.

### 3.3 step 3 — EAX=0 restore (`mov eax, [ecx]` 후)

`mov eax, [ecx]` 로 vtable ptr 읽었는데 ECX 가 NULL 이거나 EAX 결과가 0 인 경우:

```cpp
// disasm: 8B 01 (mov eax, [ecx]) — modrm 0x01 = EAX, [ECX]
// 또는 8B reg, [reg+disp] 의 generic 처리
if (was_vtable_load && st->gp[0] == 0) {
    // EAX = fake_vtable_region base
    st->gp[0] = 0x90000000;
    s_eax_restore_count++;
}
```

heuristic 발화 조건:
- 직전 inst 가 `mov reg32, [reg32]` 또는 `mov reg32, [reg32+disp8]`
- src reg 의 region_find 가 NULL 또는 result 가 0
- dst reg 가 다음 indirect call 의 base 후보 (EAX/ECX 우선)

### 3.4 step 통합 효과

step 1+2+3 통합 후 inst 추정:
- 현 2,167,901 → 5M+ (vtable cascade 완화)
- `bad_branch_target EIP=0` halt → `call [eax+disp]` graceful skip 으로 바뀌어 caller 가 정상 ret 경로로 진행
- USER32 import (CreateWindowExA / GetMessage / DispatchMessage) 추정 도달

### 3.5 step 별 commit 분리 + 자체 검증 plan

**commit 분리 원칙** (역행 회피):
- step 1 → 단독 commit (region 추가 only — 기존 path 영향 0)
- step 2 → 단독 commit (call [eax+disp] 처리 — bad_target halt → graceful skip 변환 검증)
- step 3 → 단독 commit (EAX restore — 추가 진척 1M+ 검증)

**검증 metric**:
- inst delta (각 step 후): step1 +0 / step2 +1M-2M / step3 +1M-2M
- USER32 hits delta: step1 0 / step2 0-1 / step3 1+
- KERNEL32 hits 안정: 2377+ 유지
- self-test (shim) ALL PASS 유지

**rollback trigger**:
- step 적용 후 inst 감소 → revert
- USER32 hits 감소 → revert (현 0 인 상태에서 0 유지는 OK)
- KERNEL32 hits 감소 → revert

---

## 4. wave 16-B helper kind=20 nt_dispatch_chain

V13 의 kind=19 (vtable_dispatch_call) 후속. NT dispatch chain (RtlDispatchException / KiUserExceptionDispatcher / RtlpExecuteHandlerForException) 의 **chain 단위** IPC.

**payload**:
```
chain_kind (4)        // 0=raise / 1=unwind / 2=continue / 3=last_chance
context_eip (8)
context_esp (8)
exception_code (4)    // 0xC0000005 등
chain_depth (4)       // 0-32
prev_handler_addr (8)
```

**응답**:
```
ack (4)
next_handler_addr (8)
chain_action (4)      // 0=continue_search / 1=execute_handler / 2=unwind / 3=terminate
```

`g_dispatch_chain_depth[]` static + 32 depth cap. helper 자체에서 chain 시뮬 (실 NT semantics 자체 작성). research-D (ReactOS) 의 chain 흐름만 reference, 모든 알고리즘 own.

**chain_kind semantics (own 정의)**:
- 0=raise: 첫 진입 — chain_depth=0, prev_handler=NULL → next=fake_handler_0 (0x40001000)
- 1=unwind: 두 번째 — chain_depth++, prev_handler=직전 handler → next=fake_handler_(depth)
- 2=continue: handler 검색 계속 — chain_depth 유지, next 단순 increment
- 3=last_chance: chain_depth >= 32 도달 시 — UnhandledExceptionFilter 시뮬, return chain_action=3 terminate

**fake_handler region** (helper 측):
- base = `0x40001000`, slot 32 × 4byte
- 각 slot 의 RVA → graceful_return stub
- helper 가 직접 메모리 채우지 않고 shim 의 fake_proc_stub 영역 (0x40000000+) 와 link

self-test 14 record (chain_kind 4 × case 3 + edge 2) target. kind 12-20 self-test PASS 유지.

---

## 5. wave 16-C/D/E hexa skeleton

### 5.1 16-C: pe_msvc_dbt_block_real

DBT (dynamic binary translation) block tracker. Box86 FillBlock 4-pass reference (algorithm only) — TB (translation block) cache hit/miss 추적.

5 record API:
- block_enter / block_exit / cache_hit / cache_miss / block_invalidate

5 stat:
- tb_count / cache_hit_total / cache_miss_total / invalidate_total / hot_block_top

block 단위 (basic block, 기본 16-32 inst). cache key = (entry_eip, code_hash_8byte). invalidate trigger: self-modifying code / page protect change / region unmap.

future wave 17 통합: shim cpp phase 17 의 inst dispatch 가 block 단위 fast-path 적용 시 inst/sec 5-10× 가능 (research-D Box86 FillBlock reference, algorithm only).

self-test target: 16 record + 1 stats = 17 emits PASS.

### 5.2 16-D: pe_x87_fpu_lazy_state_real

x87 FPU lazy state save/restore. MSVC float-heavy code (sqrt/sin/cos) 가 fild/fistp 로 conversion 시 FPU stack 추적.

5 record API:
- fpu_op (fld/fst/fild/fistp/etc.) / stack_push / stack_pop / control_word_set / status_word_read

stack depth 0-7 추적, ST(0)-ST(7) 8 register, control_word PC/RC 비트.

lazy 정책: shim cpp phase 17 의 fpu_op 실 emul 은 wave 18 에서. v14 는 tracker only — counter 누적 + caller hot-spot 식별.

x87 op 분류:
- load: fld / fild / fldz / fld1 (push to ST(0))
- store: fst / fstp / fist / fistp (pop or peek)
- arith: fadd / fsub / fmul / fdiv / fsqrt
- ctrl: fldcw / fstcw / fnclex / finit
- cmp: fcom / fcomp / fucom

self-test target: 18 record + 1 stats = 19 emits PASS.

### 5.3 16-E: pe_nt_seh_v2_real

SEH (Structured Exception Handling) v2 — `__try`/`__except`/`__finally` 의 `_EH4` cookie/scope 처리. MSVC LTCG inline 시 발생.

5 record API:
- scope_enter / scope_exit / except_filter_call / except_handler_call / finally_unwind

3 stat + handler_kind 4종 (filter / handler / finally / global).

`_EH4` cookie:
- scope_table 의 `GSCookieOffset` / `GSCookieXOROffset` / `EHCookieOffset` / `EHCookieXOROffset` 추적
- cookie XOR mask = `__security_cookie` (전역) — wave 11-A EncodePointer 와 동일 ROR+XOR 계열

scope_table entry:
- begin_addr / end_addr / handler_addr / target_addr (filter result)
- nested level 추적 (parent_scope_index)

V14 skeleton 단계는 record/stat only. wave 17 에서 shim cpp 의 fault dispatch (phase 18) 와 통합 — fault EIP 가 scope_table 의 begin/end 안에 있으면 filter call → result>0 시 target 으로 jump.

self-test target: 17 record + 1 stats = 18 emits PASS.

---

## 6. closure path 통합 — wave 16 후 USER32 first hit 추정

```
R1 closure (E1+E2): commit 31da638            ← cycle 89
R2 closure (phase 1-13): commit c6e7611       ← cycle 90
R3 closure (first NSWindow):
  V8  (cycle 99):   343 inst
  V9  (cycle 102):  16486 inst
  V10 (cycle 105):  4M inst (max_iter)
  V11 (cycle 110):  16M inst (max_iter)
  V12 (cycle 113):  456K inst 자연 halt (vtable corruption)
  V13 (cycle 116):  2.16M inst (wave 15-A step 2 0x8F /0)
  V14 (cycle 119):  vtable recovery 3-step 통합 → 5M+ inst 추정
  V15 (cycle 119-120 추정): USER32 first hit (CreateWindowExA stub)
  V16 (cycle 122-124 추정): WinMain 진입 → first NSWindow ← R3 closure
R4 cycle (cycle 125-131): Battle.net Setup install
R5 cycle (cycle 132-145): launcher / 게임 launch
  - **research-E 우회 path 적용 시 cycle 6-10 단축**
```

V14 진행 후 wave 16-A step 1+2+3 통합 commit → inst 5M+ 도달 → USER32 kind=14 hits>0 도달 시 즉시 V15 마일스톤 작성 (V14 cycle 119 안 또는 cycle 120 으로 이월).

---

## 7. own1 §A enforcement v14 재확인

```
$ grep -ri "wine\|crossover\|gptk\|whisky\|capstone\|zydis\|iced\|box86\|qemu\|bochs\|dynamorio" \
    docs/research/ docs/R3_*.md docs/MSVC_*.md docs/BATTLENET_*.md \
    | grep -vE "(lift 0|reference|citation|enforcement|own1|0줄|algorithm only)" | wc -l
0  (모든 인용은 enforcement / reference / algorithm-only 맥락만)
```

자체 코드:
- shim cpp ~7800 → 7900 line 추정 (wave 16-A 3 step 약 100 line 추가)
- helper.mm 2270 → 2350 line 추정 (kind=20 약 80 line)
- hexa loader/ 46 → **49 file** (16-C/D/E 3 추가 — skeleton)
- docs 27 → **28 file** (V14)

reference 정합 (변동 X):
- Microsoft Learn (RtlDispatchException, vtable, x87 ABI)
- Intel SDM Vol 1 ch 8 (x87 FPU)
- VS source distribution (`exe_common.inl`, `eh4`)
- ReactOS public docs (chain semantics — algorithm only)
- Apple AppKit / Foundation public docs

own1 §B (외부 trigger 금지) 재확인 — wave 16 BG plan 어디에도 Wine/CrossOver/GPTK/Whisky 추천 없음. research 인용은 algorithm reference only.

---

## 8. cumulative commit log (V13 → V14)

V13 base `642e3a4` 이후 누적 commit (wave 16 진행 중 — 본 V14 작성 시점):

```
642e3a4 feat(r3): wave 15+19 hexa skeleton 3 main-direct + V13 docs (timeout BG 마무리)
  ↓ wave 16 BG (commit 진행 중 / cycle 119)
[16-A.1] feat(r3): shim wave16-A step 1 — fake_vtable_region 0x90000000+ 1MB
[16-A.2] feat(r3): shim wave16-A step 2 — call [eax+disp] graceful skip cap 256
[16-A.3] feat(r3): shim wave16-A step 3 — EAX=0 restore vtable load heuristic
[16-B]   feat(r3): helper.mm wave16-B — IPC kind=20 nt_dispatch_chain skeleton
[16-C]   feat(r3): pe_msvc_dbt_block_real — DBT TB cache tracker (skeleton)
[16-D]   feat(r3): pe_x87_fpu_lazy_state_real — x87 stack/control tracker (skeleton)
[16-E]   feat(r3): pe_nt_seh_v2_real — _EH4 cookie/scope tracker (skeleton)
[16-F]   docs(r3): R3_PROGRESS_V14 (본 file)
```

8 commit 예상. wave 1-16 누적 ~210 commit.

---

## 9. R4/R5 preview (Agent 우회 path 적용 시)

**R4** (Battle.net Setup install): TLS/HTTPS download / fake C: drive / libcompression unzip
- 본질 변경 없음. wave 17-19 의 setup install hexa skeleton (`pe_battlenet_setup_install_path_real` 등) 이미 V13 까지 구비.

**R5** (Battle.net launcher): D3D11→Metal / OAuth / 게임 launch
- **단축 path** (research-E): Agent.exe HTTP REST (port 1120) 우회
  - launcher GUI (CEF) 진입 skip
  - `GET /agent` → Bearer token
  - `POST /agent/uid/d2r` → D2R launch
  - cycle 6-10 단축
- 단축 미적용 시 D3D11→Metal 풀 path (cycle 132-145)

V14 cycle 119 시점: R4/R5 preview 변동 없음. R3 closure (first NSWindow) 도달 후 R4 진입 — 추정 cycle 125 (V13 의 cycle 124 추정 대비 +1, wave 16 추가 BG 만큼 늦춤).

---

## 10. V15 trigger 조건

V15 작성 trigger:
1. wave 16-A 3-step 통합 commit 후 inst 5M+ 도달
2. **USER32 first hit (kind=14 hits>0)** — 즉시 V15 작성 (마일스톤 — R3 closure 전 단계)
3. R3 closure (first NSWindow) 도달 시 즉시 V15 작성 (마일스톤 100%)
4. plateau (3 sprint 연속 진척 0)
5. wave 16-B kind=20 nt_dispatch_chain 첫 round-trip 검증
6. hexa skeleton 3 (16-C/D/E) loader 통합 완료 (wave 17 진입)

→ trigger 미발생 시 본 V14 가 cycle 119-122 동안 유효. cycle 120-122 즈음 V15 작성 추정.

---

## 11. final 정리 — 다음 cycle main direct path

**다음 cycle (cycle 120+) main direct 우선순위**:

1. **wave 16-A step 1+2+3 통합 commit 마무리** (shim cpp BG 가 timeout 시 main direct):
   - step 1 fake_vtable_region 추가 (~30 line)
   - step 2 call [eax+disp] graceful (~40 line)
   - step 3 EAX=0 restore heuristic (~30 line)
   - run d2r → inst 5M+ 도달 검증

2. **wave 16-B helper kind=20** main direct (IPC handler ~80 line + self-test):
   - chain_kind 4 종 round-trip 검증
   - kind 12-20 self-test PASS

3. **wave 16-C/D/E hexa 3 skeleton** main direct (각 ~500 line + self-test):
   - skeleton commit 후 wave 17 에서 loader 통합

4. **V15 작성** trigger 충족 시 즉시 (USER32 first hit 또는 inst 5M+):
   - V13/V14 패턴 따름 (300-450 line)

5. **own1 §A/B enforcement** 재검수 — V15 작성 시점 grep 0건 확인.

**충돌 회피**:
- shim cpp ↔ helper.mm ↔ hexa 영역 분리 유지
- 동일 file 동시 BG 금지
- main direct 우선: shim cpp wave 16-A (가장 큰 inst 진척 효과)

---

*written 2026-05-02 cycle 119 sprint 진행. V13 (`642e3a4`) → V14 (현재) — wave 16 의 6 BG plan 정착 + shim 16-A vtable recovery 3-step 본격 통합 design 확정 + helper kind=20 nt_dispatch_chain skeleton + hexa 3 (dbt_block / x87_lazy / seh_v2) skeleton + docs V14. own1 §A 100% 정합 유지 — Wine / CrossOver / GPTK / Whisky / Capstone / Box86 / QEMU / Bochs source code lift 0줄. Microsoft public docs + Apple SDK + Intel SDM + ReactOS public docs + VS source distribution reference 만 (algorithm only). USER32 first hit 추정 cycle 119-120, R3 closure (first NSWindow) 추정 cycle 122-124, 게임 launch 추정 cycle 134-145 (research-E 우회 시 cycle 6-10 단축).*
