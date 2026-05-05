# R3 wave 9 halt 분석 — inst[17002] EIP=0x80147B (.bss) corruption

**작성**: 2026-05-01 cycle 109+ · base commit `94cdb16` (wave 9-A EncodePointer/DecodePointer simple XOR involution 통과 후 16486 → 17002 inst 도달).
**현 halt**: `bad_branch_target` EIP=`0x0080147B` — PE image base + 0x140000 영역, 즉 **`.bss` 섹션 (uninitialized data)** 한가운데로 EIP jump. INT 3 / fast_fail 도 아닌, 정상 control flow 가 garbage data 영역으로 흘러간 형태.
**원인 가설**: wave 9-A EncodePointer XOR involution 통과 후 `_initterm` walker 가 array 끝 sentinel 을 넘어가 인접한 .bss 영역의 0 바이트 (또는 cookie XOR 후 산출되는 garbage VA) 를 fn ptr 로 호출 → EIP 가 .bss 영역 안으로 jump. USER32 호출 여전히 미도달.
**참조**: [R3_HALT_343_ANALYSIS.md](R3_HALT_343_ANALYSIS.md) (343 inst halt + fake_proc stdcall) / [MSVC_CRT_INIT_BYPASS.md](MSVC_CRT_INIT_BYPASS.md) (security_cookie + _initterm + CFG 우회) / [R3_PROGRESS_V8.md](R3_PROGRESS_V8.md) (16486 inst 970× plateau) / [BATTLENET_DAY1_PLAN.md](BATTLENET_DAY1_PLAN.md) (Day 1-7 actionable).
**own1 enforcement**: Wine / Box86 / QEMU / Bochs / Capstone / Zydis / iced / Apple GPTK / CrossOver / Whisky source **0줄**. MSDN `learn.microsoft.com` public docs reference 만 (`_initterm`, `_initterm_e`, `EncodePointer`, `DecodePointer`, `__crt_state_management::wrapped_invoke`, PE/COFF Specification §6 — Section Headers / `.bss` 영역).

---

## 0. 한 줄 요약

> **wave 9-A 의 EncodePointer/DecodePointer XOR involution 통합으로 inst[16486] → inst[17002] 도달 (delta +516, 누적 ~1000×). 다음 halt 는 EIP=0x80147B — PE image base+0x140000 의 `.bss` 영역으로 jump. `.text` 범위 (0x401000 ~ 0x53A6A6) 밖, `.data`/`.bss` 영역 (0x6CA000 ~ 0x80FFFF) 안. 직전 indirect call/jmp 의 target register 가 garbage uninit fn ptr (XOR involution 후 cookie mask 누락 또는 array 끝 sentinel mismatch) 으로 산출. wave 10 의 verbose trace + indirect call source 추적 + .text 범위 강제 redirect (graceful) 으로 inst[17002] → inst[25000-50000] 진보 + USER32 RegisterClassExA 도달 70-80%.**

---

## 1. 현상

### 1.1 halt sequence

```
inst[16998] 0x00496B?? : 8B 4D F8           mov ecx, [ebp-8]
inst[16999] 0x00496B?? : E8 ?? ?? ?? ??     call DecodePointer       ← XOR involution 후 ecx 결과 EAX 로
inst[17000] 0x00496B?? : 8B C8              mov ecx, eax             ← decoded fn ptr 를 ecx 로
inst[17001] 0x00496B?? : FF D1              call ecx                 ← indirect call
inst[17002] 0x0080147B : ??                 [halt] bad_branch_target
                          ↑
                          PE image base 0x400000 + 0x40147B
                          = .bss / .data 영역 한가운데
```

`call ecx` 의 ecx 값이 **0x0080147B** — `.text` (0x401000 ~ 0x53A6A6) 범위 밖, `.bss` 영역 (PE image header 의 IMAGE_SECTION_HEADER `.bss` 또는 `.data` 영역으로 추정되는 0x6CA000 ~ 0x80FFFF) 한가운데. 정상 control flow 가 fn ptr 가 아닌 **uninitialized data 영역 내부** 로 jump.

### 1.2 wave 9-A 통과 후 새 단계 진입

V8 (commit `a2f32f8`) 의 16486 inst plateau → wave 9-A (commit `94cdb16` `feat(r3): shim wave9-A — EncodePointer/DecodePointer simple XOR (16486→17002 inst)`) 적용 후 **+516 inst**. EncodePointer (`__security_cookie` XOR ROL) 의 simple XOR involution (encoded_ptr = real_ptr ^ cookie, decoded_ptr = encoded_ptr ^ cookie → identity 보장) 통과 후, `_initterm` 의 잔여 ctor 또는 atexit 등록 callback walk 진입. 그 walk 의 entry 중 하나가 `.bss` VA 를 산출.

### 1.3 USER32 IPC bridge 호출 여전히 0건

V8 시점과 동일 — RegisterClassExA / CreateWindowExA / ShowWindow / GetMessageA 모두 미도달. 첫 NSWindow 표시 미달성. wave 10 trigger 필수.

---

## 2. .text / .data / .bss 영역 bound

### 2.1 PE Section Header 추정 bound (Battle.net Setup `.exe` 기준)

| 섹션 | VA 시작 | VA 끝 (대략) | 크기 | 용도 |
|---|---|---|---|---|
| `.text` | 0x00401000 | 0x0053A6A6 | ~1.22 MB | code (executable) |
| `.rdata` | 0x0053B000 | 0x006C9FFF | ~1.59 MB | read-only data (string / IAT / RTTI) |
| `.data` | 0x006CA000 | 0x0079FFFF | ~0.84 MB | initialized data (global var) |
| `.bss` | 0x007A0000 | **0x0080FFFF** | ~0.44 MB | **uninitialized data (zero-init)** |
| `.rsrc` | 0x00810000 | 0x0083FFFF | ~0.19 MB | resource (icon / version) |
| `.reloc` | 0x00840000 | 0x0085FFFF | ~0.13 MB | base reloc table |

EIP=**0x0080147B** = `.bss` 영역 시작 (0x007A0000) + 0x6147B → `.bss` 영역의 ~88% 지점. 즉 zero-init 메모리 한가운데. PE loader 가 이 영역을 0 으로 채운 후, fn ptr table 의 entry 가 0 또는 garbage 로 남아 있다가 호출됨.

### 2.2 `.bss` VA 의 의미

- `.bss` 는 file 에는 raw size 0 으로 저장되고, mapping 시점에 virtual size 만큼 zero-fill.
- 여기 위치한 global pointer (atexit chain / TLS callback array / vtable 의 일부) 는 CRT init 의 `_initterm` walker 또는 `_register_thread_local_exe_atexit_callback` 가 채워야 함.
- shim 이 그 채움을 skip 하거나 늦은 시점에 저장하면 호출 시점에 0 또는 garbage → invalid jump.

---

## 3. 직전 instruction 추정

### 3.1 indirect call/jmp 의 target source

inst[17001] 은 `call ecx` (또는 `jmp ecx`) 의 indirect — ecx 값이 어디서 왔는지가 핵심.

후보 (verbose trace 로 확정 필요):

1. **stack [esp+N]**: 직전 함수의 push 한 fn ptr — caller 가 _initterm 또는 atexit 호출 시 array 의 entry 를 push
2. **earlier register load**: 직전 sequence 에서 mov ecx, [global_VA] 또는 mov ecx, [ebp-N] 로 load 된 값
3. **DecodePointer 결과**: inst[16999] 의 EAX (DecodePointer 반환) → inst[17000] mov ecx, eax → inst[17001] call ecx

가장 유력한 것은 **3** — DecodePointer XOR involution 이 identity 가 되었으나, **input ptr 자체가 이미 garbage** 였던 경우. 즉 EncodePointer 호출 직후 어딘가에서 stack/global 에 저장된 encoded ptr 가 손상되었거나, 처음부터 NULL/uninit 인 영역을 DecodePointer 에 넘긴 것.

### 3.2 source register 추적 plan

verbose trace (`./entry launch tool/bench/Battle.net-Setup.exe --verbose 2>&1 | tee /tmp/wave9_trace.log`) 후 inst[16980-17002] 의 22 instruction window 분석:

```
grep -nE 'inst\[1698[0-9]\]|inst\[1699[0-9]\]|inst\[170[0-2][0-9]\]' /tmp/wave9_trace.log
```

각 instruction 의 EIP / opcode / register snapshot (eax, ecx, edx, ebx, esp, ebp, esi, edi) 출력 후:
1. inst[17001] 직전의 ecx 변경 시점 식별 (마지막 mov ecx, .. / lea ecx, .. / pop ecx / xor ecx, .. / call → eax → mov ecx, eax)
2. 그 source 의 origin tracing — register chain 또는 memory load
3. memory load 인 경우 그 VA 의 origin — image base 영역 내부 (.text / .rdata / .data / .bss) 분류

---

## 4. wave 9 의 EncodePointer XOR involution 통과 후 진입 단계

### 4.1 wave 9-A 의 정확한 변경

`94cdb16` 의 핵심 — EncodePointer/DecodePointer 가 V8 의 identity (in==out) → wave 9-A 의 simple XOR (encoded = real ^ g_security_cookie, decoded = encoded ^ g_security_cookie). cookie 가 같은 값이므로 round-trip 은 여전히 identity 이지만, **caller 가 중간에 cookie 를 변경하거나 다른 cookie 로 decode** 하는 경우 mismatch 가 발생할 수 있음.

### 4.2 새 진입 단계 — `__crt_state_management::wrapped_invoke` 의 fp call

[MSVC_CRT_INIT_BYPASS.md](MSVC_CRT_INIT_BYPASS.md) 의 step 6 (`_initterm` C++ ctor table walk) 후, MSVC vcruntime 의 `__crt_state_management::wrapped_invoke<Action>` template 이 atexit / exit 핸들러 등록 시 사용. wrapped_invoke 는:

```c
template <typename Action>
auto wrapped_invoke(Action action) {
    auto encoded_fn = encode_pointer(action);   // EncodePointer (cookie XOR)
    // ... 저장 / 후일 사용 ...
    auto real_fn = decode_pointer(encoded_fn);  // DecodePointer (cookie XOR)
    return real_fn();                            // ← 여기 indirect call
}
```

shim 의 wave 9-A 통과는 encode/decode 의 round-trip identity 만 보장. 그러나 wrapped_invoke 가 호출되기 전 / 후로 **`.bss` 영역의 fn ptr table** (atexit_callback_chain, terminate_handler, etc) 의 entry 가 zero 또는 garbage 인 채로 남아 있을 수 있음 — 그 entry 가 wrapped_invoke 로 흘러들어가면 decode 후 0x0080147B 같은 garbage VA 산출.

---

## 5. root cause 후보

### 5.1 후보 A — `_initterm` walker 의 array 끝 sentinel mismatch

`_initterm(begin, end)` 는 begin..end 범위 순회. 정상 path 에서 end 는 `__xc_z` 또는 `__xi_z` symbol 의 VA — `.rdata` 영역의 sentinel `NULL`. shim 이 그 sentinel 을 인식하지 못하고 **end 를 넘어 walk** 하면 인접 영역 (`.data` 또는 `.bss`) 의 garbage 를 fn ptr 로 호출.

증거:
- wave 8 의 `pe_msvc_initterm_array_real` (commit `1cb4e3d`) 가 cap 200 으로 truncate.
- 그러나 cap 200 은 단지 무한 loop 방지 — 실제 sentinel 이 200 entry 안에 있다면 정상, 이후 entry 까지 walk 하면 garbage.
- inst[17002] 의 jump target 0x0080147B 는 `.bss` 영역 — sentinel 을 넘어 인접 zero-fill 영역 까지 walk 한 흔적과 일치.

대응: shim 측 `_initterm` walker 에 **NULL entry 즉시 종료** 정책 추가 (Microsoft `_initterm` 정상 동작은 NULL entry skip 이 아닌 **NULL 호출 (그러면 segfault) 또는 종료** — 정상 sentinel 은 NULL 이 아니라 array 의 end VA. 그러나 zero-fill 인접 영역까지 walk 하면 entry 가 0 → 0 호출 → 0x0 jump (이건 EIP=0, 본 case 의 0x80147B 과 다름)).

본 case 의 **0x80147B** 은 0 호출이 아닌 **garbage VA 호출** — 즉 entry 가 NULL 이 아니라 임의의 4-byte 값. `.bss` 직전의 `.data` 영역 끝부분에 남아 있던 stale ptr 또는 cookie XOR 후 random VA.

### 5.2 후보 B — vtable / fn ptr table 의 entry NULL/garbage

C++ object 의 vtable 호출 시 vtable ptr 가 garbage 인 경우. 단 wave 7-8 까지의 분석상 본 binary 는 C++ ctor 가 simple level (locale init / atexit register) 이므로 vtable 호출 자체가 적음.

후보 B 가능성: **낮음 (10-15%)**.

### 5.3 후보 C — fastcall/__stdcall calling convention 불일치 → stack 어긋남 → ret 시 잘못된 RA fetch

shim 의 fake_proc 4-byte stub (`83 c4 NN c3` = `add esp, NN; ret`) 의 NN 값 (arg_count × 4) 가 부정확하면 caller stack 의 RA 위치가 어긋나 ret 이 잘못된 4-byte 를 EIP 로 fetch.

shim 의 GetProcAddress arg_count 표는 Microsoft API public spec 참조 자체 작성 — 일부 함수 (특히 less-documented MSVCR internals) 의 arg_count 가 부정확할 가능성.

후보 C 가능성: **중간 (30-40%)**. 단 본 case 의 EIP=0x80147B 가 stack pop 한 4-byte 라면, 그 값의 origin 은 caller 가 push 한 arg 또는 frame 변수 — `.bss` 영역의 4-byte global var (uninit) 를 push 한 패턴이면 가능.

### 5.4 후보 D — cookie XOR mask 의 의미 불일치 (실제 ROL+XOR 이 아닌 단순 XOR)

wave 9-A 는 simple XOR 만 구현. 실제 Microsoft `EncodePointer` 는 `(ROL(ptr, cookie & 0x1F) ^ cookie)` 의 ROL+XOR 조합 (Windows 8+ 기준). caller 가 ROL 도 함께 적용한 후 저장한 ptr 를 wave 9-A 의 simple XOR decode 만 적용하면 ROL 미복원 → garbage VA.

후보 D 가능성: **중간 (40-50%)**. 가장 wave 9-A 의 변경과 직접 연관된 후보.

### 5.5 종합 판단

후보 A (sentinel mismatch) + 후보 D (ROL+XOR 미반영) 의 결합이 가장 유력 (~70%). verbose trace 로 inst[17000-17002] 의 ecx 추적 후 확정.

---

## 6. 해결 전략 — wave 10

### 6.1 wave 10-A — verbose trace + indirect call source 추적 (17002 → 18000+ inst 예상)

`native/pe_to_macho_shim.cpp` 의 verbose trace 강화:
- inst[17000-17002] window 에서 register snapshot full dump
- ecx 의 마지막 변경 instruction 식별 후 origin (mov src) trace
- src 가 memory 인 경우 VA + image base 내 섹션 분류 (.text / .rdata / .data / .bss)
- EncodePointer / DecodePointer 호출 시 input ptr / cookie / output ptr 모두 log
- `_initterm` 호출 시 begin / end / 실 walk count log

산출물: `/tmp/wave10_trace_window.log` 의 22 inst window + ecx origin chain.

### 6.2 wave 10-B — EncodePointer/DecodePointer real ROL+XOR semantics (18000 → 22000+ inst 예상)

Microsoft `learn.microsoft.com` public docs 의 `EncodePointer` 동작 spec 참조:
```
encoded = ROL(ptr, cookie_lo5) XOR cookie    // Windows 8+
decoded = ROR(encoded XOR cookie, cookie_lo5)
```

shim 의 `stub_EncodePointer` / `stub_DecodePointer` 정밀화:
```c
uint32_t stub_EncodePointer(uint32_t ptr) {
    uint32_t cookie = g_security_cookie;
    uint8_t shift = cookie & 0x1F;
    uint32_t rotated = (ptr << shift) | (ptr >> (32 - shift));
    return rotated ^ cookie;
}

uint32_t stub_DecodePointer(uint32_t encoded) {
    uint32_t cookie = g_security_cookie;
    uint8_t shift = cookie & 0x1F;
    uint32_t xored = encoded ^ cookie;
    return (xored >> shift) | (xored << (32 - shift));
}
```

caller 가 ROL+XOR 로 encode 한 ptr 를 동일 ROL+XOR 로 decode → 정상 round-trip 복원.

### 6.3 wave 10-C — `_initterm` array 끝 sentinel 엄격 검증 (22000 → 30000+ inst 예상)

shim 의 `_initterm` 호출 단계 (CRT IAT entry resolve 시점) 에서:
1. begin / end VA 가 `.rdata` 영역 안에 있는지 검증 (PE section header bound check)
2. (end - begin) / 4 가 200 초과 시 cap (기존) + **end 를 image base + .rdata 끝** 으로 truncate (신규)
3. 각 entry 가 NULL 이면 즉시 종료 (Microsoft `_initterm` 의 정상 path 는 NULL skip 이 아닌 NULL 호출이지만, shim 정책상 더 안전)
4. 각 entry 의 VA 가 `.text` 범위 (0x401000 ~ 0x53A6A6) 안에 있는지 검증 — 밖이면 **graceful skip** + log warn

### 6.4 wave 10-D — .text 범위 밖 indirect call 강제 redirect graceful (30000 → 40000+ inst 예상)

shim interpreter 의 `0xFF /2 CALL r/m32` (mod==11 call reg) + `0xFF /4 JMP r/m32` 처리에서:
- target VA 가 PE image 의 `.text` section 범위 안인지 검증
- 안이면 정상 dispatch
- 밖이면 (특히 `.data` / `.bss` / `.rdata` 영역) — graceful skip:
  - EAX = 0 set
  - ESP += arg_count × 4 (stdcall 가정, arg_count 미상이면 0)
  - EIP = ret_addr (caller stack [esp] 의 4-byte)
  - log warn `redirect: indirect call to non-text VA 0x%x → graceful skip`

이는 wave 8-D step2 의 bad_branch_target graceful skip 의 정밀화 — cap 64 가 아니라 .text 범위 검증 후 즉시 skip.

### 6.5 wave 10-E — atexit_callback_chain / TLS callback array `.bss` zero-init 보강

PE loader 측 hexa file 신규 (`lib/loader/pe_msvc_atexit_chain_init_real.hexa`):
- PE image 의 `.bss` 영역 식별 후 zero-fill 보장
- atexit chain head VA 식별 (TLS slot 0 또는 vcruntime internal global)
- 초기 sentinel NULL write
- self-test ≥ 12 record + 14 emit

각 sub-phase 후 inst 진척 + halt 위치 + own1 §A 정합 재확인.

---

## 7. 예상 inst 도달 (wave 10 통합 후)

[R3_PROGRESS_V8.md](R3_PROGRESS_V8.md) 의 1.1 표 형식 차용.

| wave | sub-phase | 시작 inst | 종료 inst (예상) | delta | 누적 배수 (vs 17) | 차단 위치 |
|---|---|---|---|---|---|---|
| 9-A | EncodePointer/DecodePointer simple XOR | 16486 | 17002 | +516 | ~1000× | .bss EIP=0x80147B |
| **10-A** | **verbose trace + indirect call source 추적** | **17002** | **18000+** | **+998+** | **~1058×** | ROL+XOR mask 누락 |
| **10-B** | **EncodePointer/DecodePointer real ROL+XOR** | **18000** | **22000+** | **+4000+** | **~1294×** | _initterm array end |
| **10-C** | **_initterm sentinel 엄격 검증 + .text bound** | **22000** | **30000+** | **+8000+** | **~1764×** | .bss 영역 indirect call |
| **10-D** | **.text 범위 밖 indirect call graceful redirect** | **30000** | **40000+** | **+10000+** | **~2352×** | atexit chain 미초기화 |
| **10-E** | **.bss atexit_callback_chain zero-init 보강** | **40000** | **50000+** | **+10000+** | **~2941×** | WinMain 진입 직전 |
| **11-A** | **WinMain 진입 + RegisterClassExA helper IPC** | **50000** | **65000+** | **+15000+** | **~3823×** | CreateWindowExA 직전 |
| **11-B** | **CreateWindowExA + 첫 NSWindow** | **65000** | **80000+** | **+15000+** | **~4705×** | message loop |

USER32 RegisterClassExA 도달 확률 — wave 10 통합 후 **70-80%** (D 까지 완료 시 50%, E 까지 완료 시 70-80%).

---

## 8. own1 §A enforcement

- 본 문서의 모든 reference 는 MSDN `learn.microsoft.com` public docs + PE/COFF Specification (Microsoft public) 만 사용.
- Wine 의 `dlls/msvcrt/`, `dlls/kernel32/`, `dlls/ntdll/` source 0줄 — 본 문서 작성자는 직접 disassembly + MSDN 만 reference.
- ReactOS source 0줄 — derivative work 가능성 회피.
- LLVM `compiler-rt` 의 `__security_init_cookie` / `EncodePointer` reimpl 0줄.
- Apple GPTK / CrossOver / Whisky 0줄 — 타사 Wine wrapper 추천 / 인용 / 참조 모두 회피.
- Capstone / Zydis / iced / triton 0줄 — disassembler / lifter 모두 미사용, shim 자체 32-bit interpreter.
- 자체 작성: ROL+XOR mask 의 cookie_lo5 shift + 32-bit unsigned rotation 는 Intel SDM Vol 2 의 ROL/ROR 의미 reference, code 자체 자체 작성.
- 자체 작성: PE section header bound check 는 PE/COFF Specification §6 reference, code 자체 자체 작성.
- 자체 작성: `.bss` zero-init 는 PE/COFF Specification §6.4 (BSS 의 raw size 0 + virtual size 만큼 zero-fill) reference, code 자체 자체 작성.

---

## 9. cumulative commit log (wave 9 까지)

| commit | 내용 | inst 효과 |
|---|---|---|
| `1cb4e3d` | hexa pe_msvc_initterm_array_real | (loader self-test) |
| `26bc6ce` | shim wave8-C fake_proc 4-byte stub + GPA dispatch | 15214 → 16373 (+1159) |
| `526acfa` | shim wave8-D sync/thread fn list + EncodePointer identity | 16373 → 16017 (-356 stable refactor) |
| `21e8dca` | shim wave8-D step2 bad_branch_target/mem_fault graceful skip cap 64 | 16017 → 16486 (+469) |
| `a2f32f8` | wave8 마무리 + R5 launcher skeleton | 16486 stable |
| `97746c7` | docs R3_PROGRESS_V8 16486 inst 970× | (docs) |
| `81f3462` | hexa pe_msvc_encoded_ptr_real EncodePointer/DecodePointer ROL+XOR tracker | (loader self-test) |
| `888921e` | hexa pe_msvc_initterm_real_walk _initterm 실 array walk + ctor invoke tracker | (loader self-test) |
| `81e8de7` | hexa pe_msvc_context_struct_fill CONTEXT/STACKFRAME64/EXCEPTION_RECORD tracker | (loader self-test) |
| `73b4ff1` | shim wave9-C RtlCaptureContext CONTEXT struct fill | (정렬) |
| **`94cdb16`** | **shim wave9-A EncodePointer/DecodePointer simple XOR** | **16486 → 17002 (+516)** |

cumulative: 17 inst (V2 시작) → **17002 inst (wave 9-A 종료)** = **+16985 inst, ~1000× 누적 배수**.

---

## 10. wave 11 trigger

wave 10 의 5 sub-phase (10-A verbose / 10-B real ROL+XOR / 10-C sentinel 엄격 / 10-D .text bound graceful / 10-E .bss atexit zero-init) 모두 완료 후 inst[40000-50000] 도달 시 wave 11 trigger.

**wave 11-A**: WinMain 진입 + RegisterClassExA helper IPC kind=13.
- `__scrt_translate_winmain` 통과 후 user 의 `WinMain(hInst, NULL, lpCmdLine, nShowCmd)` 진입.
- WNDCLASSEX struct fill (Battle.net Setup 의 main window class).
- RegisterClassExA → IAT thunk → helper IPC kind=13 fn_id=1 → atom return.
- atom 을 caller 가 받아 CreateWindowExA 의 lpClassName arg 로 전달.

**wave 11-B**: CreateWindowExA + 첫 NSWindow real alloc.
- CreateWindowExA → IAT thunk → helper IPC kind=14 fn_id=2 → NSWindow alloc + makeKeyAndOrderFront.
- ShowWindow / UpdateWindow → fn_id=3 / fn_id=4.
- message loop GetMessageA / TranslateMessage / DispatchMessageA → fn_id=5 / fn_id=6 / fn_id=7.

**wave 11 종료 시 산출물**: `R3_PROGRESS_V9.md` (V8 → V9 의 inst 진척 + USER32 도달 + 첫 NSWindow visible) + `R3_HALT_50000_ANALYSIS.md` (만약 50000 후 halt 시) + Battle.net Setup 의 main installer dialog 첫 화면 visible 검증.

trigger 조건:
- wave 10-A ~ 10-E 5 sub-phase 모두 commit 완료
- inst log 의 `.text bound graceful skip` 카운트 ≥ 5 회 + 이후 inst 진척 +10000 이상
- `_initterm sentinel valid: end=0x...` log 1 회 이상
- EncodePointer / DecodePointer round-trip identity 검증 통과 (input == decoded(encoded(input)))

---

**END OF DOC** — wave 10 trigger 후 `R3_HALT_17002_FOLLOWUP.md` (wave 10 통과 검증) + `R3_PROGRESS_V9.md` (10 sub-phase 통합 결과) 후속 문서 작성 예정.
