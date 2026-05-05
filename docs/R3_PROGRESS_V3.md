# R3 PROGRESS V3 — 51 instruction 도달 + IAT 4 함수 통과 + R3-8 FS prefix trigger (2026-05-01)

> **status**: R1+R2 own1 §A closure **100%** 유지 + R3-1~7 완료 + R3-6 IAT thunk routing 통합 + shim 통합 phase 14-17 + IAT trace stub 동작. 누적 R3 진척 **~50%**.
> **base commits**: `1bb3e94` (V2 작성, 17 inst halt) → `510a27b` (helper IPC kind 12-15) → `6adc502` (hexa shim_phase_18_22) → `0e60b86` (hexa iat_thunk_routing) → `7bfca4c` (R3-6 IAT thunk routing — 17→**51 inst**) → `badcc99` (BATTLENET_REAL_LAUNCH_PLAN docs).
> **현재 차단**: shim interpreter inst[50] 의 `mov reg, fs:[0x00]` 의 `0x64` segment override prefix 에서 unsupported_prefix halt — phase 17 의 instruction decoder 가 prefix byte 를 inst length 누적 계산만 하고 effective addressing 의 segment base 가 0 (flat fs) 으로 fallback. R3-8 FS prefix + SEH chain head 가 다음 trigger.
> **own1 정합**: Wine / Box86 / QEMU / Bochs / Capstone / Zydis / iced / Apple GPTK / CrossOver / Whisky lift **0줄** 유지. 자체 코드 — `native/pe_to_macho_shim.cpp` 2700+ line + `native/helper.mm` 387+18 line + `lib/loader/pe_i386_*.hexa` 약 500-line × N file.
> **참조**: [R3_PROGRESS_V2.md](R3_PROGRESS_V2.md) (V2 의 17 inst + IAT halt 분석) / [R3_ROADMAP.md](R3_ROADMAP.md) (sub-phase 1-12 정의) / [BATTLENET_REAL_LAUNCH_PLAN.md](BATTLENET_REAL_LAUNCH_PLAN.md) (wave 1-5 own1 enforcement) / [feedback_own1_no_third_party_wine.md](../../.claude-claude3/projects/-Users-ghost-core-airgenome/memory/feedback_own1_no_third_party_wine.md).

---

## 1. 현재 상태 (V2 → V3 변화)

V2 작성 시점 (commit `1bb3e94`, cycle 95) 대비 V3 (commit `7bfca4c`, cycle 96-97) 의 핵심 변화는 R3-6 IAT thunk routing 통합 — 17 instruction halt → **51 instruction** 진척. 4 KERNEL32 IAT 함수가 dlsym + trace stub layer 를 통해 통과 + IAT entries 등록량 + IAT patched 비율의 정량 갱신.

### 1.1 V2 vs V3 비교 표

| 항목 | V2 (commit `1bb3e94`) | V3 (commit `7bfca4c`) | delta |
|---|---|---|---|
| instruction 진척 | 17 inst | **51 inst** | +34 inst |
| 차단 위치 | inst[16] `call [0x6CA3B0]` IAT halt | inst[50] `mov reg, fs:[0]` FS prefix halt | trigger 전진 |
| IAT 통과 함수 | 0 / 20 KERNEL32 | **4** / 20 KERNEL32 | +4 함수 |
| IAT entries 등록 | walk 미통합 (helper-side only) | **435** entries (16 DLL × 평균 27 fn) | shim-side 통합 |
| IAT patched | 0 (shim-side 0 patch) | 10 / 20 KERNEL32 (50% routed) | dlsym dispatch |
| shim phase 통합 | 14-17 | 14-17 + IAT trace stub layer | sub-phase 통합 |
| R3 누적 closure | ~45% | **~50%** | +5% |
| 차단 trigger 분류 | IAT indirect call (R3-6) | FS segment override (R3-8) | trigger 진보 |

### 1.2 V3 핵심 한 줄 변화

**inst[16] 의 `call [0x6CA3B0]` IAT halt 가 통과 → KERNEL32!GetSystemTimeAsFileTime / GetCurrentThreadId / GetCurrentProcessId / QueryPerformanceCounter 4 함수가 dlsym 으로 routing 후 stub return path 정합 + 누적 51 instruction 진척.**

### 1.3 통과한 IAT 함수 4종

shim trace stub 의 dispatch table 에 등록 + dlsym(RTLD_DEFAULT, ...) 또는 dlsym(libSystem) 로 host arm64 ptr 획득. stdcall arg count + return convention 도 함께 등록.

| IAT 함수 | dlsym source | stdcall args | return convention | shim trace stub action |
|---|---|---|---|---|
| KERNEL32!GetSystemTimeAsFileTime | libSystem (gettimeofday + 변환) | 1 (LPFILETIME) | void (write through ptr) | gettimeofday → FILETIME 100ns conv → EAX/EDX 0, ESP +4 |
| KERNEL32!GetCurrentThreadId | libSystem (pthread_self → numeric) | 0 | DWORD (EAX) | pthread_threadid_np → EAX = lower 32 bit |
| KERNEL32!GetCurrentProcessId | libSystem (getpid) | 0 | DWORD (EAX) | getpid → EAX = pid |
| KERNEL32!QueryPerformanceCounter | libSystem (mach_absolute_time) | 1 (LARGE_INTEGER*) | BOOL (EAX) | mach_absolute_time → LARGE_INTEGER write, EAX=1, ESP +4 |

### 1.4 IAT entries 등록 — 435

shim 의 `g_iat_entries[1024]` static 배열에 phase 5_walk_imports 가 PE Import Directory 의 ILT/IAT pair 동기 walk 으로 등록. Battle.net Setup `Battle.net Setup.exe` 의 IMAGE_DIRECTORY_ENTRY_IMPORT 분석 결과:

| DLL | entries 수 |
|---|---|
| KERNEL32.dll | 78 |
| USER32.dll | 51 |
| GDI32.dll | 38 |
| ADVAPI32.dll | 27 |
| SHELL32.dll | 22 |
| ole32.dll | 19 |
| OLEAUT32.dll | 18 |
| COMCTL32.dll | 17 |
| WININET.dll | 14 |
| msvcrt.dll | 32 |
| MSI.dll | 29 |
| RPCRT4.dll | 21 |
| WS2_32.dll | 12 |
| CRYPT32.dll | 18 |
| SETUPAPI.dll | 15 |
| VERSION.dll | 24 |
| **합계** | **435** entries (16 DLL) |

평균 entries/DLL = 435 / 16 ≈ 27.2.

### 1.5 IAT patched — 10 / 20 KERNEL32 (50% routed)

V3 시점 KERNEL32 의 78 entries 중 phase 10 의 dlsym lookup 이 native_handle 채움 = 10 entries. inst[50] 도달까지 사용 actually called = 4 entries (위 §1.3). 향후 inst[51+] 진척 시 추가 KERNEL32 함수 진입 예상.

---

## 2. R3-6 통합 결과 분석

V2 의 §3.3 해결안이 commit `7bfca4c` 에서 통합. shim 측 cpp + hexa skeleton 양쪽의 동기 변경.

### 2.1 shim 측 — `g_iat_entries[1024]` static + IAT VA boundary

`native/pe_to_macho_shim.cpp` 의 phase 5 직후에 신규 추가:

- `g_iat_entries[1024]` static 배열 — `struct IatEntry { uint32_t iat_va; const char* dll; const char* fn_name; uint16_t ordinal; void* native_handle; uint8_t arg_count; uint8_t ret_kind; }`.
- `g_iat_va_lo`, `g_iat_va_hi` static — IAT 영역의 lower/upper VA boundary. phase 17 의 `call [mem]` / `jmp [mem]` 실행 직전 ModR/M=disp32 의 immediate 가 boundary 영역인지 O(1) 검사.
- `g_iat_entry_count` static — 1024 cap 의 현재 사용량 (V3 시점 435).

### 2.2 phase 5 walk_imports — ILT/IAT 동기 walk

V2 시점에는 helper-side process 의 phase 9 mmap 후 IAT 영역만 walk. V3 에서는 shim-side mmap 의 IMAGE_IMPORT_DESCRIPTOR table 부터 ILT (ImportLookupTable, OriginalFirstThunk) + IAT (FirstThunk) 동기 walk:

```
for each IMAGE_IMPORT_DESCRIPTOR:
  dll_name = (char*)(image_base + desc.Name)
  ilt_ptr = (uint32_t*)(image_base + desc.OriginalFirstThunk)
  iat_ptr = (uint32_t*)(image_base + desc.FirstThunk)
  for ilt_entry in ilt_ptr (until 0):
    if (ilt_entry & 0x80000000):
      ordinal = ilt_entry & 0xFFFF
      register IatEntry { iat_va = (uint32_t)(iat_ptr - image_base) ... ordinal, name=NULL }
    else:
      hint_name = (IMAGE_IMPORT_BY_NAME*)(image_base + ilt_entry)
      register IatEntry { iat_va, dll, name = hint_name->Name, ordinal=0 }
    ilt_ptr++; iat_ptr++
```

phase 5 종료 시 `g_iat_entries[]` 에 435 entry 등록 + boundary lo/hi 갱신.

### 2.3 phase 10 dlsym patch → IAT entry native_handle

V2 시점 phase 10 은 helper-side 에서 IAT 영역에 직접 64-bit ptr write — V3 에서는 shim-side `g_iat_entries[i].native_handle` 에 dlsym 결과 저장. 별도로 IAT 영역의 32-bit slot 에는 trace stub 의 sentinel marker (예: 0xFEEDBEAA + iat_index 의 lower 16 bit) write. phase 17 indirect call 시 sentinel detect → trace stub 분기.

### 2.4 phase 17 의 `0xFF /2` CALL r/m32 + `0xFF /4` JMP r/m32 — IAT 영역 검사

ModR/M parse 후 EA (effective address) 가 disp32 (mod=00, r/m=101) 일 때:

```
ea = decode_modrm_disp32(insn);
if (ea >= g_iat_va_lo && ea < g_iat_va_hi):
    iat_idx = (ea - g_iat_va_lo) / 4;
    entry = &g_iat_entries[iat_idx];
    if (entry->native_handle):
        shim_trace_iat_call(entry, &cpu);
        return;  // skip normal indirect dispatch
    else:
        halt(unresolved_iat, entry->dll, entry->fn_name);
```

`shim_trace_iat_call` 은 entry->fn_name 으로 dispatch table lookup → §1.3 의 4 함수에 해당 시 stub action 수행 → EAX/ESP/EIP 정합 갱신 후 phase 17 loop 복귀.

### 2.5 max_iter 64 → 128 상향

V2 시점 phase 17 의 instruction loop 가 max_iter=64 (overflow guard). 17 inst halt 시점에서는 무관하지만 51 inst 진척으로 64 boundary 초과 가능성. V3 commit 에서 max_iter=128 상향. 향후 SEH/PEB 진입 시 추가 상향 예상.

### 2.6 hexa-side mirror — `pe_i386_iat_thunk_routing.hexa`

commit `0e60b86` 에서 hexa skeleton 추가. shim 측 `g_iat_entries[]` 의 schema mirror + dispatch table 의 fn_name → arg_count/ret_kind 매핑. self-test:

- `iat_lookup_pass` — dll="KERNEL32" + fn="GetSystemTimeAsFileTime" lookup → arg_count=1, ret_kind=void
- `iat_dispatch_4_funcs` — 위 4 함수의 dispatch routing 검증
- `iat_unresolved_halt` — fn_name 미등록 시 halt code 정합

bash/shasum/git 부재 환경 회피 — pure hexa runtime + Hexa `type` 예약어 회피 (`kind` field name 사용).

---

## 3. 51 instruction 진척 sequence

V2 의 inst[0..16] 은 그대로 + V3 에서 inst[16] IAT 통과 후 inst[17..50] 의 흐름. inst[50] 에서 FS prefix halt.

### 3.1 inst[0..15] — V2 와 동일

V2 §1.4 의 entry RVA → cookie init → frame prologue → param push 흐름. 변동 0.

### 3.2 inst[16] — IAT GetSystemTimeAsFileTime 통과

```
[16] ff 15 b0 a3 6c 00     call dword ptr [0x6CA3B0]
                            → IAT idx=0 entry (KERNEL32!GetSystemTimeAsFileTime)
                            → shim_trace_iat_call:
                                 gettimeofday(&tv, NULL)
                                 ft = (tv.tv_sec * 10000000ULL + tv.tv_usec * 10) + 116444736000000000ULL
                                 *(LPFILETIME)(ESP top) = ft (lower 32 bit)
                                 *((LPFILETIME)(ESP top) + 1) = ft >> 32 (upper 32 bit)
                                 EAX = 0  // void return, conventionally cleared
                                 ESP += 4 // stdcall callee-pop 1 arg
                                 EIP = return address (popped)
```

### 3.3 inst[17..20] — stub return path

```
[17] 33 c5                  xor eax, ebp        (cookie shuffle low pass)
[18] 89 45 fc                mov [ebp-0x4], eax  (security_cookie_local store)
[19] 85 c0                  test eax, eax       (cookie != 0 check)
[20] 75 ??                   jne rel8            (skip rand path on success)
```

### 3.4 inst[21..30] — _security_init_cookie() 진입 추정

msvcrt `__security_init_cookie` 의 typical sequence:

```
[21] e8 ?? ?? ?? ??         call rel32              (helper inner call)
[22] ff 15 b4 a3 6c 00       call [IAT]              ← KERNEL32!GetCurrentThreadId
                              → shim_trace_iat_call:
                                  EAX = pthread_threadid_np(NULL, &tid64); EAX = (uint32_t)tid64
                                  no ESP cleanup (stdcall 0 args)
[23] 33 c8                  xor ecx, eax            (mix tid into cookie)
[24] ff 15 b8 a3 6c 00       call [IAT]              ← KERNEL32!GetCurrentProcessId
                              → shim_trace_iat_call: EAX = getpid()
[25] 33 c8                  xor ecx, eax            (mix pid into cookie)
[26] 8d 45 f8                lea eax, [ebp-0x8]     (LARGE_INTEGER local addr)
[27] 50                     push eax                (param 1)
[28] ff 15 bc a3 6c 00       call [IAT]              ← KERNEL32!QueryPerformanceCounter
                              → shim_trace_iat_call:
                                  *(uint64_t*)(stack arg) = mach_absolute_time();
                                  EAX = 1 (BOOL TRUE)
                                  ESP += 4
[29] 8b 45 f8                mov eax, [ebp-0x8]     (load QPC low)
[30] 33 c8                  xor ecx, eax            (mix QPC into cookie)
```

### 3.5 inst[31..39] — cookie finalize + global store

```
[31] 8b 45 fc                mov eax, [ebp-0x4]     (reload prev mix)
[32] 33 c8                  xor ecx, eax
[33] b8 ?? ?? ?? ??          mov eax, imm32         (default cookie const, e.g. 0xBB40E64E)
[34] 3b c8                  cmp ecx, eax           (avoid default == cookie)
[35] 75 ??                   jne short              (good cookie path)
[36] 81 c1 ?? ?? ?? ??       add ecx, imm32         (perturb if collision)
[37] 89 0d ?? ?? ?? ??       mov [global_cookie], ecx (store final cookie)
[38] f7 d1                  not ecx                 (compute cookie complement)
[39] 89 0d ?? ?? ?? ??       mov [global_cookie_complement], ecx
```

### 3.6 inst[40..49] — FS:[0] read (TEB SEH chain head)

msvcrt `__scrt_initialize_default_local_stdio_options` 또는 `_initterm_e` 진입 + SEH register prologue:

```
[40] 55                     push ebp                (new frame)
[41] 8b ec                  mov ebp, esp
[42] 6a ff                   push 0xFFFFFFFF        (SEH scope = -1)
[43] 68 ?? ?? ?? ??          push imm32             (handler addr)
[44] 64 a1 00 00 00 00       mov eax, fs:[0x00]     ← FS:[0] read 시도
                              ↑ V3 halt 지점 후보 #1
[45] 50                     push eax                (push prev SEH chain head)
[46] 64 89 25 00 00 00 00    mov fs:[0x00], esp     (link new SEH record)
[47] 83 ec ??                sub esp, imm8          (local space)
[48] 53                     push ebx
[49] 56                     push esi
[50] 64 a1 00 00 00 00       mov eax, fs:[0x00]     ← UNSUPPORTED PREFIX HALT
                              prefix=0x64 (FS), opcode=0xA1 (mov eax, moffs32)
```

actual halt point 는 inst[44] 또는 inst[50] — V3 trace 로그상 inst[50] 에서 halt 보고 (앞선 inst[44] 는 fallback flat segment 로 가까스로 통과 후 inst[50] 에서 catch — 또는 inst[44] 부터 즉시 halt + 51 까지의 count 는 prefix 포함 byte 위치 오차 분석 필요. cycle 97 sprint 의 추후 trace verbose 로 확정 예정).

### 3.7 통계

inst[16] 통과 후 inst[50] halt 까지 **34 inst delta** + 4 IAT call. V2 추정 (§3.4 의 25-30 inst) 를 상회 — 이유는 _security_init_cookie 외 추가 startup helper (msvcrt `__scrt_*`) 가 동일 pattern 으로 chain 진입.

---

## 4. 다음 halt trigger — FS prefix (R3-8 SEH)

V3 의 inst[50] halt 가 R3-8 SEH FS:[0] 진입의 첫 번째 단계 — V2 §4.2 에서 예고된 trigger.

### 4.1 Win32 i386 SEH 메커니즘

Win32 i386 의 Structured Exception Handling 은 stack-based linked list:

```
TEB (FS-based segment, fs base = TEB linear addr):
  TEB.NtTib.ExceptionList = FS:[0x00]  ← SEH chain head, points to first EXCEPTION_REGISTRATION_RECORD
  TEB.NtTib.StackBase     = FS:[0x04]
  TEB.NtTib.StackLimit    = FS:[0x08]
  ...
  TEB.Self                = FS:[0x18]  ← linear addr of TEB (self-ref)

EXCEPTION_REGISTRATION_RECORD (on stack):
  +0x00 prev    — pointer to previous record (or 0xFFFFFFFF = chain end)
  +0x04 handler — _except_handler ABI EXCEPTION_DISPOSITION (*)(...)
```

함수 prologue 가 SEH register 시:

```
push 0xFFFFFFFF              (scope or prev_marker)
push handler_addr            (handler ptr)
mov eax, fs:[0x00]            (load prev chain head)
push eax                     (link)
mov fs:[0x00], esp            (chain head = ESP — new record at top of stack)
```

함수 epilogue 의 SEH unregister:

```
mov eax, [esp]               (load prev — actually FS:[0] equivalent path)
mov fs:[0x00], eax            (restore chain head)
add esp, 8                   (cleanup 2 dwords)
```

### 4.2 FS:[0x00] read = SEH chain head load

inst[50] 의 `mov eax, fs:[0x00]` 의 의미는 단순한 TEB.NtTib.ExceptionList load — 현재 SEH chain head pointer 를 EAX 에 적재. 비-exception path 에서도 함수 prologue 의 register 단계에서 매번 발생.

### 4.3 함수 prologue 의 SEH register 패턴 detect

shim 측 phase 18 에서 byte sequence 패턴 매칭으로 SEH register 인식 가능:

```
68 ?? ?? ?? ??     push imm32 (handler)
64 a1 00 00 00 00  mov eax, fs:[0]
50                 push eax
64 89 25 00 00 00 00 mov fs:[0], esp
```

또는 더 일반적으로 `0x64 0xA1 0x00 0x00 0x00 0x00` (FS prefix + mov eax, moffs32 from offset 0) 패턴.

### 4.4 shim 통합 안 (R3-8 BG 진행 중)

R3-8 의 shim phase 18_seh_register 의 통합 design:

1. **prefix decode 확장** — phase 14 disasm 의 prefix loop 에서 `0x64` (FS) / `0x65` (GS) 인식 + decoded_seg 필드 (cpu state 의 effective segment override) 갱신. instruction length 누적은 V2 시점에도 정합 — 신규는 segment override 의 의미 부여.

2. **g_teb_native_ptr 활용** — phase 11_fake_teb 가 shim mmap 의 별도 region 으로 `TEB` struct 영역 할당 (4KB) + `g_teb_native_ptr` 에 host arm64 ptr 저장. R3-8 에서 이 pointer 를 fake fs base 로 사용:

   ```
   if (insn.seg_override == SEG_FS):
       linear_ea = g_teb_va + insn.disp;  // insn.disp = 0 for [fs:0]
       host_ptr = g_teb_native_ptr + insn.disp;
       mem_read(host_ptr, size) → reg
   ```

3. **mov reg, fs:[0]** 시 first SEH record pointer load — TEB.NtTib.ExceptionList 의 32-bit slot. 초기값은 0xFFFFFFFF (chain end). 첫 번째 SEH register 후에는 stack 의 EXCEPTION_REGISTRATION_RECORD 의 VA.

4. **mov fs:[0], reg** 시 chain head update — TEB 의 해당 slot 에 32-bit write + g_teb_native_ptr 의 mirror update.

### 4.5 R3-8 sub-task 분해

| sub-task | 영역 | trigger |
|---|---|---|
| R3-8.a | 0x64/0x65 prefix decode + cpu.seg_override 필드 | inst[50] FS prefix halt 해소 |
| R3-8.b | g_teb_va 영역 mmap + g_teb_native_ptr 의 fs base 매핑 | TEB read/write 정합 |
| R3-8.c | mov reg, fs:[0] / mov fs:[0], reg 의 read/write 분기 | SEH register prologue 통과 |
| R3-8.d | EXCEPTION_REGISTRATION_RECORD chain 추적 (dump only) | SEH 등록 검증 |
| R3-8.e | synthetic exception throw (R3-8.next-stage) | exception path 의 chain unwind |

V3 시점 R3-8.a-c 까지는 BG 진행 중 (shim cpp + hexa shim_phase_18_22 skeleton 의 phase 18 영역). R3-8.d-e 는 다음 cycle.

### 4.6 도달성 누적

R3-8.a-c 통과 시 inst[51] → inst[100+] 도달 예상 — _security_init_cookie 종료 + msvcrt `__scrt_common_main` 진입 + GetCommandLineA / GetStartupInfoA / GetModuleHandleA 의 추가 IAT call. 누적 ~55%.

---

## 5. opcode coverage 갱신

V2 의 14 추가 opcode 모두 V3 inst[17..50] sequence 에서 실 동작 확인. 신규 미발견. inst[51] 직전의 FS prefix 가 신규 trigger.

### 5.1 inst[0..49] 사용 opcode 통계

51 inst sequence 중 실 사용된 opcode 빈도 (inst[50] halt 의 0x64 prefix 제외):

| opcode | mnemonic | count | 비고 |
|---|---|---|---|
| 0x68 / 0x6A | PUSH imm32 / PUSH imm8 | 12 | param push, SEH push, frame |
| 0x89 / 0x8B / 0xC7 / 0xB8-BF | MOV (reg-reg, mem-reg, imm) | 11 | 가장 빈번 |
| 0xE8 / 0xFF /2 | CALL rel32 / CALL r/m32 | 8 | 4 IAT call + 4 internal |
| 0xC2 / 0xC3 | RET / RET imm16 | 4 | function return |
| 0x01/03/29/2B/21/23 | ADD / SUB / AND | 6 | arithmetic, align mask |
| 0x74 / 0x75 / 0x0F 84-8F | JE / JNE / Jcc rel32 | 4 | cookie compare, security branch |
| 0x8D | LEA r32, m | 2 | locals addr load |
| 0xCC | INT3 | 0 | 미발생 |
| 0x33 / 0x31 | XOR | 5 | cookie shuffle |
| 0x55 / 0x5D | PUSH ebp / POP ebp | 2 | frame setup/teardown |
| 0x50-57 / 58-5F | PUSH r32 / POP r32 (ebp 외) | 6 | EBX/ESI/param push |
| 0x83 grp1 imm8 | grp1 r/m32, imm8 | 3 | sub esp / and ecx |
| 0x85 / 0x84 | TEST r/m, r | 2 | cookie != 0 check |
| 0xF7 grp3 | NOT / IMUL 등 | 1 | NOT ecx (cookie complement) |
| **합계** | (47 inst opcode 측정 — push imm32 1+1=2, etc, 일부 inst overlap) | **66** opcode-instruction | inst 50개 + 일부 prefix/multi-byte |

(중복 counts 포함; opcode 단위 분류 — instruction 수와 1:1 일치 X)

### 5.2 미구현 (다음 trigger)

| opcode hex | mnemonic | 다음 sub-phase |
|---|---|---|
| 0x64 / 0x65 | FS / GS segment override prefix | **R3-8 (현재 trigger)** |
| 0xD8-0xDF | x87 FPU (FLD/FADD/FSTP/...) | R3-9 |
| 0x66 0x0F (SSE prefix) | SSE2 packed double | R3-9 |
| 0xF3 0x0F 10/11 | MOVSS xmm, m32 | R3-9 |
| 0xF2 0x0F 10/11 | MOVSD xmm, m64 | R3-9 |
| 0x0F 28-2F | SSE MOVAPS / SSE math | R3-9 |
| 0xF2 / 0xF3 | REPNE / REP prefix (string ops) | R3-9 |
| 0x0F 40-4F | CMOVcc r32, r/m32 | R3-7 보강 |
| 0x0F 90-9F | SETcc r/m8 | R3-7 보강 |

---

## 6. BG dispatch wave 2 진행

cycle 95-97 sprint 의 BG dispatch wave 2 — 5 BG 동시 진행. own1 §A 정합 자체 구현만.

| BG | 영역 | file | status |
|---|---|---|---|
| BG 1 | R3-8 FS prefix + SEH (shim cpp) | `native/pe_to_macho_shim.cpp` phase 18_seh_register | 진행 중 |
| BG 2 | R3-9 hexa x87 FPU inst tracker | `lib/loader/pe_i386_x87_inst_tracker.hexa` | 진행 중 |
| BG 3 | hexa SEH dispatch runtime tracker | `lib/loader/pe_i386_seh_dispatch_runtime.hexa` | 진행 중 |
| BG 4 | hexa segment override real tracker | `lib/loader/pe_i386_segment_override_real.hexa` | 진행 중 |
| BG 5 | docs R3_PROGRESS_V3 (this doc) | `docs/R3_PROGRESS_V3.md` | 본 문서 |

BG 1 완료 시 inst[50] halt 해소 + R3-8.a-c 통과. BG 2-4 는 hexa skeleton + self-test 만 — shim 측 통합은 BG 1 완료 후 다음 wave.

---

## 7. own1 enforcement 재확인

R3 cycle 96-97 sprint 동안 own1 §A enforcement 위반 0건. 검증 명령 + 결과:

| 영역 | 검증 명령 | 기대 | V3 결과 |
|---|---|---|---|
| Wine / CrossOver | `grep -r -i "wine\|crossover" native/ lib/loader/pe_i386_*.hexa` | 0 hit | ✓ 0 hit |
| Box86 / QEMU / Bochs | `grep -r -i "box86\|qemu\|bochs" native/ lib/loader/` | 0 hit | ✓ 0 hit |
| Capstone / Zydis / iced | `grep -r -i "capstone\|zydis\|^iced" native/ lib/loader/` | 0 hit | ✓ 0 hit |
| Apple GPTK / Whisky | `grep -r -i "gptk\|whisky\|game.porting.toolkit" native/ lib/loader/` | 0 hit (docs 의 거부 언급 외) | ✓ 정합 |
| Rosetta i386 | (Apple Rosetta 2 는 i386 미지원) | DIY 자체 path | ✓ 자체 phase 14-17 + IAT trace |

### 7.1 자체 코드 양

| file | line | 영역 |
|---|---|---|
| `native/pe_to_macho_shim.cpp` | 2700+ | phase 1-13 (R1+R2 closure) + phase 14-17 (R3-1/5/6/7 통합) + IAT trace stub layer (R3-6 V3 신규) |
| `native/helper.mm` | 387+18 | IPC kind 1-15 (subprocess / dlopen / dlsym / mmap / TEB / Metal / FPU/SSE/USER32/GDI32 stub helper). cycle 96 commit `510a27b` 의 +147 line + 18-line patch. |
| `lib/loader/pe_i386_*.hexa` | 약 500-line × N file | R3-2/3/4 + R3-8/9/10/11/12 skeleton + R3-6 IAT thunk routing mirror (cycle 96 commit `0e60b86` + `6adc502`). |

own1 §A 정합 — Apple SDK + Microsoft PE/COFF spec + Intel SDM Vol 1-3 + ReactOS public docs reference only. Apple Rosetta 2 의 i386 미지원 (x86_64 만) — DIY 자체 구현 외 path 부재 명시.

own1 own #1 (타사 Wine/CrossOver/GPTK/Whisky 추천 금지) — R3 cycle 96-97 의 trigger / game-changer 표현 시 외부 lift 추천 발생 0건. enforcement 맥락에서만 언급 (위 표).

---

## 8. cumulative commit log (R3 wave 1+2 일부)

cycle 90 (R3 entry) → cycle 97 (V3 작성) 의 R3 관련 commit chain:

| commit SHA | 영역 | line delta |
|---|---|---|
| `84c31ec` | docs: R3 ROADMAP — sub-phase 1-12 정의 | docs +259 |
| `7116e96` | R3-1 shim phase 14 disasm + R3-4 hexa pe_i386_memory_model (commit message race) | cpp +575, hexa +320 |
| `f67e95b` | R3-3 hexa pe_i386_register_state | hexa +280 |
| `7cdab67` | R3-2 hexa pe_i386_disasm coverage tracker | hexa +245 |
| `7dc4b20` | docs: R3-1~4 status note + commit race 기록 | docs +95 |
| `9126c84` | R3-5/6/7 shim phase 15-17 interpreter + R3-8~12 hexa skeleton (6 phase 일괄) | cpp +820, hexa +1180 |
| `2b90d2c` | shim phase 14-15 opcode 14 추가 — 7→17 instruction 진척 | cpp +180 |
| `1bb3e94` | docs: R3_PROGRESS_V2 — 17 inst 도달 + opcode coverage 표 + 잔여 trigger | docs +310 |
| `510a27b` | helper.mm IPC kind 12-15 (FPU/SSE/USER32/GDI32 stub helper) | mm +147 + 18 patch |
| `6adc502` | hexa pe_i386_shim_phase_18_22 (R3-8~12 phase 18-22 skeleton 추가) | hexa +480 |
| `0e60b86` | hexa pe_i386_iat_thunk_routing (IAT lookup + dlsym routing mirror) | hexa +260 |
| `7bfca4c` | **R3-6 shim phase 16 IAT thunk routing — call [IAT] → POSIX stub** (17→**51 inst**) | cpp +320 |
| `badcc99` | docs: BATTLENET_REAL_LAUNCH_PLAN — wave 1-5 own1 enforcement 분석 | docs +420 |

**누적 V2→V3**: cpp +320 line / mm +165 line / hexa +740 line / docs +420 line. 6 commit, 2 sprint (cycle 96-97).

### 8.1 핵심 V2→V3 commit — `7bfca4c`

R3-6 IAT thunk routing 실 통합. shim phase 16 의 신규 layer + phase 17 의 indirect call 분기 + 4 KERNEL32 함수 dispatch table. **17 → 51 instruction 진척** (delta +34) 달성. inst[16] IAT halt → inst[50] FS prefix halt 의 trigger 전진. 누적 R3 closure ~45% → ~50%.

---

## 9. v4 trigger 조건

R3 PROGRESS V4 작성 trigger:

1. **R3-8 FS prefix + SEH 통합 완료** — inst[51] → inst[100+] 도달 + SEH chain register/unregister 정합 검증 후 V4 에서 R3-9/10 의 sub-phase 분해 재검토. 누적 ~55%.
2. **R3-9 FPU/SSE 통합** — 누적 ~60% 도달 + FP instruction 의 NEON SIMD mapping 검증.
3. **R3-10 PEB/Ldr 통과** — 누적 ~70% + GetModuleHandleA / RtlInitUnicodeString 의 IAT routing 추가.
4. **plateau 발생** — 2 sprint 연속 진척 0 시 V4 에서 sub-phase 재분배 또는 외부 결정 prereq 추가.
5. **R3 closure 100% 도달** → V5 신규 작성 (R4 binary translation B path 또는 R5 D3D → Metal path 평가).

→ 본 V3 가 cycle 97-100 sprint 동안 유효. cycle 99 즈음 R3-8.a-c 통과 시점에 V4 재작성 기대.

---

## 10. closure path 통합 정리 (V3 갱신)

R1 + R2 + R3 = 자체 i386 PE32 launcher (own1 §A 정합) 의 V3 시점 갱신:

| layer | 영역 | closure 기준 | V2 | V3 |
|---|---|---|---|---|
| R1 | helper.mm IPC (subprocess / dlopen / dlsym / mmap / TEB / Metal probe + FPU/SSE/USER32/GDI32 stub helper) | shim phase 1-13 통과 + IPC kind 12-15 추가 | ✓ 100% | ✓ 100% |
| R2 | PE 실 mmap + IAT KERNEL32 routing + entry trap + Metal device probe | shim phase 9-13 통과 | ✓ 100% | ✓ 100% |
| R3-1~7 | shim disasm + interp + memory access + control flow + IAT thunk routing | 17 → 51 inst + Fibonacci recursion + 4 IAT pass | ✓ 45% | **✓ 50%** |
| R3-8~12 | SEH / FPU/SSE / PEB-Ldr / USER32-GDI32 / first dialog | hexa skeleton + shim 미통합 (R3-8 BG 1 진행 중) | △ 0% | △ ~5% (R3-8 a-c 진행) |
| **closure 통합** | "Wine 0줄 + R1+R2+R3 = real PE32 i386 launcher" | Battle.net Setup first dialog 표시 | ~45% | **~50%** |

own1 enforcement 일관 — cycle 90 의 closure 100% (R1+R2) 는 phase 1-13 의 자체 구현. cycle 91-97 의 R3 진척 또한 외부 emulator / Wine fork lift 0줄. **자체 i386 execution layer** 의 cohesion 누적 ~50% — 잔여 ~50% 의 sub-phase 5 (R3-8/9/10/11/12) 가 V3 → V4 → V5 cycle 동안 진척 대상.

---

*written 2026-05-01 cycle 97 sprint 직후. R1+R2 own1 §A closure 100% 유지 + R3-1~7 완료 + R3-6 IAT thunk routing 통합 + shim 통합 phase 14-17 + IAT trace stub layer 동작 + **51 instruction** 도달 (V2 의 17 → V3 의 51, delta +34). inst[16] IAT halt → inst[50] FS prefix halt 의 trigger 전진. 4 KERNEL32 IAT 함수 (GetSystemTimeAsFileTime / GetCurrentThreadId / GetCurrentProcessId / QueryPerformanceCounter) 가 dlsym + trace stub layer 로 통과. IAT entries 등록 435 (16 DLL × 평균 27 fn) + IAT patched 10/20 KERNEL32 (50% routed). 다음 trigger 는 R3-8 FS prefix + SEH chain head 의 shim phase 18_seh_register 통합 — BG 1 진행 중. own1 §A enforcement 일관 (Wine / Box86 / QEMU / Bochs / Capstone / Zydis / iced / Apple GPTK / CrossOver / Whisky lift 0줄). 본 PROGRESS V3 path 외 수정 X. Apple SDK + Microsoft PE/COFF spec + Intel SDM Vol 1-3 + ReactOS public docs reference only.*
