# REACTOS / BOX86 ARCHITECTURE 분석

생성일: 2026-05-02
범위: ReactOS (BSD-like / GPLv2 mix) 와 Box86 (MIT) 의 architecture/algorithm 분석.
own1 정합: code lift 0줄 — 함수명/구조명만 citation, 구현은 자력. Wine / CrossOver / GPTK / Whisky 는 trigger 로 언급하지 않음.
clone:
- `/tmp/reactos-research/` (`https://github.com/reactos/reactos.git`, depth=1)
- `/tmp/box86-research/` (`https://github.com/ptitSeb/box86.git`, depth=1)

---

## 1. ReactOS architecture — Win32 PE loader / SEH / CRT

### 1-1. NDK (Native Developer Kit) 가 제공하는 Win32 internal struct

위치: `/tmp/reactos-research/sdk/include/ndk/`

핵심 header (citation only — 구조 정의는 자력 재작성 필요):
- `ldrtypes.h` — `LDR_DATA_TABLE_ENTRY`, `PEB_LDR_DATA`, `LDR_DLL_LOAD_REASON`
- `ldrfuncs.h` — `LdrLoadDll`, `LdrGetProcedureAddress`, `LdrUnloadDll`
- `ketypes.h` — `KTHREAD`, `KPROCESS`, `KAPC`
- `pstypes.h` — `PEB`, `TEB`, `RTL_USER_PROCESS_PARAMETERS`
- `rtltypes.h` — `EXCEPTION_REGISTRATION_RECORD`, `DISPATCHER_CONTEXT`
- `i386/ketypes.h` — i386 `KTRAP_FRAME`, `FX_SAVE_AREA`

→ airgenome 의 wave 15+ 에서 PEB/TEB stub 정확화 시, **field 이름과 offset 만 NDK 를 reference, 구현은 ReactOS code 0줄 lift 없이 자력 작성**.

### 1-2. PE loader IAT 해소 — `LdrpSnapThunk` 알고리즘

원본: `/tmp/reactos-research/dll/ntdll/ldr/ldrpe.c:937`

알고리즘 요약 (citation only):

1. `LdrpWalkImportDescriptor` 가 `IMAGE_IMPORT_DESCRIPTOR` 를 순회, dll-by-dll 처리
2. `LdrpSnapIAT` (`ldrpe.c:26`) 가 한 dll 에 대한 모든 thunk 를 처리
3. thunk 별로 `LdrpSnapThunk` 호출 (`ldrpe.c:937`):
   - `IMAGE_SNAP_BY_ORDINAL(OriginalThunk->u1.Ordinal)` — ordinal import branch
   - 아니면 `IMAGE_IMPORT_BY_NAME` 로 hint+name 추출
   - **hint 최적화**: `Hint < ExportDirectory->NumberOfNames` 이고 `NameTable[Hint]` 의 string 이 `ImportName` 와 strcmp match → ordinal 즉시 결정
   - hint miss → `LdrpNameToOrdinal` 가 binary search (export name 은 정렬 보장)
4. ordinal → `AddressOfFunctions[Ordinal]` 가 RVA, 더해서 absolute VA 를 IAT slot 에 기록
5. forwarder (RVA 가 export directory 범위 안) → "DllName.FuncName" 문자열 parse → 재귀 resolve

airgenome 적용 가능 pattern:
- `wave 15+` 의 `IAT_REAL_RESOLVE` 에서 hint-fast-path + binary search fallback 를 자력 구현
- forwarder chain 처리 (현재 wave 12 까지 미지원) — `EATForwardedExport` 케이스 추가
- 코드 lift X — algorithm 만 적용

### 1-3. SEH dispatch — `RtlDispatchException`

원본: `/tmp/reactos-research/sdk/lib/rtl/i386/except.c:67`

i386 SEH chain 의 핵심 (citation):

1. `RtlpGetExceptionList()` → `fs:[0]` 에서 첫 `EXCEPTION_REGISTRATION_RECORD*` 획득
2. linked list 순회:
   - frame pointer 가 `StackLow..StackHigh` 안에 있는지 검증
   - `EXCEPTION_CHAIN_END` (0xFFFFFFFF) 까지
3. `RtlpExecuteHandlerForException` 가 handler 호출 (asm thunk — `except_asm.s`)
4. disposition 분기:
   - `ExceptionContinueExecution` — 정상 복귀
   - `ExceptionContinueSearch` — 다음 frame
   - `ExceptionNestedException` — `EXCEPTION_NESTED_CALL` flag 설정 후 nested frame tracking
   - 그 외 → `STATUS_INVALID_DISPOSITION` 재던짐
5. `RtlUnwind` 는 unwind pass — `EXCEPTION_UNWINDING` flag 로 handler 가 cleanup mode 인지 인지

airgenome 적용:
- 현재 native helper 가 `__except_handler3` / `__except_handler4` 를 stub 으로 두는데, `wave 16` 에서 dispatch 단계 (loop, disposition switch) 를 자력 구현
- ReactOS 의 nested-exception flag 처리 pattern 을 reference (Microsoft 호환 binary 의 EH 가 nested frame 을 기대)

### 1-4. `__scrt_common_main_seh` / `__tmainCRTStartup` 동작

원본: `/tmp/reactos-research/sdk/lib/crt/startup/crtexe.c:203` (mingw runtime, public domain)

순서 (citation):

1. `__native_startup_lock` 를 InterlockedCompareExchange 로 잡음 — multi-thread CRT init race 방지, `fiberid = NtCurrentTeb()->StackBase` 를 lock token 으로 사용 (nested 검출)
2. `_initterm(__xi_a, __xi_z)` — `.CRT$XIA` ~ `.CRT$XIZ` 의 C initializer 실행
3. `_initterm(__xc_a, __xc_z)` — `.CRT$XCA` ~ `.CRT$XCZ` 의 C++ static constructor 실행
4. `__dyn_tls_init_callback(NULL, DLL_THREAD_ATTACH, NULL)` — TLS callback
5. `_pei386_runtime_relocator()` — pseudo-relocation
6. `SetUnhandledExceptionFilter(_gnu_exception_handler)` — top-level filter 설치
7. `_fpreset()` — x87 control word reset
8. `__getmainargs` / `__wgetmainargs` 로 argc/argv/envp 수집
9. `mainret = main(argc, argv, envp)` 호출
10. `exit(mainret)` 이 `_initterm(__xt_a, __xt_z)` (atexit table) 실행

→ `MSVC_CRT_INIT_BYPASS.md` 의 stub 표를 ReactOS reference 로 검증 가능. **MS 의 `__scrt_common_main_seh` 가 더 thin** (init lock + initterm + main + exit) — mingw 의 `__tmainCRTStartup` 와 동일 골격이지만 SEH frame 으로 main 을 감쌈 (`__scrt_common_main` 이 `__try { main } __except (filter) {}`).

airgenome 적용:
- wave 13 에서 `_initterm` stub 이 단순 호출만 하는데, `.CRT$XIA..XIZ` / `.CRT$XCA..XCZ` section 를 PE loader 에서 발견한 뒤 `_initterm` 이 실제로 walk
- `__native_startup_lock` race 보호는 single-thread 모드에서는 생략 가능 — wave 17 multi-thread 진입 시 필요

### 1-5. RTL primitives — `sdk/lib/rtl/`

reference 가능한 module (citation):
- `actctx.c` — activation context (manifest)
- `heap.c`, `heappage.c`, `heapuser.c` — Win32 HeapAlloc/HeapFree (`RtlAllocateHeap`)
- `critical.c` — `RtlInitializeCriticalSection`
- `bitmap.c` — `RTL_BITMAP`
- `image.c` — `RtlImageNtHeader`, `RtlImageDirectoryEntryToData` (PE parse 핵심)
- `unicode.c` — `RtlUnicodeStringToAnsiString`

`RtlImageDirectoryEntryToData` 는 PE loader 의 모든 entry 가 의지 — section walk 후 RVA → VA 변환. wave 15 의 자력 PE parser 가 동일 함수를 자체 이름으로 갖는 것이 정합.

---

## 2. Box86 architecture — DBT / FPU / vtable

### 2-1. dynarec 4-pass 구조

원본: `/tmp/box86-research/src/dynarec/dynarec_arm.c:437` (`FillBlock`)

block 생성 pipeline (citation):

1. **pass 0** (`arm_pass0`) — addresses, x86 jump target, block size 결정. `helper.abort` 시 즉시 cancel.
2. block hash: `X31_hash_code` (FNV-like) — self-modifying code 검출용
3. jump target 의 binary search (`while(search) { dichotomy }`) → `helper.insts[i].x86.jmp_insts` 에 native instruction index 캐시
4. **pass 1** — predecessor 정보 채움
5. **pass 2** — register allocation / barrier 결정
6. **pass 3** — actual ARM machine code emit
7. `__clear_cache(actual_p, actual_p+sz)` — i-cache flush, ARM dyncode 실행 전 필수

`MAX_INSTS` 로 cap 두고 `helper.insts[]` array 에 IR 비슷한 표현. native instruction 시작 주소를 `B+4` 부터 두고, block 끝에 `dynablock_t* self` pointer + `arm_epilog` jump 을 두는 layout 은 `block_layout` 주석 (`dynarec_arm.c:438-449`) 에 명시.

airgenome 적용 (wave 16+ DBT cache):
- 현재 wave 12 까지 interpret loop 만 — wave 16 에 `FillBlock`-equivalent 4-pass 자력 작성
- block hash 로 SMC 검출 + page-protect 트리거 unmap
- ARM64 host (M1) 이므로 `dynarec_arm64_*.c` 가 더 직접적 reference (clone path 에 동일 디렉토리 존재).

### 2-2. i386 → ARM trans — `dynarec_arm_XX.c` 분할

opcode prefix 별 file split (citation):
- `dynarec_arm_00.c` — base table (ADD/OR/ADC/SBB/AND/SUB/XOR/CMP/MOV/PUSH/POP)
- `dynarec_arm_0f.c` — 2-byte opcodes (CMOV, MOV reg, SSE2 부분)
- `dynarec_arm_66.c` / `dynarec_arm_660f.c` — operand size override (16-bit / SSE)
- `dynarec_arm_67.c` — address size override
- `dynarec_arm_d8.c` ~ `dynarec_arm_df.c` — x87 FPU (8개 escape)
- `dynarec_arm_f0.c` — LOCK prefix
- `dynarec_arm_f30f.c` — REP / SSE (e.g. `0xF3 0x0F 0x1F` = ENDBR / `REP NOP`)

→ airgenome 의 wave 16 dispatcher 도 prefix 별 file split 적용 가능. 단 our impl 은 자력 - opcode mapping table 자체가 IP.

### 2-3. polling spin loop — REP NOP 처리

원본: `/tmp/box86-research/src/dynarec/dynarec_arm_f30f.c:195`

```
case 0x1F:
    INST_NAME("REP NOP");
    nextop = F8;
    FAKEED;
    break;
```

→ Box86 는 `pause` (F3 90) / `rep nop` 를 **NOP 로 emit + ED parse 만**. spin-wait 자체에 yield/sleep 을 inject 하지 **않음**. 대신:
- block 끝에 dispatcher 가 signal pending 검사 (epilog)
- `box86_dynarec_log` LOG_DEBUG 로 spin 검출 시 사용자에게 표시

airgenome game 의 `EncodePointer` / `Sleep(0)` 폴링 처리 wave 12 이슈와 정합:
- pure spin 은 native ARM busy-loop 으로 emit (CPU cost 그대로)
- 단, audio thread 의 polling 이 wallclock-bound 인 경우 → wrapper 단에서 yield (Box86 가 `wrappedlibpthread.c` 의 `sched_yield` wrapper 로 처리)
- **own1**: timing emulation 은 자력 — Box86 의 `signals.c:480` (my_sigactionhandler_oldcode) algorithm pattern reference, code lift X

### 2-4. x87 FPU — `dynarec_arm_helper.c`

원본: `/tmp/box86-research/src/dynarec/dynarec_arm_helper.c:605` 이하

핵심 자료구조 (citation):
- `dyn->n.x87cache[]` — 8개 stack slot 의 host (ARM NEON D-reg) 매핑 캐시
- `dyn->n.x87stack` — pending push/pop count (지연 sync)
- `x87_stackcount(dyn, ninst, scratch)` — `emu->fpu_stack` 와 `top` 을 한꺼번에 sync (8-modulo)
- `x87_unstackcount(dyn, ninst, scratch, count)` — branch merge 시 reset
- `x87_do_push` / `x87_do_pop` — ST(0) push 시 모든 cache entry 의 stack offset 를 1 증가

알고리즘 essence: x87 의 stack-based register file 을 ARM 의 flat NEON register 로 mapping 하기 위해 **lazy stack pointer sync** 사용. branch barrier 마다 commit.

airgenome 적용:
- wave 16 의 `interp_x87.c` 는 현재 매 instruction 마다 `emu->fpu_top` 을 직접 update — DBT 진입 시 lazy sync 도입 (4-5x 속도 가능)
- code lift X — algorithm idea 만 차용

### 2-5. C++ vtable / exception 처리

원본: `/tmp/box86-research/src/wrapped/wrappedd3dadapter9_gen.h`

vtable 처리 pattern (citation):

```
static void make_vtable_IDirect3DDevice9Vtbl(x86emu_t* emu, IDirect3DDevice9Vtbl* new, IDirect3DDevice9Vtbl* real)
{
    GO(QueryInterface, iFppp, 3);
    GO(AddRef, uFp, 1);
    ...
}
```

`GO(name, signature, argcount)` 매크로가 real native fn 를 wrapper bridge 로 감싼 i386 callable address 로 변환 후 `new->name` 슬롯에 채워 넣는 방식. `iFppp` 는 type signature: `int Function(ptr, ptr, ptr)`.

C++ exception:
- Box86 는 별도 `wrappedstdcpp.c` 가 **없음** (Linux x86 binary 가 system libstdc++ 를 그대로 사용 → ELF resolution 으로 충분, wrapper 불필요)
- `__cxa_atexit` / `__cxa_finalize` / `__cxa_pure_virtual` 만 `wrappedlibc.c:492+` 에 wrapper (libc.so 의 일부)

airgenome 적용 (`I386_EMUL_VTABLE_RESEARCH.md` 보강):
- vtable rewrite pattern: i386 caller 가 `mov eax, [ecx]; call [eax+offset]` 로 vtable dispatch → host wrapper slot 으로 redirect
- 우리는 Win32 COM (`IDirect3D9` 등) 이 target 이라 Box86 의 ELF 케이스보다 **순수 binary vtable + GUID** 처리 필요. signature 전달 ABI (i386 thiscall: ecx=this) 만 algorithm reference.

### 2-6. bridge — i386 callable → host fn

원본: `/tmp/box86-research/src/tools/bridge.c:91` (`AddBridge`)

trick:
```
b->b[sz].CC = 0xCC;       // INT3
b->b[sz].S = 'S'; b->b[sz].C = 'C';
b->b[sz].w = wrapper_fn;  // host signature
b->b[sz].f = real_fn;     // host real
b->b[sz].C3 = N ? 0xC2 : 0xC3;  // RET / RET imm16
b->b[sz].N = N;           // imm16 stack pop
```

i386 emu 가 `0xCC` (INT3) 만나면 → `IsBridge` 검사 → `'S' 'C'` magic 확인 → `wrapper_fn(emu, real_fn)` 호출 → return 시 `0xC3` 가 stack pop 처리. emit 한 brick 의 `&b->b[sz].CC` 가 i386 binary 가 호출 가능한 주소가 됨.

→ airgenome wave 11 의 `import_table_real_resolve` 에서 native helper 함수를 i386 callable address 로 노출하는 방식으로 동일 pattern 자력 구현 가능. **0xCC magic + wrapper struct** 만 idea reference.

---

## 3. airgenome-gamebox wave 15+ 적용 plan

| wave | 차용 pattern (algorithm only) | source citation |
|------|------------------------------|-----------------|
| 15 | PE loader IAT hint-fast-path | `dll/ntdll/ldr/ldrpe.c:LdrpSnapThunk` |
| 15 | forwarder chain resolve | `ldrpe.c:LdrpSnapThunk` forwarder branch |
| 15 | RVA→VA `RtlImageDirectoryEntryToData` semantics | `sdk/lib/rtl/image.c` |
| 16 | DBT 4-pass FillBlock 골격 | `box86/src/dynarec/dynarec_arm.c:FillBlock` |
| 16 | block hash (FNV/X31) for SMC | `box86/src/dynarec/dynablock.c:X31_hash_code` |
| 16 | x87 lazy stack pointer sync | `box86/src/dynarec/dynarec_arm_helper.c:x87_stackcount` |
| 16 | bridge magic (INT3 + sig + RET) | `box86/src/tools/bridge.c:AddBridge` |
| 17 | SEH dispatch loop + nested frame | `sdk/lib/rtl/i386/except.c:RtlDispatchException` |
| 17 | `_initterm` 의 `.CRT$X*` walk | `sdk/lib/crt/startup/crtexe.c:__tmainCRTStartup` |
| 17 | TLS callback dispatch | `crtexe.c:__dyn_tls_init_callback` |
| 18 | vtable rewrite ABI (thiscall ecx=this) | `box86/src/wrapped/wrappedd3dadapter9_gen.h:make_vtable_*` |

---

## 4. own1 정합 enforcement

- ReactOS license: 대부분 GPLv2 + LGPL/BSD-mixed. 우리는 **algorithm 와 함수 이름 reference 만**, source 0줄 lift.
- Box86 license: MIT. 위와 동일 — algorithm reference only.
- 본 문서는 **citation + 우리 plan** 만 기록 — implementation 은 별도 file 에서 자력 작성.
- own1 trigger: 본 문서 어디에도 Wine / CrossOver / GPTK / Whisky 가 trigger 로 등장하지 않음 (reference 로 metadata 비교만 외부 file).

## 5. 추가 deep-dive — ReactOS LdrpWalkImportDescriptor 호출 graph

원본: `/tmp/reactos-research/dll/ntdll/ldr/ldrpe.c` (지원 함수)

PE loader 의 import resolution 전체 파이프라인 (citation):

```
LdrpInitializeProcess
  -> LdrpWalkImportDescriptor(DllPath, DataTableEntry)
     -> for each IMAGE_IMPORT_DESCRIPTOR in IDT:
        -> RtlDosApplyFileIsolationRedirection (manifest redirect)
        -> LdrpLoadImportModule (LoadLibraryEx 등가, 재귀 가능)
        -> LdrpSnapIAT(ExportLdrEntry, ImportLdrEntry, IatEntry, EntriesValid)
           -> RtlImageDirectoryEntryToData (EXPORT)
           -> RtlImageDirectoryEntryToData (IAT) — 없으면 import dir scan
           -> NtProtectVirtualMemory(IAT, PAGE_READWRITE) — IAT 가 .rdata 라 RW 풀어야 됨
           -> for each thunk:
              -> LdrpSnapThunk(...) — 위 1-2 절 algorithm
           -> NtProtectVirtualMemory(IAT, OldProtect) — RW 다시 RO 잠금
        -> InsertTailList(InInitializationOrderModuleList)
  -> LdrpInitializeThread (DLL_PROCESS_ATTACH 호출)
```

핵심 invariant:
- IAT 는 보통 `.rdata` section 안에 위치 → write 직전에 `NtProtectVirtualMemory` 로 PAGE_READWRITE 변경, 끝나면 원복
- bound import 는 별도 path (`LdrpCheckForLoadedDll` 가 module base hint 일치 시 snap skip)
- delayload (helper.c) 는 별도 — `__delayLoadHelper2` callback 이 첫 호출 시 trigger

→ airgenome wave 15 `iat_resolver.mm` 에서 mprotect (mac equiv) 로 IAT 페이지 RW 풀고 patch 후 RO 복원하는 sequence 가 정합. 현재 wave 13 까지는 mmap PROT_READ|PROT_WRITE 로 통째로 잡아 둠 — wave 15 에서 정확화.

## 6. Box86 에 없는 것 — Win32 specific 차이

Box86 는 **순수 Linux x86 binary** 만 — Win32 PE 는 처리하지 않음. 따라서:

- ❌ PE parse / IAT walk
- ❌ SEH chain (fs:[0])
- ❌ TLS slot via fs/gs segment (Box86 는 Linux TLS 만)
- ❌ activation context (manifest)
- ❌ `__scrt_common_main_seh` / mingw `_initterm`
- ✅ ELF DT_PLTGOT / R_386_JMP_SLOT relocation 처리만

→ Win32 binary 처리를 위해서는 ReactOS algorithm 가 메인 reference, Box86 는 **i386 → host machine code translation 부분만** reference. 두 source 의 역할 분담이 자연스러움.

추가로, Box86 의 `signals.c:480` 의 `my_sigactionhandler_oldcode` 는 **POSIX SIGSEGV → i386 emu 로 deliver** pattern. Win32 의 SEH 와는 architecturally 다름:

- POSIX: kernel → SIGSEGV → handler → emu fault inject
- Win32: kernel → KiUserExceptionDispatcher → ntdll RtlDispatchException → user handler chain

airgenome 은 macOS 위에서 Win32 emulation 이라 **두 layer 모두** 필요:
1. mac kernel SIGBUS/SIGSEGV → mach exception port → 우리 handler
2. 우리 handler → Win32 EXCEPTION_RECORD 합성 → emu 의 SEH chain dispatch (RtlDispatchException 자력)

## 7. Box86 의 dynarec barrier — branch merge 알고리즘

원본: `/tmp/box86-research/src/dynarec/dynarec_arm.c:497-542`

핵심 idea (citation):
- 모든 jump target 에 `BARRIER_FULL` 마크 → register cache flush 강제
- 자기 자신을 jump 하는 loop (`j == helper.insts[i].x86.addr`) → block 전체에 `always_test` flag → 매 진입마다 hash 검증
- jump target 이 block 밖 → `X_PEND` flag (eflags lazy eval pending) — block exit 직전에 commit

algorithm essence: register cache 와 lazy eflags 가 control flow merge 점에서 deterministic 한 상태가 되도록, **dichotomy search 로 target index 를 찾고 barrier mark**.

airgenome wave 16 적용:
- 우리 IR (생각 중) 도 동일하게 `JumpTargetSet` + `BarrierMap` 자료구조 자력 작성
- lazy eflags 는 **현재 wave 12 에서 매 instruction 마다 eager compute** — DBT 도입 후 lazy 화

## 8. 검증 방법 — ReactOS / Box86 source 와 비교 없이

own1 enforcement:
- 우리 구현은 own1 정합 자력 작성. **결과 검증은 동일 binary 의 같은 분기로 수행** (e.g. mfc100 의 IAT entry resolve 후 첫 export `__crtCorExitProcess` address 가 Microsoft 가 제공한 export table 의 entry 와 byte-for-byte 일치).
- Source 비교 X — algorithm 만 reference 하고 결과는 binary I/O 로 검증.

## 9. Action items

1. wave 15 IAT resolver 의 hint-fast-path 자력 구현 — `native/iat_resolver.mm` 에 함수 추가
2. wave 15 IAT page 보호 mprotect sequence — `native/pe_loader.mm` 에 RW/RO 토글
3. wave 16 DBT skeleton — `lib/dbt/fillblock.ts` 4-pass 자력 작성
4. wave 16 block hash (X31 / FNV-1a) — SMC 검출용
5. wave 16 jump barrier + dichotomy target search — IR pass 추가
6. wave 17 `_initterm` walker — `entry/crt_init.ts` 에 `.CRT$XIA..XIZ` section 검색
7. wave 17 SEH dispatch loop — `entry/seh_dispatch.ts` (`RtlDispatchException` algorithm 자력)
8. wave 17 mach exception port → SEH bridge — `native/mach_to_seh.mm`
9. wave 18 vtable rewrite — `lib/com/vtable_bridge.ts` (i386 thiscall ABI marshaling)
10. wave 18 bridge magic stub (INT3+S+C+RET) 자력 — `lib/dbt/bridge_stub.ts`

## 10. 결론

ReactOS 와 Box86 는 서로 직교적 reference:
- **ReactOS**: Win32 OS surface (PE, SEH, CRT, TLS, RTL) — algorithm + function name + struct layout
- **Box86**: i386 → host (ARM) 의 DBT 기법 (4-pass, lazy eflags, x87 lazy stack, bridge stub)

둘을 합치면 airgenome 의 wave 15-18 청사진 거의 완성. 단, **모든 구현은 own1 정합 자력** — 본 문서는 citation 과 plan 만 담음. code lift 0줄 — Wine/CrossOver/GPTK/Whisky 미언급.

문서 끝.

