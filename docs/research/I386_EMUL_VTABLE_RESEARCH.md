# I386_EMUL_VTABLE_RESEARCH — i386 emulation, C++ vtable, partial buffer init

**date**: 2026-05-02
**scope**: airgenome-gamebox / wave 15-A vtable corruption recovery 의 *외부 reference 자료* 정리.
**own1 §A enforcement**: citation only — Wine / CrossOver / GPTK / Whisky / Box86 / Hangover / FEX / QEMU / Bochs / iced 등 community PE-호환 layer 코드 단 1 줄도 lift 하지 않음. Microsoft 공식 docs (learn.microsoft.com), Apple SDK reference, Intel SDM, 학술 논문, 일반 reverse-engineering 블로그만 reference.

---

## 0. 본 문서의 성격

본 문서 = wave 15-A (vtable corruption recovery — DecodePointer ROL+XOR 미반영 + sentinel mismatch) 진입 직전의 *literature survey*. 어떠한 finding 도 lib/ 또는 native/ 의 코드로 직접 lift 되지 않으며, 모든 hexa 구현은 자체 작성 (own1 §A § rule b 준수). 본 finding 은 알고리즘적 *방향* 결정 (예: ROL 를 simple XOR 다음에 추가할지 vs 역순) 에만 영향.

각 query 는 keyword 기반 web search 로 진행했으며 본 문서 끝의 §13 Sources 에 URL 을 명시.

---

## 1. Query 1 — i386 instruction emulation interpreter techniques

### finding

Instruction-set simulator (ISS) 는 3 가지 주요 기법으로 구현된다 (Wikipedia ISS 항목):

1. **Interpretation** — 매 instruction 을 직접 fetch/decode/execute. 가장 간단하고 정확. JIT 대비 ~10× 느림.
2. **Static binary translation (SBT)** — ahead-of-time 으로 host ISA 로 변환. 자가-수정 코드 (self-modifying) 에 약함.
3. **Dynamic binary translation (DBT) / JIT** — basic block 단위로 lazy 변환 + cache. QEMU TCG, Box86 의 DynaRec 가 이 방식. interpreter 대비 5-10× 빠름.

### 본 project 적용 path

airgenome-gamebox 의 Phase 1 stage2 (현재 R3_PROGRESS_V12 cycle 113) 는 **interpretation** path 를 채택 — DIY hexa 로 i386 fetch/decode loop. 이유: own1 §A 가 community DBT 코드 (Box86/QEMU TCG) 사용을 금지하며, JIT 은 ARM64 host 의 mprotect RWX 권한 관리 + W^X bypass entitlement 가 필요해 macOS 14+ 에서 sandbox 충돌 위험. wave 15-A 에서 inst 1M+ 도달이 목표 → interpretation 만으로도 USER32 first hit (kind=14) 까지 충분 (R3_ROADMAP v2 cycle 117-118).

향후 (Phase 5 D3D 단계) JIT 도입 검토 가능하나, own1 §A 의 hexa-only mandate 에 따라 self-hosted DBT 를 별도 작성 (community lift 0).

---

## 2. Query 2 — C++ vtable layout i386 calling convention (MSVC RTTI)

### finding

MSVC ABI vtable 의 i386 layout (Microsoft 공식 + reverse-engineering 블로그 종합):

- object 의 첫 dword = **vfptr** (vtable 의 시작 주소).
- vfptr[-4] = **RTTICompleteObjectLocator** pointer (RTTI enabled 시).
- vfptr[0], vfptr[4], vfptr[8] ... = method function pointer 들. i386 에서 thiscall convention — `this` 가 ECX 에 전달.
- multiple inheritance 시 **여러 vfptr** 가 object 내 여러 offset 에 박힘. virtual inheritance 는 추가로 **vbtable** (virtual base table) 사용.
- Itanium ABI (Linux) 와 layout 이 완전히 다름 — RTTI 위치, vbtable 의 offset 표현, name mangling prefix 모두 차이.

### 본 project 적용 path

R3_BSS_CORRUPTION_ANALYSIS §5.2 의 후보 B (vtable entry NULL/garbage) 가능성을 wave 15-A 에서 재평가:

- 본 binary (Battle.net Setup) 의 C++ ctor 가 simple level (locale init / atexit register only) — vtable 호출 자체가 적음 (분석 §5.2 가능성 10-15% 결론).
- 단 RTTI 가 enabled 면 vfptr[-4] 의 COL ptr 가 .rdata 로 향하는데, 본 emulator 는 .rdata 매핑은 OK 이므로 RTTI 자체 corruption 가능성 낮음.
- thiscall 의 ECX 전달 convention — wave 15-A 의 `call ecx` 가 사실 **method 호출 entry 의 정상 prologue** 일 수 있음 (즉 ecx = this, 그 후 vtable 참조). 단 본 case 의 inst[17001] 의 ecx 가 garbage VA 였다는 trace 결과는 후보 D (ROL+XOR 미반영) 에 부합.

**결론**: vtable layout 자체 reference 는 수확 — 단 본 case 의 primary cause 는 vtable 보다 **EncodePointer 의 ROL 누락** (§3 finding 과 결합).

---

## 3. Query 3 — MSVC LTCG inline _initterm partial buffer / static init order fiasco

### finding

`_initterm` 동작 (Microsoft 공식):

- function pointer table 을 walk — start ptr 부터 end ptr 까지 sequential.
- NULL entry 는 **skip** (continue), non-NULL entry 는 호출.
- exception 발생 시 `std::terminate()` 호출.

LTCG (link-time code generation) 의 영향:

- LTCG 는 `mainCRTStartup` 을 redefine 하면 user code 가 global ctor 전에 실행되는 *unpredictable behavior* 발생 가능 (Microsoft 공식 경고).
- LTCG 는 또한 `_initterm` 자체를 inline 화 가능 — 이 경우 binary trace 에 `_initterm` symbol 이 사라지고 indirect call loop 만 남음.

### 본 project 적용 path

R3_BSS_CORRUPTION_ANALYSIS §2 의 핵심 가설 (BSS 에 위치한 atexit chain / TLS callback array 의 partial init) 을 보강:

1. 본 emulator 는 BSS 매핑 시 zero-fill 만 수행 (CRT init 은 내부 _initterm walker 가 채움).
2. wave 15-A 의 `call ecx` garbage VA 는 _initterm walker 가 NULL skip 정상 동작했으나, **LTCG inline 으로 인해 NULL 검사 자체가 없는 path 로 fallthrough** 한 가능성.
3. 대안: shim 의 `__scrt_initialize_onexit_tables` + `__scrt_dllmain_before_initialize` 가 VS17.7+ 에서 mandatory (이전엔 optional). 본 emulator 의 stub table 에 두 함수 명시적 추가 검토.

**wave 15-A action item**: hexa 측 fake_proc table 에 `_initterm`, `_initterm_e`, `__scrt_initialize_onexit_tables` 의 NULL-skip semantics 보장 stub 추가 (4-byte `83 c4 NN c3` 보다 구조화된 "iter+skip" stub).

---

## 4. Query 4 — PE loader graceful skip mem_fault / binary translation NULL deref recovery

### finding

**Recovery shepherding** (학술 논문 — MIT/PLDI):
- compile 된 binary 에 JIT instrumentation (Pin) 으로 NULL deref / div-by-zero 같은 fatal error 를 lightweight recover.
- 핵심 idea: fault site 직후 instruction 을 skip 하거나 dummy zero 반환 후 progress 시도.

**radare2 의 NULL ptr deref 사례** (CVE):
- bin_dyldcache.c 의 load() 가 invalid file 에 segfault — fix 는 dereference 전 NULL check 추가.

**malisal/loaders, abhisek/Pe-Loader-Sample**:
- in-memory PE loader 들은 invalid section header 를 graceful skip 하지 않고 abort 가 default (안전성 위해).

### 본 project 적용 path

wave 15-A vtable corruption recovery 의 **graceful skip** 정책 결정 근거:

1. **recovery shepherding 의 skip-and-continue** 패턴을 own-built emulator 에 적용 — `call ecx` 의 ecx 가 매핑되지 않은 VA 면 (a) 4-byte ret stub (0xC3) 을 emit 후 거기로 jump 하거나 (b) 다음 instruction (call 의 다음 주소) 으로 skip + return value 를 0 으로 강제.
2. R3_PROGRESS_V11 의 sentinel-FF detector 와 결합 — FF 패턴 detect 시 즉시 skip path 진입.
3. 이 정책은 *생산성 (USER32 first hit 까지 진척)* 우선이며, 정확성보다 progress 를 선택하는 trade-off. 본 phase 의 목표 (closure 96.5% → 99%) 에 부합.

---

## 5. Query 5 — polling spin loop break in DBT timing emulation

### finding

DBT 에서 spin-loop 처리 (학술 논문 + emulator 작성 howto 종합):

- spin-loop 은 emul time 을 real time 에 맞추는 *cycle-balancing* mechanism 으로 사용 가능.
- 최적화: **backward branch** 시점에만 event check insertion — forward straight-line code 는 거의 없으므로 backward branch 가 spin 검출 trigger.
- timing side-channel: QEMU/HQEMU 가 non-constant-time 변환 코드 삽입으로 leak 발생 (보안 관점).

### 본 project 적용 path

R3_PROGRESS_V12 의 **inst 16M (max_iter spin)** 문제 직접 연관:

1. 본 emulator 의 fetch/decode loop 가 backward branch (jmp short -2 형태의 `EB FE` 자기-루프) 에서 spin 발생 가능 → 16M instruction 도달 후 max_iter cap 으로 abort.
2. wave 14-A 의 byte-scan FF 보강이 "natural halt 456,790" 으로 진척 — 즉 spin 진입 전에 sentinel 검출로 break.
3. wave 15-A 에서 추가 spin-detector: **same EIP 의 N-회 재방문** (e.g. N=64) 시 즉시 break + emit `spin_detected` event. backward-branch 검출보다 가벼움.

---

## 6. Query 6 — QEMU TCG i386 user mode emulation

### finding

QEMU linux-user mode (qemu-i386):
- TCG (Tiny Code Generator) 가 host ISA 로 translation block (TB) 단위 변환 + cache.
- syscall 은 host Linux syscall 로 generic translation.
- thread structure mirror — clone() 은 host clone() 으로 구현.

### 본 project 적용 path

**reference only** — own1 §A § rule e 가 QEMU 사용을 명시 금지 (Hangover/Box64/FEX 와 같은 community PE 호환 layer 와 동급).

QEMU 의 *접근 방식* (TB cache, syscall mirror) 은 흥미로우나 architectural pattern level 에서만 참고. 코드 lift 0. 향후 hexa 에서 자체 TB cache 작성 시 own-built 보장 필수.

---

## 7. Query 7 — ARM64 macOS i386 emulation / Rosetta 2

### finding

- Rosetta 2 는 **x86-64 만 지원** — i386 (32-bit) 미지원. 과거에 일부 32-bit 지원이 있었으나 deprecated.
- macOS Catalina 10.15+ 자체가 32-bit app deprecated.
- workaround: x64 의 **compatibility mode** + macOS `i386_set_ldt` (segment descriptor table entry 생성) 로 32-bit Windows binary 를 Rosetta 위에서 실행시키는 hack 가능 (neugierig.org 블로그).

### 본 project 적용 path

own1 §A § rule d 는 Rosetta 2 사용을 **허용** (instruction stream 변환 layer 로). 단 i386 미지원이 결정적 제약:

1. **Phase 1 의 binary 가 i386 (PE32) vs x86_64 (PE32+) 인지 즉시 분기**. PE32+ 인 D2R/D4 는 Rosetta 2 위에서 실행 가능 (단 Win32 shim 은 여전히 hexa 자체 작성).
2. 본 case 의 Battle.net Setup 이 i386 이면 → Rosetta 2 fallback 불가 → DIY hexa interpreter (Phase 1 stage2) 가 *유일* path. `i386_set_ldt` hack 은 own1 §A 의 community workaround 금지 항목으로 분류 (neugierig.org 글이 community origin).
3. R3_PROGRESS_V12 의 native i386 emulator path 는 **본 finding 으로 정당화** — Rosetta 2 i386 부재 → 자체 작성 외 대안 0.

---

## 8. Query 8 — binary translation fake_proc stub stdcall / DBT IAT thunk

### finding

IAT (Import Address Table):
- DLL ref 를 dictionary 형태로 보관 — key = library name, value = function pointer thunk.
- caller 의 `call` 은 thunk 주소 indirect — loader 가 IAT 에 실제 주소 채움.

DBT 에서 fake_proc:
- taviso/loadlibrary 의 stub 패턴 — symbol 미해결 시 debug break 트리거 함수로 대체.
- QEMU 의 thunk — endian/32-64 mismatch 처리용 추가 계산 injection.

### 본 project 적용 path

본 emulator 의 fake_proc stub (`83 c4 NN c3` = `add esp, NN; ret`) 분석:

1. R3_BSS_CORRUPTION_ANALYSIS §5.3 의 후보 C (stdcall arg_count 부정확 → stack 어긋남) 가능성 30-40%.
2. wave 15-A 에서 **arg_count table 검증 강화** — Microsoft 공식 docs 의 함수 prototype parsing 자동화. 특히 less-documented MSVCR internal (e.g. `__crtFlsAlloc`, `_lock`, `_unlock`) 의 stdcall arg 수 정확화.
3. taviso 의 debug-break stub 패턴은 own1 §A § rule b 위반 (community) — 단 *접근 방식* 은 reference 가능. hexa 자체 작성 한 stub 에 동등한 "missing symbol → controlled trap" 의무화.

---

## 9. Query 9 — MSVC FlsAlloc TLS i386 fiber local storage

### finding

- FLS (Fiber Local Storage) — Windows XP+ 도입. TLS 와 동일 semantic 이나 fiber 단위 + SwitchToFiber 시 swap.
- MSVC CRT 의 `__acrt_initialize_ptd` 가 `__acrt_FlsAlloc` 호출 → 이후 모든 thread 가 per-thread data 저장.
- C runtime 이 exception handling 에 TLS 헤비 사용 → FLS 도입 동기.
- /GT 컴파일 옵션 = `__declspec(thread)` 의 fiber-safe 보장.

### 본 project 적용 path

본 emulator 의 TLS/FLS shim 작성 우선순위:

1. **wave 15-A 의 inst[17001] garbage VA 가 FLS 미초기화 결과일 가능성** — `__acrt_initialize_ptd` stub 가 zero 반환만 하면 이후 `_lock`/exception handler 가 NULL TLS slot 참조.
2. R3_BSS_CORRUPTION_ANALYSIS §2 의 BSS 매핑 직후 _initterm 첫 함수가 `__scrt_initialize_default_local_stdio_options` 같은 TLS-touching path 면 거기서 garbage 시작.
3. **action**: lib/loader/win32_*.hexa 에 `FlsAlloc` / `FlsGetValue` / `FlsSetValue` / `TlsAlloc` 의 minimal in-memory dict 구현 추가 (per-thread map). own1 §A § rule b 준수 — Microsoft docs 만 reference.

---

## 10. Query 10 — Windows COM CoCreateInstance vtable dispatch

### finding

- COM `CoCreateInstance` → CLSID 로 class object 연결 → default-init instance 생성 → IID interface 반환.
- 반환 ppv = vtable pointer. vtable[0..N] = interface method pointer.
- IDispatch (COM Automation) — vtable 또는 dispatch ID 둘 다 호출 가능. dual interface 권장.

### 본 project 적용 path

본 phase 에선 **deferred** — Battle.net Setup 의 i386 path 가 COM 호출까지 도달하기 전 (USER32 first hit 직후) 의 단계. Phase 8 (Battle.net 우회) 또는 Phase 5 (DirectX → D3DMetal) 에서 COM dispatch 필요.

향후 **D3DMetal 호출 path** 에서 COM vtable dispatch 직접 emulation 필수 — `IDXGIFactory::CreateSwapChain` 등의 vtable[N] 호출. own1 §A § rule c 의 D3DMetal Apple binary 는 native call 가능, 그 사이의 vtable thunk 만 자체 작성.

---

## 11. Query 11 (보강) — EncodePointer / DecodePointer ROL XOR cookie

### finding

Microsoft 공식 + Old New Thing (Raymond Chen) + reverse-engineering 글 종합:

- Windows XP 도입.
- 알고리즘: per-process secret cookie 와의 XOR. **Windows 8+ 에선 ROL 추가** — `(ROL(ptr, cookie & 0x1F) ^ cookie)`.
- EncodeSystemPointer = per-machine salt (ASLR 보강).
- C runtime 의 atexit / exception handler chain 이 광범위하게 사용.

### 본 project 적용 path

R3_BSS_CORRUPTION_ANALYSIS §5.4 의 후보 D (ROL 미반영) 가 가장 유력 (40-50%):

1. wave 9-A 의 simple XOR 만 구현된 DecodePointer stub → caller 가 ROL+XOR encode 한 ptr 를 단순 XOR 만 decode → ROL 미복원 → garbage VA.
2. **wave 15-A 의 핵심 fix**: hexa 측 DecodePointer 를 `ROR(input ^ cookie, cookie & 0x1F)` 로 보강 (ROL 의 inverse 는 ROR). cookie = per-process random 32-bit (test 환경에선 deterministic 0xDEADBEEF 같은 값 사용 후 production 에선 `arc4random`).
3. EncodePointer 도 동일하게 `ROL(ptr, cookie & 0x1F) ^ cookie` 로 update — symmetric 보장.

이 update 만으로 후보 D 해결 → R3_PROGRESS_V12 의 closure 96.5% → 98.5% 도달 추정. 후보 A (sentinel mismatch) 는 wave 14-A 에서 이미 처리됨.

---

## 12. Query 12 (보강) — x86 indirect call ECX vtable corruption recovery

### finding

- `mov ecx, [eax]; call [ecx+0xc]` 패턴 — vtable 의 4번째 method (offset 0xc) 호출.
- GCC VTV (vtable verification) — class hierarchy + vtable ptr 의 valid set 을 build 후 runtime 검증.
- Google patent 8745741 — vtable ptr corruption detection (compile-time instrument).
- NoVT (Bauer 2021 EuroSP) — virtual call 자체를 직접 함수 호출로 변환해 vtable hijack 제거.

### 본 project 적용 path

wave 15-A 의 vtable corruption recovery 알고리즘 (자체 작성 — community lift 0):

1. **valid VA range 검증**: `call ecx` 직전 ecx 값이 mapped section (`.text` / `.rdata`) 범위 내 인지 확인. 미범위 시 graceful skip.
2. **sentinel 검사**: ecx 의 첫 4-byte 가 0xFF 또는 0xCC pattern (uninit / int3) 이면 corrupted 로 판정.
3. **emit telemetry**: `vtable_corruption_recovered` event 로 EIP, ecx, sentinel 종류 기록 — 향후 audit/regression 가능 (own2 § rule a-e 준수).
4. **decode-and-retry** (선택적): ecx 를 `ROR(ecx ^ cookie, cookie & 0x1F)` 로 한 번 decode 시도 — encoded ptr 일 가능성 대비.

---

## 13. 본 project 의 wave 15-A 적용 종합 path

위 12 query 의 finding 을 wave 15-A 의 vtable corruption recovery 알고리즘에 적용:

```
[hexa pseudo, NOT lifted from any source]

fn dispatch_indirect_call(eip: int, target_va: int) -> int:
    # query 4 + query 12 — valid range + sentinel
    if not is_mapped(target_va):
        emit("vtable_skip", "eip", eip, "reason", "unmapped")
        return advance_eip(eip)  # graceful skip

    # query 11 — DecodePointer ROL+XOR retry
    if looks_encoded(target_va):
        decoded = ror(target_va xor cookie, cookie and 0x1F)
        if is_mapped(decoded):
            emit("vtable_decode_recovered", "eip", eip, "from", target_va, "to", decoded)
            target_va = decoded

    # query 5 — spin detection
    if revisit_count(eip) > 64:
        emit("spin_detected", "eip", eip)
        return halt_with_progress()

    return execute(target_va)
```

**기대 성과**:
- R3_PROGRESS_V12 의 closure 96.5% → 98-99% (wave 15-A 후 V13 작성 시점).
- USER32 first hit (kind=14) 도달 trigger 가 query 11 (ROL+XOR) 의 fix 로 해소될 추정.
- own1 §A § rule b 100% 준수 — 본 hexa pseudo 는 *어떤* 외부 코드도 lift 하지 않음. 알고리즘은 Microsoft EncodePointer docs + 본 project 의 R3_BSS_CORRUPTION_ANALYSIS 분석에서 자체 도출.

---

## 14. own1 §A enforcement (citation only — 100% 정합 확인)

본 research 의 reference 종류 별 own1 §A 검증:

| reference | type | own1 §A 적용 |
|-----------|------|--------------|
| learn.microsoft.com (PE/COFF, _initterm, EncodePointer, FlsAlloc, CoCreateInstance) | Microsoft 공식 docs | rule f 허용 (spec 참조 only) |
| Old New Thing (Raymond Chen) | Microsoft 공식 블로그 | rule f 허용 |
| Wikipedia (ISS, binary translation, calling conventions) | 일반 백과사전 | reference 허용 (코드 lift X) |
| 학술 논문 (recovery shepherding, NoVT, DBT timing) | 학술 reference | reference 허용 (코드 lift X) |
| neugierig.org (Rosetta 2 i386 hack) | community 블로그 | rule e workaround 식별만 — *사용 X* |
| taviso/loadlibrary | community PE loader | rule e 금지 (Hangover/Box64/FEX 동급) — 접근 방식만 reference, 코드 lift 0 |
| Box86 / QEMU TCG / Hangover | community DBT | rule e 명시 금지 — 코드 lift 0 |
| GCC VTV / NoVT 논문 | 학술 | reference 허용 (코드 lift X) |
| Apple SDK reference | Apple 공식 | rule c 허용 |

**결론**: 본 문서는 own1 §A 의 모든 sub-rule (a-f) 와 100% 정합. 코드 lift = 0 줄. wave 15-A 구현 시 본 finding 을 reference 로만 사용하고, 모든 hexa code 는 lib/loader/*.hexa 에 자체 작성.

---

## 15. follow-up

- **R3_PROGRESS_V12 update** — wave 15-A 의 algorithmic basis 가 본 research 임을 명시.
- **lib/loader/win32_*.hexa** — DecodePointer ROL+XOR 보강, FlsAlloc minimal dict 구현, _initterm NULL-skip semantic 보장 stub.
- **tests/test_vtable_recovery.hexa** — wave 15-A 의 graceful skip + ROR decode-retry + spin detect 의 self_test (own2 § rule b).
- **R3_PROGRESS_V13** — wave 15-A 결과 + 본 research 의 효과 측정 (closure %, USER32 first hit 도달 여부).

---

## 16. Sources

각 query 별 검색 결과 중 직접 인용/참고된 URL:

### query 1 (i386 emul / interpreter)
- https://en.wikipedia.org/wiki/Instruction_set_simulator
- https://learn.microsoft.com/en-us/windows/arm/apps-on-arm-x86-emulation
- https://www.unicorn-engine.org/
- https://github.com/wfeldt/libx86emu (reference only, not used)

### query 2 (vtable / RTTI)
- https://nimrod.blog/posts/what-does-cpp-object-layout-look-like/
- https://blog.rop.la/en/reversing/2022/12/13/identifying-vftables-through-ms-cpp-rtti.html
- https://devblogs.microsoft.com/oldnewthing/20040205-00/?p=40733
- https://www.lukaszlipski.dev/post/rtti-msvc/

### query 3 (LTCG / _initterm)
- https://www.kdab.com/msvc-debugging-the-static-initialization-order/
- https://www.gonwan.com/2014/02/13/msvc-crt-initialization/
- https://learn.microsoft.com/en-us/cpp/build/reference/ltcg-link-time-code-generation
- https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/initterm-initterm-e
- https://learn.microsoft.com/en-us/cpp/c-runtime-library/crt-initialization

### query 4 (graceful skip / NULL deref)
- https://github.com/malisal/loaders (reference only)
- https://github.com/abhisek/Pe-Loader-Sample (reference only)
- https://www.s3.eurecom.fr/~darion/papers/raid2021.pdf

### query 5 (spin loop / DBT timing)
- https://johntortugo.wordpress.com/2012/08/07/an-emulator-writers-howto-for-static-binary-translation/
- https://link.springer.com/chapter/10.1007/978-3-030-41050-6_10

### query 6 (QEMU TCG)
- https://www.qemu.org/docs/master/about/emulation.html
- https://www.qemu.org/docs/master/user/main.html
- https://www.qemu.org/docs/master/devel/multi-thread-tcg.html

### query 7 (Rosetta 2 i386)
- https://neugierig.org/software/blog/2023/08/x86-x64-aarch64.html (workaround 식별만)
- https://github.com/lima-vm/lima/discussions/2215
- https://forums.macrumors.com/threads/wished-apple-was-more-up-front-about-rosettas-limitations.2327132/

### query 8 (fake_proc stub / IAT thunk)
- https://en.wikipedia.org/wiki/Binary_translation
- https://blog.neteril.org/blog/2016/12/23/diverting-functions-windows-iat-patching/
- https://github.com/taviso/loadlibrary (reference only — own1 §A § rule e)

### query 9 (FlsAlloc / TLS)
- https://learn.microsoft.com/en-us/windows/win32/api/fibersapi/nf-fibersapi-flsalloc
- https://learn.microsoft.com/en-us/windows/win32/procthread/fibers
- https://colinfinck.de/posts/that-time-i-had-to-patch-the-universal-crt/

### query 10 (COM CoCreateInstance)
- https://learn.microsoft.com/en-us/windows/win32/api/combaseapi/nf-combaseapi-cocreateinstance
- https://github.com/MicrosoftDocs/win32/blob/docs/desktop-src/LearnWin32/com-coding-practices.md

### query 11 (EncodePointer ROL XOR)
- https://learn.microsoft.com/en-us/previous-versions/bb432254(v=vs.85)
- https://learn.microsoft.com/en-us/previous-versions/bb432242(v=vs.85)
- https://devblogs.microsoft.com/oldnewthing/20201113-00/?p=104447
- https://wiki.sei.cmu.edu/confluence/display/c/WIN04-C.+Consider+encrypting+function+pointers
- https://rentry.co/kernel32-obfuscated-pointers

### query 12 (vtable corruption recovery)
- https://learn.microsoft.com/en-us/windows-hardware/drivers/debugger/annotated-x86-disassembly
- https://gcc.gnu.org/legacy-ml/gcc-patches/2012-11/msg00078.html
- https://publications.cispa.saarland/3364/1/NoVT-for-EuroSP-2021-cr1.pdf
- https://en.wikipedia.org/wiki/X86_calling_conventions

### bonus — atexit / DllMain
- https://learn.microsoft.com/en-us/cpp/build/run-time-library-behavior
- https://ikrima.dev/dev-notes/cpp/crt-init/
- https://github.com/ElliotKillick/operating-system-design-review/blob/main/code/windows/atexit/README.md

---

*written 2026-05-02. own1 §A § rule a-f 100% 정합. R3_PROGRESS_V12 cycle 113 직후 wave 15-A 진입 직전 literature survey. 본 finding 은 알고리즘 방향 결정에만 사용되며 코드 lift = 0 줄.*
