# MSVC CRT init 우회 전략 — `__scrt_common_main_seh` + `__security_init_cookie` + `_initterm` + CFG

**작성**: 2026-05-01 cycle 105+ · base commit `0a24e22` (wave 6 8360 inst stable) → wave 7-A 종료 시점 inst[15214] 도달 후 정체.
**현 halt**: `ret_to_unmapped` EIP=`0x00000007` — `__report_gsfailure` 의 abort path 진입 후 INT3 / fast_fail trampoline 의 0x7 sentinel 으로 EIP corrupt.
**원인 가설**: MSVC LTCG (Link-Time Code Generation) + CFG (Control Flow Guard) 빌드의 stack canary check (`__security_check_cookie`) 가 invalid cookie 검출 → `__report_gsfailure` abort path 진입 → graceful return 부재.
**참조**: [R3_HALT_343_ANALYSIS.md](R3_HALT_343_ANALYSIS.md) (343 inst halt + fake_proc_stub stdcall) / [R3_PROGRESS_V7.md](R3_PROGRESS_V7.md) (8360 inst plateau) / [BATTLENET_DAY1_PLAN.md](BATTLENET_DAY1_PLAN.md) (Day 1-7 actionable).
**own1 enforcement**: Wine / Box86 / QEMU / Bochs / Capstone / Zydis / iced / Apple GPTK / CrossOver / Whisky source 0줄. MSDN `learn.microsoft.com` public docs reference 만 (`__scrt_common_main_seh`, `__security_init_cookie`, `_initterm`, `IMAGE_LOAD_CONFIG_DIRECTORY`, `IMAGE_GUARD_CF_FUNCTION_TABLE`).

---

## 0. 한 줄 요약

> **wave 7-A 의 inst[15214] halt 는 `__report_gsfailure` abort path 의 INT3 (0xCC) → EIP+1 → 0x07 unmapped — 즉 MSVC `/GS` (buffer security check) 의 cookie validation 실패가 본질. wave 8 의 3 sub-phase (security_cookie + _initterm 단순화 + abort path graceful) 로 inst[15000] → inst[80000-150000] 로 진보 + WinMain 진입 + 첫 NSWindow 표시 (inst ~100000-200000) 도달 가능.**

---

## 1. MSVC `__scrt_common_main_seh` 정상 sequence

MSDN 의 `vcruntime` source spec 및 PE binary 의 `__scrt_common_main_seh` symbol disassembly 기준 (Microsoft Docs `Welcome to vcruntime` / `_initterm, _initterm_e` / `__security_init_cookie` 공개 문서 참조) — Visual C++ 2015+ runtime 의 entry path 는 다음 순서로 흐른다.

### 1.1 sequence 표

| step | 함수 | 역할 | 실패 조건 | abort 경로 |
|---|---|---|---|---|
| 1 | `__scrt_initialize_crt(__crt_module_type::dll \| exe)` | CRT 모듈 type 결정 | type 미상 | abort |
| 2 | `__scrt_acquire_startup_lock()` | startup mutex 획득 (다중 thread CRT init race 차단) | already-acquired sentinel | early return (skip) |
| 3 | `__scrt_initialize_thread_safe_statics()` | thread-safe static 초기화 | TLS slot 부재 | abort |
| 4 | **`__security_init_cookie()`** | `/GS` cookie 생성 (canary) | RNG 부재 | weak cookie + 경고 |
| 5 | `_initterm_e(__xi_a, __xi_z)` | C init (errno 검사 ext init) | non-zero return | abort with errno |
| 6 | `_initterm(__xc_a, __xc_z)` | C++ ctor table walk | ctor 내 throw | terminate |
| 7 | `_set_app_type(_CONSOLE_APP \| _GUI_APP)` | app type | (no-op stub OK) | — |
| 8 | `_get_initial_narrow_environment()` / `_get_narrow_winmain_command_line()` | env block + cmdline | NULL pointer | abort |
| 9 | `__scrt_translate_winmain()` | WinMain shim wrapper | — | — |
| 10 | **`WinMain(hInstance, NULL, lpCmdLine, nShowCmd)`** | user entry | — | — |
| 11 | `__scrt_common_main_seh` 의 SEH frame 종료 | `_cexit` + `ExitProcess(retval)` | — | — |

### 1.2 wave 7-A 까지의 도달 분포

- step 1-3: wave 5 (commit `52502ca`) 의 `__scrt_initialize_crt` / `__scrt_acquire_startup_lock` stub return (acquired sentinel 0/1) 으로 통과.
- step 4 (`__security_init_cookie`): wave 6/7-A 에서 **abort 진입** — 본 문서의 main subject.
- step 5-6 (`_initterm_e` / `_initterm`): wave 5 의 no-op return 0 통합 OK. 단 array walk 내 ctor 가 cookie 의존 시 step 4 실패가 전파.
- step 7-10: wave 7-A 미도달.

---

## 2. `fake_eax=0` 만으로 trigger 되는 abort path 의 정확한 chain

MSVC `/GS` (buffer security check) 컴파일 옵션은 함수 prologue 에 `mov eax, [__security_cookie]; xor eax, ebp; mov [ebp-4], eax` 의 canary write 를 삽입하고, epilogue 에 `mov ecx, [ebp-4]; xor ecx, ebp; call __security_check_cookie` 를 삽입한다. `__security_check_cookie` 는 `cmp ecx, [__security_cookie]; jne __report_gsfailure` 단순 비교.

### 2.1 abort path step-by-step

```
[step A] __security_init_cookie 호출
   ↓
   GetSystemTimeAsFileTime(&ft)        ← shim stub: arg[0] FILETIME* 무시 후 EAX=0 return
   ↓                                   ← FILETIME 영역 zero 잔류
   GetCurrentThreadId()                ← shim stub: EAX=0 return (가짜 tid)
   ↓
   GetCurrentProcessId()               ← shim stub: EAX=0 return (가짜 pid)
   ↓
   QueryPerformanceCounter(&pc)        ← shim stub: arg[0] LARGE_INTEGER* 무시
   ↓                                   ← LARGE_INTEGER 영역 zero 잔류
   cookie 계산 = ft ^ tid ^ pid ^ pc
   ↓                                   ← 모두 0 → cookie = 0
   __security_cookie = 0xBB40E64E (default cookie sentinel for /GS)
   ↓                                   ← MSVC 의 /GS 는 cookie==default 면 weak cookie 경고

[step B] WinMain prologue 의 canary write
   ↓
   mov eax, [__security_cookie]        ← EAX = 0xBB40E64E (default sentinel)
   xor eax, ebp                        ← canary = ebp ^ default
   mov [ebp-4], eax                    ← stack 에 write

[step C] WinMain epilogue 의 canary check
   ↓
   mov ecx, [ebp-4]                    ← canary read
   xor ecx, ebp                        ← decode
   call __security_check_cookie

[step D] __security_check_cookie
   ↓
   cmp ecx, [__security_cookie]
   ↓                                   ← 정상 path 에서는 동일 → ret
   ↓                                   ← 단 EBP 가 wave 7 의 fake stack 진입 중에 변경되었다면 mismatch
   jne __report_gsfailure              ← mismatch → abort

[step E] __report_gsfailure
   ↓
   __fastfail(FAST_FAIL_STACK_COOKIE_CHECK_FAILURE)  ← INT 29h (CD 29)
   또는
   INT 3                               ← debugger break (0xCC)
   ↓                                   ← shim 의 INT 3 핸들러: EIP += 1, 계속 → EIP=0x07 (rebase 0x00000007 unmapped)
   [halt] ret_to_unmapped EIP=0x7
```

### 2.2 inst[15214] 의 정확 위치

- inst[15214] 의 직전 EIP 는 `__report_gsfailure` symbol 영역 (PE LoadConfig directory 의 `GuardCFFunctionTable` 에 등록된 fast_fail trampoline) 으로 추정.
- INT 3 (0xCC) 1-byte → shim interpreter 의 unknown opcode 처리: EIP += 1, EAX=0 → 다음 byte (random padding) 가 `0x07 pop es` (32-bit invalid in protected mode but shim 미검증) → EIP = stack [esp] 의 4-byte 가 `0x00000007` 로 unmapped.
- 즉 abort path 는 **graceful return** 이 아닌 **deliberate crash** 이며, shim 이 그 의도를 무시하고 통과시켜야 진행 가능.

---

## 3. 해결 전략 — wave 8 의 3 sub-phase

### 3.1 wave 8-A — security_cookie / initterm / FILETIME stub 정밀화 (15000 → 25000+ inst 예상)

**3.1.1 `GetSystemTimeAsFileTime` stub 정밀화**

현재 (wave 7-A 까지): EAX=0 return, arg[0] FILETIME* 무시.
wave 8-A: arg[0] FILETIME* 에 100ns since 1601-01-01 epoch 의 valid 값 write.

```
// shim 의 stub (의사 코드)
void stub_GetSystemTimeAsFileTime(uint32_t* esp) {
    uint32_t lpft = guest_read32(esp + 4);  // arg[0]
    if (lpft == 0) return;
    // macOS gettimeofday → 100ns since 1601-01-01
    struct timeval tv; gettimeofday(&tv, NULL);
    uint64_t ft = ((uint64_t)tv.tv_sec * 10000000ULL)
                + ((uint64_t)tv.tv_usec * 10ULL)
                + 116444736000000000ULL;  // 1970→1601 offset
    guest_write32(lpft + 0, (uint32_t)(ft & 0xFFFFFFFF));
    guest_write32(lpft + 4, (uint32_t)(ft >> 32));
    set_eax(0);
}
```

**3.1.2 `GetCurrentThreadId` / `GetCurrentProcessId` stub 정밀화**

```
EAX = (uint32_t)getpid();  // real pid (cookie entropy 충분)
EAX = (uint32_t)pthread_mach_thread_np(pthread_self());  // real tid
```

**3.1.3 `QueryPerformanceCounter` stub 정밀화**

arg[0] LARGE_INTEGER* (8-byte) 에 mach_absolute_time() 의 64-bit nanosecond 값 write.

**3.1.4 LoadConfig 의 SecurityCookie RVA 사전 write**

PE optional header 의 LoadConfig directory (`IMAGE_DIRECTORY_ENTRY_LOAD_CONFIG = 10`) 에 `IMAGE_LOAD_CONFIG_DIRECTORY32::SecurityCookie` field (offset 0x3C in 32-bit) 가 있음. 이 RVA 가 가리키는 4-byte 위치에 PE loader 가 cookie 를 write 한다. shim 의 PE mmap 단계에서 그 위치를 식별 후, image base + RVA 에 random 4-byte (mach_absolute_time 의 lower 32-bit XOR pid) 를 사전 write.

```
// PE loader 측 (lib/loader/pe_*.hexa)
let lc_dir = pe.optional_header.data_directory[10]
if lc_dir.size >= 0x40:
    let lc = pe.image_base + lc_dir.virtual_address
    let cookie_rva = read_u32(lc + 0x3C)
    if cookie_rva != 0:
        let cookie_va = pe.image_base + (cookie_rva - pe.image_base)  // SecurityCookie 는 VA
        let entropy = mach_absolute_time() ^ getpid()
        write_u32(cookie_va, entropy & 0xFFFFFFFF)
```

**3.1.5 `_initterm` array walk 단순화 (cap 200 ctor)**

`_initterm(begin, end)` 는 begin..end 범위의 함수 ptr table 을 순회하며 각 entry 가 NULL 이 아니면 호출. shim 측에서:
- `_initterm` 진입 시 begin/end 를 args 에서 읽음
- (end - begin) / 4 가 200 초과 시 **cap 200** 으로 truncate (악성 entry 무시)
- 각 entry 가 image base 영역 밖 또는 NULL 이면 skip
- 정상 entry 는 fake call (EAX=0 return + ESP 정리)
- 단 entry 가 cookie write 함수 (`__security_init_cookie` resolved import) 면 **3.1.4 의 cookie 사전 write 와 정합 보장**

**inst 진척 예상**: 15214 → 25000+ (security_init_cookie 통과 + _initterm array walk 의 200 ctor 통과).

### 3.2 wave 8-B — abort path graceful 처리 (25000 → 40000+ inst 예상)

**3.2.1 INT 3 (0xCC) 의 graceful 처리**

shim interpreter 의 unknown opcode → halt 정책을 INT 3 한정으로 수정.

```
case 0xCC:
    // INT 3 — debugger break, normally crash
    // 하지만 wave 8-B 에서는 graceful: EIP += 1, EAX=0, continue
    eip += 1;
    set_eax(0);
    log_warn("INT3 ignored at EIP=0x%x (graceful)", eip - 1);
    break;
```

**3.2.2 `__fastfail` (CD 29) 의 graceful exit**

```
case 0xCD:  // INT imm8
    {
        uint8_t imm = guest_read8(eip + 1);
        if (imm == 0x29) {
            // __fastfail — Microsoft 의 deliberate process termination
            // shim 측: graceful exit (NSApplication terminate) 또는 ret to caller
            log_warn("INT 29h fastfail at EIP=0x%x — graceful exit", eip);
            // option A: graceful exit
            graceful_exit();
            // option B: ret to caller (stack 의 RA 로 jump, EAX=0)
            // eip = pop_stack();
            return HALT_GRACEFUL;
        }
        // 다른 imm8 은 기존대로
        eip += 2;
    }
    break;
```

**3.2.3 `__report_gsfailure` symbol 자체 stub return 0**

PE binary 의 IAT / 내부 symbol table 에 `__report_gsfailure` resolve 시점에 shim 이 fake addr 발급 + dispatch 단계에서 단순 `ret` 처리. caller 측 cleanup 은 caller 의 stack frame 이 이미 corrupt 인 상태이므로 best-effort.

**inst 진척 예상**: 25000 → 40000+ (abort path graceful 우회 후 _initterm 의 잔여 ctor + WinMain prologue 진입).

### 3.3 wave 8-C — fake_proc 4-byte stub (40000 → 60000+ inst 예상)

[R3_HALT_343_ANALYSIS.md](R3_HALT_343_ANALYSIS.md) 의 **차선 fix** path 통합. GetProcAddress 가 반환하는 fake addr 자체가 4-byte stdcall stub 이 되도록.

```
// shim 측: dispatch_addr_table entry 등록 시 4-byte stub mmap
//   83 c4 NN c3 = add esp, NN; ret
//   NN = arg_count × 4 (stdcall callee-pop)
// 함수별 unique stub addr 발급 → caller 의 indirect call 이 정상 stack 정리 + ret
```

**inst 진척 예상**: 40000 → 60000+ (모든 indirect call 정상 unwind + WinMain 진입 직전).

---

## 4. CFG (Control Flow Guard) 우회 전략

### 4.1 CFG 의 PE 구조

PE optional header 의 LoadConfig directory 내 `GuardCFFunctionTable` (RVA) + `GuardCFFunctionCount` 가 indirect call target 의 whitelist 를 정의. 정상 Windows loader 는 이 table 을 GuardCFCheckFunctionPointer (`_guard_check_icall`) 에 등록하여 indirect call 마다 verify.

### 4.2 shim 의 우회

- shim 은 Windows loader 가 아니므로 `_guard_check_icall` 가 호출되더라도 **whitelist 무시** 후 EAX=0 / `ret` 처리.
- PE binary 의 prologue 에서 `_guard_check_icall` 이 IAT 또는 내부 symbol 로 resolve 되는 시점에 shim stub 이 단순 `ret` 발급 — indirect call 직전의 verify call 은 no-op.
- **Indirect Call Verification 미대응 시 silent fail 위험** — CFG enabled binary 의 일부 함수는 `_guard_check_icall` 호출 후 EAX 또는 EFLAGS 의 특정 flag 를 본다. wave 8-C 후 silent fail 발생 시 EAX=대상 addr (passthrough) 로 stub 강화.

```
// shim 측 _guard_check_icall stub
void stub_guard_check_icall(uint32_t* esp) {
    // arg[0] = target indirect call addr (ECX in fastcall, [esp+4] in stdcall)
    uint32_t target = get_ecx();
    // CFG 의 verify 결과 = 항상 valid
    // EAX 는 변경 X (caller 가 indirect call 직전에 EAX 보존 안 함)
    // EFLAGS 의 ZF 도 무관 (CFG 는 보통 jne __report_cfg_failure 와 함께 사용)
    set_zf(0);  // not equal → fail path 진입 안 함 (단, CFG 의 cmp 결과는 보통 ZF=1 이 success)
    // 보수적: ZF=1 (success)
    set_zf(1);
    return;  // ret 0
}
```

### 4.3 GuardCFFunctionTable 의 조기 우회

PE loader 단계에서 LoadConfig 의 `GuardFlags` field 의 `IMAGE_GUARD_CF_FUNCTION_TABLE_PRESENT` (0x400) 비트를 **clear** 후 image 에 write — Microsoft loader 가 그 binary 를 CFG-disabled 로 인식하도록 위장. shim 자체는 어차피 CFG 무시이지만, binary 내부의 `_guard_check_icall` 호출 자체가 사라지면 inst 효율 향상.

---

## 5. abort path 의 graceful 처리 정책

| signal | 의미 | shim 의 처리 (wave 8-B) | 잔여 위험 |
|---|---|---|---|
| INT 3 (0xCC) | debugger break | EIP += 1, EAX = 0, continue | next byte 가 invalid opcode 일 시 다음 cycle 에 halt — 추가 1-byte skip 필요 |
| INT 29h (CD 29) | `__fastfail` | graceful exit (NSApplication terminate) | exit code 손실 |
| `__report_gsfailure` 호출 | `/GS` cookie mismatch | stub return 0, caller cleanup | caller 의 stack frame 이 이미 corrupt 일 가능성 |
| `__report_rangecheckfailure` | `/RTCs` 검사 실패 | stub return 0 | bound check skip (silent) |
| `_invalid_parameter_noinfo` | CRT param check | stub return 0 | bad arg 무시 |
| `abort()` / `exit()` | 명시적 abort | graceful exit | 정상 path 와 구분 X — log 만 남김 |
| UD2 (0F 0B) | undefined opcode | EIP += 2, EAX = 0, continue | binary translation cache miss 와 충돌 위험 |

---

## 6. inst 도달 추정 표

[R3_PROGRESS_V7.md](R3_PROGRESS_V7.md) 의 wave6 표 형식 차용.

| wave | sub-phase | 시작 inst | 종료 inst (예상) | delta | 누적 배수 (vs 17) | 차단 위치 |
|---|---|---|---|---|---|---|
| 6-ε | max_iter cap 65536 | 2899 | 8360 | +5461 | 491.8× | CRT init 깊이 |
| 7-A | 분산 — locale init / atexit | 8360 | 15214 | +6854 | 894.9× | `__report_gsfailure` abort |
| **8-A** | **security_cookie + _initterm + FILETIME** | **15214** | **25000+** | **+9786+** | **~1470×** | abort path INT3 |
| **8-B** | **abort path graceful (INT3 / fastfail / gsfailure)** | **25000** | **40000+** | **+15000+** | **~2350×** | indirect call stack corrupt |
| **8-C** | **fake_proc 4-byte stub (stdcall callee-pop)** | **40000** | **60000+** | **+20000+** | **~3530×** | WinMain prologue 진입 직전 |
| **8-D** | **WinMain 진입 + RegisterClassExA 도달** | **60000** | **80000-150000** | **+20000-90000** | **~4700-8800×** | CreateWindowExA 직전 |
| **8-E** | **CreateWindowExA + helper IPC kind=14 — 첫 NSWindow** | **80000-150000** | **100000-200000** | **+20000-50000** | **~5880-11760×** | message loop |

---

## 7. 위험 요소

### 7.1 LTCG (Link-Time Code Generation) 의 inline / aggressive opt

- LTCG 빌드는 함수 경계가 모호 — `__security_init_cookie` 가 호출자 (`__scrt_common_main_seh`) 안에 inline 되어 별도 symbol 없이 prologue 안에 분산 배치 가능.
- shim 의 stub dispatch 가 symbol 단위로 동작하므로, inline 된 cookie 생성 path 는 stub 이 아닌 **interp 의 일반 명령 처리** 로 흐름.
- 대응: 3.1.4 의 LoadConfig SecurityCookie RVA 사전 write 가 inline 여부와 무관하게 동작 — **첫번째 방어선**.

### 7.2 CFG 의 Indirect Call Verification 미대응 silent fail

- `_guard_check_icall` 의 결과를 cmp + jcc 로 확인하는 binary 의 경우, shim 의 ZF=1 stub 만으로 부족 가능.
- 대응: wave 8-C 후 silent fail 관찰 시 ECX = target_addr passthrough + EAX = 0 + ZF=1 + CF=0 의 다중 flag set.

### 7.3 Battle.net Setup 의 Microsoft Authenticode 서명

- Battle.net Setup `.exe` 는 Microsoft Authenticode 서명 (PE Certificate Table, IMAGE_DIRECTORY_ENTRY_SECURITY = 4).
- 일부 buildflow 는 `WinVerifyTrust` 호출 후 fail 시 abort.
- shim stub 의 `WinVerifyTrust` → return 0 (ERROR_SUCCESS) 처리. WINTRUST.dll dispatch table 추가 (KERNEL32 / USER32 / GDI32 / MSVCR / NTDLL / ADVAPI32 / WINTRUST).
- 단 self-verify 가 binary 내부 (Crypt32 + bcrypt 결합) 로 수행되는 경우 stub 만으로 부족 — 그 경우 `CryptVerifySignatureA` / `BCryptVerifySignature` stub 도 추가.

### 7.4 own1 enforcement

- 본 문서의 모든 reference 는 MSDN `learn.microsoft.com` public docs 와 PE/COFF Specification (Microsoft public) 만 사용.
- Wine 의 `dlls/msvcrt/`, `dlls/kernel32/` source 0줄 — 본 문서 작성자는 직접 disassembly + MSDN 만 reference.
- ReactOS 도 derivative work (Wine 일부 lift) 가능성으로 본 wave 8 에서는 reference 제외.
- LLVM `compiler-rt` 의 `__security_init_cookie` reimpl 도 reference X (BSD/MIT 라이선스이지만 own1 §A 의 "trigger" 절대 회피).

---

## 8. 검증 plan

| 항목 | 방법 | 통과 조건 |
|---|---|---|
| security_cookie 사전 write | shim log 의 `cookie_va = 0x...` 와 `entropy = 0x...` 출력 | non-zero entropy + LoadConfig RVA 일치 |
| INT3 graceful | wave 8-B 빌드 후 inst log 의 `INT3 ignored at EIP=0x...` 카운트 | ≥ 1 회 + 이후 inst 진척 +5000 이상 |
| _initterm cap 200 | log 의 `_initterm ctor walk: N entries (cap 200)` 출력 | N ≤ 200 + 모두 fake call OK |
| WinMain 진입 | log 의 `WinMain entered: hInstance=0x..., lpCmdLine=...` | inst 80000+ 시점 |
| RegisterClassExA | helper.mm IPC log 의 `kind=13 RegisterClassExA recv` | 1 회 이상 |
| CreateWindowExA + 첫 NSWindow | helper.mm log 의 `kind=14 CreateWindowExA recv` + NSWindow alloc | 1 회 이상 + NSWindow 의 windowNumber non-zero |

---

## 9. own1 §A 정합 재확인

- 본 문서 신규 lift 0줄. Wine source / Box86 source / Capstone / Zydis / iced / Apple GPTk / CrossOver / Whisky 0줄.
- MSDN public docs reference: `__scrt_common_main_seh` (Microsoft Docs `Welcome to vcruntime`), `_initterm, _initterm_e` (Microsoft Docs `_initterm`), `IMAGE_LOAD_CONFIG_DIRECTORY32` (Microsoft PE/COFF Specification §6.10), `IMAGE_GUARD_CF_FUNCTION_TABLE` (Microsoft Docs `Control Flow Guard`).
- 자체 작성: shim stub 의 `GetSystemTimeAsFileTime` / `QueryPerformanceCounter` / `_guard_check_icall` 의 macOS gettimeofday + mach_absolute_time + pthread_mach_thread_np mapping 은 자체 표.
- 자체 작성: cookie 사전 write 의 LoadConfig RVA 추출 + 4-byte write 는 PE/COFF Specification 만 reference, 코드 자체는 자체.

---

## 10. wave 8 trigger 정의

- **wave 8-A**: shim `pe_to_macho_shim.cpp` 의 `stub_GetSystemTimeAsFileTime` / `stub_QueryPerformanceCounter` 정밀화 + `stub_GetCurrentThreadId` / `stub_GetCurrentProcessId` 의 real id mapping + LoadConfig SecurityCookie RVA 사전 write (PE loader 측 hexa file 신규) + `_initterm` cap 200.
- **wave 8-B**: shim interpreter 의 INT 3 (0xCC) / INT 29h (CD 29) graceful 처리 + `__report_gsfailure` symbol stub return 0 + `_invalid_parameter_noinfo` / `__report_rangecheckfailure` stub return 0.
- **wave 8-C**: shim 의 dispatch_addr_table entry 별 4-byte stdcall stub mmap (`83 c4 NN c3`) + GetProcAddress 반환 fake addr 가 unique stub 위치를 가리키도록.
- **wave 8-D**: WinMain 진입 + RegisterClassExA 의 helper IPC kind=13 + window class map.
- **wave 8-E**: CreateWindowExA 의 helper IPC kind=14 + NSWindow real alloc + 첫 NSWindow 표시 visible 검증.

각 sub-phase 종료 시 inst 진척 + halt 위치 + own1 §A 정합 재확인 후 `R3_PROGRESS_V8.md` 신규 작성 + commit.

---

**END OF DOC** — wave 8 trigger 후 `R3_HALT_15214_ANALYSIS.md` (wave 8-A 통과 검증) + `R3_PROGRESS_V8.md` (8 sub-phase 통합 결과) 후속 문서 작성 예정.
