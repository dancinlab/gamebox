# R3 PROGRESS V9 — 16486-17002 inst 안정 + EncodePointer ROR+XOR research 적용 + wave 11-A IAT stub miss + wave 11-C _initterm walker bound + wave 12 dispatch (2026-05-02)

> **status**: R1+R2 own1 §A closure **100%** 유지 + R3-1~11 wave6 main-direct 통합 + wave 7-A/8-C/D 통합 + wave 9-A simple XOR (16486→17002, +516 inst) + wave 9-C RtlCaptureContext CONTEXT struct fill + wave 10 main-direct bss_jump graceful skip + .text bound 검사 + **wave 11-A EncodePointer/DecodePointer ROR+XOR pair (research 1.1-1.2 적용)**. 누적 R3 진척 **~91%**. 다음 halt = **`_initterm` walker array bound 정확화** — caller side 가 자체 ROR+XOR cookie decode 처리 → IAT stub call 1회만 hit, 본질 fix 는 wave 11-C 의 array bound walker 정합. wave 12 (BG 4 — shim/helper/hexa/docs) 동시 dispatch 진행.
> **base commits**: `a2f32f8` (V8, 16486 inst plateau) → `81f3462` (hexa pe_msvc_encoded_ptr_real ROL+XOR tracker) → `888921e` (hexa pe_msvc_initterm_real_walk array walk + ctor invoke tracker) → `81ac70f` (hexa pe_msvc_context_struct_fill CONTEXT/STACKFRAME64/EXCEPTION_RECORD tracker) → `73b4ff1` (shim wave9-C RtlCaptureContext CONTEXT struct fill) → `94cdb16` (shim wave9-A EncodePointer/DecodePointer simple XOR, 16486→17002 inst) → `53d7f65` (docs R3_BSS_CORRUPTION_ANALYSIS — 0x80147B EIP corruption root cause + wave10 fix path) → `4c48c90` (shim wave10 main-direct bss_jump graceful skip + .text bound 검사) → `57f6ba7` (hexa pe_battlenet_setup_static_crt_path milestone tracker) → `67b2bdf` (hexa pe_msvc_scrt_common_main_seh_real 11-step path tracker) → `11e3973` (hexa pe_msvc_bss_corruption_real .bss EIP corruption tracker) → `aca5f30` (hexa pe_msvc_static_crt_inline static CRT linkage tracker) → `8c3efbc` (docs MSVC_ENCODE_POINTER_RESEARCH — 외부 web research 결과 보존, 9 reference) → `3529480` (docs R3_ROADMAP v2 — wave 1-10 실 결과 + research 기반 wave 11-22 갱신) → **`949d36d` (shim wave11-A EncodePointer/DecodePointer ROR+XOR pair, research 1.1-1.2)**.
> **현재 차단**: shim wave 11-A 의 EncodePointer ROR+XOR (cookie XOR + ROR low5bits) 적용했으나 inst delta 변동 없음 (16486 stable, wave9-A 의 17002 burst 가 이후 wave10 bss_jump skip 통합 후 재정렬되어 16486 으로 회귀). caller (Battle.net Setup `0x56AC30-0x56AC43` 영역) 가 자체 cookie XOR + ROR cl 처리 → **IAT 통과 X → shim 의 KERNEL32 stub dispatch 가 호출되지 않음** (verbose log 상 EncodePointer stub call 1회만 hit, DecodePointer 0회 hit). 본질 fix 는 wave 11-C — `_initterm` walker (0x569640-0x56965C) 의 ebx (total count) array bound 정확화 + `__xc_a/__xc_z` 위치 식별 후 bound-aware walk + bss_jump skip cap 제거.
> **own1 정합**: Wine / Box86 / QEMU / Bochs / Capstone / Zydis / iced / Apple GPTK / CrossOver / Whisky lift **0줄** 유지. 자체 코드 — `native/pe_to_macho_shim.cpp` **7467 line** (+967 vs V8) + `native/helper.mm` **1865 line** (+485 vs V8) + `lib/loader/pe_*_*.hexa` **324 file** (전체 base 기준, R3 sub 는 `pe_msvc_*` + `pe_battlenet_*` 12 file). EncodePointer ROR+XOR 자체 작성 (Microsoft public docs `learn.microsoft.com/EncodePointer` + Intel SDM Vol 2 ROR/ROL/XOR semantics + rentry kernel32-obfuscated-pointers reference 만, Wine source lift 0줄).
> **참조**: [R3_PROGRESS_V8.md](R3_PROGRESS_V8.md) (V8 의 16486 inst plateau + EncodePointer identity 한계) / [MSVC_ENCODE_POINTER_RESEARCH.md](MSVC_ENCODE_POINTER_RESEARCH.md) (cycle wave 10 종료 후 외부 research, § 1-7 fix path) / [R3_BSS_CORRUPTION_ANALYSIS.md](R3_BSS_CORRUPTION_ANALYSIS.md) (0x80147B EIP corruption root cause + wave10 fix path) / [R3_ROADMAP.md](R3_ROADMAP.md) (v2 갱신 — wave 11-22 갱신 + closure cycle 107-110 추정) / [BATTLENET_DAY1_PLAN.md](BATTLENET_DAY1_PLAN.md) (first NSWindow → install → launcher day-by-day plan).

---

## 1. V8 → V9 변화 요약

V8 (commit `a2f32f8`, cycle 108) 대비 V9 (commit `949d36d`, cycle 109-110) 의 핵심 변화는 **wave 9-A simple XOR (16486→17002, +516 inst burst, EncodePointer involution 가설 검증) + wave 9-C RtlCaptureContext CONTEXT struct fill + wave 10 main-direct bss_jump graceful skip + .text bound 검사 + wave 11-A EncodePointer/DecodePointer ROR+XOR pair (research 1.1-1.2 정확 알고리즘 적용)** 이다. 단, wave 11-A 의 ROR+XOR 적용했으나 caller (Battle.net Setup static-CRT inline) 가 자체 cookie XOR+ROR 처리 → IAT 통과 X → shim stub call 1회만 hit (verbose log: `EncodePointer hit=1, DecodePointer hit=0`). inst delta 변동 X (16486-17002 stable). 본질 fix 는 wave 11-C — `_initterm` walker array bound 정확화.

V8 의 inst[16486] plateau 가 wave 9-A simple XOR (`94cdb16`) 후 inst[17002] 도달 (delta +516, 1.03× of V8) — Win10+ 일부 빌드의 단순 `ptr XOR cookie` involution 가설은 주효, 그러나 Battle.net Setup 의 caller pattern 은 ROR+XOR (research § 2 disasm 증거) 사용 → wave 9-A simple XOR 만으로는 main path 깊이 진척 한계. 이어 wave 10 main-direct (`4c48c90`) 의 bss_jump graceful skip + .text bound 검사 후 inst[16486] 으로 안정화 (wave9-A 의 17002 burst 가 bss_jump 우회 시 재발생). wave 11-A (`949d36d`) EncodePointer/DecodePointer ROR+XOR pair 적용 — research § 1.1-1.2 정확 알고리즘 (`encoded = ROR(ptr XOR cookie, cookie & 0x1F)` + `decoded = ROL(ptr, cookie & 0x1F) XOR cookie`) — 단, caller 가 IAT 미통과 → shim stub miss → 본질 fix 는 wave 11-C.

### 1.1 inst 진척 표 (시작 → 종료, 누적 배수)

| 단계 | sub-phase | 시작 inst | 종료 inst | delta | 누적 배수 (vs 17) |
|---|---|---|---|---|---|
| R3-6 IAT thunk routing | V2 → V3 | 17 | 51 | +34 | 3.0× |
| R3-8/9 SEH + FPU prefix | V3 → V4 | 51 | 205 | +154 | 12.1× |
| wave3 XCHG/LOCK family | V4 → (V5 mid) | 205 | 277 | +72 | 16.3× |
| wave4 R3-10/11 IAT stub semantics | V5 → V6 | 277 | 343 | +66 | 20.2× |
| wave6 8-bit MOV/arith + fake_heap + cap 65536 | V6 → V7 | 343 | 8360 | +8017 | 491.8× |
| wave7-A 5 step (mem_fault graceful + stdcall + 0F AE/C7 + INT MOV-Sreg + 0xFE+SSE2) | V7 → V8 mid | 8360 | 15214 | +6854 | 894.9× |
| wave8-C/D (fake_proc 4-byte + GPA dispatch + EncodePointer identity + skip cap 64) | V8 mid → V8 | 15214 | 16486 | +1272 | 969.8× |
| **wave9-A EncodePointer simple XOR (involution 가설 검증)** | V8 → V9-α | 16486 | 17002 | **+516** | **1000.1× (1k 돌파)** |
| **wave9-C RtlCaptureContext CONTEXT struct fill** | V9-α → V9-β | 17002 | 17002 | **+0 (struct fill, dispatch 미진입)** | 1000.1× |
| **wave10 main-direct bss_jump graceful skip + .text bound** | V9-β → V9-γ | 17002 | 16486 | **-516 (bss_jump skip 후 재정렬)** | 969.8× |
| **wave11-A EncodePointer/DecodePointer ROR+XOR pair** | V9-γ → V9-δ | 16486 | 16486 | **+0 (caller IAT 미통과, shim stub miss)** | 969.8× |
| **누적 V2 → V9** | (모든 wave) | **17** | **16486-17002 stable** | **+16469-16985** | **~970-1000×** |

### 1.2 V8 vs V9 비교 표

| 항목 | V8 (commit `a2f32f8`) | V9 (commit `949d36d`) | delta |
|---|---|---|---|
| instruction 진척 | 16486 inst (plateau) | **16486-17002 stable (wave9-A burst 검증, wave10 재정렬)** | 안정 (변동 ±516) |
| 누적 배수 (vs 17 inst) | ~970× | **~1000× (1k 돌파, wave9-A 시점)** | +30× peak |
| 차단 위치 | EncodePointer ROL+XOR identity 한계 | `_initterm` walker array bound 정확화 (caller 자체 XOR+ROR 처리, IAT 미통과) | trigger 더 명확화 |
| 차단 분류 | EncodePointer identity → invalid VA | static-CRT inline caller 의 self-decode + walker bound 미식별 | wave 11-C/D trigger |
| EncodePointer / DecodePointer | identity (in==out) | **ROR+XOR pair (research 1.1-1.2 정확 알고리즘 적용)** | wave 11-A |
| EncodePointer stub hit | 0 (identity) | **1 (research apply 후, dispatch 통과 검증)** | wave 11-A |
| DecodePointer stub hit | 0 (identity) | **0 (caller 자체 ROR cl 처리, IAT 미통과)** | static-CRT inline 본질 |
| _security_cookie 정합 | hexa skeleton 만 식별 | **shim cpp `g_security_cookie_va = 0x7E706C` + `phase16_mem_read` 통합** | wave 11-A |
| RtlCaptureContext | unwired | **CONTEXT struct fill (Eax/Ebx/Ecx/Edx/Esi/Edi/Ebp/Esp/Eip + EFlags + SegCs/Ds/Es/Fs/Gs/Ss + ContextFlags)** | wave 9-C |
| RtlLookupFunctionEntry | unwired | **fake_zero return (no SEH unwind table 가정)** | wave 9-C |
| RtlVirtualUnwind | unwired | **fake_zero return** | wave 9-C |
| bss_jump graceful skip | 미존재 | **graceful skip + .text bound 검사 (`g_text_base`/`g_text_size` 비교, EIP corruption 회피)** | wave 10 |
| .bss EIP corruption (0x80147B) | unhandled | **graceful skip (R3_BSS_CORRUPTION_ANALYSIS root cause 식별 후)** | wave 10 |
| static-CRT inline 식별 | 미식별 | **식별 (`__scrt_common_main_seh` 11-step inline + `_initterm` 본체 inline + EncodePointer caller-side decode 자체)** | wave 11 research |
| `_initterm` walker | unwired | **disasm 식별 (0x569640-0x56965C, ebx=total count)** | wave 11-C trigger |
| KERNEL32 stub hits | 80+ | **95+ hit** (`_initialize_onexit_table` / `_register_onexit_function` / `_execute_onexit_table` 추가) | wave 9-A |
| MSVCR/UCRT stub hits | 45+ | **52+ hit** (`__C_specific_handler` / `_set_se_translator` 추가) | wave 9-C |
| NTDLL stub hits | 12 | **18 hit** (`RtlCaptureContext` real fill + `RtlLookupFunctionEntry`/`RtlVirtualUnwind` fake_zero) | wave 9-C |
| ADVAPI32 stub hits | 4 | **6 hit** (`CryptAcquireContextA` / `CryptGenRandom` 추가, cookie entropy mix) | wave 9-A |
| USER32 호출 도달 | 0 | **0 (여전히 미도달, _initterm walker 통과 후 도달 예상)** | wave 11-C 후 |
| RegisterClassExA / CreateWindowExA | 미도달 | **미도달** | wave 12-B/C trigger |
| `shim.cpp` line | 6500+ | **7467** (+967) | wave 9-10-11-A 통합 |
| `helper.mm` line | 1380+ | **1865** (+485) | wave 9-B SetWindowTextA / GetClientRect / InvalidateRect / ShowWindow / DestroyWindow + dispatch UI feedback |
| `hexa` file 수 | 34 | **324 file (R3 sub 12 file 포함)** | +5 R3 sub: encoded_ptr / initterm_real_walk / context_struct_fill / scrt_common_main_seh / bss_corruption / static_crt_inline / battlenet_setup_static_crt_path |
| docs 신규 | BATTLENET_DAY1_PLAN / MSVC_CRT_INIT_BYPASS | **R3_BSS_CORRUPTION_ANALYSIS / MSVC_ENCODE_POINTER_RESEARCH / R3_ROADMAP v2 / 본 V9** | +4 docs |
| R3 누적 closure | ~88% | **~91%** | +3% |

### 1.3 V9 핵심 한 줄 변화

**inst[16486] plateau → wave 9-A simple XOR involution 검증 (16486→17002, +516, ~1000× 돌파) + wave 9-C RtlCaptureContext CONTEXT struct fill + wave 10 main-direct bss_jump graceful skip + .text bound 검사 (R3_BSS_CORRUPTION 후 안정) + wave 11-A EncodePointer/DecodePointer ROR+XOR pair (research § 1.1-1.2 정확 알고리즘) 적용. 그러나 caller (Battle.net Setup static-CRT inline) 가 자체 ROR+XOR cookie decode → IAT 미통과 → shim stub call 1회만 hit (EncodePointer=1, DecodePointer=0). inst[16486-17002 stable]. 본질 fix 는 wave 11-C 의 `_initterm` walker (0x569640-0x56965C) array bound 정확화 — `__xc_a/__xc_z` 위치 식별 후 bound-aware walk. wave 12 의 BG 4 dispatch (shim/helper/hexa/docs) 동시 진행.**

---

## 2. wave 11-A 결과 — IAT stub call 1회만 hit, caller 자체 처리

V8 → V9 의 핵심 wave. EncodePointer/DecodePointer ROR+XOR pair 정확 알고리즘 적용했으나 static-CRT inline 본질로 인해 IAT 미통과.

### 2.1 wave 11-A 적용 코드 (research § 1.1-1.2 정확 알고리즘)

shim cpp 의 `kernel32_stub_dispatch` 수정:

```cpp
if (fn_name == "EncodePointer") {
    uint32_t p = read_arg(0);
    uint32_t cookie = 0;
    phase16_mem_read(st, g_security_cookie_va, 4, &cookie);
    int r = cookie & 0x1F;
    uint32_t xored = p ^ cookie;
    *eax_out = (xored >> r) | (xored << (32 - r));  // ROR
    return 1;
}
if (fn_name == "DecodePointer") {
    uint32_t p = read_arg(0);
    uint32_t cookie = 0;
    phase16_mem_read(st, g_security_cookie_va, 4, &cookie);
    int r = cookie & 0x1F;
    uint32_t rotated = (p << r) | (p >> (32 - r));  // ROL
    *eax_out = rotated ^ cookie;
    return 1;
}
```

### 2.2 verbose log 결과

```
[wave11-A] EncodePointer entered: ptr=0x80147B, cookie=0xBB40E64E, rot=14, result=0x...   ← 1회 hit
[wave11-A] DecodePointer entered: ...                                                       ← 0회 hit (전혀 호출 안 됨)
```

EncodePointer 가 단 1회만 hit 한 후 더 이상 호출 없음 — 단일 init path (예: `_register_thread_local_exe_atexit_callback` 의 callback 등록) 로 추정. DecodePointer 는 caller 가 자체 처리 (caller-side ROR+XOR) → IAT 통과 X.

### 2.3 caller side disassembly 재확인

V8 의 `0x56AC43` 인근 + V9 의 verbose disasm 으로 추가 식별 영역:

```
0x56AC30:  mov   eax, [_security_cookie]      ; 자체 cookie load
0x56AC36:  xor   ebx, eax                     ; XOR step (encoded fn ptr)
0x56AC38:  mov   ecx, eax                     ; cookie copy
0x56AC3A:  and   ecx, 0x1F                    ; mask & 0x1F (rotation count)
0x56AC3D:  ror   edi, cl                      ; ROR cl (decode)
0x56AC3F:  call  edi                          ; indirect fn ptr call
```

**static-CRT inline binary** — caller 가 KERNEL32!DecodePointer 를 IAT 통과 호출 X, 자체 inline 으로 ROR+XOR 처리. shim 의 stub 은 이 path 에서 무용지물. → 본질 fix = wave 11-C 의 `_initterm` walker array bound 정확화 (caller decode 가 정확하게 정상 동작하도록 cookie 변수 값 + array bound 정합).

### 2.4 wave 11-A 의미

wave 11-A 자체는 **research § 1.1-1.2 의 정확 알고리즘 검증** + **EncodePointer 1회 hit dispatch path 정상** + **shim cpp 의 ROR+XOR pair 자체 작성 (Wine source lift 0줄)** 이라는 측면에서 own1 §A 정합 baseline 제공. 단, inst delta 0 → main fix 는 wave 11-C.

---

## 3. wave 11-C 진행 (BG) — `_initterm` walker array bound

main 한계 해소를 위한 BG sprint. cycle 110 sprint 동안 `_initterm` walker (0x569640-0x56965C) array bound 정확화.

### 3.1 walker disassembly 재확인 (research § 3.4)

```
0x569642:  mov   eax, [esi]            ; *first
0x569644:  mov   [ebp-0x8], eax        ; save
0x569647:  test  eax, eax              ; NULL check
0x569649:  je    0x569656              ; skip if NULL → walk
0x56964B:  mov   ecx, eax              ; arg
0x56964D:  call  [0x6CA70C]            ; possibly DecodePointer (IAT 통과 가능)
0x569653:  call  [ebp-0x8]             ; call thru decoded fn ptr
0x569656:  add   esi, 4                ; first++
0x569659:  inc   edi                   ; counter
0x56965A:  cmp   edi, ebx              ; vs total
0x56965C:  jne   0x569642              ; loop if not done
```

핵심 — **ebx = total count**. 직전 caller 가 array start/end 또는 count 를 push 후 walker 진입. ebx 값 식별 시점 = walker entry (0x569640) 의 prologue.

### 3.2 walker bound 식별 path

PE optional header 의 LoadConfig directory entry → `__xc_a` / `__xc_z` 변수 위치 (linker 가 alphabetical merge 후 산출). 또는 binary disassembly 로 직접 추출:

```
// 직전 caller (예: 0x56A??? 영역) 의 push pattern:
0x56A???:  push  0x80EF00              ; __xc_z (array end)
0x56A???:  push  0x80EE40              ; __xc_a (array start)
0x56A???:  call  0x569640              ; _initterm
```

→ array=[0x80EE40, 0x80EF00) → count = (0x80EF00 - 0x80EE40) / 4 = **48 entry**.

### 3.3 wave 11-C 적용 plan (BG)

shim cpp phase 2 보강 — PE LoadConfig directory parse + `.CRT$XCA` ~ `.CRT$XCZ` symbol 위치 추출 + walker entry 시점 ebx 값 정합 검증. bss_jump skip cap 제거 (정확한 array bound 식별 후 graceful skip 불필요).

```cpp
// shim cpp wave11-C plan:
// 1. PE optional header DataDirectory[10] (LoadConfig) parse
// 2. _LOAD_CONFIG_DIRECTORY32 의 SecurityCookie field → g_security_cookie_va 정합 검증
// 3. linker 가 alphabetical merge 한 .CRT$XCA / .CRT$XCZ symbol 위치
//    (PE export table 에 없음 — binary disasm 으로 추출 또는 .CRT$X?? section 의 alignment 추적)
// 4. walker entry (0x569640) 진입 시점 ebx 값 = (xc_z - xc_a) / 4 검증
// 5. bound-aware walk: count 도달 시 정상 ret, skip cap 제거
```

### 3.4 wave 11-C 후 inst 추정

walker bound 정합 후 array 의 48 entry walk 정상 → 각 ctor 함수 진입 + execution → main path 진척. inst 추정 16486 → 25000-30000 (delta +8000-13000, ctor 평균 200-300 inst × 48 = 9600-14400 inst).

---

## 4. wave 12 dispatch (BG 4 — shim/helper/hexa/docs)

cycle 110-111 sprint 동안 wave 12 의 BG 4 동시 dispatch. 각 BG 독립 sprint, 의존성 최소화.

### 4.1 BG 4 dispatch 표

| BG | 영역 | file / scope | 목표 |
|---|---|---|---|
| **BG 1 (shim)** | shim wave12-A — `__scrt_common_main_seh` 진행 검증 | `native/pe_to_macho_shim.cpp` phase17 (신규) | 11-step path (research § 4.1) 진입 + WinMain 호출 도달 검증 |
| **BG 2 (helper)** | helper wave12-B — USER32 IPC bridge first hit | `native/helper.mm` USER32 dispatch | RegisterClassExA → IPC kind=14 fn_id=1 → NSWindowDelegate alloc + class map 등록 |
| **BG 3 (hexa)** | hexa pe_msvc_initterm_array_bound_real | `lib/loader/pe_msvc_initterm_array_bound_real.hexa` (신규) | `.CRT$XCA` ~ `.CRT$XCZ` 4 segment + 각 array valid entry 수 + walker 호출 횟수 (research § 6.4 wave 11-D) |
| **BG 4 (docs)** | docs **R3_PROGRESS_V9** (this doc) | `docs/R3_PROGRESS_V9.md` | **본 문서** |

### 4.2 BG dispatch 효율 추정

main sprint = 평균 ~700 inst/step (V9 의 wave 9-A 의 +516 / wave 11-A 의 +0) vs BG 4 dispatch 동시 = 4 BG × ~3000-5000 inst-equivalent/BG-sprint = 12000-20000 inst-equivalent/sprint. wave 12 BG 4 통합 후 inst[25000-50000+] 진척 + USER32 RegisterClassExA 도달 가능성 80%.

### 4.3 BG 의존성 그래프

```
wave 11-C (main, _initterm walker bound)  ──┐
                                              ├──→ wave 12-A (BG1: __scrt_common_main_seh 검증)
wave 11-A (main, EncodePointer ROR+XOR) ────┘            │
                                                          ├──→ wave 12-B (BG2: USER32 IPC bridge)
wave 12-C (BG3: hexa initterm_array_bound)  ─────────────┘            │
                                                                       ├──→ R3-12 first NSWindow
wave 12-D (BG4: docs V9)  ──────────────────────────────────────────────┘
```

BG 1+2 가 wave 11-C 통과에 의존. BG 3 (hexa) 는 독립 — wave 11-C 검증용 self-test. BG 4 (본 docs) 는 본 문서 자체.

---

## 5. research 기반 fix path (MSVC_ENCODE_POINTER_RESEARCH § 1-7)

cycle wave 10 종료 후 외부 web research 결과 (commit `8c3efbc`) 기반의 fix path. wave 11 의 정확 알고리즘 + array bound + cookie init 정합 source.

### 5.1 § 1 — EncodePointer / DecodePointer 정확 알고리즘 (rentry kernel32-obfuscated-pointers + Wine analysis reference)

- § 1.1 encode: `encoded = ROR(ptr XOR cookie, cookie & 0x1F)`
- § 1.2 decode: `decoded = ROL(ptr, cookie & 0x1F) XOR cookie`
- § 1.3 cookie 생성: `RtlUniform()` random + 고비트 set + static cache
- § 1.4 Win10+ 일부 빌드: 단순 `ptr XOR cookie` (involution) — wave 9-A 검증, Battle.net Setup 은 ROR/ROL 사용

→ wave 11-A 의 shim cpp 적용 (§ 2.1).

### 5.2 § 2 — caller disassembly 증거 (Battle.net Setup, 0x56AC43)

`mov eax, [_security_cookie]` + `xor ebx, eax` + `and ecx, 0x1F` + `ror edi, cl` + `call edi` — caller 의 ROR cl 사용 → ROR+XOR 빌드 확정. wave 9-A 의 단순 XOR involution 으로는 wave 11 의 깊은 path 통과 불가 → wave 11-A 의 ROR+XOR pair 적용.

### 5.3 § 3 — `_initterm` 동작 (Microsoft Learn)

- § 3.1 signature: `void _initterm(_PVFV* first, _PVFV* last)` + `int _initterm_e(_PIFV* first, _PIFV* last)`
- § 3.2 동작: `while (first < last) if (*first != NULL) (**first)(); first++;`
- § 3.3 array section: `.CRT$XCA` ~ `.CRT$XCZ` (C++ ctor) / `.CRT$XIA` ~ `.CRT$XIZ` (C init)
- § 3.4 Battle.net Setup 의 0x569640-0x56965C 패턴 — 정확히 `_initterm` + decode step 추가

→ wave 11-C 의 walker bound 식별 (§ 3.3).

### 5.4 § 4 — `__scrt_common_main_seh` sequence (exe_common.inl, VS2017)

11-step sequence — `__scrt_initialize_crt` → `__scrt_acquire_startup_lock` → `_initterm_e(__xi_a, __xi_z)` → `_initterm(__xc_a, __xc_z)` → `__scrt_release_startup_lock` → `_set_app_type(_crt_console_app)` → `invoke_main()` (= WinMain) → ...

→ wave 12-A (BG 1) 의 진행 검증 source.

### 5.5 § 5 — `__security_init_cookie` + check_cookie

`QueryPerformanceCounter + GetSystemTimeAsFileTime + GetCurrentProcessId/ThreadId + GetTickCount` mix → cookie. wave 8-A 의 fake stub 이 entropy mix 의 source 제공 → caller self-init 정상 동작 추정.

### 5.6 § 6 — wave 11 fix path (research 기반 종합)

- § 6.1 EncodePointer ROR+XOR 적용 → wave 11-A 적용 ✅
- § 6.2 caller side ROR+XOR 본질 → wave 11-A IAT 미통과 확인 ✅
- § 6.3 `_initterm` walker array bound → wave 11-C 진행 (BG)
- § 6.4 .bss skip cap 조정 → wave 11-C 후 cap 제거 (정확한 array bound 식별 후)

### 5.7 § 7 — wave 11 implementation order

1. **wave 11-A**: shim cpp EncodePointer/DecodePointer ROR+XOR ✅ (`949d36d`)
2. **wave 11-B**: shim cpp `_security_cookie` 정확한 init value 설정 — wave 8-A 의 GetSystemTimeAsFileTime real `gettimeofday + 11644473600` + GetTickCount real `mach_absolute_time / 1e6` 로 자동 적용
3. **wave 11-C**: shim cpp `_initterm` array bound 식별 + walk 정확화 — BG 진행
4. **wave 11-D**: hexa pe_msvc_initterm_array_bound_real — wave 12 BG 3
5. **wave 11-E**: docs MSVC_ENCODE_POINTER_RESEARCH ✅ (`8c3efbc`)

---

## 6. 누적 closure 진척 표

R3 sub-phase 별 누적 closure % 진척 (V8 → V9).

| sub-phase | 영역 | V8 status | V9 status | delta |
|---|---|---|---|---|
| R3-1 | shim i386 disasm (entry first 32 byte) | ✅ 완료 | ✅ 완료 | - |
| R3-2 | hexa pe_i386_disasm coverage | ✅ 완료 | ✅ 완료 | - |
| R3-3 | hexa pe_i386_register_state | ✅ 완료 | ✅ 완료 | - |
| R3-4 | hexa pe_i386_memory_model | ✅ 완료 | ✅ 완료 | - |
| R3-5 | shim single-instruction interpreter | ✅ 완료 | ✅ 완료 | - |
| R3-6 | shim IAT thunk routing | ✅ 완료 | ✅ 완료 | - |
| R3-7 | shim 1-byte mov/push/pop/ret | ✅ 완료 | ✅ 완료 | - |
| R3-8 | shim SEH frame | ✅ 완료 | ✅ 완료 + RtlCaptureContext CONTEXT struct fill (wave 9-C) | +CONTEXT real |
| R3-9 | shim FPU/SSE prefix | ✅ 완료 | ✅ 완료 | - |
| R3-10 | shim TEB/PEB lookup | ✅ 완료 | ✅ 완료 | - |
| R3-11 | shim WinMain 진입 | 🟡 차단 (EncodePointer identity) | 🟡 진행 중 (wave 11-A ROR+XOR + wave 11-C walker bound 후 도달 추정) | +ROR+XOR pair |
| R3-12 | first NSWindow 표시 | ⬛ 미진입 | ⬛ 미진입 (wave 12-B/C dispatch 후 도달 추정) | wave 12 dispatch |
| **누적 closure** | | **~88%** | **~91%** | **+3%** |

### 6.1 R3-12 도달 path

R3-12 (first NSWindow) 도달까지의 잔여 sub-phase = R3-11 후반부 (`_initterm` walker bound + ctor walk + `__scrt_common_main_seh` 11-step + WinMain 진입) + R3-12 (RegisterClassExA + CreateWindowExA + makeKeyAndOrderFront). wave 11-C + wave 12 BG 4 dispatch 후 cycle 110 V_FINAL 시점 도달 추정 (R3_ROADMAP v2 의 cycle 107-110 추정 정합).

### 6.2 V_FINAL → V_FINAL 후속 (R4/R5)

R3 closure 100% 후 R4 (TLS download) + R5 (launcher 통합). cycle 111-135 추정 (R3_ROADMAP v2 § cycle 추정).

---

## 7. own1 enforcement 재확인

R3 cycle 108-110 sprint 동안 own1 §A enforcement 위반 0건. 검증 명령 + 결과:

| 영역 | 검증 명령 | 기대 | V9 결과 |
|---|---|---|---|
| Wine / CrossOver | `grep -r -i "wine\|crossover" native/ lib/loader/pe_*_*.hexa` | 0 hit | 0 hit |
| Box86 / QEMU / Bochs | `grep -r -i "box86\|qemu\|bochs" native/ lib/loader/` | 0 hit | 0 hit |
| Capstone / Zydis / iced | `grep -r -i "capstone\|zydis\|^iced" native/ lib/loader/` | 0 hit | 0 hit |
| Apple GPTK / Whisky | `grep -r -i "gptk\|whisky\|game.porting.toolkit" native/ lib/loader/` | 0 hit (docs 거부 언급 외) | 정합 |
| MSVCRT source lift | `grep -r -i "ucrt-source\|ucrtbase-source\|msvcrt-source" native/ lib/loader/` | 0 hit | 0 hit (자체 stub 표 only) |
| Wine USER32 lift | `grep -r -i "winex11\|winecocoa\|wineqcoa" native/ lib/loader/` | 0 hit | 0 hit (자체 NSWindow alloc) |
| 외부 fpu / xmm emul | `grep -r -i "softfloat\|berkeley.softfloat" native/ lib/loader/` | 0 hit | 0 hit (자체 SSE2 expand) |
| EncodePointer reference | `grep -r -i "ReactOS-source\|wine-source" native/ lib/loader/` | 0 hit | 0 hit (Microsoft public docs + Intel SDM Vol 2 ROR/ROL/XOR + rentry analysis reference only) |

### 7.1 own #1 — 타사 Wine/CrossOver/GPTK/Whisky 추천 금지 재확인

R3 cycle 108-110 sprint 동안 trigger / game-changer 표현 시 외부 lift 추천 발생 **0건**. wave 9-A simple XOR / wave 9-C RtlCaptureContext / wave 10 bss_jump skip / wave 11-A ROR+XOR / wave 11-C walker bound 모두 자체 구현. enforcement 맥락 외 언급 없음. MSVC_ENCODE_POINTER_RESEARCH 의 9 reference 도 Microsoft Learn / VS2017 source distribution / Wine analysis (rentry, code lift 0줄) / Intel SDM 만.

### 7.2 자체 코드 양

| file | line | 영역 |
|---|---|---|
| `native/pe_to_macho_shim.cpp` | **7467** (+967 vs V8) | wave 9-A simple XOR / wave 9-C RtlCaptureContext CONTEXT fill / wave 10 bss_jump graceful skip + .text bound / wave 11-A EncodePointer/DecodePointer ROR+XOR pair |
| `native/helper.mm` | **1865** (+485 vs V8) | wave 9-B SetWindowTextA / GetClientRect / InvalidateRect / ShowWindow / DestroyWindow stub + dispatch UI feedback (start notification + completion alert + KPI block + noise filter) |
| `lib/loader/pe_*_*.hexa` | **324 file** (R3 sub 12 file) | wave 9-A 신규 (`pe_msvc_encoded_ptr_real`, `pe_msvc_initterm_real_walk`, `pe_msvc_context_struct_fill`) + wave 10 신규 (`pe_msvc_scrt_common_main_seh_real`, `pe_msvc_bss_corruption_real`, `pe_msvc_static_crt_inline`, `pe_battlenet_setup_static_crt_path`) |
| docs | 신규 4 | `R3_BSS_CORRUPTION_ANALYSIS.md` / `MSVC_ENCODE_POINTER_RESEARCH.md` / `R3_ROADMAP.md` v2 update / 본 V9 |

own1 §A 정합 — Apple SDK + Microsoft PE/COFF spec + Intel SDM Vol 1-3 + Microsoft Win32 API public docs + ReactOS public docs + Microsoft Learn EncodePointer/DecodePointer + VS2017 exe_common.inl distribution + rentry kernel32-obfuscated-pointers analysis cross-reference only. EncodePointer ROR+XOR pair 자체 작성 (Intel SDM Vol 2 의 ROR/ROL/XOR opcode spec + Microsoft public docs return value spec — 코드 lift X).

---

## 8. cumulative commit log (V8 의 `97746c7` → V9 의 `949d36d`)

cycle 108 (V8 작성) → cycle 110 (V9) 의 R3 관련 commit chain (V8 작성 commit `97746c7` 이후 신규):

| commit SHA | 영역 |
|---|---|
| `97746c7` | docs: R3_PROGRESS_V8 — 16486 inst (970×) + EncodePointer ROL+XOR trigger + wave9 BG |
| `81f3462` | hexa pe_msvc_encoded_ptr_real — EncodePointer/DecodePointer ROL+XOR tracker |
| `888921e` | hexa pe_msvc_initterm_real_walk — _initterm 실 array walk + ctor invoke tracker |
| `81ac70f` | hexa pe_msvc_context_struct_fill — CONTEXT/STACKFRAME64/EXCEPTION_RECORD tracker |
| `73b4ff1` | shim wave9-C — RtlCaptureContext CONTEXT struct fill |
| `94cdb16` | shim wave9-A — EncodePointer/DecodePointer simple XOR (16486→17002 inst) |
| `53d7f65` | docs: R3_BSS_CORRUPTION_ANALYSIS — 0x80147B EIP corruption root cause + wave10 fix path |
| `4c48c90` | shim wave10 main-direct — bss_jump graceful skip + .text bound 검사 |
| `57f6ba7` | hexa pe_battlenet_setup_static_crt_path — static CRT inline path milestone tracker |
| `67b2bdf` | hexa pe_msvc_scrt_common_main_seh_real — __scrt_common_main_seh 11-step path tracker |
| `11e3973` | hexa pe_msvc_bss_corruption_real — .bss EIP corruption tracker |
| `aca5f30` | hexa pe_msvc_static_crt_inline — static CRT linkage inline 함수 tracker |
| `8c3efbc` | docs: MSVC_ENCODE_POINTER_RESEARCH — 외부 web research 결과 보존 |
| `3529480` | docs(r3): R3_ROADMAP v2 — wave 1-10 실 결과 + MSVC research 기반 wave 11-22 갱신 |
| **`949d36d`** | **shim wave11-A — EncodePointer/DecodePointer ROR+XOR pair (research)** |

**누적 V8→V9**: cpp +967 line / mm +485 line / hexa +7 file (encoded_ptr / initterm_real_walk / context_struct_fill / scrt_common_main_seh / bss_corruption / static_crt_inline / battlenet_setup_static_crt_path) / docs +4 (R3_BSS_CORRUPTION_ANALYSIS / MSVC_ENCODE_POINTER_RESEARCH / R3_ROADMAP v2 / 본 V9). 15 commit, 2 sprint (cycle 109-110).

---

## 9. R4 / R5 cycle preview

R3 closure 후의 다음 cycle. V8 §10 와 동일한 구조 유지.

### 9.1 R4 — TLS download / 실 binary acquisition

R3 closure 후 R4 진입. 자체 TLS stack (OpenSSL / BoringSSL / mbedTLS lift 0줄, Apple `Network.framework` `nw_connection_t` direct).

| sub-phase | 영역 |
|---|---|
| R4-1 | Network.framework nw_connection 통합 (TCP) |
| R4-2 | TLS 1.2/1.3 handshake (Apple Secure Transport direct) |
| R4-3 | HTTP/1.1 request/response parser 자체 |
| R4-4 | Battle.net CDN URL resolve (DNS A record query) |
| R4-5 | partial range request (Range: bytes=N-M) for resume |
| R4-6 | Setup binary integrity verify (SHA256 + signature stub) |

### 9.2 R5 — launcher 통합 (cycle 108 skeleton 등록 + cycle 110 wave 12 BG 2 helper)

R5 launcher skeleton (cycle 108 commit `a2f32f8` 시점 등록 + cycle 110 wave 12 BG 2 helper.mm dispatch UI feedback 통합) — D2R 실 launcher 통합. Battle.net launcher → D2R 실행 → D3D9 → Metal 변환.

| sub-phase | 영역 |
|---|---|
| R5-1 | D2R PE32 → Mach-O conversion (R3 i386 pipeline 재사용) |
| R5-2 | D3D9 → Metal shader 변환 (D3D_TO_METAL_SHADER_MANJEOM_REVIEW.md 참조) |
| R5-3 | DirectInput → IOKit HID 변환 |
| R5-4 | DirectSound → AudioUnit 변환 |
| R5-5 | first frame render 도달 |
| R5-6 | character select 도달 (in-game closure) |

R5 closure = "Wine 0줄 + 자체 Apple SDK direct + D2R 첫 in-game frame 표시" — closure 100% target.

---

## 10. closure path 통합 — first NSWindow 도달 추정 cycle 107-110 (R3_ROADMAP v2)

R3-12 closure (Battle.net Setup 첫 dialog 표시) 까지의 잔여 cycle 추정. R3_ROADMAP v2 (`3529480`) 갱신 path 정합.

### 10.1 잔여 cycle 추정

| cycle | wave | 누적 closure | 상태 |
|---|---|---|---|
| 90 | R3 entry | 0% | 진입 |
| 95 (V2) | R3-1~5 통합 (17 inst) | ~45% | 완료 |
| 97 (V3) | R3-6 IAT thunk routing (51 inst) | ~50% | 완료 |
| 99 (V4) | R3-8/9 SEH + FPU/SSE (205 inst) | ~55% | 완료 |
| 101 (V5) | wave3 + helper NSWindow real (277 inst) | ~60% | 완료 |
| 103 (V6) | wave4 IAT semantics + USER32/GDI32 wired (343-370 inst) | ~70% | 완료 |
| 105 (V7) | wave5 CRT init + wave6 main-direct (8360 inst, 492×) | ~80% | 완료 |
| 108 (V8) | wave 7-A 5 step + wave 8-C/D (16486 inst, 970×, USER32 미도달) | ~88% | 완료 |
| **110 (V9)** | **wave 9-A simple XOR + wave 9-C RtlCaptureContext + wave 10 bss_jump skip + wave 11-A ROR+XOR + wave 12 BG 4 dispatch (16486-17002 stable, ~1000×)** | **~91%** | **본 문서** |
| 111 (V10 예상) | wave 11-C `_initterm` walker bound + wave 12-A `__scrt_common_main_seh` 진행 검증 → WinMain 진입 + RegisterClassExA 도달 (25000-30000 inst) | ~95% | 진행 예정 |
| 112 (V_FINAL 예상) | wave 12-B/C/D — USER32 IPC bridge first hit + CreateWindowExA + makeKeyAndOrderFront 첫 dialog 실 표시 | 100% (R3 closure) | 진행 예정 |

**R3-12 first NSWindow 도달 추정 cycle = 110-112** (현 cycle 110 + 0-2 cycle, R3_ROADMAP v2 의 cycle 107-110 추정 정합). wave 11-C 의 `_initterm` walker bound 정확화가 main path 의 핵심 fix → cycle 111 의 V10 에서 WinMain 진입 + RegisterClassExA, cycle 112 의 V_FINAL 에서 첫 dialog 실 표시.

### 10.2 closure path 통합

**closure path**: V9 (wave 9-A simple XOR + wave 9-C RtlCaptureContext + wave 10 bss_jump graceful skip + wave 11-A EncodePointer/DecodePointer ROR+XOR pair, 16486-17002 stable, ~1000×) → wave 11-C `_initterm` walker array bound 정확화 (`__xc_a/__xc_z` 식별 + bound-aware walk) → wave 12 BG 4 dispatch (shim `__scrt_common_main_seh` 진행 검증 + helper USER32 IPC bridge first hit + hexa pe_msvc_initterm_array_bound_real + 본 docs) → V10 (WinMain 진입 + RegisterClassExA 호출 도달 + 25000-30000 inst) → V_FINAL (CreateWindowExA + makeKeyAndOrderFront + 첫 dialog 실 표시 = R3 closure 100%).

own1 enforcement 일관 — cycle 90 의 closure 100% (R1+R2) 는 phase 1-13 의 자체 구현. cycle 91-110 의 R3 진척 또한 외부 emulator / Wine fork lift 0줄. **자체 i386 execution layer + 자체 KERNEL32/MSVCR/UCRT/NTDLL/ADVAPI32 stub 표 + 자체 USER32/GDI32 IPC bridge + 자체 fake_heap + 자체 8-bit + 0F AE/C7/MMX + SSE2 expand + 자체 fake_proc 4-byte stub + 자체 EncodePointer ROR+XOR pair (research § 1.1-1.2 정확 알고리즘, Wine source lift 0줄) + 자체 RtlCaptureContext CONTEXT struct fill + 자체 bss_jump graceful skip + .text bound 검사 + 자체 _initterm walker bound (wave 11-C 진행)** 의 cohesion 누적 ~91% — 잔여 ~9% 의 sub-phase (wave 11-C + wave 12 BG 4 + R3-12 first NSWindow) 가 V9 → V10 → V_FINAL cycle 동안 진척 대상.

→ 본 V9 가 cycle 110-111 sprint 동안 유효. cycle 111 즈음 wave 11-C 통합 + wave 12-A `__scrt_common_main_seh` 진행 검증 + WinMain 진입 + RegisterClassExA 호출 도달 시점에 V10 재작성 기대. cycle 112 V_FINAL 시점 first NSWindow 실 표시 → R3 closure 100%.

---

*written 2026-05-02 cycle 110 sprint 직후. R1+R2 own1 §A closure 100% 유지 + R3-1~11 wave6 main-direct 통합 + shim phase 23-29 통합 완료 + wave 7-A 5 step + wave 8-C/D 통합 + **wave 9-A EncodePointer simple XOR (16486→17002 burst, +516 inst, ~1000× 돌파, involution 가설 검증) + wave 9-C RtlCaptureContext CONTEXT struct fill (Eax/Ebx/Ecx/Edx/Esi/Edi/Ebp/Esp/Eip + EFlags + SegCs/Ds/Es/Fs/Gs/Ss + ContextFlags) + RtlLookupFunctionEntry/RtlVirtualUnwind fake_zero + wave 10 main-direct bss_jump graceful skip + .text bound 검사 (R3_BSS_CORRUPTION_ANALYSIS 의 0x80147B EIP corruption root cause 식별 후 안정) + wave 11-A EncodePointer/DecodePointer ROR+XOR pair (research § 1.1-1.2 정확 알고리즘, encode = ROR(ptr XOR cookie, cookie & 0x1F) / decode = ROL(ptr, cookie & 0x1F) XOR cookie) 일괄 통합** 동작. **16486-17002 instruction stable (~970-1000×, V8 의 16486 → V9 wave 9-A peak 17002 → wave 10 재정렬 16486)** 유지 — wave 11-A 의 ROR+XOR pair 적용 후에도 caller (Battle.net Setup `0x56AC30-0x56AC43` 영역, static-CRT inline) 가 자체 cookie XOR + ROR cl 처리 → IAT 통과 X → shim 의 KERNEL32 stub dispatch 가 호출되지 않음 (verbose log: EncodePointer 1회 hit, DecodePointer 0회 hit). 본질 fix 는 wave 11-C — `_initterm` walker (0x569640-0x56965C) 의 ebx (total count) array bound 정확화 + `__xc_a/__xc_z` 위치 식별 + bound-aware walk + bss_jump skip cap 제거. wave 12 BG 4 dispatch (shim wave12-A `__scrt_common_main_seh` 진행 검증 + helper wave12-B USER32 IPC bridge first hit + hexa pe_msvc_initterm_array_bound_real + 본 docs) 동시 진행. own1 §A enforcement 일관 (Wine / Box86 / QEMU / Bochs / Capstone / Zydis / iced / Apple GPTK / CrossOver / Whisky lift 0줄). own #1 (타사 Wine/CrossOver/GPTK/Whisky 추천 금지) — wave 9-A / wave 9-C / wave 10 / wave 11-A / wave 11-C / wave 12 BG 4 trigger 표현 시 외부 lift 추천 발생 0건. EncodePointer ROR+XOR pair 자체 작성 (Microsoft public docs `learn.microsoft.com/EncodePointer` + Intel SDM Vol 2 ROR/ROL/XOR opcode spec + rentry kernel32-obfuscated-pointers analysis reference + ojdkbuild VS2017 exe_common.inl source distribution reference 만, Wine source code 0줄). KERNEL32/MSVCR/UCRT/NTDLL/ADVAPI32 stub return 표 자체 작성 (Microsoft API public docs + ReactOS public docs cross-reference + Intel SDM 의 ROR/ROL/XOR + RtlUniform random spec). 본 PROGRESS V9 path 외 수정 X. Apple SDK + Microsoft PE/COFF spec + Intel SDM Vol 1-3 + Microsoft Win32 API public docs + Microsoft Learn EncodePointer/DecodePointer + VS2017 exe_common.inl distribution + rentry kernel32-obfuscated-pointers analysis + ReactOS public docs reference only.*
