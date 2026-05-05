# MSVC EncodePointer / _initterm 웹 research 결과

**작성**: 2026-05-02 cycle wave 10 종료 후 · base commit `aca5f30`
**목적**: wave 11 의 정확한 fix path 결정용 외부 research 결과 보존

---

## 1. EncodePointer / DecodePointer 정확 알고리즘

**source**: [rentry kernel32-obfuscated-pointers](https://rentry.co/kernel32-obfuscated-pointers)
(원본은 Wine source code 의 dlls/kernel32/process.c 의 `EncodePointer` 구현 분석)

### 1.1 encode (XP/2003 / 일부 Win7-8 빌드)

```c
ULONG_PTR EncodePointer(ULONG_PTR ptr) {
    ULONG_PTR cookie = pointer_obfuscator;  // static cached value
    ULONG_PTR result = ptr ^ cookie;
    return _rotr(result, cookie);  // rotate-right by cookie value
}
```

의사코드:
```
encoded = ROR(ptr XOR cookie, cookie & 0x1F)
```

### 1.2 decode

```c
ULONG_PTR DecodePointer(ULONG_PTR ptr) {
    ULONG_PTR cookie = pointer_obfuscator;
    ULONG_PTR rotated = _rotl(ptr, cookie);  // rotate-left
    return rotated ^ cookie;
}
```

의사코드:
```
decoded = ROL(ptr, cookie & 0x1F) XOR cookie
```

### 1.3 cookie 생성

`RtlUniform()` 으로 random 값 생성, 고비트 set 으로 dereferencing crash attempts 회피.
한 번 init 후 `pointer_obfuscator` static 변수에 cache → 후속 호출 모두 재사용.

### 1.4 Win10+ 일부 빌드의 단순화

Win10+ 일부 빌드는 단순 `ptr XOR cookie` (involution) — wave 9-A 의 발견.
그러나 Battle.net Setup 의 caller disassembly pattern 은 ROR/ROL 사용 확인됨.

---

## 2. caller disassembly 증거 (Battle.net Setup, 0x56AC43)

R3_PROGRESS_V8.md 에서 식별된 caller pattern:

```
0x56AC43: mov eax, [_security_cookie]     ; load cookie
0x56AC??: xor ebx, eax                    ; XOR step
0x56AC??: and ecx, 0x1F                   ; mask cookie & 0x1F
0x56AC??: ror edi, cl                     ; rotate-right by (cookie & 0x1F)
0x56AC??: call edi                        ; call decoded fn ptr
```

caller 의 `ror` 사용 → **rotate 가 필요한 ROL+XOR 빌드**. wave 9-A 의 단순 XOR
involution 으로는 wave 11 의 깊은 path 통과 못 함.

---

## 3. _initterm 동작

**source**: [Microsoft Learn — _initterm, _initterm_e](https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/initterm-initterm-e?view=msvc-170)

### 3.1 signature

```c
void __cdecl _initterm(_PVFV* first, _PVFV* last);
int __cdecl _initterm_e(_PIFV* first, _PIFV* last);

typedef void (__cdecl *_PVFV)(void);
typedef int  (__cdecl *_PIFV)(void);
```

### 3.2 동작 (Microsoft 공식 doc)

> "walk the table of function pointers from the bottom up,
>  until the end is encountered.
>  if current table entry is non-NULL, call thru it."

의사코드:
```c
void _initterm(_PVFV* first, _PVFV* last) {
    while (first < last) {
        if (*first != NULL)
            (**first)();  // call thru fn ptr
        first++;
    }
}
```

`_initterm_e` 는 동일하나 fn 의 return 값이 0 아니면 즉시 abort.

### 3.3 array section

- `.CRT$XCA` ~ `.CRT$XCZ`: C++ static ctor (CRT init)
- `.CRT$XIA` ~ `.CRT$XIZ`: C init array
- `.CRT$XPA` ~ `.CRT$XPZ`: pre-CRT
- linker 가 alphabetical merge → array bound 자동 산출

### 3.4 Battle.net Setup 의 0x569640-0x56965C 패턴

```
0x569642: mov eax, [esi]    ; *first
0x569644: mov [ebp-0x8], eax ; save
0x569647: test eax, eax      ; NULL check
0x569649: je 0x569656        ; skip if NULL → walk
0x56964B: mov ecx, eax       ; arg
0x56964D: call [0x6CA70C]    ; possibly DecodePointer or KERNEL32 fn
0x569653: call [ebp-0x8]     ; call thru decoded fn ptr
0x569656: add esi, 4         ; first++
0x569659: inc edi            ; counter
0x56965A: cmp edi, ebx       ; vs total
0x56965C: jne 0x569642       ; loop if not done
```

**정확히 `_initterm` 패턴** + decode step 추가 (call [0x6CA70C]).
0x6CA70C 는 IAT entry — KERNEL32!DecodePointer 가능성 높음. ebx = total count.

---

## 4. __scrt_common_main_seh sequence

**source**: [exe_common.inl — VS2017 MSVC source](https://github.com/ojdkbuild/tools_toolchain_vs2017bt_1416/blob/master/VC/Tools/MSVC/14.16.27023/crt/src/vcruntime/exe_common.inl)

### 4.1 11-step sequence (정상 path)

```c
int __scrt_common_main_seh(void) {
    if (!__scrt_initialize_crt(__scrt_module_type::exe))
        __scrt_fastfail(FAST_FAIL_FATAL_APP_EXIT);
    
    bool has_cctor = false;
    __scrt_acquire_startup_lock();
    
    if (__scrt_current_native_startup_state == initialized)
        has_cctor = true;
    else if (__scrt_current_native_startup_state == uninitialized) {
        __scrt_current_native_startup_state = initializing;
        
        if (_initterm_e(__xi_a, __xi_z) != 0)  // C init
            return 255;
        _initterm(__xc_a, __xc_z);              // C++ ctor
        __scrt_current_native_startup_state = initialized;
    }
    else has_cctor = true;
    __scrt_release_startup_lock();
    
    _set_app_type(_crt_console_app);  // or gui_app
    
    int main_result = invoke_main();   // ← WinMain / main
    
    if (!__scrt_is_managed_app())
        exit(main_result);
    
    return main_result;
}
```

### 4.2 init_array 자체

- `_initterm_e(__xi_a, __xi_z)` — C init function array (return non-0 → abort)
- `_initterm(__xc_a, __xc_z)` — C++ static ctor (void return)

---

## 5. __security_init_cookie + check_cookie

**source**: [flysand7's blog — How security cookie works](https://flysand7.hashnode.dev/how-security-cookie-works)
(403 으로 못 fetch 했지만 search 결과 요약)

### 5.1 cookie 생성

```c
extern uintptr_t __security_cookie;

void __security_init_cookie(void) {
    LARGE_INTEGER counter;
    QueryPerformanceCounter(&counter);
    
    FILETIME ft;
    GetSystemTimeAsFileTime(&ft);
    
    DWORD pid = GetCurrentProcessId();
    DWORD tid = GetCurrentThreadId();
    
    uintptr_t cookie = counter.QuadPart;
    cookie ^= ft.dwLowDateTime;
    cookie ^= ft.dwHighDateTime;
    cookie ^= pid;
    cookie ^= tid;
    cookie ^= GetTickCount();
    
    // make sure cookie is not 0 and not equal to default
    if (cookie == 0 || cookie == DEFAULT_SECURITY_COOKIE)
        cookie = DEFAULT_SECURITY_COOKIE + 1;
    
    __security_cookie = cookie;
    __security_cookie_complement = ~cookie;
}
```

### 5.2 check_cookie (function epilogue)

```asm
__security_check_cookie:
    cmp ecx, [__security_cookie]
    jne fail
    ret
fail:
    jmp __report_gsfailure
__report_gsfailure:
    int 29h  ; __fastfail FAST_FAIL_STACK_COOKIE_CHECK_FAILURE
```

### 5.3 LoadConfig directory 의 SecurityCookie field

PE optional header 의 LoadConfig directory entry 의 `SecurityCookie` RVA →
binary 의 `__security_cookie` 변수 위치. wave 8 가 이미 식별: `g_security_cookie_va = 0x7E706C`.

---

## 6. wave 11 fix path (research 기반)

### 6.1 EncodePointer ROL+XOR 적용

shim cpp 의 `kernel32_stub_dispatch` 에서:

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

### 6.2 caller side ROR+XOR 본질

caller 가 직접 ROR+XOR 으로 decode 하면 (IAT 통과 X), shim 측 stub 이 호출되지
않음. 그러나 caller 의 `_security_cookie` 변수 값이 정확하면 caller 의 자체
decode 가 정상 동작 → 0x56AC43 의 `ror edi, cl` 후 valid fn ptr 추출.

따라서 핵심은:
1. `_security_cookie` 변수의 값을 caller 가 원하는 entropy 로 init
2. encoded fn ptr table 의 값들이 그 cookie 로 정확히 ROR+XOR encode 됐다고 가정

### 6.3 _initterm walker 의 array bound

caller 가 `cmp edi, ebx; jne 0x569642` — ebx 가 array length. 정확한 array
bound 식별 필요:
- PE LoadConfig directory 의 `__xc_a` / `__xc_z` symbol RVA (Win SDK link options)
- 또는 binary disassembly 로 array start/end 상수 추출

### 6.4 .bss skip cap 조정

wave 10 의 cap 128 graceful skip — array 끝 도달 후에도 skip 시도. cap 늘리면
nonsense path 진입. 정확한 array bound 식별 후 skip 제거가 정답.

---

## 7. wave 11 의 implementation order

1. **wave 11-A**: shim cpp EncodePointer/DecodePointer ROR+XOR (research 1.1-1.2)
2. **wave 11-B**: shim cpp `_security_cookie` 정확한 init value 설정 (research 5.1)
3. **wave 11-C**: shim cpp `_initterm` 의 array bound 식별 + walk 정확화 (research 3.2)
4. **wave 11-D**: hexa pe_msvc_initterm_array_bound_real (PE LoadConfig + binary disasm)
5. **wave 11-E**: docs MSVC_ENCODE_POINTER_RESEARCH (this doc)

각 step 후 inst 진척 + USER32 IPC hits 추적. 도달 전 wave 12 / 13 등 추가
cycle 필요 가능성 있으나 root cause 명확화로 진척 가속화.

---

## 8. own1 §A enforcement 재확인

### 8.1 reference 정합

- Microsoft public docs (Microsoft Learn / MSDN): OK
- VS2017 MSVC source code 의 `exe_common.inl`: ojdkbuild GitHub mirror — Microsoft 공개
  source distribution. **Wine source 0줄**.
- Wine source 분석 (rentry doc) 는 reference 만 — 코드 lift 는 X.

### 8.2 자체 구현

ROR/ROL pair 자체 작성:
```c
uint32_t enc(uint32_t p, uint32_t cookie) {
    int r = cookie & 0x1F;
    uint32_t x = p ^ cookie;
    return (x >> r) | (x << (32 - r));
}
```
3 line shifted-or — Wine implementation lift X, Intel SDM Vol 2 의 ROR semantics
직접 따름.

---

## 9. 잔여 위험 + 회피

- Battle.net Setup 의 binary 가 Microsoft Authenticode 서명 — 단, shim 은 검증 안 함
- LTCG / inline 으로 함수 경계 모호 — _initterm 의 `0x569640` location 은 disassembly 로만 식별
- CFG (Control Flow Guard) 의 `_guard_check_icall` — 별도 stub 처리 필요 (wave 12+)
- Static-CRT 빌드의 EncodePointer 는 IAT 호출 X → caller 의 cookie XOR + ROR
  자체 처리 → shim 의 stub 이 호출되지 않음 → cookie 변수 값이 더 중요

---

## 10. references (markdown link 보존)

- [Microsoft Learn — _initterm, _initterm_e](https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/initterm-initterm-e?view=msvc-170)
- [rentry kernel32-obfuscated-pointers](https://rentry.co/kernel32-obfuscated-pointers)
- [exe_common.inl — VS2017 MSVC source](https://github.com/ojdkbuild/tools_toolchain_vs2017bt_1416/blob/master/VC/Tools/MSVC/14.16.27023/crt/src/vcruntime/exe_common.inl)
- [MSVC CRT Initialization | gonwan](https://www.gonwan.com/2014/02/13/msvc-crt-initialization/)
- [Understanding __scrt_common_main_seh](https://copyprogramming.com/howto/what-where-is-scrt-common-main-seh)
- [How security cookie works — flysand7's blog](https://flysand7.hashnode.dev/how-security-cookie-works)
- [BEARWINDOWS — MSVC tips](https://bearwindows.zcm.com.au/msvc.htm)
- [EncodePointer function (Windows) | Microsoft Learn](https://learn.microsoft.com/en-us/previous-versions/bb432254(v=vs.85))
- [Modern Visual Studio meets ancient Windows — ColinFinck](https://colinfinck.de/posts/modern-visual-studio-meets-ancient-windows/)

---

본 문서는 wave 10 종료 시점 외부 research 결과 보존. wave 11 의 정확한 fix
path 결정 + own1 §A 정합 reference 만 사용 명시. shim cpp 변경 X (research only).
