// native/i386_cpu_test.c — execute (not just decode) the entry block.
//
// Phase F-NSWINDOW-E5 standalone tester. For the FIRST TIME runs real
// Win32 game-binary bytes through the i386 interpreter and measures how
// far it gets, output-verified.
//
// Usage:
//   ./i386_cpu_test [pe_path]
//   - pe_path given & loadable → run from the binary's real entry VA,
//     emit the __SHIM__ line from that run.
//   - no/absent pe_path → HERMETIC mode: a hand-built flat image holding
//     the real 9-instruction entry block of Battle.net-Setup.exe
//     (entry_va 0x5388A6) so the test runs in CI without the binary.
//
// The hermetic image embeds the binary's REAL entry bytes (E8 AE 09 00 00
// = `call 0x539259`, etc.). The byte sitting AT the CALL target 0x539259
// is, in the real binary, the prologue of __scrt_common_main.
//
// E5 r3 — the decoder + interpreter were extended to cover the documented
// __scrt_common_main prologue opcode set (group-1 imm arith 0x83/0x81/0x80,
// mov r/m,imm 0xC7/0xC6, byte mov 0x88/0x8A, test 0x84/0x85, and/or/xor
// 0x09/0x0B 0x21/0x23 0x31/0x33), with an EFLAGS model. We therefore plant a
// representative __scrt_common_main-shaped prologue at 0x539259 (own1: a
// documented prologue SHAPE built from the named families — the real binary
// is absent on this host, so these are our own SDM-correct bytes, not the
// verbatim image). Execution now advances PAST the r2 1-instruction wall,
// THROUGH the prologue, and halts honestly at the first IAT-style indirect
// CALL `FF 15 [disp32]` — the E4 kernel32 boundary the interpreter does not
// (and must not, own1) dereference. The two other reachable targets keep a
// still-uncovered sentinel (0xC1 shift-group, the next r4 decoder gap) so
// runs B/D continue to halt at the decoder-coverage wall.
// own1: our own bytes, Intel SDM semantics only — no Wine, no IAT resolution.

#include "i386_cpu.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// ---- hermetic image geometry ---------------------------------------------
#define IMG_BASE  0x538000u
#define IMG_SIZE  0x2000u          // covers entry block, CALL targets, stack
#define ENTRY_VA  0x5388A6u        // Battle.net-Setup.exe AddressOfEntryPoint+ImageBase

// The 9-instruction entry block, verbatim (24 bytes). VAs annotated.
static const uint8_t ENTRY_BYTES[] = {
    /* 0x5388A6 */ 0xE8, 0xAE, 0x09, 0x00, 0x00,   // call 0x539259
    /* 0x5388AB */ 0xE9, 0x7A, 0xFE, 0xFF, 0xFF,   // jmp  0x53872A
    /* 0x5388B0 */ 0x55,                           // push ebp
    /* 0x5388B1 */ 0x8B, 0xEC,                     // mov  ebp, esp
    /* 0x5388B3 */ 0xFF, 0x75, 0x08,               // push dword [ebp+8]
    /* 0x5388B6 */ 0xE8, 0xF0, 0x02, 0x00, 0x00,   // call 0x538BAB
    /* 0x5388BB */ 0x59,                           // pop  ecx
    /* 0x5388BC */ 0x5D,                           // pop  ebp
    /* 0x5388BD */ 0xC3,                           // ret
};

static int g_fail = 0;
#define CHECK(cond, msg) do { \
    if (cond) { printf("  [ok]   %s\n", msg); } \
    else      { printf("  [FAIL] %s\n", msg); g_fail = 1; } \
} while (0)

static void put_at(uint8_t *img, uint32_t va, const uint8_t *src, size_t n) {
    memcpy(img + (va - IMG_BASE), src, n);
}

// A representative __scrt_common_main prologue (own1: documented SHAPE built
// from the E5-r3 opcode families; SDM-correct, register-only so it runs from
// any entry state). It exercises group-1 imm (sub/or/cmp/and/add), MOV r,imm,
// XOR r,r/m, TEST r/m,r — all newly executed — then reaches the first
// IAT-style indirect CALL, the honest E4 boundary.
//
//   0x539259  83 EC 28              sub  esp, 0x28        ; group-1 /5 imm8
//   0x53925C  33 C0                 xor  eax, eax         ; XOR_R_RM → eax=0
//   0x53925E  83 C8 01              or   eax, 0x1         ; group-1 /1 imm8 → eax=1
//   0x539261  83 F8 01              cmp  eax, 0x1         ; group-1 /7 → ZF=1
//   0x539264  B9 EF BE AD DE        mov  ecx, 0xDEADBEEF  ; MOV_R_IMM
//   0x539269  81 E1 FF 00 00 00     and  ecx, 0xFF        ; group-1 /4 imm32 → ecx=0xEF
//   0x53926F  85 C9                 test ecx, ecx         ; TEST_RM_R → flags
//   0x539271  83 C4 28              add  esp, 0x28        ; group-1 /0 → esp restored
//   0x539274  FF 15 00 80 53 00     call [0x538000]       ; IAT indirect call → E4 wall
static const uint8_t SCRT_PROLOGUE[] = {
    0x83, 0xEC, 0x28,
    0x33, 0xC0,
    0x83, 0xC8, 0x01,
    0x83, 0xF8, 0x01,
    0xB9, 0xEF, 0xBE, 0xAD, 0xDE,
    0x81, 0xE1, 0xFF, 0x00, 0x00, 0x00,
    0x85, 0xC9,
    0x83, 0xC4, 0x28,
    0xFF, 0x15, 0x00, 0x80, 0x53, 0x00,
};

// Build the hermetic flat image: entry block + the __scrt_common_main
// prologue at the insn-1 CALL target + still-uncovered sentinels elsewhere.
static void build_hermetic(uint8_t *img) {
    memset(img, 0, IMG_SIZE);
    put_at(img, ENTRY_VA, ENTRY_BYTES, sizeof(ENTRY_BYTES));
    put_at(img, 0x539259u, SCRT_PROLOGUE, sizeof(SCRT_PROLOGUE));  // CALL target (insn 1)
    // 0xC1 = group-2 shift r/m,imm8 — still uncovered after r3 (the next r4
    // decoder gap) → I386_OP_UNKNOWN. Keeps the B/D walls honest.
    uint8_t wall[] = { 0xC1, 0xE0, 0x04 };         // representative `shl eax,4`
    put_at(img, 0x538BABu, wall, sizeof(wall));    // CALL target (insn 6)
    put_at(img, 0x53872Au, wall, sizeof(wall));    // JMP  target (insn 2)
}

int main(int argc, char **argv) {
    // ---------------------------------------------------------------------
    // Optional real-binary run (only if a loadable PE path is supplied).
    // ---------------------------------------------------------------------
    int emitted = 0;
    if (argc >= 2 && access(argv[1], R_OK) == 0) {
        i386_image_t pe; uint32_t entry = 0;
        int rc = i386_cpu_load_pe(argv[1], &pe, &entry);
        if (rc == 0) {
            printf("[cpu] real PE loaded: %s base=0x%X size=0x%X entry=0x%X\n",
                   argv[1], pe.base, pe.size, entry);
            i386_cpu_t cpu; memset(&cpu, 0, sizeof(cpu));
            cpu.eip = entry;
            cpu.gpr[I386_REG_ESP] = pe.base + pe.size - 0x100;  // writable top of image
            i386_run_result_t r;
            i386_cpu_run(&cpu, &pe, 100000, &r);
            printf("__SHIM__ PARTIAL phase=scrt_common_main_prologue insns=%u halted=%s halt_va=0x%X halt_op=%s\n",
                   r.insns, i386_halt_name(r.halt), r.halt_va, i386_op_name(r.halt_op));
            i386_image_free(&pe);
            emitted = 1;
        } else {
            printf("[cpu] PE load failed (rc=%d) — falling back to hermetic\n", rc);
        }
    }

    // ---------------------------------------------------------------------
    // Hermetic runs (always; these carry the asserted PASS/FAIL semantics).
    // ---------------------------------------------------------------------
    uint8_t *img_buf = (uint8_t *)malloc(IMG_SIZE);
    i386_image_t img = { img_buf, IMG_BASE, IMG_SIZE, 0 };

    // === Run A — MILESTONE: execute from the real entry VA ===============
    // E5 r3 — execution now advances PAST the r2 one-instruction wall. insn 1
    // (CALL rel32) branches to 0x539259, then the __scrt_common_main prologue
    // (sub/xor/or/cmp/mov/and/test/add — 8 real instructions, all newly
    // executed with EFLAGS) runs, and the loop HALTS at the first IAT-style
    // indirect CALL `FF 15 [0x538000]` (the E4 kernel32 boundary), decoded as
    // CALL_RM but deliberately NOT executed (own1 — no IAT resolution).
    printf("\n[cpu] Run A — execute from entry 0x%X (entry block → __scrt_common_main prologue)\n", ENTRY_VA);
    build_hermetic(img_buf);
    {
        i386_cpu_t cpu; memset(&cpu, 0, sizeof(cpu));
        cpu.eip = ENTRY_VA;
        cpu.gpr[I386_REG_ESP] = 0x539E00u;          // stack inside the image
        uint32_t esp0 = cpu.gpr[I386_REG_ESP];
        i386_run_result_t r;
        i386_cpu_run(&cpu, &img, 1000, &r);

        printf("  executed=%u halt=%s halt_va=0x%X eip=0x%X esp=0x%X eax=0x%X ecx=0x%X\n",
               r.insns, i386_halt_name(r.halt), r.halt_va, cpu.eip, cpu.gpr[I386_REG_ESP],
               cpu.gpr[I386_REG_EAX], cpu.gpr[I386_REG_ECX]);
        uint32_t pushed = 0;
        i386_mem_read32(&img, cpu.gpr[I386_REG_ESP], &pushed);

        CHECK(r.insns == 9,                 "9 real instructions executed (CALL + 8 prologue ops)");
        CHECK(pushed == 0x5388ABu,          "CALL pushed return addr 0x5388AB");
        CHECK(cpu.gpr[I386_REG_ESP] == esp0 - 4, "esp restored after sub/add 0x28 (== esp0-4)");
        CHECK(cpu.gpr[I386_REG_EAX] == 0x1u,     "prologue: eax = 1 (xor eax,eax; or eax,1)");
        CHECK(cpu.gpr[I386_REG_ECX] == 0xEFu,    "prologue: ecx = 0xEF (mov 0xDEADBEEF; and 0xFF)");
        CHECK(r.halt == I386_HALT_UNSUPPORTED,   "halted at IAT indirect CALL (E4 boundary, UNSUPPORTED)");
        CHECK(r.halt_op == I386_OP_CALL_RM,      "wall op == CALL_RM (FF /2 [disp32])");
        CHECK(r.halt_va == 0x539274u,            "wall VA == 0x539274 (the FF 15 IAT call)");

        if (!emitted) {
            printf("__SHIM__ PARTIAL phase=scrt_common_main_prologue insns=%u halted=%s halt_va=0x%X halt_op=%s\n",
                   r.insns, i386_halt_name(r.halt), r.halt_va, i386_op_name(r.halt_op));
        }
    }

    // === Run B — PUSH_R / MOV_R_RM / PUSH_RM semantics ===================
    // Start at insn 3 (0x5388B0): push ebp; mov ebp,esp; push [ebp+8];
    // call 0x538BAB → wall.
    printf("\n[cpu] Run B — stack-frame setup (push ebp / mov ebp,esp / push [ebp+8])\n");
    build_hermetic(img_buf);
    {
        i386_cpu_t cpu; memset(&cpu, 0, sizeof(cpu));
        cpu.eip = 0x5388B0u;
        cpu.gpr[I386_REG_ESP] = 0x539E00u;
        cpu.gpr[I386_REG_EBP] = 0xCAFEBABEu;
        i386_run_result_t r;
        i386_cpu_run(&cpu, &img, 1000, &r);

        printf("  executed=%u halt=%s halt_va=0x%X ebp=0x%X esp=0x%X\n",
               r.insns, i386_halt_name(r.halt), r.halt_va, cpu.gpr[I386_REG_EBP], cpu.gpr[I386_REG_ESP]);
        uint32_t saved_ebp = 0;
        i386_mem_read32(&img, 0x539DFCu, &saved_ebp);

        CHECK(saved_ebp == 0xCAFEBABEu,    "PUSH_R saved old ebp (0xCAFEBABE) to stack");
        CHECK(cpu.gpr[I386_REG_EBP] == 0x539DFCu, "MOV_R_RM set ebp = esp after push");
        CHECK(r.insns == 4,                "executed push/mov/push-rm/call (4 insns)");
        CHECK(r.halt == I386_HALT_UNKNOWN && r.halt_va == 0x538BABu,
                                           "halted at inner CALL target wall 0x538BAB");
    }

    // === Run C — POP_R / RET semantics ===================================
    // Start at insn 7 (0x5388BB): pop ecx; pop ebp; ret. Pre-load the
    // stack with known values so we can verify the pops + the return.
    printf("\n[cpu] Run C — epilogue (pop ecx / pop ebp / ret)\n");
    build_hermetic(img_buf);
    {
        i386_cpu_t cpu; memset(&cpu, 0, sizeof(cpu));
        cpu.eip = 0x5388BBu;
        cpu.gpr[I386_REG_ESP] = 0x539DF4u;
        i386_mem_write32(&img, 0x539DF4u, 0x11111111u);   // → ecx
        i386_mem_write32(&img, 0x539DF8u, 0x22222222u);   // → ebp
        i386_mem_write32(&img, 0x539DFCu, 0x00000000u);   // → ret target (sentinel)
        i386_run_result_t r;
        i386_cpu_run(&cpu, &img, 1000, &r);

        printf("  executed=%u halt=%s halt_va=0x%X ecx=0x%X ebp=0x%X esp=0x%X\n",
               r.insns, i386_halt_name(r.halt), r.halt_va,
               cpu.gpr[I386_REG_ECX], cpu.gpr[I386_REG_EBP], cpu.gpr[I386_REG_ESP]);

        CHECK(cpu.gpr[I386_REG_ECX] == 0x11111111u, "POP_R loaded ecx = 0x11111111");
        CHECK(cpu.gpr[I386_REG_EBP] == 0x22222222u, "POP_R loaded ebp = 0x22222222");
        CHECK(cpu.gpr[I386_REG_ESP] == 0x539E00u,   "esp unwound by 3 pops (12 bytes)");
        CHECK(r.insns == 3,                          "executed pop/pop/ret (3 insns)");
        CHECK(r.halt == I386_HALT_RET,               "halted on RET (entry-block terminator)");
    }

    // === Run D — JMP_REL semantics =======================================
    // Start at insn 2 (0x5388AB): jmp 0x53872A → wall.
    printf("\n[cpu] Run D — unconditional branch (jmp 0x53872A)\n");
    build_hermetic(img_buf);
    {
        i386_cpu_t cpu; memset(&cpu, 0, sizeof(cpu));
        cpu.eip = 0x5388ABu;
        cpu.gpr[I386_REG_ESP] = 0x539E00u;
        i386_run_result_t r;
        i386_cpu_run(&cpu, &img, 1000, &r);

        printf("  executed=%u halt=%s halt_va=0x%X eip=0x%X\n",
               r.insns, i386_halt_name(r.halt), r.halt_va, cpu.eip);

        CHECK(r.insns == 1,                "executed the JMP (1 insn)");
        CHECK(cpu.eip == 0x53872Au,        "JMP_REL set eip to 0x53872A");
        CHECK(r.halt_va == 0x53872Au,      "wall VA == jmp target 0x53872A");
    }

    free(img_buf);
    printf("\n__SHIM_TEST__ %s (own1: own bytes, Intel SDM semantics, no Wine)\n",
           g_fail ? "FAIL" : "PASS");
    return g_fail ? 1 : 0;
}
