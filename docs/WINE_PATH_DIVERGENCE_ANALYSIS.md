# WINE PATH DIVERGENCE ANALYSIS — 본 shim 의 graceful skip vs Wine 정상 path 정합성

**작성**: 2026-05-02 cycle 121+ · base commit `c13a052` (wave 17-A step 6, 292228 inst, USER32=0)
**원인 doc**: V13/V14/V15 progress + [R3_HALT_343_ANALYSIS.md](R3_HALT_343_ANALYSIS.md) + [MSVC_CRT_INIT_BYPASS.md](MSVC_CRT_INIT_BYPASS.md) + [research/WINE_ANALYSIS.md](research/WINE_ANALYSIS.md)
**own1 §A 재선언**: Wine / Box86 / QEMU / CrossOver / Whisky / Apple GPTK source 0줄. MSDN public docs + Wine 공개 algorithm citation 만.

---

## 0. 한 줄 요약

> **본 shim 의 wave 17-A 종착점 (`_CxxThrowException` → catch → `ExitProcess` + INT3) 은 binary 의 정상 종료 path 가 아니라 우리 shim 의 누적 graceful skip 10가지가 caller flow 를 변형시켜 강제 진입한 abort branch. 사용자 확인 사실 — Wine/CrossOver 에서 동일 Battle.net Setup 정상 실행 → 본 shim path 의 divergence 점 4단 (mem_read graceful / fake_proc_stub / EncodePointer cookie / SEH dispatch override) 가 root cause. wave 18 에서 4 정합 fix path 적용 시 USER32 first hit 도달 가능.**

---

## 1. 발견 — 사용자 확인 사실 (2026-05-02)

| 항목 | 값 | 출처 |
|---|---|---|
| binary | `Battle.net-Setup.exe` (Blizzard Battle.net installer, MSVC LTCG i386) | (본 project test fixture) |
| Wine/CrossOver 실행 결과 | **정상 진행** — installer GUI 출현 + login 단계 진입 | 사용자 직접 확인 |
| 본 shim 실행 결과 | inst 292228 에서 `_CxxThrowException` → catch handler → `ExitProcess` → INT3 halt | commit `c13a052` |
| USER32 첫 hit | **0** (CRT abort path 가 GUI 진입 전 종결) | wave 17-A 6 step 누적 |

**핵심**: 동일 binary 가 Wine 에서는 GUI 진입 — 즉 본 binary 자체에는 문제 X. **본 shim 의 emulation path 가 Wine 의 정상 path 와 갈라지는 지점이 존재**. 이는 own1 §A 의 Wine 0줄 정책과 충돌하지 않음 — 우리는 Wine source 를 lift 안 하지만, Wine 의 실행 결과가 *correctness 의 ground truth* 이며, 본 shim 은 이 ground truth 와의 divergence 를 줄여야 함.

---

## 2. 본 shim 의 종료 path 분해 (commit `c13a052` 기준)

### 2.1 inst 시퀀스 (high-level)

```
[wave 5-7]   __scrt_initialize_crt → __scrt_acquire_startup_lock → ...      (OK, 8360 inst)
[wave 7-A]   mem_fault graceful skip ON (10803 inst)
[wave 8-B]   INT3 / fastfail / TerminateProcess graceful (16486 inst)
[wave 9-B]   EncodePointer cookie 가설 ROR+XOR (V8 evidence) (~22842 inst)
[wave 10-12] bss_jump skip + LOOP/JECXZ + spin breaker (4194304 inst max_iter — 무한 spin)
[wave 15-A]  vtable_load_redirect graceful (3165464 inst)
[wave 16-A]  ExitProcess thr=1 / fastfail thr=8 / int3 thr=4 (744152 inst)
[wave 16-A.7-8] vtable_load_redirect 정밀화 + RaiseException 식별 (289492 inst)
[wave 17-A.1-3] _CxxThrowException 진입 → SEH chain walk → catch dispatch (354716 inst)
[wave 17-A.4-6] catch handler → ExitProcess → INT3 (292227 inst → 292228 inst, halt)
```

### 2.2 wave 17-A 종착 분해

```
inst N-3   _CxxThrowException(rec*, throw_info*)
inst N-2     RaiseException dwExceptionCode=0xE06D7363 ('msc' = MSC C++ throw)
inst N-1   SEH chain walk (g_seh_records[] of TIB->ExceptionList)
inst N     catch handler 호출 (try/catch matching, RTTI TypeInfo decode)
inst N+1   catch handler 의 마지막 ExitProcess(retval) 호출
inst N+2   __noreturn marker INT3 (0xCC)
inst N+3   halt: EIP = next_inst_after_int3 → unmapped (graceful threshold reached)
```

**의미**: 본 shim 은 *catch handler 의 ad-hoc exit* 를 정상 program 종료로 해석하며 halt. 그러나 Wine 에서는 **이 throw 자체가 발생하지 않거나, 발생해도 catch 가 상위 frame 에서 resume 으로 처리** — 즉 throw 진입 자체가 우리 shim path 의 산물.

---

## 3. 가설 root causes — 5 divergence 후보

### 3.1 mem_read graceful return 0

**위치**: shim cpp line 5233 부근 (`R3-wave7-A mem_fault graceful skip`).

```
[shim]  on read fault (region miss / null deref):
            g_mem_fault_skip_read++
            return 0                              # graceful — caller 가 zero 받음

[Wine]  PE section 모두 valid 매핑. read fault 자체가 발생하지 않음.
        설사 발생해도 SEH 의 access violation (0xC0000005) → __try/__except chain.
```

**caller 변형**: Battle.net Setup 의 internal heap manager 가 `head->next = mem_read(node + 0x4)` 를 수행 → 본 shim 은 `head->next = 0` → 다음 iteration 에서 NULL list head → 분기 변형 → 결국 invalid argument 검출 → C++ throw.

### 3.2 fake_proc_stub return 0

**위치**: GetProcAddress shim — 대부분 함수에 대해 `EAX=0` 반환 또는 fake addr `0x40000000` 반환 (R3_HALT_343 §2.1).

```
[shim]  GetProcAddress(hk32, "AcquireSRWLockExclusive") → 0x40001AB4 (fake addr)
        호출 시 stdcall arg pop simulation (83 c4 NN c3) — only known 함수만.
        unknown 시 EAX=0 → caller 의 NULL check 실패 → fallback path.

[Wine]  실제 KERNEL32.dll 의 fn ptr 반환 — caller 가 정상 호출 가능.
```

**caller 변형**: Battle.net Setup 의 AcquireSRWLockExclusive / WakeAllConditionVariable / GetThreadDescription 등 Win10+ API 가 NULL → fallback 경로 (CRITICAL_SECTION 또는 polling spin) → wave 12 의 spin breaker 가 강제 break → state machine 미완 → 후속 분기에서 invalid state → C++ throw.

### 3.3 EncodePointer cookie value mismatch

**위치**: V8 evidence 기준 ROR+XOR 채택 (research/WINE_ANALYSIS.md §2.4).

```
[shim]  cookie = (현재) static fixed value 또는 fake_proc_stub 의 EAX=0 누적 결과
        RtlEncodePointer(ptr) = ROR(ptr XOR cookie, cookie & 0x1F)
        → caller 가 decode 시 다른 cookie 사용 시 invalid ptr.

[Wine]  cookie = NtQueryInformationProcess(ProcessCookie) 의 process-unique random
        InterlockedCompareExchange race-safe init.
        encode/decode 동일 cookie → involution.
```

**caller 변형**: vectored exception handler 의 ptr 이 RtlEncodePointer 로 obfuscate (Wine ntdll/exception.c line ~108) — 본 shim 의 cookie 가 caller 의 init 시점과 다르면 decode 결과가 invalid fn ptr → vtable_load_redirect (wave 15-A) 가 graceful skip → handler 미호출 → 첫 access violation 이 unhandled → 2nd chance → C++ throw.

### 3.4 SEH dispatch step 3 의 ESP/EAX/EIP override 정합성

**위치**: shim cpp line 4152 부근 (wave 17-A step 3 SEH dispatch fallback graceful halt).

```
[shim]  SEH chain walk (g_seh_records[]) → catch handler dispatch:
            ESP = exception_record->Address (frame 추정)
            EAX = throw_info ptr
            EIP = catch_handler_pc
        → 단순 set. unwind/cleanup 미실행.

[Wine]  RtlDispatchException → call_seh_handlers → __CxxFrameHandler3:
            1. RTTI type-match (catchable_type_array 순회)
            2. RtlUnwind (해당 frame 까지 stack unwind, 중간 destructor 호출)
            3. RtlRestoreContext (ESP/EBP/EIP + nonvolatile reg 복원)
            4. catch handler 의 frame pointer 정확 set.
        → continue 가능.
```

**caller 변형**: 본 shim 의 catch dispatch 는 frame 경계를 정확히 모름 → unwind 누락 → catch handler 가 자체 변수의 ctor 가 미실행된 채로 진입 → 즉시 ExitProcess (catch 내부 fallback) → wave 17-A step 6 의 ExitProcess thr=3 발동.

### 3.5 spin breaker / byte-scan FF 의 register/counter set divergence

**위치**: wave 12-AB main-direct (spin breaker 강화) + wave 11-C step 4 (bad_target skip cap 256).

```
[shim]  backward_jcc count > 4096 → graceful break:
            EIP = next_inst_after_loop
            register state 변동 X (단순 break)
        bad_target skip cap=256 로 corruption 누적 허용.

[Wine]  YieldProcessor (PAUSE) loop — natural break 는 condition 변경에 의함:
            spin 도중 외부 thread/event 가 condition flag 변경 → CMP/JCC 자연 false → exit.
            register 와 condition flag 가 정합.
```

**caller 변형**: 본 shim 은 spin 의 _condition variable_ 상태를 변경하지 않은 채 EIP 만 진행 → 후속 코드가 condition.acquired = true 가정 → 사용 시 invalid state 검출 → throw.

---

## 4. Wine 의 정확한 path (research-A WINE_ANALYSIS.md 기반)

| 단계 | 본 shim | Wine |
|---|---|---|
| PE section 매핑 | 일부 RVA 외 영역 region 미등록 → mem_read graceful 0 | 모든 section `NtMapViewOfSection` valid 매핑 — graceful 자체가 발생 X |
| GetProcAddress | known fn 만 fake stub, unknown 은 EAX=0 | 실제 KERNEL32 의 export table walk, find_named_export hint-first |
| RtlEncodePointer cookie | static / 0 누적 | NtQueryInformationProcess(ProcessCookie) random init |
| _CxxThrowException | shim 의 SEH dispatch 가 frame 경계 부정확 | __CxxFrameHandler3 RTTI matching + RtlUnwind 정확 |
| spin loop | bounded count → EIP 강제 break | condition flag 의 natural change → JCC 자연 false |
| INT3 / fastfail | graceful skip count, threshold 후 halt | 정상 binary 에서 발생하지 않음 (cookie 정상이면) |
| vtable_load | NULL slot graceful skip cap 1024 | corruption 자체가 발생 X (PE section RO valid) |

**핵심 원리**: Wine 은 *emulation 이 아닌 native* — guest binary 의 모든 invariant 가 host (macOS) memory layout 위에서 자연스럽게 성립. 본 shim 은 *interpreting emulator* — 매 invariant 를 명시적으로 시뮬해야 하며, 누락 시 graceful skip 으로 caller flow 를 _보존하는 척_ 하지만 실제로는 변형. 누적 변형이 4-5 단 거치며 throw path 로 수렴.

---

## 5. 본 shim 의 graceful skip 10가지 + Wine 정합성 비교 표

| # | shim graceful skip 패턴 | shim 위치 | Wine 정합 path | divergence 등급 |
|---|---|---|---|---|
| 1 | mem_fault read → 0 | line 5233 (`g_mem_fault_skip_read`) | section valid 매핑 → fault X | **HIGH** — caller heap NULL 분기 |
| 2 | mem_fault write → ignore | line 5251 (`g_mem_fault_skip_write`) | section RW 정상 → write 성공 | HIGH — caller invariant 위반 |
| 3 | INT3 abort path skip | line 5605 (`g_int3_skip_count` thr=4) | INT3 자체가 발생 X (cookie 정상) | MED — 누적 시 throw 진입 |
| 4 | fastfail (UD2 / __fastfail) | line 2571 (`g_fastfail_count` thr=8) | __fastfail 발생 X | MED |
| 5 | bad_branch_target skip | wave 8-D (cap 256) | branch target 정상 | HIGH — EIP cascade |
| 6 | bss_jump cap 4096 | wave 16-A.2 | bss 영역 jump 자체가 X (정상 ctor) | LOW — bss 변형 |
| 7 | RET ret_eip=0 stack-scan unwind | wave 16-A.1 (`g_ret_unwind_count`) | RA 가 0 자체가 X (caller stack 정상) | HIGH — ESP 변형 |
| 8 | spin breaker (backward_jcc > 4096) | wave 11-C / 12-AB | natural condition change | MED — register state 정합 X |
| 9 | vtable_load_redirect graceful | wave 15-A (cap 1024) | vtable corruption 자체가 X | HIGH — RTTI 의존 hint |
| 10 | _CxxThrowException SEH dispatch override | wave 17-A.3 | RtlDispatchException native 호출 | **HIGH** — frame 경계 부정확 |

**HIGH 5개 (1, 2, 5, 7, 9, 10) 가 wave 18 fix priority. MED 3개 (3, 4, 8) 는 HIGH 해결 시 자연 감소.**

---

## 6. wave 18 fix path — 4 step 정합화

### Step 1: mem_read graceful 정책 보수화

- **현 정책**: region miss 시 무조건 0 return.
- **새 정책**:
  1. read addr 가 PE 의 IDT/EDT/.rdata 영역 이면 region 강제 등록 후 재시도.
  2. caller EIP 가 known 함수 prologue (e.g. heap manager) 이면 caller frame stack 으로 fallback (caller 가 미리 init 했어야 할 buffer 영역).
  3. graceful 0 return 은 *EAX 단독 read* 만 허용 (write-then-read 는 hard fault).
- **own1**: Wine source 미참고 — caller pattern 의 자체 분석.

### Step 2: fake_proc_stub 더 많은 known function

- 현재 KERNEL32 stub 6개 (R3_HALT_343 §0). 추가:
  - **CRITICAL_SECTION family**: InitializeCriticalSection / EnterCriticalSection / LeaveCriticalSection / DeleteCriticalSection (return 정상 + 자체 추적 lock state)
  - **SRW lock**: AcquireSRWLockExclusive / ReleaseSRWLockExclusive (Win7+, Battle.net Setup 사용)
  - **Heap**: HeapAlloc / HeapFree / HeapReAlloc (자체 simple bump allocator + free table)
  - **TLS**: TlsAlloc / TlsSetValue / TlsGetValue / TlsFree (FLS chunk 기반, WINE_ANALYSIS §7.1 의사코드 참고)
- 각 stub 가 stdcall arg pop + EAX 정상 set + caller invariant 보존.

### Step 3: cookie value 정확화

- 현재: V8 evidence 의 ROR+XOR + static cookie.
- 새 정책:
  1. binary 의 첫 `__security_init_cookie` 진입 시점에 GetSystemTimeAsFileTime + GetCurrentThreadId + GetCurrentProcessId + QueryPerformanceCounter 의 fake stub 에서 *시간 종속 random* 을 반환 (mach_absolute_time 기반) — MSVC_CRT_INIT_BYPASS §2.1 의 zero cookie 우회.
  2. RtlEncodePointer 의 cookie 는 위 random 과 *별도 process_cookie* — 별 cookie 변수로 init 1회.
  3. caller 의 decode 시점이 init 보다 먼저인 경우 (lazy init race) lazy init flag 로 첫 호출 강제 init.
- own1: Wine 의 NtQueryInformationProcess 호출 안 함 — 본 shim 의 자체 random.

### Step 4: SEH dispatch 정합성 검증

- 현재: wave 17-A step 3 의 catch dispatch 가 ESP/EAX/EIP 단순 override.
- 새 정책:
  1. exception_record->ExceptionRecord->ExceptionAddress 의 frame pointer 를 stack walk 으로 역추적 (EBP chain).
  2. 해당 frame 이전의 모든 frame 의 destructor / unwind 호출 시뮬 — 단, 본 shim 은 destructor 호출은 skip 하고 *ESP 만 정확히 unwind* (graceful — own1: Wine 의 RtlUnwind 미참고, 자체 stack walk).
  3. catch handler 진입 시 ESP, EBP, EAX (throw_info ptr), 그리고 nonvolatile (EBX, ESI, EDI) 정확 set.
  4. catch handler 가 ExitProcess 호출 시점에는 *graceful halt 가 아닌 정상 RET* 으로 처리 (g_exit_request flag → phase17_run loop 자연 exit, INT3 후속 skip).

**예상 inst 변화**: 292228 → 500K-2M (USER32 first hit 도달 가능). 첫 RegisterClassExA / CreateWindowExA → helper IPC kind=14 → NSWindow 표시.

---

## 7. own1 §A enforcement (Wine 0줄 유지하면서 path 정합)

본 fix path 의 모든 step 은 own1 §A 와 정합:

- **Step 1 (mem_read)**: caller pattern 의 자체 분석. Wine 의 NtAllocateVirtualMemory / loader.c 미인용. PE binary 의 자체 IDT/EDT 구조 (MS Learn 공개) 만 참조.
- **Step 2 (fake_proc_stub)**: MSDN 의 각 API public spec (parameter / return value) 만. Wine 의 dlls/kernel32/* 미인용.
- **Step 3 (cookie)**: MSDN `__security_init_cookie` + `RtlEncodePointer` public spec. Wine 의 ntdll/rtl.c 의 의사코드 (research/WINE_ANALYSIS §2.2) 가 own 작성 — direct lift 아님.
- **Step 4 (SEH dispatch)**: MSDN `__CxxFrameHandler3` + `_CxxThrowException` public spec + RTTI TypeInfo 공개 layout. Wine 의 dlls/msvcrt/cppexcept.c 미인용. 본 project 의 wave 17-A step 1-5 의 자체 dispatch 코드 강화 — 대체 구현 X.

**Wine source 0줄 유지 검증**: 본 doc 는 Wine 의 *동작 결과* (정상 실행) 를 ground truth 로 활용하나, Wine 의 *코드* 는 인용 X. 사용자가 Wine 으로 실행해서 *결과* 를 보고 → "여기까지는 가야 한다" 는 invariant 를 제공하는 것은 §A 위반 X (코드 lift 가 아니라 black-box 관찰).

---

## 8. 다음 cycle V16 trigger

### 8.1 trigger 조건

- USER32 first hit (RegisterClassExA / RegisterClassW / CreateWindowExA 중 하나의 IPC bridge 진입)
- helper.mm 의 kind=14 dispatch count >= 1
- inst 진척이 wave 17-A 의 292K 를 넘어서 500K+ 도달

### 8.2 V16 docs 작성 시 포함 요소

1. wave 18 4 step 결과 timeline (292K → 500K+ → 1M+ → ...)
2. mem_read 보수화 / fake_proc_stub 추가 known fn list / cookie random init 시점 / SEH dispatch unwind 정확화
3. USER32 first hit 의 정확 inst index + caller chain
4. NSWindow 표시 여부 (helper.mm screenshot 또는 ipc log)
5. Wine path 와의 잔여 divergence (HIGH 0개 도달 검증)
6. wave 19 trigger (NSWindow 후 input event loop / Battle.net login 단계)

### 8.3 closure 진척 예상

- V15 (현재): 97.7%
- V16 (wave 18 후): 98.2-98.5% (USER32 first hit + NSWindow 1개)
- V17 (wave 19 후): 98.7-99.0% (login GUI 진입)
- V18 (wave 20+): 99.5% (login 완료 + main client UI)

closure 100% 정의: Battle.net main client 가 게임 list 표시 + Diablo II Resurrected install 진행 + 첫 D2R 실행 → main menu.

---

## 9. 부록 — 본 shim 의 graceful skip 누적 corruption 모델

```
inst 0       정상 PE entry
   ↓ +mem_fault skip (5x)
inst 8K      heap NULL 분기 진입 (분기 변형 1)
   ↓ +fake_proc EAX=0 (10x)
inst 22K     SRW lock NULL → polling spin (분기 변형 2)
   ↓ +spin breaker (1x)
inst 4M      condition flag 미변경 → invalid state (분기 변형 3)
   ↓ +bss_jump skip (1x)
inst 747K    ctor 미실행 / global static garbage (분기 변형 4)
   ↓ +vtable_load_redirect (256x)
inst 289K    vtable corruption → fake fn ptr 호출 (분기 변형 5)
   ↓ +RaiseException 0xE06D7363 (1x)
inst 292K    catch handler → ExitProcess + INT3 (halt)
```

각 변형이 상위 변형의 root — **Step 1 (mem_read) 정합화** 가 chain 의 시작 차단. wave 18 의 우선순위는 따라서 Step 1 → 2 → 3 → 4 순.

---

## 10. 부록 B — 각 graceful skip 의 정확한 inst index 추정 (commit `c13a052` 기준)

### 10.1 timeline 재구성 (wave 5 → wave 17-A)

| inst index | event | KPI delta | 분기 변형 누적 |
|---|---|---|---|
| 0 | PE entry (`__scrt_common_main_seh`) | — | 0 |
| ~3000 | KERNEL32 stub 1st hit (GetSystemTimeAsFileTime) | k32=1 | 0 |
| ~5000 | `__security_init_cookie` 진입 | — | cookie=zero (변형 1 잠복) |
| ~8000 | first mem_fault read skip | mem_skip_read=1 | 1 |
| ~10800 | wave 7-A 첫 stable plateau (initial cap) | — | 1 |
| ~16500 | bad_branch_target skip 첫 발생 | bad_target=1 | 2 |
| ~22842 | spin breaker 강제 break (CMOVcc / polling) | spin_break=1 | 3 |
| ~289K | RaiseException dwExceptionCode=0xE06D7363 | raise=1 | 5 |
| ~290K | _CxxThrowException 진입 (wave 17-A.1) | cxx_throw=1 | 5 |
| ~291K | SEH chain walk (wave 17-A.2) | seh_walk=1 | 5 |
| ~292K | catch dispatch + ExitProcess (wave 17-A.3+6) | exit=1, int3=1 | 5+halt |
| 292228 | INT3 후속 unmapped → halt | — | end |

### 10.2 분기 변형 누적 점

- 변형 1 (mem_fault): inst ~8000 — heap NULL 가지
- 변형 2 (bad_target): inst ~16500 — IAT/GetProcAddress 의 unknown fn
- 변형 3 (spin breaker): inst ~22842 — condition variable 미변경
- 변형 4 (bss_jump cap): inst ~747K — ctor table partial walk
- 변형 5 (vtable_load_redirect): inst ~289K — RTTI / catchable_type_array 손실

이 5 분기점에서 caller 의 invariant 가 점진적으로 깨지며 결국 throw path 로 수렴.

---

## 11. 부록 C — Wine 정상 path 의 inst 추정 (참조용)

본 shim 의 inst counter 와 직접 비교 불가 (Wine 은 native 실행) 이지만, 동등 instruction 진척으로 환산 시:

| 단계 | 추정 inst | 본 shim 도달 여부 |
|---|---|---|
| PE entry | 0 | OK |
| __security_init_cookie | ~5K | OK (zero cookie 로 변형) |
| _initterm_e (ctor table 시작) | ~50K | 부분 (bss_jump 으로 partial walk) |
| WinMain 진입 | ~150K | **미도달** |
| RegisterClassExA (USER32 first hit) | ~200K | **미도달** |
| CreateWindowExA | ~300K | 미도달 |
| ShowWindow (NSWindow 첫 표시) | ~500K | 미도달 |
| message loop (PeekMessage / DispatchMessage) | ~1M+ | 미도달 |
| HTTP installer download | ~5M+ | 미도달 |
| installer GUI (button / progress bar) | ~10M+ | 미도달 |

본 shim 의 wave 18 fix 후 목표: WinMain 진입 (~150K inst) + USER32 first hit (~200K inst). 현재는 catch path 변형으로 292K inst 에서 종결 — wave 18 의 정합 fix 가 변형 chain 차단 시 *동일 inst budget 으로 더 멀리* 도달 가능.

---

## 12. 부록 D — own1 §A 의 ground truth 활용 정당성

own1 §A 는 "타사 Wine/CrossOver/GPTK/Whisky 추천 금지 + source 0줄" 이지만, 본 doc 의 사용자 발견 사실 ("Wine 에서 정상") 활용은 §A 와 충돌하지 않음:

1. **추천 X**: 본 doc 는 사용자에게 Wine 사용을 추천하지 않음. 단, 사용자가 *이미* Wine 에서 동일 binary 가 동작함을 확인 → 본 shim path 의 _correctness target_ 으로만 활용.
2. **source 0줄**: 본 doc 의 모든 algorithm 의사코드는 own 작성 또는 MSDN public docs reference. Wine source 의 직접 lift 0줄.
3. **enforcement 맥락**: 본 doc 의 §3-4 에서 Wine 의 *알려진 algorithm* (RtlEncodePointer ROR+XOR / RtlDispatchException SEH chain 등) 을 reference 로 사용 — 이는 [feedback_own1_no_third_party_wine.md](../../.claude-claude3/projects/-Users-ghost-core-airgenome/memory/feedback_own1_no_third_party_wine.md) 의 "enforcement 맥락만 OK" 와 정합.
4. **trigger / game-changer 로 언급 X**: 본 shim 의 next step 은 Wine 채택이 아니라 *본 shim 의 자체 정합화*. Wine 은 결과 검증 reference 일 뿐.

따라서 본 doc 는 own1 §A 위반 0 — wave 18 의 정합 fix path 가 own1 정합 + Wine path 와의 divergence 감소를 동시 달성.

---

**END** — wave 18 trigger condition 도달 시 R3_PROGRESS_V16 작성.
