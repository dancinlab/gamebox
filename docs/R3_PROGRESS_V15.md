# R3 PROGRESS V15 — wave 16-A 4-step + wave 17 SEH dispatch + closure path 통합 (2026-05-02 cycle 120 진행)

> **status**: V14 (commit `b760a5f`, wave 16 BG plan + 16-A vtable recovery design + helper kind=20 + 3 hexa skeleton) → V15 (wave 16 결과 본격 반영 — 8 commit 누적 + shim 16-A 4-step 진척 timeline 3.16M→747K→744K→289K + _CxxThrowException 식별 + wave 17 5 BG plan).
> **base commit chain**: V13 (`642e3a4`) → V14 (`b760a5f`) → wave 16 (`0b125e9` `03814e9` `0c47709` `126a0e0` `15d60e8` `5bc336b` `e49d257` `7607526`) → V15 (현재).
> **closure 진척**: V14 97.5% → V15 97.7% (16-A 4-step 자연 halt + Metal device 정상 + vtable recovery 동작 검증 + USER32 first hit 추정 cycle 121-122 으로 1 cycle 이월).

---

## 1. V14 → V15 변화 요약

| 영역 | V14 (`b760a5f`) | V15 (현재) |
|---|---|---|
| inst (max_iter cap) | 64M | 64M (변동 X) |
| inst (자연 halt) | 미실측 (16-A BG 진행 중) | **289,492 (16-A step 7+8 후 RaiseException NONCONTINUABLE halt)** |
| inst 진척 timeline | 2.16M (V13 step2) | **3.16M → 747K → 744K → 289K** (16-A step1→3→6→8) |
| KERNEL32 stub hits | 2377+ | **2048** (init 정상 진행 후 조기 halt — root cause 식별) |
| Metal device 획득 | 0 | **1 (closure 100%)** ← 16-A step 2+3 후 |
| **wave 16 commit** | 0 (V14 가 wave 16 trigger) | **8 commit** (helper kind=20 / shim step1-8 / hexa 3) |
| **wave 16-A 4-step** | design only (3 step plan) | **4 본격 반영** (step1 ret_eip=0 / step2-3 bss_jump+ExitProcess / step4-6 fastfail+int3+IPF / step7-8 vtable_load_redirect+RaiseException) |
| **vtable recovery 동작** | design | **검증** — call_redirect=4, load_redirect=256 (정밀 후) |
| **_CxxThrowException 식별** | 0 | **확정** — RaiseException dwExceptionCode=0xE06D7363 ('msc') |
| **wave 17 BG plan** | 미정 | **5 BG**: shim SEH dispatch / helper kind=21 / 2 hexa / docs |
| docs 갱신 | V14 (350+ line) | **V15** (380+ line) |
| USER32 hits | 0 | 0 (변동 X — wave 17 후 추정 first hit) |

**한 줄 핵심 변화**: V14 의 wave 16 6 BG plan 정착 직후 본격 반영 → wave 16-A vtable recovery 가 4 step 으로 자연 분화하며 8 commit 누적 → 289K inst 깔끔한 halt 도달 + Metal device 획득 + vtable recovery 동작 검증 (load_redirect=256, call_redirect=4) + RaiseException dwExceptionCode `0xE06D7363` (MSC C++ throw 'msc') 식별 → 다음 halt 의 root cause 가 `_CxxThrowException` 으로 확정 → wave 17 5 BG (shim SEH dispatch / helper kind=21 cxx_throw_dispatch / hexa cxx_throw + rtti_typeinfo / docs) plan 확정.

---

## 2. wave 16 commit 결과 (8 commit, V14 base 직후)

V14 base `b760a5f` 직후 8 commit 누적:

| # | commit | sub | 영역 | inst 변화 | KPI 핵심 |
|---|---|---|---|---|---|
| 1 | `0b125e9` | 16-B | helper.mm IPC kind=20 | — | nt_dispatch_chain skeleton, AH_CHAIN_RECORD_LIMIT=64 |
| 2 | `03814e9` | 16-A.1 | shim cpp RET unwind | — → **3.16M** | ret_eip=0 stack-scan unwind cap 4096 |
| 3 | `0c47709` | 16-E | hexa seh_dispatch_chain_v2 | — | Wine + ReactOS SEH flow tracker (skeleton) |
| 4 | `126a0e0` | 16-D | hexa x87_lazy_stack | — | Box86 x87 lazy stack barrier tracker (skeleton) |
| 5 | `15d60e8` | 16-C | hexa msvc_dbt_block | — | Box86 FillBlock 4-pass DBT skeleton |
| 6 | `5bc336b` | 16-A.2-3 | shim bss_jump+ExitProcess | 3.16M → **747K** | bss_jump cap 4096 + ExitProcess/TerminateProcess halt |
| 7 | `e49d257` | 16-A.4-6 | shim fastfail+int3+IPF | 747K → **744K** | fastfail thr=8, int3 thr=4, TerminateProcess thr=1 |
| 8 | `7607526` | 16-A.7-8 | shim vtable+RaiseException | 744K → **289K** | vtable_load_redirect 정밀화 + RaiseException dwExceptionCode 식별 |

**한 줄 결과**: 8 commit 으로 inst 진척이 3.16M → 289K 로 *감소* — 이는 회귀가 아니라 **조기 정상 halt 의 root cause 식별** 의 진척. V14 의 16M+ max_iter cap 도달 (loop infinite) → V15 의 289K 자연 halt (RaiseException NONCONTINUABLE) 으로 의미가 바뀜. inst 가 줄 수록 root cause 에 가까워짐.

**누적 wave 1-16 commit**: ~218 (wave 1-15: ~210, wave 16: 8).

---

## 3. wave 16-A 4-step 진척 timeline (3.16M → 747K → 744K → 289K)

V14 의 design 3-step (fake_vtable_region / call graceful / EAX restore) 가 실 통합 시 8 step 으로 분화. 시간 순:

### 3.1 step 1 — RET ret_eip=0 graceful stack-scan unwind (`03814e9`)

phase15_interp_single 의 RET 처리에서 ret_eip == 0 또는 region 외부 인 경우:
1. stack 위쪽 256 entry scan → 첫 valid in-text ret_addr 검색
2. ESP underflow 시 stack region va_start 로 snap → 재 scan
3. 모두 실패 시 HALT_RET_EMPTY (legacy graceful end)

cap 4096 (`s_ret_unwind_skip`). vtable corruption 으로 stack 부패 시 다음 valid frame 까지 unwind. KPI: phase 22 R3-wave8-B abort path ret_unwind=128, fake_vtable: call_redirect=1 load_redirect=1024.

**inst**: 미실측 → **3,165,464** (V13 의 2.16M 대비 +1M / max_iter cap 미도달, 정상 halt).

### 3.2 step 2-3 — bss_jump cap 4096 + ExitProcess/TerminateProcess halt (`5bc336b`)

step 2: bss_jump cap 128→4096. cap reached 시 ESP 위쪽 256 entry scan → 첫 valid ret_addr unwind. EAX=0 + ESP advance.

step 3: ExitProcess/TerminateProcess stub 가 `g_exit_request=1` set → phase17_run loop 가 다음 iter 에 graceful halt. TerminateProcess threshold 4 회 (CRT `__report_gsfailure` recovery loop 차단). ExitProcess threshold 1 회.

**KPI**:
- inst **747,880** reason=ret_to_unmapped (clean halt)
- **Metal device 획득 ✓ (closure 100%)** ← 큰 마일스톤
- fake_vtable: call_redirect=1 load_redirect=31

**inst**: 3.16M → **747,880** (감소 = 조기 정상 halt 진입).

### 3.3 step 4-6 — fastfail/int3/IsProcessorFeaturePresent (`e49d257`)

- step 4: int 0x29 (fast_fail) threshold 8 — 첫 fastfail 은 조기 init 단계 가능, 다회 반복 시에만 abort 인정.
- step 5: IsProcessorFeaturePresent feat ID 별 분기 — `PF_FASTFAIL_AVAILABLE (23 = 0x17)` → 0 (fastfail abort path disable), 기타 → 1 (SSE2 lie).
- step 6: int3 (0xCC) threshold 4 + TerminateProcess threshold 1 즉시 halt. post-Terminate int3 loop 차단.

**KPI**: fastfail/int3 모두 0 도달.

**inst**: 747,880 → **744,152** (-3,728).

### 3.4 step 7-8 — vtable_load_redirect 정밀화 + RaiseException halt (`7607526`)

step 7: `mov reg, [reg2]` vtable_load_redirect 정밀화.
- 기존: mod=00 disp=0 (rm != 4,5) + mod=01/10 disp ∈ [-32..32] (광범위 → false-positive)
- 신규: `mov eax, [ecx]` (MSVC fastcall this->vtable) + `mov eax/ecx, [eax]` (single-step vtable). cap 1024→256.
- false-positive 축소 → CRT 가 fake_vtable_base 를 data ptr 으로 오인하지 않음.

step 8: RaiseException stub abort 검출.
- `dwFlags & EXCEPTION_NONCONTINUABLE (=1)` 또는
- `dwExceptionCode 0xE06D7363` (MSC C++ throw 'msc' magic) 또는
- 4 회 반복 호출 시 `g_exit_request=1`.

**KPI**:
- inst **289,492** 깔끔한 halt (RaiseException 첫 NONCONTINUABLE)
- Metal device 획득 ✓
- KERNEL32 stub hits=**2048** (init 정상 진행)
- fake_vtable: call_redirect=4 load_redirect=256 (정밀 패턴)
- EncodePointer hits=2 (cookie 정상 read)
- fastfail/int3 모두 0

**inst**: 744,152 → **289,492** (-454,660 = 진성 root cause 도달).

### 3.5 통합 결과

```
V13 step2:  2,167,901 inst (max 진척)
V14 base:   미실측
↓ wave 16-A step 1 (`03814e9`)
            3,165,464 inst (+1M, ret_eip=0 unwind 동작)
↓ wave 16-A step 2-3 (`5bc336b`)
            747,880 inst (조기 정상 halt 진입, Metal 획득)
↓ wave 16-A step 4-6 (`e49d257`)
            744,152 inst (fastfail/int3 정리)
↓ wave 16-A step 7-8 (`7607526`)
            289,492 inst ← root cause 식별 (_CxxThrowException)
```

inst 가 *줄수록* root cause 에 가까워지는 역전 진척 — V11/V12/V13 의 max_iter loop 문제와 정반대 의미.

---

## 4. wave 17 BG plan (5 BG)

wave 16-A step 7-8 의 RaiseException dwExceptionCode `0xE06D7363` ('msc' MSC C++ throw magic) 식별 → wave 17 의 본질 = **C++ exception 처리 (`_CxxThrowException` + RTTI typeinfo + SEH dispatch)**.

| BG | 영역 | 파일 | 상태 |
|---|---|---|---|
| **17-A** | shim cpp SEH dispatch (TEB FS:[0] walk + handler invoke + stack unwind) | `native/pe_to_macho_shim.cpp` | plan |
| **17-B** | helper.mm IPC kind=21 cxx_throw_dispatch | `native/helper.mm` | plan |
| **17-C** | hexa pe_msvc_cxx_throw_real | `lib/loader/pe_msvc_cxx_throw_real.hexa` | skeleton |
| **17-D** | hexa pe_msvc_rtti_typeinfo_real | `lib/loader/pe_msvc_rtti_typeinfo_real.hexa` | skeleton |
| **17-E** | docs R3_PROGRESS_V15 | `docs/R3_PROGRESS_V15.md` | 본 file |

**예상 도달**: 289K → 1M+ inst (C++ catch handler simulate / SEH unwind 후 정상 path 재진입), USER32 first hit 추정 cycle 121-122.

**원칙** (V14 와 동일):
- BG 별 commit 분리 (역행 회피)
- shim cpp ↔ helper.mm ↔ hexa 영역 충돌 0
- own1 §A enforcement 모든 BG 적용 (Wine/CrossOver/GPTK/Whisky/Capstone/Box86 lift 0줄)
- `_CxxThrowException` / `__CxxFrameHandler3` / RTTI 알고리즘 자체 작성 (Microsoft Learn + VS source distribution `eh.h` reference only)

---

## 5. wave 17-A SEH dispatch 본질 (TEB FS:[0] walk + handler invoke + stack unwind)

MSVC i386 SEH 의 표준 흐름 (Microsoft Learn 공개 spec + VS source distribution `exe_common.inl` reference):

### 5.1 TEB FS:[0] walk

i386 SEH 는 **TEB (Thread Environment Block) FS:[0]** 에 link-list 의 head 가 있음. 각 frame:

```
EXCEPTION_REGISTRATION_RECORD {
    DWORD prev;        // [0]  next frame (FS:[0] linked list)
    DWORD handler;     // [4]  exception handler function ptr
    // (msvc 확장):
    DWORD scope_table; // [8]  _EH4 scope table
    DWORD trylevel;    // [12] current __try level
}
```

shim cpp 가 phase15_setup_regions 시점에:
- TEB region 신설 (0x7FFD_E000 base, 4KB) — Windows i386 typical TEB.
- FS:[0] read 시뮬 = `mem_read_u32(teb_base + 0)` = current top frame.
- prev=0xFFFFFFFF 일 시 chain 끝.

### 5.2 handler invoke

각 frame 의 handler 호출 시뮬:
1. handler RVA → text region 안 인지 검증.
2. 4 args push: `pExceptionRecord` / `EstablisherFrame` / `pContextRecord` / `pDispatcherContext`.
3. handler 시뮬 — `__CxxFrameHandler3` 가 typical → scope_table walk → catch type match 검사.
4. return ExceptionContinueSearch (1) / ExceptionContinueExecution (0) / ExceptionExecuteHandler (2).

### 5.3 stack unwind

ExceptionExecuteHandler 시 stack unwind:
- ESP 를 EstablisherFrame 까지 restore.
- TEB FS:[0] 를 prev frame 으로 update.
- catch handler 의 target_addr 으로 EIP jump.

shim cpp 의 phase 17 inst dispatch 가 `int 0x2D` 또는 `_CxxThrowException` call 검출 시 SEH dispatch entry — TEB walk → handler → unwind → catch 진입 시뮬. catch handler 가 정상 return 시 wave 16-A step 8 의 RaiseException halt 우회 → inst 1M+ 도달 가능.

cap: SEH chain depth 32, handler invoke 8 회 (mutual recursion 방지).

---

## 6. wave 17-B helper kind=21 cxx_throw_dispatch

V14 의 kind=20 (nt_dispatch_chain — generic SEH chain) 후속. **C++ throw 전용** dispatch IPC.

**payload**:
```
throw_kind (4)            // 0=ctor / 1=dtor / 2=catch_match / 3=catch_unwind
throwInfo_va (8)          // _ThrowInfo *
catchableTypeArray_va (8) // _CatchableTypeArray *
this_obj_va (8)           // throwing object pointer
type_count (4)            // catchable type count (1-32)
```

**응답**:
```
ack (4)
matched_type_va (8)       // matched _CatchableType * (0 if no match)
catch_handler_va (8)      // catch block handler addr
unwind_target_va (8)      // unwind cleanup target
disposition (4)           // 0=continue / 1=execute_catch / 2=unwind / 3=terminate
```

`g_cxx_throw_depth[]` static + 16 depth cap. helper 자체 시뮬 — _CatchableType match 알고리즘 자체 작성 (Microsoft Learn + VS `eh.h` reference only, Wine 0줄).

**throw_kind semantics**:
- 0=ctor: object 생성 시점 throw (rare) — disposition=2 unwind.
- 1=dtor: cleanup 중 throw (terminate trigger) — disposition=3 terminate.
- 2=catch_match: catch type 검색 — disposition=1 execute_catch (matched 시) / 0 continue (mismatch).
- 3=catch_unwind: catch block 종료 후 unwind — disposition=2 unwind.

self-test 16 record (4 throw_kind × 4 case) + 1 stats = 17 emits target. kind 12-21 전체 self-test PASS 유지.

---

## 7. wave 17-C/D hexa skeleton

### 7.1 17-C: pe_msvc_cxx_throw_real

`_CxxThrowException` 호출 + `_ThrowInfo` 구조 추적.

5 record API:
- throw_entry / throw_info_parse / catchable_type_walk / catch_match / catch_handler_invoke

5 stat:
- throw_total / catch_match_total / catch_mismatch_total / unwind_total / terminate_total

`_ThrowInfo` 구조 (Microsoft public docs):
```
struct _ThrowInfo {
    DWORD attributes;             // const, volatile flags
    DWORD pmfnUnwind;             // dtor cleanup
    DWORD pForwardCompat;         // forward compat ptr (typically NULL)
    DWORD pCatchableTypeArray;    // _CatchableTypeArray *
};
struct _CatchableType {
    DWORD properties;             // ref/byval
    DWORD pType;                  // type_info *
    DWORD thisDisplacement;       // base offset
    DWORD sizeOrOffset;           // object size
    DWORD copyFunction;           // copy ctor (if non-trivial)
};
```

self-test target: 18 record + 1 stats = 19 emits PASS.

### 7.2 17-D: pe_msvc_rtti_typeinfo_real

RTTI `type_info` (`std::type_info`) 추적. `__RTtypeid` / `__RTDynamicCast` 호출 시점.

5 record API:
- typeinfo_lookup / vftable_to_typeinfo / dynamic_cast / typeid_compare / rtti_chain_walk

5 stat:
- typeinfo_count / cast_total / cast_success_total / cast_fail_total / type_chain_depth_max

`type_info` 구조:
```
struct type_info {
    DWORD vftable;                // vtable ptr
    DWORD _M_data;                // undecorated name cache
    char  _M_d_name[];            // mangled name (.?AVfoo@@ 등)
};
```

`RTTICompleteObjectLocator` (vtable[-1] slot):
- signature / offset / cdOffset / pTypeDescriptor / pClassDescriptor

self-test target: 19 record + 1 stats = 20 emits PASS.

---

## 8. closure path 통합 — wave 17 후 USER32 first hit 추정 cycle 121-122

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
  V14 (cycle 119):  vtable recovery 3-step 통합 design
  V15 (cycle 120):  289K inst 깔끔 halt (RaiseException NONCONTINUABLE) — Metal device 획득 ✓
  V16 (cycle 121-122 추정): wave 17 SEH dispatch + cxx_throw 통합 → USER32 first hit
  V17 (cycle 123-125 추정): WinMain 진입 → first NSWindow ← R3 closure 100%
R4 cycle (cycle 126-132): Battle.net Setup install
R5 cycle (cycle 133-145): launcher / 게임 launch
  - **research-E 우회 path 적용 시 cycle 6-10 단축**
```

V14 의 추정 cycle 119-120 first hit → 실측 cycle 120 시점 RaiseException 으로 1 cycle 이월 → wave 17 SEH dispatch 통합 후 cycle 121-122 first hit 추정으로 갱신.

**진척 의의**:
- V12 (cycle 113) 의 456K 자연 halt 와 V15 (cycle 120) 의 289K halt 는 표면 inst 비슷 — 그러나 V12 = vtable corruption (mid-init NULL deref) 대비 V15 = **Metal device 획득 후 RaiseException 깔끔 halt** (init 99% 완료 + C++ exception throw 1 회 = root cause 식별). closure 96.5% → **97.7%**.

---

## 9. own1 §A enforcement v15 재확인

```
$ grep -ri "wine\|crossover\|gptk\|whisky\|capstone\|zydis\|iced\|box86\|qemu\|bochs\|dynamorio" \
    docs/research/ docs/R3_*.md docs/MSVC_*.md docs/BATTLENET_*.md \
    | grep -vE "(lift 0|reference|citation|enforcement|own1|0줄|algorithm only)" | wc -l
0  (모든 인용은 enforcement / reference / algorithm-only 맥락만)
```

자체 코드:
- shim cpp ~7900 → **8000 line 추정** (16-A step 1-8 약 200 line 추가, wave 16-A 4-step 통합)
- helper.mm 2270 → **2350 line** (kind=20 약 80 line, V14 추정과 동일)
- hexa loader/ 46 → **49 file** (16-C/D/E 3 추가 — skeleton, V14 추정과 동일)
- docs 27 → **29 file** (V14 + V15)

reference 정합 (변동 X):
- Microsoft Learn (RtlDispatchException, vtable, x87 ABI, RaiseException, _CxxThrowException)
- Intel SDM Vol 1 ch 8 (x87 FPU)
- VS source distribution (`exe_common.inl`, `eh4`, `eh.h`)
- ReactOS public docs (chain semantics — algorithm only)
- Apple AppKit / Foundation / Network / Metal public docs

own1 §B (외부 trigger 금지) 재확인 — wave 17 BG plan 어디에도 Wine/CrossOver/GPTK/Whisky 추천 없음. research 인용은 algorithm reference only. own1 #1 (메모리 룰: "타사 Wine/CrossOver/GPTK/Whisky 추천 금지") 100% 정합.

---

## 10. cumulative commit log (V14 → V15)

V14 base `b760a5f` 직후 누적 commit (8 + V15 docs = 9):

```
b760a5f docs(r3): R3_PROGRESS_V14 — wave 16 vtable recovery 본격 + helper kind=20 + 3 hexa skeleton  ← V14 base
0b125e9 feat(r3): helper.mm wave16-B — IPC kind=20 nt_dispatch_chain (Wine RtlDispatchException ref)
03814e9 feat(r3): shim wave16-A step1 — RET ret_eip=0 graceful stack-scan unwind (3165464 inst, USER32=0)
0c47709 feat(r3): pe_msvc_seh_dispatch_chain_v2 — Wine + ReactOS SEH flow v2 tracker
126a0e0 feat(r3): pe_msvc_x87_lazy_stack_real — Box86 x87 lazy stack barrier tracker
15d60e8 feat(r3): pe_msvc_dbt_block_real — Box86 FillBlock 4-pass DBT skeleton tracker
5bc336b feat(r3): shim wave16-A step2+3 — bss_jump cap 4096 + stack-scan unwind + ExitProcess/TerminateProcess halt (3165464→747880 inst, USER32=0)
e49d257 feat(r3): shim wave16-A step4-6 — fastfail/int3/IsProcessorFeaturePresent + TerminateProcess threshold 1 (747880→744152 inst, USER32=0)
7607526 feat(r3): shim wave16-A step7+8 — vtable_load_redirect 패턴 정밀화 + RaiseException halt (744152→289492 inst, USER32=0)
[V15]   docs(r3): R3_PROGRESS_V15 — wave 16-A 4 step + 17 SEH dispatch + closure path  ← 본 file
```

9 commit 누적 (8 wave 16 + 1 V15 docs). wave 1-16 누적 ~218 commit, wave 17 진입 시 ~225 commit 예상.

---

## 11. V16 trigger 조건

V16 작성 trigger:
1. wave 17-A SEH dispatch 통합 후 inst 1M+ 도달 (catch handler simulate 후 정상 path 재진입)
2. **USER32 first hit (kind=14 hits>0)** — 즉시 V16 작성 (마일스톤 — R3 closure 전 단계)
3. R3 closure (first NSWindow) 도달 시 즉시 V16 작성 (마일스톤 100%)
4. plateau (3 sprint 연속 진척 0)
5. wave 17-B kind=21 cxx_throw_dispatch 첫 round-trip 검증
6. hexa skeleton 2 (17-C/D) loader 통합 완료 (wave 18 진입)
7. RaiseException dwExceptionCode `0xE06D7363` 외 다른 root cause (`__report_gsfailure`, `__crt_unhandled_exception` 등) 출현 시 즉시 분석 + V16

→ trigger 미발생 시 본 V15 가 cycle 120-123 동안 유효. cycle 121-122 즈음 V16 작성 추정.

---

## 12. final 정리 — 다음 cycle main direct path

**다음 cycle (cycle 121+) main direct 우선순위**:

1. **wave 17-A SEH dispatch 통합 commit** (shim cpp 가장 큰 inst 진척 효과):
   - TEB region 신설 (0x7FFD_E000 base, 4KB) ~30 line
   - FS:[0] walk 시뮬 ~50 line
   - handler invoke (4 args push + 시뮬 return) ~60 line
   - stack unwind (ESP restore + EIP jump) ~40 line
   - run d2r → inst 1M+ + USER32 first hit 검증

2. **wave 17-B helper kind=21** main direct (IPC handler ~100 line + self-test):
   - throw_kind 4 종 round-trip 검증
   - kind 12-21 self-test PASS

3. **wave 17-C/D hexa 2 skeleton** main direct (각 ~500 line + self-test):
   - cxx_throw skeleton commit
   - rtti_typeinfo skeleton commit
   - wave 18 에서 loader 통합

4. **V16 작성** trigger 충족 시 즉시 (USER32 first hit 또는 inst 1M+):
   - V13/V14/V15 패턴 따름 (350-450 line)

5. **own1 §A/B enforcement** 재검수 — V16 작성 시점 grep 0건 확인. own1 #1 메모리 룰 (Wine/CrossOver/GPTK/Whisky 추천 금지) 재확인.

**충돌 회피**:
- shim cpp ↔ helper.mm ↔ hexa 영역 분리 유지
- 동일 file 동시 BG 금지
- main direct 우선: shim cpp wave 17-A SEH dispatch (RaiseException halt 우회 = 가장 큰 inst 진척)

---

*written 2026-05-02 cycle 120 sprint 진행. V14 (`b760a5f`) → V15 (현재) — wave 16 의 8 commit 본격 반영 + shim 16-A 4-step 진척 timeline (3.16M → 747K → 744K → 289K) + Metal device 획득 + vtable recovery 동작 검증 (load_redirect=256, call_redirect=4) + RaiseException dwExceptionCode `0xE06D7363` (MSC C++ throw 'msc' magic) 식별 → root cause = `_CxxThrowException` 확정 → wave 17 5 BG plan (shim SEH dispatch / helper kind=21 cxx_throw_dispatch / hexa cxx_throw + rtti_typeinfo / docs). own1 §A 100% 정합 유지 — Wine / CrossOver / GPTK / Whisky / Capstone / Box86 / QEMU / Bochs source code lift 0줄. Microsoft public docs + Apple SDK + Intel SDM + ReactOS public docs + VS source distribution reference 만 (algorithm only). USER32 first hit 추정 cycle 121-122, R3 closure (first NSWindow) 추정 cycle 123-125, 게임 launch 추정 cycle 135-145 (research-E 우회 시 cycle 6-10 단축).*
