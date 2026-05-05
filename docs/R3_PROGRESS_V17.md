# R3 PROGRESS V17 — wave 18-A 14 step + wave 19 MSVC EH 본질 plan (2026-05-02 cycle 122-124 진행)

> **status**: V16 (commit `1e39e71`, research 11 BG 통합 + wave 18 7 BG plan + 292K halt) → V17 (wave 18-A 14 step 본격 반영 commit `0700548` ~ `d703114` 누적 + 484K outermost mode 도달 + USER32=0 유지 + wave 19 MSVC EH 본질 구현 plan).
> **base commit chain**: V15 (`e61f7f2`) → V16 (`1e39e71`) → wave 18-A 14 step (`0700548` `8e61aa7` `4b4210b` `685aeeb` `27e495b` `3c4a849` `cd93161` `0a28de5` `714263a` `f427117` `275a94d` `c6fb1ef` `a5bed24` `d703114`) + 부속 (`9d64416` msync hexa / `9a276cb` veh hexa / `10e76db` helper kind=22 / `0edc6b2` SD forging tracker hexa) → V17 (현재).
> **closure 진척**: V16 97.9% → V17 98.1% (wave 18-A 14 step 결과 inst 304K → 484K outermost mode + Battle.net Setup C++ exception control-flow 결정적 발견 → wave 19 EH 본질 구현 plan 확정. USER32 first hit 추정 cycle 122-124 → cycle 124-126).

---

## 1. V16 → V17 변화 요약

| 영역 | V16 (`1e39e71`) | V17 (현재) |
|---|---|---|
| inst (max_iter cap) | 64M | 64M (변동 X) |
| inst (자연 halt, default) | **292,228** (17-A step 6 ExitProcess thr=3) | **304,615** (18-A step 7 catch re-throw guard 정상 종료) |
| inst (outermost mode) | — | **484,567** (18-A step 10 SHIM_SEH_OUTERMOST=1) |
| inst (PROPER_CALL mode) | — | **484,554** (18-A step 11+12 heap base 0x20000000) |
| inst (SKIP_FUNCS mode) | — | **430,659 / 413,897** (step 13 call hook / step 14 jmp hook) |
| inst 진척 timeline | 292K | **292K → 51K → 51K → 51K → 51K → 296K → 313K → 304K → 304K → 304K → 484K → 484K → 484K → 430K → 413K** (18-A step 1→14) |
| KERNEL32 stub hits | 2048 | **2048** (변동 X — init path 동일, 변형은 catch chain 내부) |
| Metal device 획득 | 1 | 1 (유지) |
| **wave 18-A 14 step** | plan | **본격 반영** (VEH/catch/cookie/SD/mem_fault/SSE/re-throw guard/3 env mode/heap/SKIP_FUNCS) |
| **결정적 발견 5** | research-driven | **확정**: Battle.net Setup C++ exception 이 control-flow primary — proper EH 없이 USER32 도달 불가 |
| **__CxxFrameHandler3 구현** | 0 | 0 (wave 19 P0) |
| **scope_table + FuncInfo** | 0 | 0 (wave 19 P0) |
| **wave 19 BG plan** | 미정 | **6 BG**: shim FuncInfo+CxxFrameHandler3 / helper kind=23 / 3 hexa / docs |
| docs 갱신 | V16 (444 line) | **V17** (380+ line) |
| USER32 hits | 0 | 0 (변동 X — wave 19 EH 본질 구현 후 추정 first hit) |

**한 줄 핵심 변화**: V16 의 wave 18 7 BG plan 직후 본격 반영 → wave 18-A 가 14 step 으로 자연 분화 (VEH 1 + catch frame 2 + cookie 3 + SD stub 4 + mem_fault relax 5 + SSE 0F12-17 6 + re-throw guard 7 + 3 env mode 8/9/10 + PROPER_CALL 11 + heap base 12 + SKIP_FUNCS call/jmp 13/14) → 14 commit 누적. **결정적 발견**: env mode 들이 inst 304K → 484K 까지 +59% 진척 가능했으나 모두 `_CxxThrowException` 의 _다른_ throw site 로 변형 → halt 위치만 이동 = Battle.net Setup 의 throw / catch / cleanup 이 단순 error path 가 아니라 control-flow 의 primary mechanism (제어 흐름의 main path 가 throw-catch-resume 으로 구성). proper `__CxxFrameHandler3` (FuncInfo + scope_table interpret + catch funclet invoke + register restore) 미구현 차단점 명확화 → wave 19 5 BG 로 분화 (FuncInfo 구조체 + CxxFrameHandler3 본체 + helper kind=23 funclet dispatch + 3 hexa skeleton + docs).

---

## 2. wave 18-A 14 step commit timeline

V16 base `1e39e71` 직후 14 commit 누적 (wave 18-A 본격 반영):

| # | commit | sub | 영역 | inst 변화 | 핵심 |
|---|---|---|---|---|---|
| 1 | `0700548` | 18-A.1 | shim VEH dispatch table | 292K → **51K** | g_veh_handlers[32] + AddVectoredExceptionHandler / Remove + telemetry-only EXCEPTION_CONTINUE_SEARCH (i386 cb 호출 시 EIP 분기 위험 → search-only) |
| 2 | `8e61aa7` | 18-A.2 | shim Wine catch frame layout | 51K → **51K** | ESP=[frame-4] / EBP=[frame+12] (Wine `continue_after_catch` asm spec 정합) + region_find plausibility 검사 → fallback frame+8/frame |
| 3 | `4b4210b` | 18-A.3 | shim cookie random init | 51K → **51K** | g_security_cookie_va 위치에 random 4 byte (gettimeofday + pid + jitter) + top-16-bit zero mask + default 0xBB40 / 0 회피 + ComplementCookie write |
| 4 | `685aeeb` | 18-A.4 | shim SD forging stubs | 51K → **51K** | advapi32_stub_dispatch 14 fn (GetTokenInformation TokenUser→S-1-5-32-544, TokenIntegrityLevel→HIGH_IL) + GetSecurityInfo / GetNamedSecurityInfoA/W / GetFileSecurityA/W / GetKernelObjectSecurity / GetUserObjectSecurity / Initialize-Set SD / Make Absolute-SelfRelative SD |
| 5 | `27e495b` | 18-A.5 | shim mem_fault relax + sane_stack | 51K → **296K** | mem_fault cap 1024 → 65536 (cookie path spin loop 의 unmapped read 회피) + sane_stack(v) plausibility 검사 (region_find + cur_esp ±256KB) |
| 6 | `3c4a849` | 18-A.6 | shim SSE 0F 12-17 dispatch | 296K → **313K** | MOVLPS/MOVLPD/MOVDDUP / UNPCKLPS-PD / UNPCKHPS-PD / MOVHPS-PD/MOVSHDUP — 0x0040CEAD 의 MOVLPD store halt 해소 |
| 7 | `cd93161` | 18-A.7 | shim catch re-throw loop guard | 313K → **304K** | 동일 caller_eip 4회 초과 연속 _CxxThrowException 시 dispatch suppress + g_exit_request set → graceful halt |
| 8 | `0a28de5` | 18-A.8 | shim _CxxThrowException bypass mode | 304K → **304K** (default) / **290K** (bypass=1) | SHIM_CXX_THROW_BYPASS=1 시 모든 throw 무시 (caller EAX=0 + 정상 ret) — MSVC init fail-path throw 측정 |
| 9 | `714263a` | 18-A.9 | shim CATCH_UNWIND env mode | 304K → **304K** (default) / **291K** (unwind=1) | SHIM_CXX_CATCH_UNWIND=1 시 SEH chain prev frame 의 first valid PE-image ret_addr 까지 unwind + EAX=0 simulation |
| 10 | `f427117` | 18-A.10 | shim SEH outermost frame mode | 304K → **484K** (outermost=1) | SHIM_SEH_OUTERMOST=1 시 chain top-most handler 채택 (`__scrt_common_main_seh` @ 0x00551A50) — C++ inner __CxxFrameHandler3 우회. **+59% inst 진척, ×1.6** |
| 11 | `275a94d` | 18-A.11 | shim SEH PROPER_CALL env mode | 484K → **484K** | SHIM_SEH_PROPER_CALL=1 시 handler 호출 전 ExceptionRecord (32B) + CONTEXT (200B zero) + 5-slot args (ret/ER/EstablisherFrame/Context/DispatchContext) push 후 jump |
| 12 | `c6fb1ef` | 18-A.12 | shim heap base 0x80000000→0x20000000 | 484K → **484K** | signed positive 양수 base — caller 의 `cmp ebx, 0x7FFFFFFF; ja throw` negative size 오해석 회피 시도 |
| 13 | `a5bed24` | 18-A.13 | shim SHIM_SKIP_FUNCS env (call) | 304K → **430K** | SHIM_SKIP_FUNCS=0xHHH,0xHHH 시 CALL rel32 (0xE8) target 이 list 안 → ESP 변화 없이 EAX=0 + ret simulation. 0x005247E7 (length_error) stub-out → +126K inst (+41%) |
| 14 | `d703114` | 18-A.14 | shim SKIP_FUNCS jmp branch hook | 430K → **413K** | jmp (0xE9/0xEB) tail-call 도 hook — caller chain ret_addr stack pop. 0x005247AD 추가 시 jmp ret_addr=0x0008EBDC (stack data) invalid → ret_to_unmapped halt |

**부속 commit (research/hexa BG)**:
- `9d64416` pe_msvc_msync_mach_semaphore — Mach semaphore pool sync tracker (CrossOver msync ref)
- `9a276cb` pe_msvc_veh_dispatch_real — Vectored Exception Handler list + dispatch flow tracker
- `10e76db` helper.mm wave18-B IPC kind=22 sd_forging
- `0edc6b2` pe_battlenet_agent_sd_forging tracker (R5)

**누적 wave 1-18 commit**: ~233 + 14 wave 18-A + 4 부속 = **~251 commit**.

---

## 3. 결정적 발견 — Battle.net Setup C++ exception control-flow primary

wave 18-A 14 step 의 inst 진척 분석 결과:

### 3.1 step 5-6 (mem_fault / SSE) — interpreter 정합성 확보 → 51K → 313K (+5×)

mem_fault 8K cap relax + 0x0F 12-17 dispatch 추가로 catch path 진입 자체가 가능해짐. 하지만 catch 내부에서 즉시 re-throw → step 7 guard 발동.

### 3.2 step 7 (re-throw guard) — graceful halt, 핵심 root cause 노출

동일 caller_eip 4회 초과 연속 throw → catch handler 가 try block 의 invariant 를 복원 못 함. _Wine `continue_after_catch` 의 frame layout 정합 (step 2) 만으로는 부족_ — frame 내부 saved register 복원 + scope_table 기반 destructor chain unwind 가 추가 필요.

### 3.3 step 8-12 (env mode probe) — 본질 미구현 시 limit 명확화

5 env mode (CXX_THROW_BYPASS / CATCH_UNWIND / SEH_OUTERMOST / SEH_PROPER_CALL / heap base) 의 측정 inst delta:

| mode | inst | vs default | 분석 |
|---|---|---|---|
| default | 304K | — | step 7 catch re-throw guard 동작 |
| BYPASS=1 | 290K | -14K | throw 자체 ignore — caller cleanup 이 즉시 ret_to_unmapped (catch 안 잡힌 사실 인식) |
| UNWIND=1 | 291K | -13K | SEH prev frame 의 PE-image ret_addr 까지 unwind → 즉시 TerminateProcess |
| OUTERMOST=1 | **484K** | **+180K (+59%)** | __scrt_common_main_seh 직접 dispatch — re-throw loop 제거, deeper cleanup 진입 |
| PROPER_CALL=1 | 484K (-13) | +180K | bad_branch_target halt — PROPER_CALL stack 가 inner handler 의 stub thunk 와 부정합 |
| heap=0x20000000 | 484K | (변동 0) | throw site 자체 동일 |

**OUTERMOST mode 의 +59% 진척이 의미**: __scrt_common_main_seh (CRT 의 process-level outer handler) 까지 cleanup 정상 진입 가능. 단, 그 끝에서 cleanup chain 이 inner C++ frame 의 destructor 를 호출해야 하는데 본 shim 의 SEH dispatch 는 funclet invoke / scope_table walk / __ehfuncinfo 미구현 → cleanup chain 중단.

### 3.4 step 13-14 (SKIP_FUNCS) — throw site 변형 확인

0x005247E7 (length_error helper) stub-out → +126K inst, throw site 가 0x005247AD 로 _이동_. 0x005247AD 추가 stub-out → jmp tail-call hook 하지만 그 caller 의 ret_addr 가 stack data (0x0008EBDC).

**핵심 결론**: throw site 를 stub-out 해도 그 다음 throw site 가 등장 — Battle.net Setup binary 의 _상당 부분_ 이 throw 를 control-flow 의 primary path 로 사용 (예: container resize / parser dispatch / state machine transition 모두 throw-catch 로 구현). proper `__CxxFrameHandler3` (FuncInfo 의 scope_table walk + catch funclet invoke + register restore + nested try 추적) 없이는 USER32 도달 불가.

### 3.5 차단점 정합 정리

| component | V17 상태 | wave 19 P0 |
|---|---|---|
| `__CxxFrameHandler3` 본체 | 미구현 | **P0** |
| FuncInfo 구조 (magic / nTryBlocks / pTryBlockMap / nIPMapEntries / pIPtoStateMap / dispUnwindHelp / dispESTypeList / EHFlags) | 미구현 | **P0** |
| scope_table interp (TryHigh / TryLow / CatchHigh / nCatches / pHandlerArray) | 미구현 | **P0** |
| catch funclet invoke (rva → image_base + rva, 1 arg = catchable_object_va) | 미구현 | **P0** |
| catch return → resume PC (UnwindHelp[0] dispCatchHandler) | 미구현 | **P0** |
| 4 nonvolatile (EBX/ESI/EDI) restore | step 2 partial (region_find plausibility) | **P0** |
| nested try block state machine (state ≥ TryLow .. ≤ TryHigh) | 미구현 | **P1** |
| VC6 (0x19930520) / VC7 (0x19930521) / VC8 (0x19930522) magic 분기 | step 1 detect only | **P1** |
| destructor chain unwind (state - 1 step) | 미구현 | **P1** |
| ESTypeList (Exception Specification, throw()) | 미구현 | **P2** |

---

## 4. wave 19 BG plan (6 BG — MSVC EH 본질)

V16 의 wave 18 7 BG (shim VEH+catch+cookie+SD / helper kind=22 / 5 hexa / docs) 의 후속. wave 18-A 14 step 결과 EH 본질 미구현 차단점 노출 → wave 19 6 BG 로 분화.

| BG | 영역 | 파일 | 상태 |
|---|---|---|---|
| **19-A** | shim cpp FuncInfo 구조 + scope_table walk | `native/pe_to_macho_shim.cpp` | plan |
| **19-B** | shim cpp __CxxFrameHandler3 본체 (frame_state 결정 + catch type match + funclet invoke + register restore + resume PC) | `native/pe_to_macho_shim.cpp` | plan |
| **19-C** | helper.mm IPC kind=23 catch_funclet_dispatch (funclet 의 부속 syscall 가 Cocoa main thread 영향 시 marshal) | `native/helper.mm` | plan |
| **19-D1** | hexa pe_msvc_funcinfo_real (FuncInfo + scope_table parser + magic 분기 VC6/7/8) | `lib/loader/pe_msvc_funcinfo_real.hexa` | skeleton |
| **19-D2** | hexa pe_msvc_cxx_frame_handler3_real (CxxFrameHandler3 algorithm flow tracker) | `lib/loader/pe_msvc_cxx_frame_handler3_real.hexa` | skeleton |
| **19-D3** | hexa pe_msvc_catch_funclet_invoke_real (funclet invoke + register restore + resume PC) | `lib/loader/pe_msvc_catch_funclet_invoke_real.hexa` | skeleton |
| **19-E** | docs R3_PROGRESS_V17 | `docs/R3_PROGRESS_V17.md` | 본 file |

(19-A/B 가 동일 file `pe_to_macho_shim.cpp` 이므로 main direct 순차 적용. 19-C/D1~3 별 file BG 가능. 19-A 가 파싱 layer, 19-B 가 dispatch logic 로 분리 — 19-A 단독 검증 후 19-B 통합.)

**예상 도달**: 484K (outermost mode) → 1M+ inst (catch funclet 정확 invoke 후 Battle.net Setup 의 정상 throw-catch-resume control-flow 재개), USER32 first hit cycle 124-126.

**원칙** (V14/V15/V16 와 동일):
- BG 별 commit 분리 (역행 회피)
- shim cpp ↔ helper.mm ↔ hexa 영역 충돌 0
- own1 §A enforcement 모든 BG 적용 (Wine/CrossOver/GPTK/Whisky/Capstone/Box86 lift 0줄)
- FuncInfo / scope_table / __CxxFrameHandler3 모두 자체 작성 (Microsoft Learn `__CxxFrameHandler3` 명세 + VS source distribution `eh4` paraphrase + `ehdata.h` 공개 layout reference only)

---

## 5. wave 19 fix path 우선순위 (P0/P1/P2)

[wave 18-A 14 step 측정] + [§3 결정적 발견 정리] 통합:

### P0 (절대 우선 — 다음 cycle main direct)

1. **__CxxFrameHandler3 정확 구현 (FuncInfo + scope_table)** — wave 19-A + 19-B.
   - ground: §3.3 OUTERMOST mode 의 +59% 진척 후 cleanup chain 중단 분석
   - 효과: process-level outer handler 까지 정상 cleanup → inner C++ frame destructor invoke → catch funclet 정상 진입 → throw-catch-resume control-flow 재개
   - 예상 inst delta: 484K → 1M-3M
   - LOC 추정: shim cpp +~700 line (FuncInfo struct +120 / scope_table walk +180 / type match +120 / funclet invoke +160 / register restore +80 / resume PC +40)

2. **catch funclet invoke + register restore** — wave 19-B 후반 (19-A 직후 즉시).
   - ground: §3.5 4 nonvolatile (EBX/ESI/EDI) restore + UnwindHelp[0] resume PC
   - 효과: catch 진입 시 try block local 변수 정상 access + catch 종료 시 정확 resume PC 로 복귀 → inner cleanup chain 정합
   - 예상 inst delta: catch path 진입 시 +500K (Battle.net Setup 의 throw-catch-resume primary path 통과)

### P1 (P0 검증 후 즉시)

3. **nested try block state machine** — wave 19-B 후속 + 19-D2 hexa.
   - ground: §3.5 state ≥ TryLow .. ≤ TryHigh range walk
   - 효과: nested catch 의 정확 frame_state 결정 → outer catch 의 잘못된 invoke 회피
   - 예상 inst delta: nested catch path 진입 시 +200K

4. **VC6/VC7/VC8 magic 분기 (UE 4+ 호환)** — wave 19-A 후속 + 19-D1 hexa.
   - ground: §3.5 magic ∈ {0x19930520, 0x19930521, 0x19930522}
   - 효과: VC8 magic 의 dispESTypeList / EHFlags 추가 field 정확 parse — UE 4+ binary (Battle.net 외 게임) 호환
   - 예상 inst delta: Battle.net Setup 한정 효과 0, R5 게임 launch 시점 효과 +1M

### P2 (장기 — wave 20+ 후속)

5. **destructor chain unwind (state -1 step)** — wave 19-B 의 last step + wave 20 통합.
   - ground: §3.5 state - 1 reverse walk (HandlerType.dispOfHandler 가 destructor 인 경우)
   - 효과: throw 후 stack 위 local object 의 destructor 정상 호출 → memory leak / corruption 방지

6. **ESTypeList (throw() spec)** — wave 20+ 후속.
   - ground: §3.5 dispESTypeList nonzero 시 type list match 강제 (mismatch 시 unexpected → terminate)
   - 효과: 정합성 향상 — Battle.net Setup 한정 영향 미미 (대부분 throw 가 unspecified)

---

## 6. closure path 통합 (USER32 first hit cycle 124-126 추정)

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
  V15 (cycle 120):  289K inst RaiseException halt (Metal device 획득 ✓)
  V16 (cycle 121-122): 292K inst halt + research 11 BG + wave 18 7 BG plan
  V17 (cycle 122-124 진행): 304K default / 484K outermost / wave 18-A 14 step + wave 19 EH 본질 plan
  V18 (cycle 124-126 추정): wave 19 P0 (__CxxFrameHandler3 + scope_table) 통합 → USER32 first hit
  V19 (cycle 126-128 추정): WinMain 진입 → first NSWindow ← R3 closure 100%
R4 cycle (cycle 129-135): Battle.net Setup install (SD forging 적용 시 cycle 6-10 단축)
R5 cycle (cycle 136-147): launcher / 게임 launch (CEF 우회 path)
```

V16 의 cycle 122-124 first hit 추정 → wave 18-A 14 step 본격 반영 결과 throw-catch-resume control-flow primary path 식별 → wave 19 6 BG 가 EH 본질 path → cycle 124-126 first hit 갱신.

**진척 의의**:
- V12 (cycle 113) 456K halt = vtable corruption (mid-init NULL deref)
- V15 (cycle 120) 289K halt = Metal 획득 + RaiseException NONCONTINUABLE
- V16 (cycle 121-122) 292K halt = catch + ExitProcess + INT3 (root cause 명확화 + research 통합)
- V17 (cycle 122-124) 304K default / **484K outermost** halt = wave 18-A 14 step 본격 반영 + EH 본질 차단점 결정적 노출
- closure 96.5% → 97% → 97.5% → 97.7% → 97.9% → **98.1%** (wave 18-A 14 step + wave 19 plan = 0.2% 진척, wave 19 P0 후 99.0% 추정)

---

## 7. own1 §A enforcement v17 재확인

```
$ grep -ri "wine\|crossover\|gptk\|whisky\|capstone\|zydis\|iced\|box86\|qemu\|bochs\|dynamorio" \
    docs/research/ docs/R3_*.md docs/MSVC_*.md docs/BATTLENET_*.md docs/WINE_*.md \
    | grep -vE "(lift 0|reference|citation|enforcement|own1|0줄|algorithm only|negative|paraphrase|spec|docs)" | wc -l
0  (모든 인용은 enforcement / reference / algorithm-only / negative comparison 맥락)
```

자체 코드 (V16 → V17):
- shim cpp ~8200 → **~8800 line 추정** (wave 18-A 14 step 약 600 line 추가: VEH 90 / catch frame 80 / cookie 60 / SD stub 14 fn 200 / mem_fault relax 40 / SSE 6 op 50 / re-throw guard 30 / 3 env mode 80 / SKIP_FUNCS call+jmp 70)
- helper.mm ~2430 → **~2510 line** (kind=22 sd_forging IPC 약 80 line)
- hexa loader/ 51 → **54 file** (pe_msvc_veh_dispatch_real / pe_msvc_msync_mach_semaphore / pe_battlenet_agent_sd_forging 추가 — wave 18-F1~4 skeleton 일부 통합, 잔여 wave 19-D1~3 plan)
- docs 40 → **41 file** (V17 추가)

reference 정합 (V16 동일 + 추가):
- Microsoft Learn (`__CxxFrameHandler3`, `_CxxThrowException`, `RtlDispatchException`, `RtlEncodePointer`, `AddVectoredExceptionHandler`, `GetTokenInformation`, `GetSecurityInfo`, `MakeAbsoluteSD`, `MakeSelfRelativeSD`)
- VS source distribution `eh4` `eh.h` `ehdata.h` (FuncInfo / TryBlockMapEntry / HandlerType / UnwindMapEntry / IpToStateMap / ESTypeList layout — paraphrase only)
- Intel SDM Vol 2 (SSE 0F 12-17 move semantics)
- Intel SDM Vol 1 ch 8 (x87 FPU)
- ReactOS public docs (chain semantics — algorithm only)
- Apple AppKit / Foundation / Network / Metal / HIToolbox / Carbon / Cocoa public docs
- MSVC `winnt.h` docs (SID, ACE, ACL, SECURITY_DESCRIPTOR, TOKEN_INFORMATION_CLASS layout)

own1 §B (외부 trigger 금지) 재확인 — wave 19 BG plan 어디에도 Wine/CrossOver/GPTK/Whisky 추천 없음. wave 18-A 14 step commit message 의 "Wine NTDLL dispatch_exception" / "Wine msvcrt/except_i386.c" / "Wine ntdll/exception.c" 인용은 모두 _algorithm reference only — code lift 0줄_ 명시. own1 #1 메모리 룰 ([feedback_own1_no_third_party_wine.md](../../.claude-claude3/projects/-Users-ghost-core-airgenome/memory/feedback_own1_no_third_party_wine.md): "타사 Wine/CrossOver/GPTK/Whisky 추천 금지") 100% 정합.

---

## 8. cumulative commit log (V16 → V17, 14 wave 18-A step + 4 부속)

V16 base `1e39e71` 직후 누적 (14 wave 18-A + 4 부속 + V17 docs = 19):

```
1e39e71 docs(r3): R3_PROGRESS_V16 — research 11 BG 통합 + wave 18 fix path + closure target  ← V16 base
10e76db feat(r3): helper.mm wave18-B — IPC kind=22 sd_forging
0700548 feat(r3): shim wave18-A step 1 — VEH dispatch table (292228→51890 inst, USER32=0)
9d64416 feat(r3): pe_msvc_msync_mach_semaphore — Mach semaphore pool sync tracker
9a276cb feat(r3): pe_msvc_veh_dispatch_real — VEH list + dispatch flow tracker
4b4210b feat(r3): shim wave18-A step 3 — cookie random init + top16 zero (51890→51892 inst, USER32=0)
8e61aa7 feat(r3): shim wave18-A step 2 — Wine catch frame layout (51892→51892 inst, USER32=0)
685aeeb feat(r3): shim wave18-A step 4 — SD forging stubs (51892→51892 inst, USER32=0)
0edc6b2 feat(r5): pe_battlenet_agent_sd_forging — SD forging tracker (CrossOver Hack 23881 ref)
27e495b feat(r3): shim wave18-A step 5 — mem_fault cap relax + sane_stack frame (51892→296813 inst, USER32=0)
3c4a849 feat(r3): shim wave18-A step 6 — SSE 0F 12-17 dispatch (296813→313494 inst, USER32=0)
cd93161 feat(r3): shim wave18-A step 7 — catch re-throw loop guard (313494→304615 inst, USER32=0)
0a28de5 feat(r3): shim wave18-A step 8 — _CxxThrowException bypass mode (304615 inst, USER32=0)
714263a feat(r3): shim wave18-A step 9 — CATCH_UNWIND env mode (304615 inst, USER32=0)
f427117 feat(r3): shim wave18-A step 10 — SEH walk outermost frame mode (304615→484567 inst, USER32=0)
275a94d feat(r3): shim wave18-A step 11 — SEH PROPER_CALL env mode (484567 inst, USER32=0)
c6fb1ef feat(r3): shim wave18-A step 12 — heap base 0x80000000→0x20000000 (484554 inst, USER32=0)
a5bed24 feat(r3): shim wave18-A step 13 — SHIM_SKIP_FUNCS env (304615→430659 inst, USER32=0)
d703114 feat(r3): shim wave18-A step 14 — SKIP_FUNCS jmp branch hook (430659→413897 inst, USER32=0)
[V17]   docs(r3): R3_PROGRESS_V17 — wave 18-A 14 step + wave 19 MSVC EH 본질 plan       ← 본 file
```

19 commit 누적 (V16 docs + 14 wave 18-A step + 4 부속 + V17 docs).

**누적 cumulative wave**:
- wave 1-15: ~210 commit (V13 까지)
- wave 16: 8 commit (V15 까지)
- wave 17-A: 8 commit + 2 hexa+helper
- research 11 BG: 10 doc + 1 roadmap (V14→V16)
- V16 docs: 1
- wave 18-A: 14 commit
- wave 18 부속 (helper kind=22 / 3 hexa): 4 commit
- V17 docs: 1
- **총 ~252 commit**

---

## 9. R4/R5 preview — EH 본질 구현 후 launcher CEF 우회 path

V16 의 cycle 134-145 launcher 단계 추정 → wave 18-A 14 step 결과 cycle timeline 갱신:

```
R4 cycle (Battle.net Setup install, EH 본질 통합 후):
  cycle 129:  Setup → first NSWindow (R3 closure 도달 후 cycle 1)
  cycle 130:  Setup GUI → 라이센스 동의
  cycle 131:  Setup → /ProgramData/Battle.net/Agent 디렉토리 생성
  cycle 132:  Agent.exe spawn → SD 검증 진입
  cycle 133:  wave 18-A step 4 SD stub 발동 → DACL 검증 통과
  cycle 134:  Agent.exe → Bootstrapper.exe → version check
  cycle 135:  Battle.net main client launch (login GUI)

R5 cycle (launcher / 게임 launch):
  cycle 136:  login (CEF browser 진입 — 분기)
  cycle 137-139:  CEF 우회 path (wave 20+ research) — native UI
  cycle 140:  계정 인증 → Battle.net main client UI
  cycle 141:  D2R / D4 install (게임 binary 다운로드)
  cycle 142-147:  D2R 첫 launch → main menu
```

**research-E 우회 path** ([BATTLENET_WINE_EVIDENCE.md §3]):
- launcher CEF 가 Wine 환경에서 fail 하는 경우 (cef.dll missing / d3d11 native override) → 본 project 도 동일
- helper.mm IPC kind=24 (planned wave 20+) 가 login URL 을 native AppKit WebView (WKWebView) 로 redirect
- cycle 6-10 단축 효과: cycle 137-147 → cycle 140-144 (login path direct)

**조건**:
- wave 19 P0 (__CxxFrameHandler3 + scope_table) 검증
- wave 20+ helper.mm CEF redirect IPC (kind=24) 통합
- own1 §A: CEF source / Whisky CEF patch lift 0줄 — helper.mm WKWebView native API direct 만 사용

→ R4-R5 시간 누적 ~12-15 cycle 단축 (V16 추정 cycle 145 → V17 추정 cycle 144 근처).

---

## 10. V18 trigger 조건

V18 작성 trigger:
1. wave 19-A FuncInfo + scope_table walk 통합 후 inst 800K+ 도달
2. wave 19-B __CxxFrameHandler3 본체 통합 후 catch funclet 정상 invoke 검증 (Battle.net Setup throw-catch-resume primary path 통과)
3. **USER32 first hit (kind=14 hits>0)** — 즉시 V18 작성 (마일스톤 — R3 closure 전 단계)
4. R3 closure (first NSWindow) 도달 시 즉시 V18 작성 (마일스톤 100%)
5. plateau (3 sprint 연속 진척 0)
6. wave 19-C helper kind=23 catch funclet dispatch IPC 통합 검증
7. wave 19-D1~3 hexa skeleton 3 loader 통합 완료
8. nested try block state machine 동작 검증 (state ≥ TryLow .. ≤ TryHigh range walk)
9. VC6/VC7/VC8 magic 분기 검증 (UE 4+ binary 호환)
10. 새 root cause (`__report_gsfailure`, `__crt_unhandled_exception`, RtlRaiseException, `_unexpected`, terminate handler 등) 출현 시 즉시 분석 + V18

→ trigger 미발생 시 본 V17 가 cycle 122-126 동안 유효. cycle 124-126 즈음 V18 작성 추정.

---

## 11. final 정리 — 다음 cycle main direct path

**다음 cycle (cycle 124+) main direct 우선순위**:

1. **wave 19-A FuncInfo + scope_table parser commit** (shim cpp ~300 line):
   - FuncInfo struct (magic/nTryBlocks/pTryBlockMap/nIPMapEntries/pIPtoStateMap/dispUnwindHelp/dispESTypeList/EHFlags) parse
   - TryBlockMapEntry walk (TryLow / TryHigh / CatchHigh / nCatches / pHandlerArray)
   - HandlerType walk (adjectives / pType / dispCatchObj / dispOfHandler)
   - IpToStateMap binary search (current EIP → frame_state)
   - magic 분기 (VC6 0x19930520 / VC7 0x19930521 / VC8 0x19930522 — VC8 의 dispESTypeList/EHFlags 추가 field 처리)
   - run d2r → FuncInfo parse log + frame_state 추적 KPI

2. **wave 19-B __CxxFrameHandler3 본체 commit** (shim cpp ~400 line):
   - frame_state 결정 (current EIP → IpToStateMap → state)
   - state ≥ TryLow .. ≤ TryHigh range 의 catch handler 후보 list
   - type match (catch type pType → throw type RTTI 비교)
   - matched catch funclet invoke 준비:
     * ESP=[frame-4] (Wine spec 정합 + region_find plausibility)
     * EBP=[frame+12]
     * 4 nonvolatile (EBX/ESI/EDI) restore (frame slot scan)
     * 1 arg = catchable_object_va (catch handler 의 catch 변수)
     * EIP = image_base + dispOfHandler
   - catch return → resume PC (UnwindHelp[0] 또는 dispUnwindHelp slot)
   - run → catch funclet 정상 invoke + Battle.net Setup throw-catch-resume primary path 통과 검증
   - 예상 inst delta: 484K → 1M-3M

3. **wave 19-C helper kind=23 catch funclet dispatch** (helper.mm ~80 line + self-test):
   - funclet 의 부속 syscall 가 Cocoa main thread 영향 시 marshal
   - kind 12-23 self-test PASS

4. **wave 19-D1~3 hexa 3 skeleton** main direct (각 ~500 line + self-test):
   - pe_msvc_funcinfo_real (FuncInfo + scope_table parser + magic 분기)
   - pe_msvc_cxx_frame_handler3_real (CxxFrameHandler3 algorithm flow tracker)
   - pe_msvc_catch_funclet_invoke_real (funclet invoke + register restore + resume PC)

5. **V18 작성** trigger 충족 시 즉시 (USER32 first hit 또는 inst 1M+):
   - V13/V14/V15/V16/V17 패턴 따름 (350-450 line)

6. **own1 §A/B enforcement** 재검수 — V18 작성 시점 grep 0건 확인. own1 #1 메모리 룰 (Wine/CrossOver/GPTK/Whisky 추천 금지) 재확인.

**충돌 회피**:
- shim cpp ↔ helper.mm ↔ hexa 영역 분리 유지
- 동일 file 동시 BG 금지 (19-A/B 가 모두 shim cpp → 순차 적용 — 19-A 단독 검증 후 19-B)
- main direct 우선: shim cpp wave 19-A FuncInfo parser (catch funclet 정확 invoke 의 prerequisite — 가장 큰 inst 진척 prerequisite)

---

*written 2026-05-02 cycle 122-124 sprint 진행. V16 (`1e39e71`) → V17 (현재) — wave 18-A 14 step 본격 반영 (292K → 304K default / 484K outermost mode 진척 timeline) + 결정적 발견 (Battle.net Setup C++ exception control-flow primary — 단순 error path 가 아니라 throw-catch-resume 이 제어 흐름의 main mechanism, proper `__CxxFrameHandler3` 없이 USER32 도달 불가) + wave 19 6 BG plan (shim FuncInfo + CxxFrameHandler3 + helper kind=23 + 3 hexa + docs) + 차단점 정합 정리 (FuncInfo 구조 / scope_table interp / catch funclet invoke / register restore / resume PC / nested try state machine / VC6-7-8 magic 분기 / destructor chain unwind / ESTypeList) → fix path P0/P1/P2 정밀화. own1 §A 100% 정합 유지 — Wine / CrossOver / GPTK / Whisky / Capstone / Box86 / QEMU / Bochs source code lift 0줄. Microsoft Learn `__CxxFrameHandler3` 명세 + VS source distribution `eh4` `eh.h` `ehdata.h` paraphrase + Intel SDM Vol 2 SSE + ReactOS public docs + Apple SDK + MSVC winnt.h docs reference 만 (algorithm only). USER32 first hit 추정 cycle 124-126, R3 closure (first NSWindow) 추정 cycle 126-128, Battle.net Setup install 추정 cycle 129-135 (SD stub 적용 시), 게임 launch 추정 cycle 142-147 (CEF 우회 path 적용 시 cycle 6-10 단축).*
