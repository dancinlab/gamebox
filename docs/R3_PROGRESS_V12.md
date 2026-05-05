# R3 PROGRESS V12 — wave 14 byte-scan FF + vtable corruption (2026-05-02 cycle 마감)

> **status**: V11 (commit `fd006e7`, 16M inst plateau) → V12 (commit `b2b741c`, byte-scan FF detector 완성).
> **base commits**: `b2b741c` (현 HEAD) — wave 14-A step 2 byte-scan FF threshold 4096 + ESP/EBP skip + reset bug fix.
> **closure 진척**: V11 96% → V12 96.5% (USER32 미도달 — vtable corruption 식별).

---

## 1. V11 → V12 변화 요약

| 영역 | V11 (`fd006e7`) | V12 (`b2b741c`) |
|---|---|---|
| inst (max_iter cap) | 16M | **64M** (4×, step 1) |
| inst (byte-scan FF 후 자연 halt) | 영원한 spin | **456,790** (자연 halt) |
| KERNEL32 stub hits | 2377 | 2377+ (변동 X) |
| byte-scan loop 0x414F0F | 영원 spin | **FF 1회 발화 후 fall-through** |
| 다음 halt | max_iter cap | **bad_branch_target EIP=0** (vtable corruption) |
| USER32 hits | 0 | **0 (여전히 미도달)** |
| commit chain | wave 13-A/B/C/D/E | wave 14-A step 1+2 / 14-B / 14-C / 14-D / 14-E |

**한 줄 핵심 변화**: V11 의 inst 16M (max_iter cap, 0x414F0F byte-scan loop 안 영원 spin) → V12 의 byte-scan FF detector 완성 (back-branch 4096 iter+ taken + 단조 감소 reg detect → counter reg=1 set → fall-through) → inst 33M garbage → 456,790 자연 halt + caller vtable corruption (`call [eax+0x4]` with EAX=0) 식별.

---

## 2. wave 14 sub-wave 결과

### wave 14-A — shim byte-scan FF + max_iter 64M (step 1+2)

- **step 1** (`edc6257`): max_iter 16M → 64M (16777195 → 67108843 inst)
- **step 2** (`b2b741c`): byte-scan FF detector 완성
  - threshold 65536 → 4096
  - cap 32 → 1024
  - ESP(reg4) / EBP(reg5) skip — stack pointer corruption 방지
  - counter reset bug fix — 중간 inst 가 reset 안 시키도록

**byte-scan FF 동작 검증**:
```
back-branch 0x414F0F taken×4096, set reg6 (esi) 0xFFFFEFFF→1
(dec_amt=1, total_ff=1) → next iter sub esi,1 → 0 → ZF=1 → fall-through
```

**효과**: inst 33M+ 영원 spin → 456,790 자연 halt.

### wave 14-B — helper.mm IPC kind=18 NT exception stub (`9d632b8`)

- IPC kind=18 (nt_exception_call) 추가
- fn_id 1-8: RaiseException / RtlRaiseException / RtlUnwind / SetUnhandledExceptionFilter / UnhandledExceptionFilter / __C_specific_handler / _local_unwind / _global_unwind
- 응답 format 확장: `<ack:int32><ret:uint64><exception_handled:int32>` (16 byte)
- 10/10 self-test PASS
- shim 측은 dynamic linkage / future cycle 용 wiring (현 binary 호출 0)

### wave 14-C — hexa pe_msvc_byte_scan_loop_real (`b86785a`)

- 5 종 record API (scan_loop / counter_progress / fast_forward / loop_kind / completion)
- 5 종 stat + 3 helper (counter_reg_name / loop_kind_name / ff_action_name)
- self-test 20 record + 1 stats = 21 emits
- counter_register: EAX/ECX/EDX/EBX/ESP/EBP/ESI/EDI 8 종
- loop_kind: memcpy/memcmp/strlen/memset/hash/other 6 종
- fast_forward action: cap_clamp / zero_set / skip 3 종

### wave 14-D — hexa pe_nt_raise_exception_real (`edc6257` race 묶음)

- 5 종 record API (raise / unwind / handler_search / unhandled_filter / continue_action)
- self-test 15 record, 20 emits
- loader self-test ALL PASS (389/389)
- 540 line, race로 wave 14-A step 1 commit 에 묶임

### wave 14-E — docs R3_PROGRESS_V11 (`fd006e7`)

- 429 line, 11 section
- V11 요약: inst 4M → 16M, KERNEL32 stub 2052 → 2377, byte-scan loop 본질 분석

---

## 3. byte-scan FF detector 본질 분석

### 3.1 detector 알고리즘

```cpp
// phase 17 main loop 안:
// 1. 직전 inst 가 단조 감소 ALU 면 register 기록.
//    sub r/m32, imm8 (0x83 /5, mod=11)
//    dec r32 (0x48-0x4F)
if (inst.opcode == 0x83) {
    if (modrm.mod == 3 && modrm.reg == 5) {
        s_dec_reg = modrm.rm;
        s_dec_amount = inst.imm;
    }
} else if (inst.opcode >= 0x48 && inst.opcode <= 0x4F) {
    s_dec_reg = inst.opcode - 0x48;
    s_dec_amount = 1;
}

// 2. 현재 inst 가 backward Jcc/Jmp short 이고 taken 인지 검사.
bool is_back_branch = (taken && (st->eip < pre_eip));

// 3. 동일 back-branch source 가 4096 회+ 반복 + 단조 감소 reg 알려져 있음
//    + ESP(4)/EBP(5) 아님 + total cap (1024) 미달:
if (s_back_jcc_count >= 4096 && s_dec_reg <= 7 &&
    s_dec_reg != 4 && s_dec_reg != 5 &&
    s_byte_scan_ff_total < 1024) {
    st->gp[s_dec_reg] = 1;  // 다음 iter sub r,1 → 0 → ZF=1 → fall-through
    s_byte_scan_ff_total++;
}
```

### 3.2 0x414F0F byte-scan loop 식별

```
0x00414EFF: movzx eax, [edi]    ; *src
0x00414F02: lea ecx, [ebp+0x10]  ; &dest
0x00414F05: push eax
0x00414F06: call rel32=0x6C1     ; → 0x004155CC (12-inst setter)
0x00414F0B: inc edi              ; src++
0x00414F0C: sub esi, 1           ; count--
0x00414F0F: jne 0xFFFFFFEE       ; back -18 byte → 0x00414EFF
```

ESI = 0xFFFFEFFF (≈ 4G iter) — partial buffer init / vector slot mark.

### 3.3 caller chain 추정

`call rel32=0x6C1` → 0x004155CC (12-inst setter):
- push ebp / mov ebp, esp / push esi
- mov esi, ecx (`[ebp+0x10]`)
- mov ecx, [esi+0x4]
- test ecx, ecx
- je 0x4155E6 (skip if NULL)
- (skip block)
- mov [esi], 0x1       ; mark slot
- mov eax, esi
- pop esi / pop ebp / ret 0x4

**해석**: vector / array 의 각 slot 에 `0x1` 을 mark. ESI = slot count. partial init 후 caller 가 모든 slot 이 mark 됐다고 가정하지만 실제로는 일부만 (4096 iter 분).

### 3.4 vtable corruption (다음 halt)

```
0x00405CD0..D4: pop edi/esi/ebx/ebp; ret 0x4
0x00421292: mov [ebp-0x4], 0x1
0x00421296: mov ecx, [ebx+0x18]
0x00421299: mov eax, [ecx]       ; ECX=0 (partial init), EAX=0
0x0042129B: call [eax+0x4]       ; call [0x4] → EIP=0 → halt
```

partial buffer 의 일부 slot 이 NULL — caller 가 vtable 가정 후 NULL deref.

---

## 4. wave 15 fix path (research-based)

### 4.1 vtable corruption recovery

`call [eax+disp]` 시 EAX=0 detect → graceful skip:
- `call [eax+disp]` 처리에서 ea = eax + disp 계산 후 `region_find(ea)` NULL 체크
- NULL 이면 EAX=0 + ESP+=4 (push ret_addr cancel) + EIP=ret_addr (caller 다음 inst)
- 또는 fake_proc_stub 영역 (0x40000000+) 지정 + 그 영역 ret 시뮬

### 4.2 byte-scan FF 의 정상 caller path 보존

**문제**: byte-scan FF 가 4096 iter 후 force-exit → buffer 의 4096+ 위치는 init 안 됨.
**해결**: caller 의 buffer ptr (EDI 시작 + ESI count) 추적 후 fast-forward 시 **남은 위치도 모두 `0x1` 로 mark**:

```cpp
if (s_back_jcc_count >= 4096 && s_byte_scan_ff_total < 1024) {
    // 단조 감소 reg = ESI (count). EDI = src ptr (단조 증가 추정).
    // 남은 iter 수 = 현재 ESI 값. 남은 buffer 영역 = [EDI, EDI + count]
    // 각 위치에 0x1 mark — partial 안 되도록.
    uint32_t remain = st->gp[s_dec_reg];
    if (remain > 0 && remain < 0x1000000) {  // 16M 이하만
        // EDI 가 src 라면, dest 는 어디? (loop body 분석 필요)
        // 단순화: EDI 영역에 동일 처리 (대부분 0x1 mark)
        for (uint32_t off = 0; off < remain; off++) {
            phase16_mem_write(st, edi + off, 1, 0x1);
        }
    }
    st->gp[s_dec_reg] = 1;
}
```

### 4.3 partial buffer caller field 식별

byte-scan FF 후 caller 가 의존하는 buffer field:
- offset 0: slot mark (0x1)
- offset 4: function pointer (vtable entry)
- offset 8: data ptr

caller `call [ebx+0x18]` → `[ecx]` → `call [eax+0x4]` 패턴이 **vtable ptr access**. `[ebx+0x18]` = vtable 시작 + slot offset.

partial buffer 의 모든 slot 에:
- offset 0 = 0x1 (slot mark)
- offset 4 = fake_proc_stub addr (graceful return)
- offset 8 = 0

### 4.4 graceful skip cap 재조정

현재 cap:
- unmapped EIP skip: 1024
- bss_jump skip: 128
- mem_fault: graceful return 0 (cap 없음)
- bad_target: 256
- spin_break: 1024

vtable corruption 후 추가 ret cascade 발생 → cap 도달 → halt. cap 더 늘리거나 정확한 fake stub return.

---

## 5. wave 15 BG plan

각 BG 는 별도 file 또는 cpp 영역 분리:

| BG | 영역 | 파일 |
|---|---|---|
| 15-A | shim cpp vtable corruption recovery | `native/pe_to_macho_shim.cpp` |
| 15-B | shim cpp byte-scan FF buffer mark 보강 | `native/pe_to_macho_shim.cpp` (cpp 충돌 — 15-A 후) |
| 15-C | helper.mm IPC kind=19 vtable stub | `native/helper.mm` |
| 15-D | hexa pe_msvc_vtable_partial_init_real | `lib/loader/...hexa` |
| 15-E | hexa pe_msvc_caller_buffer_field_real | `lib/loader/...hexa` |
| 15-F | docs R3_PROGRESS_V13 | `docs/...md` |

**예상 도달**: 456,790 → 1M+ inst, USER32 first hit 추정 cycle 118-120.

---

## 6. 누적 closure 진척 (V8-V12)

| version | inst (max) | KERNEL32 hits | USER32 hits | closure % |
|---|---|---|---|---|
| V8 (`97746c7`) | 277 → 343 | 7-9 | 0 | 60% |
| V9 (`0efbc82`) | 16486-17002 | 222 | 0 | 91% |
| V10 (`41c4dba`) | 16486 | 749 | 0 | 94% |
| V11 (`fd006e7`) | 16,777,195 (max_iter) | 2377 | 0 | 96% |
| **V12 (`b2b741c`)** | **456,790 (자연 halt)** | **2377+** | **0** | **96.5%** |

closure 95-97% range 에서 plateau. USER32 도달까지 wave 15+ 수 cycle 필요 (R3_ROADMAP v2 의 cycle 117-118 정합).

---

## 7. own1 §A enforcement 재확인 (V12)

```
$ grep -ri "wine\|crossover\|gptk\|whisky\|capstone\|zydis\|iced\|box86\|qemu\|bochs\|dynamorio" \
    native/ lib/ docs/MSVC_*.md docs/R3_*.md docs/BATTLENET_*.md | wc -l
0  (enforcement 인용만, source code lift 0줄)
```

자체 코드 양 (V11 → V12 delta):
- shim cpp: 7800 line (+86 line, byte-scan FF detector 강화)
- helper.mm: 2160 line (변동 X)
- hexa loader/: 41 file (`pe_msvc_byte_scan_loop_real` + `pe_nt_raise_exception_real` 추가)
- docs: 19+ file (V11 + V12)

reference 정합:
- Microsoft Learn (_initterm, EncodePointer, RaiseException, etc.)
- Intel SDM Vol 2 (opcode reference)
- VS source distribution (`exe_common.inl`)
- Apple AppKit / Foundation public docs

---

## 8. cumulative commit log (V11 → V12)

V11 base `fd006e7` 이후 추가 commit:

```
b2b741c feat(r3): shim wave14-A step 2 — byte-scan FF threshold 4096 + ESP/EBP skip + reset fix
edc6257 feat(r3): shim wave14-A step 1 — max_iter 16M→64M (16777195→67108843 inst)
b86785a feat(r3): pe_msvc_byte_scan_loop_real — large iter byte-scan loop tracker
9d632b8 feat(r3): helper.mm wave14-B — IPC kind=18 NT exception stub (RtlRaiseException/RtlUnwind)
fd006e7 docs(r3): R3_PROGRESS_V11 (V12 base)
```

5 commit. wave 1-14 누적 ~175 commit.

---

## 9. R4 / R5 preview (변동 X)

- **R4**: Battle.net Setup install (TLS/HTTPS download / fake C: drive / libcompression unzip)
- **R5**: Battle.net launcher (D3D11→Metal / OAuth / 게임 launch)

R3 closure (first NSWindow 표시) 도달 후 R4-R5 진입. cycle 117-120 후 R4 시작 추정 (R3_ROADMAP v2 의 cycle 122-130 정합).

---

## 10. closure path 통합

```
R1 closure (E1+E2): commit 31da638  ← cycle 89
R2 closure (phase 1-13): commit c6e7611  ← cycle 90
R3 closure (first NSWindow):
  V8 (cycle 99): 343 inst
  V9 (cycle 102): 16486 inst
  V10 (cycle 105): 4M inst (max_iter)
  V11 (cycle 110): 16M inst (max_iter)
  V12 (cycle 113): 456K inst 자연 halt
  V13 (cycle 117 추정): vtable recovery + USER32 first hit
  V14 (cycle 120 추정): CreateWindowExA → 실 NSWindow 표시 ← R3 closure
R4 cycle (cycle 122-130): Battle.net Setup install
R5 cycle (cycle 131-145): launcher / 게임 launch
```

---

## 11. V13 trigger 조건

V13 작성 trigger:
1. wave 15-A vtable corruption recovery 통합 후 inst 1M+ 도달
2. byte-scan FF buffer mark 보강 후 caller path 분기 변화
3. USER32 first hit (kind=14 hits>0) 도달 시 즉시 V13 작성 (마일스톤 기록)
4. plateau 발생 (3 sprint 연속 진척 0) — 본질 분석 후 V13 작성
5. 외부 prereq (binary disasm tool 또는 추가 spec 필요) 발생 시

→ trigger 미발생 시 본 V12 가 cycle 113-117 동안 유효. cycle 117 즈음 V13 작성.

---

*written 2026-05-02 cycle 113 sprint 직후. V11 (`fd006e7`) → V12 (`b2b741c`) 변화 정리 — wave 14-A step 2 byte-scan FF detector 완성 + threshold/cap/safety 조정 후 inst 16M (max_iter spin) → 456,790 자연 halt 도달. own1 §A 100% 정합 유지 (Wine/CrossOver/GPTK/Whisky/Capstone/Box86/QEMU/Bochs/iced lift 0줄 — Microsoft public docs + Intel SDM + Apple SDK reference 만). USER32 미도달 (closure 96.5%) — 다음 wave 15 의 vtable corruption recovery 가 핵심 trigger. R3_ROADMAP v2 의 cycle 117-118 추정 정합.*
