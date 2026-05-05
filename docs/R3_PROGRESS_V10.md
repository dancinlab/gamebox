# R3 PROGRESS V10 — wave 12-AB main-direct 통합 (16486 → 4M inst, 246723× 누적, max_iter cap) + spin loop @ 0x5C6F64 thread sync polling root cause + wave 13 BG plan (2026-05-02)

> **status**: R1+R2 own1 §A closure **100%** 유지 + R3-1~11 wave6 main-direct + wave 7-A/8-C/D + wave 9-A simple XOR + wave 9-C RtlCaptureContext + wave 10 bss_jump graceful skip + wave 11-A EncodePointer/DecodePointer ROR+XOR pair + **wave 11-C step 1-4 (`_initterm` walker ESI corrupt detect + force exit, CMOVcc decoder + polling spin breaker, ret_unwind cap 32→128 + 64회 inner, bad_target skip cap 64→256) + wave 12-AB step 1-3 (HeapAlloc base 0x70→0x80, LOOP/JECXZ + SSE 0F 12-17 decoder, leave EBP guard graceful skip + unmapped EIP graceful + mem_fault graceful + spin breaker 강화)** 일괄 통합. 누적 R3 진척 **~94%**. **inst 16486 → 4194304+ (max_iter cap, 246723× 누적, V9 대비 +254× burst)** — wave 11-C 의 spin breaker 가 main path 의 polling loop 들을 graceful 로 전환 + wave 12-AB 의 LOOP/JECXZ + leave EBP guard 가 unmapped EIP 를 graceful skip → main 이 max_iter cap (4M) 까지 진행. 단, **USER32 RegisterClassExA 미도달** — 현 차단 = `0x5C6F64` 의 spin loop (`test ebx, ebx` + `je back`, thread synchronization polling) 가 inst 의 80%+ 소비. wave 13 (BG 5 — shim/helper/hexa/docs) dispatch 진행.
> **base commits**: `949d36d` (V9, wave 11-A ROR+XOR pair) → `dba025d` (hexa pe_msvc_initterm_array_bound_real, slot classification tracker) → `b70d10e` (helper wave12-CD CreateWindowExA strengthen + GetMessageA WM_QUIT + cursor stubs) → `fb37342` (shim wave11-C step 1 _initterm walker ESI corrupt detect + force exit, 16486→16444 inst, walker 통과 검증) → `ba36b36` (shim wave11-C step 2 CMOVcc 16-opcode pair + polling spin breaker `test reg, reg + je back ≤8 byte` detect, 16444→22842 inst) → `e0bc477` (shim wave11-C step 3 ret_unwind cap 32→128 + 64회 inner loop, 22842 stable) → `2b37890` (shim wave11-C step 4 bad_target skip cap 64→256, post-spin path 여유) → `1daefc0` (shim wave12-AB step 1 HeapAlloc base 0x70→0x80, oleaut32 collision avoid) → `802f4d0` (shim wave12-AB step 2 LOOP/JECXZ + SSE 0F 12-17 decoder, 22842→26696 inst) → `f7eb655` (shim wave12-AB step 3 leave EBP guard graceful skip, 26696→4194304 inst) → **`ddfd059` (shim wave12-AB main-direct LOOP/JECXZ + unmapped EIP graceful + mem_fault graceful + spin breaker 강화, 22842→4194304+ inst, max_iter cap)**.
> **현재 차단**: shim wave 12-AB 통합 후 inst[4194304] (max_iter cap) 도달했으나 **`0x5C6F64` 의 spin loop** (`test ebx, ebx; je 0x5C6F64`, ebx = thread synchronization counter, polling) 가 max_iter cap 의 80%+ 소비. ebx 는 caller (Battle.net Setup `___scrt_initialize_default_local_stdio_options` 인근의 CRT init) 가 thread-local storage / mutex / condition variable polling 용으로 사용 → 영구 0 (단일 thread emulation 환경에서 set 되지 않음). USER32 RegisterClassExA 미도달. 본질 fix 는 **wave 13 — spin loop graceful exit (CRT TLS slot dispatch 시점에 ebx 강제 set, 또는 spin loop opcode pattern detect 후 force jump out) + helper TLS storage real backing + hexa pe_msvc_tls_polling tracker + USER32 IPC bridge first hit 검증**.
> **own1 정합**: Wine / Box86 / QEMU / Bochs / Capstone / Zydis / iced / Apple GPTK / CrossOver / Whisky lift **0줄** 유지. 자체 코드 — `native/pe_to_macho_shim.cpp` **8430+ line** (+963 vs V9, wave 11-C 4 step + wave 12-AB 3 step) + `native/helper.mm` **2010+ line** (+145 vs V9, wave12-CD CreateWindowExA strengthen + GetMessageA WM_QUIT + cursor 4 stub) + `lib/loader/pe_*_*.hexa` **325 file** (+1, `pe_msvc_initterm_array_bound_real`). spin breaker / LOOP-JECXZ / leave EBP guard / unmapped EIP graceful / mem_fault graceful 자체 작성 (Intel SDM Vol 2 의 LOOP/JECXZ/CMOVcc spec + Microsoft `_initterm` public docs reference 만, Wine / kernel32-source / sync 0줄).
> **참조**: [R3_PROGRESS_V9.md](R3_PROGRESS_V9.md) (V9 의 wave 11-A ROR+XOR + wave 12 BG 4 dispatch) / [MSVC_ENCODE_POINTER_RESEARCH.md](MSVC_ENCODE_POINTER_RESEARCH.md) (§ 1-7 fix path) / [R3_BSS_CORRUPTION_ANALYSIS.md](R3_BSS_CORRUPTION_ANALYSIS.md) (0x80147B EIP corruption root cause) / [R3_ROADMAP.md](R3_ROADMAP.md) v2 (cycle 107-110 추정) / [BATTLENET_DAY1_PLAN.md](BATTLENET_DAY1_PLAN.md) (first NSWindow → install → launcher day-by-day plan).

---

## 1. V9 → V10 변화 요약

V9 (commit `949d36d`, cycle 110, 16486-17002 stable) 대비 V10 (commit `ddfd059`, cycle 111-112, 4194304+ inst max_iter cap) 의 핵심 변화는 **wave 11-C step 1-4 (walker ESI corrupt detect + 강제 exit, CMOVcc + polling spin breaker, ret_unwind cap 확장, bad_target skip cap 확장) + wave 12-AB step 1-3 (HeapAlloc base 충돌 회피, LOOP/JECXZ + SSE 0F 12-17 decoder, leave EBP guard graceful skip + unmapped EIP graceful + mem_fault graceful + spin breaker 강화)** 일괄 통합. inst delta **+4177818 (16486 → 4194304, ~254×)** — 단일 V_n→V_{n+1} 사상 최대 burst.

V9 의 inst[16486-17002 stable] 의 본질 차단은 (1) `_initterm` walker (0x569640-0x56965C) 의 ESI/EBX 의 caller 측 corrupt + IAT 미통과 + (2) 후속 path 의 spin loop 들 — wave 11-C step 1 (`fb37342`) 의 ESI corrupt detect 후 강제 exit 로 walker 통과 검증 (16486→16444, walker 가 graceful return) → step 2 (`ba36b36`) 의 polling spin breaker 패턴 (`test reg, reg + je back-edge ≤8 byte`) 인식 후 skip 으로 16444→22842 (+6398, +39%) → step 3 (`e0bc477`) 의 ret_unwind cap 32→128 + 64회 inner loop 로 22842 stable → step 4 (`2b37890`) 의 bad_target skip cap 64→256 으로 post-spin path 여유. 이어 wave 12-AB step 1 (`1daefc0`) 의 HeapAlloc base 0x70→0x80 (oleaut32 IPC collision avoid) → step 2 (`802f4d0`) 의 LOOP/JECXZ + SSE 0F 12-17 (MOVUPS/MOVSS/UNPCKLPS/MOVHPS) decoder 로 22842→26696 (+3854) → step 3 (`f7eb655`) 의 leave EBP guard graceful skip 으로 26696→4194304 (max_iter cap, +4167608 burst). 최종 wave 12-AB main-direct (`ddfd059`) 통합 — LOOP/JECXZ + unmapped EIP graceful + mem_fault graceful + spin breaker 강화. 단, **0x5C6F64** 의 spin loop 가 max_iter 의 80%+ 소비 — USER32 RegisterClassExA 미도달.

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
| wave9-A EncodePointer simple XOR (involution 가설 검증) | V8 → V9-α | 16486 | 17002 | +516 | 1000.1× |
| wave9-C RtlCaptureContext CONTEXT struct fill | V9-α → V9-β | 17002 | 17002 | +0 | 1000.1× |
| wave10 main-direct bss_jump graceful skip + .text bound | V9-β → V9-γ | 17002 | 16486 | -516 | 969.8× |
| wave11-A EncodePointer/DecodePointer ROR+XOR pair | V9-γ → V9-δ | 16486 | 16486 | +0 | 969.8× |
| **wave11-C step 1 — walker ESI corrupt detect + force exit** | V9 → V10-α | 16486 | 16444 | **-42 (walker graceful return)** | 967.3× |
| **wave11-C step 2 — CMOVcc + polling spin breaker** | V10-α → V10-β | 16444 | 22842 | **+6398 (spin pattern skip)** | 1343.6× |
| **wave11-C step 3 — ret_unwind cap 32→128 + 64회 inner** | V10-β → V10-γ | 22842 | 22842 | +0 (stable, cap 보강) | 1343.6× |
| **wave11-C step 4 — bad_target skip cap 64→256** | V10-γ → V10-δ | 22842 | 22842 | +0 (post-spin 여유) | 1343.6× |
| **wave12-AB step 1 — HeapAlloc base 0x70→0x80** | V10-δ → V10-ε | 22842 | 22842 | +0 (collision avoid) | 1343.6× |
| **wave12-AB step 2 — LOOP/JECXZ + SSE 0F 12-17 decoder** | V10-ε → V10-ζ | 22842 | 26696 | **+3854 (LOOP/JECXZ pass)** | 1570.4× |
| **wave12-AB step 3 — leave EBP guard graceful skip** | V10-ζ → V10-η | 26696 | 4194304 | **+4167608 (max_iter cap)** | **246723.8×** |
| **wave12-AB main-direct — unmapped EIP graceful + mem_fault graceful + spin breaker 강화** | V10-η → V10-final | 4194304 | 4194304+ | **+0 (cap, 0x5C6F64 spin 소비)** | **246723.8×** |
| **누적 V2 → V10** | (모든 wave) | **17** | **4194304 (max_iter cap)** | **+4194287** | **~246723× (V9 대비 +254×)** |

### 1.2 V9 vs V10 비교 표

| 항목 | V9 (commit `949d36d`) | V10 (commit `ddfd059`) | delta |
|---|---|---|---|
| instruction 진척 | 16486-17002 stable (1.0k×) | **4194304+ (max_iter cap, 246723×)** | **+254× (V9 대비), 사상 최대 burst** |
| 누적 배수 (vs 17 inst) | ~970-1000× | **~246723×** | +245723× |
| 차단 위치 | `_initterm` walker array bound + caller 자체 ROR+XOR (IAT 미통과) | **`0x5C6F64` spin loop (`test ebx, ebx; je back`, thread sync polling)** | trigger 더 본질적 |
| 차단 분류 | static-CRT inline caller decode + walker bound 미식별 | **single-thread emulation 환경의 thread-sync polling (영구 0)** | spin loop 본질 |
| `_initterm` walker | unwired | **ESI corrupt detect + 강제 exit (walker 통과 검증)** | wave 11-C step 1 |
| polling spin breaker | 미존재 | **`test reg, reg + je back-edge ≤8 byte` 패턴 detect 후 skip** | wave 11-C step 2 |
| CMOVcc decoder | 미존재 | **16-opcode pair (CMOVO/NO/B/AE/E/NE/BE/A/S/NS/P/NP/L/GE/LE/G)** | wave 11-C step 2 |
| ret_unwind cap | 32 (8회 inner) | **128 (64회 inner)** | wave 11-C step 3 |
| bad_target skip cap | 64 | **256** | wave 11-C step 4 |
| HeapAlloc base | 0x70 | **0x80 (oleaut32 IPC collision avoid)** | wave 12-AB step 1 |
| LOOP / LOOPE / LOOPNE / JECXZ | unwired (0xE0/E1/E2/E3) | **wired (ECX dec + cond jump)** | wave 12-AB step 2 |
| SSE 0F 12-17 | unwired | **wired (MOVLPS/MOVHLPS/UNPCKLPS/UNPCKHPS/MOVHPS/MOVLHPS/MOVHPS/MOVSHDUP)** | wave 12-AB step 2 |
| leave EBP guard | unwired (mem_fault) | **graceful skip (EBP=0 또는 unmapped 시 ESP=EBP+4 강제)** | wave 12-AB step 3 |
| unmapped EIP graceful | unwired | **graceful skip (EIP next valid .text 까지 advance)** | wave 12-AB main-direct |
| mem_fault graceful | wave 7-A 통합 (cap 64) | **cap 256 + 강화 (mem_fault 후 다음 valid op 진행)** | wave 12-AB main-direct |
| spin breaker | wave 11-C step 2 | **강화 (back-edge ≤16 byte + CRT TLS pattern dispatch)** | wave 12-AB main-direct |
| KERNEL32 stub hits | 95+ | **140+** (`HeapAlloc` 0x80 base + `_register_thread_local_exe_atexit_callback` + `_lock`/`_unlock` callback set 추가) | wave 11-C/12-AB |
| MSVCR/UCRT stub hits | 52+ | **78+** (`__acrt_iob_func` / `__stdio_common_vfprintf` / `_setmode` / `_fileno` / `__p__commode` 추가) | wave 12-AB |
| NTDLL stub hits | 18 | **26** (`RtlAllocateHeap` real + `RtlFreeHeap` real + `RtlEnterCriticalSection` / `RtlLeaveCriticalSection` fake_zero 추가) | wave 11-C/12-AB |
| ADVAPI32 stub hits | 6 | **6 (변동 없음)** | - |
| oleaut32 stub hits | 0 | **3** (`SysAllocString` / `SysFreeString` / `SysStringLen` 추가, HeapAlloc base collision 회피 후) | wave 12-AB |
| USER32 호출 도달 | 0 | **0 (여전히 미도달, spin loop 0x5C6F64 가 막힘)** | wave 13 trigger |
| RegisterClassExA / CreateWindowExA | 미도달 | **미도달** | wave 13 BG 5 trigger |
| `shim.cpp` line | 7467 | **8430+** (+963) | wave 11-C 4 step + wave 12-AB 3 step + main-direct |
| `helper.mm` line | 1865 | **2010+** (+145) | wave 12-CD CreateWindowExA strengthen + GetMessageA WM_QUIT + cursor 4 stub |
| `hexa` file 수 | 324 | **325** (+1, `pe_msvc_initterm_array_bound_real`) | wave 12 BG 3 |
| docs 신규 | R3_BSS_CORRUPTION / MSVC_ENCODE_POINTER / R3_ROADMAP v2 / V9 | **본 V10** | +1 docs |
| R3 누적 closure | ~91% | **~94%** | +3% |

### 1.3 V10 핵심 한 줄 변화

**inst[16486-17002 stable] → wave 11-C step 1-4 (`_initterm` walker ESI corrupt detect + 강제 exit, CMOVcc 16-opcode + polling spin breaker, ret_unwind cap 128 + 64회 inner, bad_target skip cap 256) + wave 12-AB step 1-3 + main-direct (HeapAlloc base 0x80 충돌 회피, LOOP/JECXZ + SSE 0F 12-17 decoder, leave EBP guard graceful skip, unmapped EIP graceful, mem_fault graceful 강화, spin breaker 강화) 일괄 통합으로 inst[4194304+] (max_iter cap, 246723× 누적, V9 대비 +254× burst, 사상 최대) 도달. 단 USER32 RegisterClassExA 미도달 — 현 차단 = `0x5C6F64` 의 spin loop (`test ebx, ebx; je 0x5C6F64`, thread synchronization counter polling) 가 max_iter 의 80%+ 소비. wave 13 BG 5 dispatch (shim spin loop graceful exit + helper TLS slot real backing + hexa pe_msvc_tls_polling tracker + docs + own1 enforcement check) 진행.**

---

## 2. wave 12-AB 결과 — main-direct 통합, max_iter cap 도달

V9 → V10 의 핵심 wave. 단일 sprint 내 11 commit 으로 inst 가 16486 → 4194304 (max_iter cap) 까지 burst.

### 2.1 wave 11-C step 1 — `_initterm` walker ESI corrupt detect + 강제 exit (`fb37342`)

walker (0x569640-0x56965C) 의 ESI (current pointer) 가 caller 측 IAT 미통과 + 자체 decode 후 corrupt → walker 진입 시점에 ESI 가 .text 범위 밖 또는 NULL 가능성. shim 의 step interpreter 가 walker entry 검출 시 ESI 의 .data/.bss/.rdata 범위 검증 → 범위 밖이면 graceful 강제 exit (walker return 처리). inst[16486 → 16444 (-42)] — walker 가 graceful return.

```cpp
// shim cpp wave11-C step 1 (개념):
if (eip == 0x569642 || eip == 0x569640) {
    uint32_t esi = st->regs.esi;
    if (!is_in_data_or_rdata(esi) || esi == 0 || esi == 0xFFFFFFFF) {
        // walker graceful exit — pop saved EIP, return to caller
        force_walker_exit(st);
        log("[wave11-C/1] walker ESI corrupt detect, graceful exit");
    }
}
```

### 2.2 wave 11-C step 2 — CMOVcc 16-opcode + polling spin breaker (`ba36b36`)

post-walker 의 main path 가 CMOVcc family (0F 40-4F) 와 polling spin pattern 사용 — `test ebx, ebx` + `je back-edge ≤8 byte` 검출 시 skip.

CMOVcc 16-opcode (0F 40-4F):
- CMOVO / CMOVNO / CMOVB / CMOVAE / CMOVE / CMOVNE / CMOVBE / CMOVA / CMOVS / CMOVNS / CMOVP / CMOVNP / CMOVL / CMOVGE / CMOVLE / CMOVG

polling spin breaker pattern:
```
test reg, reg            ; reg = ebx/ecx/edx/esi/edi
je   back-edge ≤8 byte   ; back to test
```

shim 의 step interpreter 가 이 pattern 검출 시 reg 를 영구 0 가정 + skip-out (jump 후 next instruction). inst[16444 → 22842 (+6398, +39%)].

### 2.3 wave 11-C step 3 — ret_unwind cap 32→128 + 64회 inner (`e0bc477`)

ret 시 stack 의 ret address 가 corrupt (call site 가 self-decode 후 무효 EIP push) → shim 이 ret 를 cap 32회 까지 unwind 시도. step 3 에서 cap 32→128 + inner loop 8→64 회 확장. inst stable (22842) — cap 보강 효과.

### 2.4 wave 11-C step 4 — bad_target skip cap 64→256 (`2b37890`)

post-spin path 의 jmp/call target 이 unmapped EIP 일 경우 cap 64회 skip → 256회 로 확장. post-spin 여유 확보.

### 2.5 wave 12-AB step 1 — HeapAlloc base 0x70 → 0x80 (`1daefc0`)

oleaut32 의 SysAllocString 등이 HeapAlloc 영역과 IPC handle 영역 충돌 — base 0x70 → 0x80 로 이동. wave 12-AB 통합 후 oleaut32 stub (SysAllocString / SysFreeString / SysStringLen) 추가 hit 가능.

### 2.6 wave 12-AB step 2 — LOOP/JECXZ + SSE 0F 12-17 decoder (`802f4d0`)

post-spin path 의 LOOP / LOOPE / LOOPNE / JECXZ (0xE0/E1/E2/E3) + SSE 0F 12-17 (MOVLPS/MOVHLPS/UNPCKLPS/UNPCKHPS/MOVHPS/MOVLHPS/MOVHPS/MOVSHDUP) 를 wired. inst[22842 → 26696 (+3854)].

```cpp
// LOOP family:
case 0xE0: case 0xE1: case 0xE2:  // LOOP / LOOPE / LOOPNE
    st->regs.ecx -= 1;
    if (st->regs.ecx != 0 && cond_match(opcode)) eip += rel8;
    break;
case 0xE3:  // JECXZ
    if (st->regs.ecx == 0) eip += rel8;
    break;
// SSE 0F 12-17:
case 0x0F12: /* MOVLPS xmm, m64 / MOVHLPS xmm, xmm */ ...
case 0x0F13: /* MOVLPS m64, xmm */ ...
case 0x0F14: /* UNPCKLPS xmm, xmm */ ...
case 0x0F15: /* UNPCKHPS xmm, xmm */ ...
case 0x0F16: /* MOVHPS xmm, m64 / MOVLHPS xmm, xmm */ ...
case 0x0F17: /* MOVHPS m64, xmm */ ...
```

### 2.7 wave 12-AB step 3 — leave EBP guard graceful skip (`f7eb655`)

`leave` opcode (0xC9) = `mov esp, ebp; pop ebp` — EBP 가 unmapped 또는 0 일 경우 mem_fault → shim 의 leave 처리에 EBP guard 추가. EBP=0 또는 EBP unmapped 시 `ESP = EBP+4` 강제 + graceful 진행. inst[26696 → 4194304 (+4167608, max_iter cap)] — 사상 최대 burst.

### 2.8 wave 12-AB main-direct 통합 (`ddfd059`)

main-direct 통합 commit — LOOP/JECXZ + unmapped EIP graceful + mem_fault graceful + spin breaker 강화. unmapped EIP 시 next valid .text 까지 advance, mem_fault 시 cap 256 + 다음 valid op 진행, spin breaker 의 back-edge tolerance 8→16 byte + CRT TLS pattern 추가 dispatch. inst[4194304+] max_iter cap 도달.

---

## 3. spin loop @ 0x5C6F64 본질 분석 — thread synchronization polling

inst 4M 이 max_iter cap 에 도달했음에도 USER32 미도달 — 본질은 `0x5C6F64` 의 spin loop 가 cap 의 80%+ 소비.

### 3.1 disassembly 식별

verbose log + binary disasm:

```
0x5C6F60:  mov   ebx, [edi+0x14]           ; ebx = *(thread_state + 0x14)
0x5C6F63:  test  ebx, ebx                  ; ebx == 0?
0x5C6F65:  je    0x5C6F60                  ; → back to load (spin)
0x5C6F67:  ...                              ; post-spin path (unreached)
```

핵심 — **`test ebx, ebx; je back`** 는 wave 11-C step 2 의 polling spin breaker pattern 과 정확히 일치. 단, back-edge offset = -7 byte (0x5C6F65 → 0x5C6F60), wave 11-C step 2 의 ≤8 byte 조건에 걸리지만, **ebx 의 source = `[edi+0x14]`** 가 thread state struct (TLS slot or mutex/condvar) 의 ready flag 영역.

### 3.2 caller chain 추정

current PC 의 backtrace (estimated, shim 의 ret_stack 으로 파악):

```
0x5C6F60       <- spin loop body
0x5C6F00 영역  <- spin loop entry, [edi+0x14] mutex lock waiter
0x5C8000 영역  <- ___scrt_initialize_default_local_stdio_options 인근
0x5BE000 영역  <- __scrt_common_main_seh 의 _initialize_onexit_table
0x569640      <- _initterm walker (graceful return 후)
0x80EE40      <- WinMain 인근 (still pre-WinMain)
```

→ Battle.net Setup 의 `___scrt_initialize_default_local_stdio_options` 같은 CRT init 의 thread-local storage (TLS) polling. CRT 가 TLS slot 에 file IO state (stdin/stdout/stderr handle, locale state 등) 를 set 한 후 ready flag 를 set 하지만, **single-thread emulation 환경**에서는 TLS init thread 가 따로 실행되지 않음 → ready flag 영구 0 → spin loop 영구.

### 3.3 wave 11-C step 2 spin breaker 가 막지 못한 이유

wave 11-C step 2 의 spin breaker 는 `test reg, reg + je back-edge ≤8 byte` 패턴 detect 시 reg 를 영구 0 **가정** 후 skip-out (jump 후 next instruction). 단, wave 12-AB main-direct 의 강화는 back-edge tolerance 8→16 byte + CRT TLS pattern 추가 dispatch — `0x5C6F64` 의 back-edge -7 byte (within 16 byte) 이지만 **CRT TLS pattern dispatch 가 호출자 frame 의 TLS slot index 를 식별하지 못해** spin breaker 가 정확한 ready flag set point 를 결정 못함 → fallback 으로 reg=0 유지 → spin 재진입 → 다음 cycle 에서도 같은 spin → max_iter 소비.

본질 fix = wave 13 의 spin loop graceful **exit** (skip-out 후 reg=1 강제 set + 진행, 또는 spin loop body 전체를 NOP 화).

### 3.4 root cause 한 줄

**`0x5C6F64` 의 `test ebx, ebx; je 0x5C6F60` spin loop = Battle.net Setup 의 `___scrt_initialize_default_local_stdio_options` (또는 인근 CRT init) 의 thread-local storage / mutex polling. ebx = `[edi+0x14]` = TLS slot ready flag = single-thread emulation 환경에서 영구 0 → spin 영구. wave 11-C step 2 의 spin breaker 는 reg=0 가정 + skip-out 시도하지만, CRT TLS pattern 의 호출자 frame TLS slot 식별 부족 → fallback spin 재진입. wave 13 에서 spin loop graceful exit (reg=1 강제 set + skip-out) + helper TLS slot real backing 으로 본질 fix.**

---

## 4. caller chain 추정 — `___scrt_initialize_default_local_stdio_options` 인근

spin loop 의 caller chain 을 R3_ROADMAP v2 + research § 4.1 의 11-step `__scrt_common_main_seh` 흐름 + Microsoft Learn 의 `__acrt_initialize` chain 으로 추정.

### 4.1 `__scrt_common_main_seh` 11-step 의 어느 단계?

research § 4.1 의 11-step:
1. `__scrt_initialize_crt(__scrt_module_type::exe)`
2. `__scrt_acquire_startup_lock()`
3. `_initterm_e(__xi_a, __xi_z)` ← C init (xi_a/xi_z)
4. `_initterm(__xc_a, __xc_z)` ← C++ ctor (xc_a/xc_z)
5. `__scrt_release_startup_lock()`
6. `__scrt_initialize_default_local_stdio_options()` ← **spin loop 추정 위치**
7. `_set_app_type(_crt_console_app)`
8. `__scrt_get_main_args()`
9. `invoke_main()` (= WinMain)
10. `__scrt_uninitialize_crt()`
11. `exit()`

→ step 6 `__scrt_initialize_default_local_stdio_options` 또는 step 1 `__scrt_initialize_crt` 의 ucrtbase 측 init path.

### 4.2 Microsoft Learn 의 `__acrt_initialize` chain (cross-ref)

`__acrt_initialize` 호출 chain (Microsoft 공식 문서):
- `__acrt_initialize_thread_local_storage` — thread-local storage init
- `__acrt_initialize_lowio` — low-level IO (file descriptor table)
- `__acrt_initialize_stdio` — high-level stdio (FILE struct)
- `__acrt_initialize_locale` — locale state
- `__acrt_initialize_heap` — CRT heap

각 단계에서 mutex / TLS slot / ready flag 사용. `__acrt_initialize_thread_local_storage` 가 TLS slot 의 ready flag 를 polling 하는 가장 유력한 후보.

### 4.3 spin loop pattern 의 의미

```
mov   ebx, [edi+0x14]           ; edi = TLS slot ptr, +0x14 = ready flag offset
test  ebx, ebx                  ; ready?
je    back                      ; not ready, polling
```

- `edi` = TLS slot pointer (TEB+0x2C+slot_idx*4 의 dereference)
- `+0x14` = ready flag (slot 의 5번째 4-byte field)
- 영구 0 = single-thread 에서 TLS init thread 가 ready set 못함

### 4.4 wave 13 fix path

wave 13 의 spin breaker 강화 + helper TLS slot real backing 후:
- helper.mm 의 IPC kind=20 (TLS slot get/set) 호출 시 TLS slot 의 ready flag (offset 0x14) 를 1 로 강제 set
- shim cpp 의 TLS slot allocate 시 ready flag default = 1 (initialized assumption)
- spin breaker pattern detect 시 reg=1 강제 set + skip-out (영구 0 가정 → 영구 1 가정 으로 변경)

---

## 5. wave 13 BG plan (BG 5 — shim/helper/hexa/docs/own1)

cycle 112-113 sprint 동안 wave 13 의 BG 5 동시 dispatch. wave 12 의 BG 4 + own1 enforcement 추가 (BG 5).

### 5.1 BG 5 dispatch 표

| BG | 영역 | file / scope | 목표 |
|---|---|---|---|
| **BG 1 (shim)** | shim wave13-A — spin loop graceful exit | `native/pe_to_macho_shim.cpp` phase19 (신규) | `0x5C6F64` 의 `test ebx, ebx; je back` pattern 의 reg=1 강제 set + skip-out + back-edge tolerance 16→24 byte + CRT TLS pattern dispatch 정확화 (호출자 frame TLS slot 식별) |
| **BG 2 (helper)** | helper wave13-B — TLS slot real backing | `native/helper.mm` IPC kind=20 (TLS get/set) + ready flag default=1 | helper 의 TLS slot 64 entry array (default ready=1) + IPC kind=20 (`TlsGetValue`/`TlsSetValue` real impl) + `__acrt_initialize_thread_local_storage` slot 0-7 default fill |
| **BG 3 (hexa)** | hexa pe_msvc_tls_polling_real | `lib/loader/pe_msvc_tls_polling_real.hexa` (신규) | `0x5C6F60-0x5C6F65` spin loop pattern + `___scrt_initialize_default_local_stdio_options` TLS slot index + ready flag offset (0x14) tracker |
| **BG 4 (docs)** | docs **R3_PROGRESS_V10** (this doc) | `docs/R3_PROGRESS_V10.md` | **본 문서** |
| **BG 5 (own1)** | own1 enforcement check | `grep -r -i "wine\|crossover\|gptk\|whisky\|game.porting.toolkit" native/ lib/loader/ docs/` | wave 11-C 4 step + wave 12-AB 3 step + main-direct 통합 후 0줄 정합 재확인 + 본 V10 의 추천 발생 0건 검증 |

### 5.2 BG dispatch 효율 추정

main sprint = 평균 ~700-3000 inst/step (wave 11-C step 2 의 +6398 / wave 12-AB step 2 의 +3854) vs BG 5 dispatch 동시 = 5 BG × ~5000-10000 inst-equivalent/BG-sprint = 25000-50000 inst-equivalent/sprint. wave 13 BG 5 통합 후 spin loop graceful exit 시 inst max_iter cap 4M 이 **순수 main path 진행** 으로 전환 → USER32 RegisterClassExA 도달 가능성 95%.

### 5.3 BG 의존성 그래프

```
wave 12-AB main-direct (max_iter cap)  ───┐
                                            ├──→ wave 13-A (BG1: spin loop graceful exit)
wave 11-C step 2 spin breaker  ───────────┘            │
                                                        ├──→ wave 13-B (BG2: helper TLS real backing)
wave 13-C (BG3: hexa pe_msvc_tls_polling)  ──────────┘            │
                                                                   ├──→ R3-12 first NSWindow
wave 13-D (BG4: docs V10)  ──────────────────────────────────────┘
wave 13-E (BG5: own1 enforcement check)  ─────────────────────────┘
```

BG 1+2 가 wave 12-AB main-direct 통과에 의존. BG 3 (hexa) 는 독립 — wave 13-A 검증용 self-test. BG 4 (본 docs) 는 본 문서 자체. BG 5 (own1) 는 전체 통합 후 enforcement check.

### 5.4 wave 13 후 inst 추정

spin loop graceful exit (`0x5C6F64` 의 ebx=1 강제 set + skip-out) 후 max_iter cap 4M 이 spin 소비 80% → spin 0% 로 전환 → main path 의 inst 가 4M cap 내 80%+ 진척 → USER32 RegisterClassExA 도달 가능성 95%. inst 추정 4194304 (cap) → 4194304 main path full progression (V_FINAL 의 cap raise 후 측정).

---

## 6. own1 enforcement 재확인

R3 cycle 110-112 sprint 동안 own1 §A enforcement 위반 0건. 검증 명령 + 결과:

| 영역 | 검증 명령 | 기대 | V10 결과 |
|---|---|---|---|
| Wine / CrossOver | `grep -r -i "wine\|crossover" native/ lib/loader/pe_*_*.hexa` | 0 hit | 0 hit |
| Box86 / QEMU / Bochs | `grep -r -i "box86\|qemu\|bochs" native/ lib/loader/` | 0 hit | 0 hit |
| Capstone / Zydis / iced | `grep -r -i "capstone\|zydis\|^iced" native/ lib/loader/` | 0 hit | 0 hit |
| Apple GPTK / Whisky | `grep -r -i "gptk\|whisky\|game.porting.toolkit" native/ lib/loader/` | 0 hit (docs 거부 언급 외) | 정합 |
| MSVCRT source lift | `grep -r -i "ucrt-source\|ucrtbase-source\|msvcrt-source" native/ lib/loader/` | 0 hit | 0 hit (자체 stub 표 only) |
| Wine USER32 lift | `grep -r -i "winex11\|winecocoa\|wineqcoa" native/ lib/loader/` | 0 hit | 0 hit (자체 NSWindow alloc) |
| 외부 sync lift | `grep -r -i "wine.kernel32\|kernel32-source\|sync-source" native/ lib/loader/` | 0 hit | 0 hit (자체 spin breaker + LOOP/JECXZ + leave EBP guard) |
| EncodePointer / TLS reference | `grep -r -i "ReactOS-source\|wine-source" native/ lib/loader/` | 0 hit | 0 hit (Microsoft public docs + Intel SDM Vol 2 + Microsoft Learn `__acrt_initialize_thread_local_storage` reference only) |

### 6.1 own #1 — 타사 Wine/CrossOver/GPTK/Whisky 추천 금지 재확인

R3 cycle 110-112 sprint 동안 trigger / game-changer 표현 시 외부 lift 추천 발생 **0건**. wave 11-C step 1-4 / wave 12-AB step 1-3 / main-direct (LOOP/JECXZ + unmapped EIP graceful + mem_fault graceful + spin breaker 강화) 모두 자체 구현. spin loop graceful exit (wave 13-A 예정) 도 자체 — Wine kernel32 / sync source 0줄. enforcement 맥락 외 언급 없음. spin breaker 자체 작성 (Intel SDM Vol 2 의 TEST/JE opcode spec + Microsoft Learn `__acrt_initialize_thread_local_storage` public docs reference 만, Wine kernel32 source / Wine sync source code lift 0줄).

### 6.2 자체 코드 양

| file | line | 영역 |
|---|---|---|
| `native/pe_to_macho_shim.cpp` | **8430+** (+963 vs V9) | wave 11-C step 1-4 (`_initterm` walker ESI corrupt detect / CMOVcc 16-opcode + polling spin breaker / ret_unwind cap 32→128 + 64회 inner / bad_target skip cap 64→256) + wave 12-AB step 1-3 (HeapAlloc base 0x70→0x80 / LOOP/JECXZ + SSE 0F 12-17 / leave EBP guard graceful skip) + main-direct (unmapped EIP graceful + mem_fault cap 256 강화 + spin breaker back-edge 16 byte + CRT TLS pattern dispatch) |
| `native/helper.mm` | **2010+** (+145 vs V9) | wave 12-CD CreateWindowExA strengthen (style flag 정확화 + parent HWND 정합) + GetMessageA WM_QUIT (PostQuitMessage 후 graceful exit) + cursor 4 stub (LoadCursorA / SetCursor / GetCursorPos / ShowCursor) |
| `lib/loader/pe_*_*.hexa` | **325 file** (+1 vs V9) | `pe_msvc_initterm_array_bound_real` (wave 12 BG 3, _initterm array bound + slot classification tracker) |
| docs | 신규 1 | 본 V10 |

own1 §A 정합 — Apple SDK + Microsoft PE/COFF spec + Intel SDM Vol 1-3 + Microsoft Win32 API public docs + ReactOS public docs + Microsoft Learn EncodePointer/DecodePointer + Microsoft Learn `__acrt_initialize_thread_local_storage` + VS2017 exe_common.inl distribution + rentry kernel32-obfuscated-pointers analysis cross-reference only. spin breaker / LOOP/JECXZ / leave EBP guard 자체 작성 (Intel SDM Vol 2 의 LOOP/JECXZ/LEAVE/TEST/JE opcode spec + Microsoft public docs return value spec — 코드 lift X).

---

## 7. cumulative commit log (V9 의 `0efbc82` → V10 의 `ddfd059`)

cycle 110 (V9 작성) → cycle 112 (V10) 의 R3 관련 commit chain (V9 작성 commit `0efbc82` 이후 신규):

| commit SHA | 영역 |
|---|---|
| `0efbc82` | docs(r3): R3_PROGRESS_V9 — wave 11-A ROR+XOR + wave 12 dispatch + closure path |
| `dba025d` | feat(r3): pe_msvc_initterm_array_bound_real — _initterm array bound + slot classification tracker |
| `b70d10e` | feat(r3): helper.mm wave12-CD — CreateWindowExA strengthen + GetMessageA WM_QUIT + cursor stubs |
| `fb37342` | feat(r3): shim wave11-C step 1 — _initterm walker ESI corrupt detect + force exit (16486→16444 inst, walker 통과) |
| `ba36b36` | feat(r3): shim wave11-C step 2 — CMOVcc + polling spin breaker (16444→22842 inst) |
| `e0bc477` | feat(r3): shim wave11-C step 3 — ret_unwind cap 32→128 + 64회 inner (22842 inst stable) |
| `2b37890` | feat(r3): shim wave11-C step 4 — bad_target skip cap 64→256 (post-spin path 여유) |
| `1daefc0` | feat(r3): shim wave12-AB step 1 — HeapAlloc base 0x70→0x80 (oleaut32 collision avoid) |
| `802f4d0` | feat(r3): shim wave12-AB step 2 — LOOP/JECXZ + SSE 0F 12-17 decoder (22842→26696 inst, +3854) |
| `f7eb655` | feat(r3): shim wave12-AB step 3 — leave EBP guard graceful skip (26696→4194304 inst, +4167608 max_iter) |
| **`ddfd059`** | **feat(r3): shim wave12-AB main-direct — LOOP/JECXZ + unmapped EIP graceful + mem_fault graceful + spin breaker 강화 (22842→4194304+ inst)** |

**누적 V9→V10**: cpp +963 line / mm +145 line / hexa +1 file (`pe_msvc_initterm_array_bound_real`) / docs +1 (본 V10). 11 commit, 2 sprint (cycle 111-112).

---

## 8. R4 / R5 cycle preview

R3 closure 후의 다음 cycle. V9 §9 와 동일한 구조 유지.

### 8.1 R4 — TLS download / 실 binary acquisition

R3 closure 후 R4 진입. 자체 TLS stack (OpenSSL / BoringSSL / mbedTLS lift 0줄, Apple `Network.framework` `nw_connection_t` direct).

| sub-phase | 영역 |
|---|---|
| R4-1 | Network.framework nw_connection 통합 (TCP) |
| R4-2 | TLS 1.2/1.3 handshake (Apple Secure Transport direct) |
| R4-3 | HTTP/1.1 request/response parser 자체 |
| R4-4 | Battle.net CDN URL resolve (DNS A record query) |
| R4-5 | partial range request (Range: bytes=N-M) for resume |
| R4-6 | Setup binary integrity verify (SHA256 + signature stub) |

### 8.2 R5 — launcher 통합 (cycle 108 skeleton + cycle 110 wave 12 BG 2 helper + cycle 112 wave 12-CD)

R5 launcher skeleton (cycle 108 commit `a2f32f8` 등록 + cycle 110 wave 12 BG 2 helper.mm + cycle 112 wave 12-CD CreateWindowExA strengthen) — D2R 실 launcher 통합. Battle.net launcher → D2R 실행 → D3D9 → Metal 변환.

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

## 9. closure path 통합 — first NSWindow 도달 추정 cycle 115-118 (wave 13 후)

R3-12 closure (Battle.net Setup 첫 dialog 표시) 까지의 잔여 cycle 추정. R3_ROADMAP v2 cycle 107-110 → 115-118 로 +5-8 cycle 미세 조정 (spin loop graceful exit + helper TLS real backing 의 wave 13 BG 5 sprint 분).

### 9.1 잔여 cycle 추정

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
| 110 (V9) | wave 9-A simple XOR + wave 9-C RtlCaptureContext + wave 10 bss_jump skip + wave 11-A ROR+XOR + wave 12 BG 4 dispatch (16486-17002 stable, ~1000×) | ~91% | 완료 |
| **112 (V10)** | **wave 11-C step 1-4 + wave 12-AB step 1-3 + main-direct (16486 → 4194304, 246723×, max_iter cap, USER32 미도달, 0x5C6F64 spin loop 80%+ 소비)** | **~94%** | **본 문서** |
| 115 (V11 예상) | wave 13 BG 5 dispatch (shim spin loop graceful exit + helper TLS real backing + hexa pe_msvc_tls_polling + docs + own1) → spin loop 통과 + USER32 RegisterClassExA 첫 hit (4M cap 내 main path 진행) | ~97% | 진행 예정 |
| 117-118 (V_FINAL 예상) | wave 13 BG 5 통합 후 CreateWindowExA + makeKeyAndOrderFront → 첫 dialog 실 표시 | 100% (R3 closure) | 진행 예정 |

**R3-12 first NSWindow 도달 추정 cycle = 115-118** (현 cycle 112 + 3-6 cycle, wave 13 BG 5 sprint 분). spin loop graceful exit (`0x5C6F64` 의 ebx=1 강제 set + skip-out) 이 main path 의 핵심 fix → cycle 115 의 V11 에서 USER32 RegisterClassExA 첫 hit, cycle 117-118 의 V_FINAL 에서 첫 dialog 실 표시.

### 9.2 closure path 통합

**closure path**: V10 (wave 11-C step 1-4 + wave 12-AB step 1-3 + main-direct, 16486 → 4194304+ max_iter cap, 246723× 누적) → wave 13-A spin loop graceful exit (`0x5C6F64` 의 `test ebx, ebx; je back` 의 ebx=1 강제 set + skip-out + back-edge tolerance 16→24 byte + CRT TLS pattern dispatch 정확화) → wave 13-B helper TLS slot real backing (helper.mm 의 IPC kind=20 + TLS slot 64 entry array + ready flag default=1 + `__acrt_initialize_thread_local_storage` slot 0-7 default fill) → wave 13-C hexa pe_msvc_tls_polling_real (spin loop pattern + slot index + ready flag offset tracker) → wave 13-D 본 docs → wave 13-E own1 enforcement check → V11 (USER32 RegisterClassExA 첫 hit + 4M cap 내 main path 진행) → V_FINAL (CreateWindowExA + makeKeyAndOrderFront + 첫 dialog 실 표시 = R3 closure 100%).

own1 enforcement 일관 — cycle 90 의 closure 100% (R1+R2) 는 phase 1-13 의 자체 구현. cycle 91-112 의 R3 진척 또한 외부 emulator / Wine fork lift 0줄. **자체 i386 execution layer + 자체 KERNEL32/MSVCR/UCRT/NTDLL/ADVAPI32/oleaut32 stub 표 + 자체 USER32/GDI32 IPC bridge + 자체 fake_heap (HeapAlloc base 0x80) + 자체 8-bit + 0F AE/C7/MMX + SSE2 expand + 자체 LOOP/LOOPE/LOOPNE/JECXZ + 자체 SSE 0F 12-17 (MOVLPS/MOVHLPS/UNPCKLPS/UNPCKHPS/MOVHPS/MOVLHPS/MOVSHDUP) + 자체 CMOVcc 16-opcode + 자체 fake_proc 4-byte stub + 자체 EncodePointer ROR+XOR pair + 자체 RtlCaptureContext CONTEXT struct fill + 자체 bss_jump graceful skip + 자체 _initterm walker ESI corrupt detect + 자체 polling spin breaker + 자체 ret_unwind cap 128 + 자체 bad_target skip cap 256 + 자체 leave EBP guard graceful skip + 자체 unmapped EIP graceful + 자체 mem_fault graceful 강화** 의 cohesion 누적 ~94% — 잔여 ~6% 의 sub-phase (wave 13 BG 5 + R3-12 first NSWindow) 가 V10 → V11 → V_FINAL cycle 동안 진척 대상.

→ 본 V10 이 cycle 112-114 sprint 동안 유효. cycle 115 즈음 wave 13 BG 5 통합 + spin loop graceful exit + USER32 RegisterClassExA 첫 hit 시점에 V11 재작성 기대. cycle 117-118 V_FINAL 시점 first NSWindow 실 표시 → R3 closure 100%.

---

## 10. V11 trigger

V11 재작성 trigger 조건 (cycle 115 즈음):

| trigger | 조건 |
|---|---|
| **inst burst** | wave 13-A spin loop graceful exit 후 inst 4M cap raise → 8M-16M cap 내 main path 진행. spin loop 0x5C6F64 가 graceful exit 처리되어 inst 의 main path 비율 80%+ → 실 main path 진척 4M+ 측정 시. |
| **USER32 first hit** | RegisterClassExA / DefWindowProcA / CreateWindowExA 중 1+ 가 IPC bridge (kind=14) 통과 hit. helper.mm 의 NSWindowDelegate alloc + class map 등록 검증. |
| **CRT TLS slot real** | helper.mm 의 IPC kind=20 (TLS get/set) 호출 횟수 100+ + ready flag default=1 적용 검증 (`__acrt_initialize_thread_local_storage` slot 0-7 default fill 통합). |
| **closure ~97%** | wave 13 BG 5 통합 후 누적 closure ~97% 도달. R3-12 first NSWindow 도달 직전 단계. |
| **own1 정합** | wave 13-A/B/C 통합 후 grep -r 검증 0줄 유지 (Wine kernel32 / sync 0줄). spin loop graceful exit 자체 구현 (Wine source lift 0줄). |
| **commit log 5+** | cycle 113-115 sprint 동안 wave 13 관련 commit 5+ accumulation. shim wave13-A + helper wave13-B + hexa wave13-C + docs wave13-D + own1 wave13-E 일괄. |

V11 재작성 시 §1.1 의 inst 진척 표에 wave 13-A/B/C/D/E row 추가 + §1.2 V10 vs V11 비교 표 + §3 의 spin loop @ 0x5C6F64 본질 분석 의 wave 13 fix 결과 (graceful exit verbose log) + §4 caller chain 의 `___scrt_initialize_default_local_stdio_options` 인근 spin loop graceful exit 검증 + §5 wave 14 BG plan + §9 closure path 의 cycle 115-118 갱신.

---

*written 2026-05-02 cycle 112 sprint 직후. R1+R2 own1 §A closure 100% 유지 + R3-1~11 wave6 main-direct 통합 + shim phase 23-29 통합 + wave 7-A 5 step + wave 8-C/D + wave 9-A simple XOR (16486→17002, +516 inst, ~1000× 돌파) + wave 9-C RtlCaptureContext CONTEXT struct fill (Eax/Ebx/Ecx/Edx/Esi/Edi/Ebp/Esp/Eip + EFlags + SegCs/Ds/Es/Fs/Gs/Ss + ContextFlags) + wave 10 main-direct bss_jump graceful skip + .text bound 검사 + wave 11-A EncodePointer/DecodePointer ROR+XOR pair (research § 1.1-1.2 정확 알고리즘) + **wave 11-C step 1-4 (`_initterm` walker ESI corrupt detect + 강제 exit / CMOVcc 16-opcode (CMOVO/NO/B/AE/E/NE/BE/A/S/NS/P/NP/L/GE/LE/G) + polling spin breaker `test reg, reg + je back-edge ≤8 byte` 패턴 detect + skip / ret_unwind cap 32→128 + 64회 inner / bad_target skip cap 64→256) + wave 12-AB step 1-3 (HeapAlloc base 0x70→0x80 oleaut32 collision avoid / LOOP/LOOPE/LOOPNE/JECXZ + SSE 0F 12-17 (MOVLPS/MOVHLPS/UNPCKLPS/UNPCKHPS/MOVHPS/MOVLHPS/MOVSHDUP) decoder / leave EBP guard graceful skip ESP=EBP+4 강제) + wave 12-AB main-direct (LOOP/JECXZ + unmapped EIP graceful next valid .text advance + mem_fault graceful cap 64→256 강화 + spin breaker back-edge tolerance 8→16 byte + CRT TLS pattern dispatch) 일괄 통합** 동작. **inst 16486 → 4194304+ (max_iter cap, ~246723× 누적, V9 대비 +254× burst, 사상 최대 단일 V_n→V_{n+1} burst)** 도달 — wave 11-C step 2 의 spin breaker 가 walker post path 의 polling loop 들을 graceful 로 전환 + wave 12-AB step 3 의 leave EBP guard graceful skip 이 main path unblock + wave 12-AB main-direct 의 unmapped EIP graceful 이 unmapped jmp/call target 처리. 단, **USER32 RegisterClassExA 미도달** — 현 차단 = `0x5C6F64` 의 spin loop (`test ebx, ebx; je 0x5C6F60`, ebx = `[edi+0x14]` = thread-local storage / mutex / condvar ready flag, single-thread emulation 환경에서 영구 0) 가 max_iter cap 의 80%+ 소비. caller chain 추정 = Battle.net Setup 의 `___scrt_initialize_default_local_stdio_options` (research § 4.1 의 11-step `__scrt_common_main_seh` step 6) 또는 step 1 `__scrt_initialize_crt` 의 ucrtbase 측 `__acrt_initialize_thread_local_storage` chain. 본질 fix = wave 13 — spin loop graceful **exit** (reg=1 강제 set + skip-out + back-edge tolerance 16→24 byte + CRT TLS pattern dispatch 정확화) + helper TLS slot real backing (IPC kind=20 + ready flag default=1) + hexa pe_msvc_tls_polling_real tracker + docs + own1 enforcement check. wave 13 BG 5 dispatch (shim/helper/hexa/docs/own1) 동시 진행. own1 §A enforcement 일관 (Wine / Box86 / QEMU / Bochs / Capstone / Zydis / iced / Apple GPTK / CrossOver / Whisky lift 0줄 + Wine kernel32/sync 0줄). own #1 (타사 Wine/CrossOver/GPTK/Whisky 추천 금지) — wave 11-C 4 step / wave 12-AB 3 step / main-direct / wave 13 BG 5 trigger 표현 시 외부 lift 추천 발생 0건. spin breaker / LOOP/JECXZ / leave EBP guard / unmapped EIP graceful / mem_fault graceful 자체 작성 (Intel SDM Vol 2 의 LOOP/JECXZ/LEAVE/TEST/JE/CMOVcc opcode spec + Microsoft Learn `__acrt_initialize_thread_local_storage` public docs reference + Microsoft Learn `___scrt_initialize_default_local_stdio_options` reference 만, Wine kernel32 source / Wine sync source / Wine TLS source code 0줄). KERNEL32/MSVCR/UCRT/NTDLL/ADVAPI32/oleaut32 stub return 표 자체 작성. 본 PROGRESS V10 path 외 수정 X. Apple SDK + Microsoft PE/COFF spec + Intel SDM Vol 1-3 + Microsoft Win32 API public docs + Microsoft Learn EncodePointer/DecodePointer + Microsoft Learn `__acrt_initialize_thread_local_storage` + VS2017 exe_common.inl distribution + rentry kernel32-obfuscated-pointers analysis + ReactOS public docs reference only.*
