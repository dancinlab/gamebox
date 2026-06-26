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
// is, in the real binary, the prologue of __scrt_common_main — which the
// E2 decoder does NOT yet cover (group-1 imm arith 0x83/0x81/0x80, etc.,
// per r1). We therefore plant a representative uncovered opcode (0x83) at
// that VA so the hermetic run reproduces the exact measured outcome: one
// real instruction executed, branch to the real VA 0x539259, HALT at the
// decoder-coverage wall. own1: our own bytes, Intel SDM semantics only.

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

// Build the hermetic flat image: entry block + uncovered-opcode sentinels
// at the three real CALL/JMP targets the entry block can reach.
static void build_hermetic(uint8_t *img) {
    memset(img, 0, IMG_SIZE);
    put_at(img, ENTRY_VA, ENTRY_BYTES, sizeof(ENTRY_BYTES));
    // 0x83 = group-1 r/m32,imm8 (e.g. `sub esp,imm`) — a real, very common
    // prologue opcode the E2 decoder does not cover yet → I386_OP_UNKNOWN.
    uint8_t wall[] = { 0x83, 0xEC, 0x28 };         // representative `sub esp,0x28`
    put_at(img, 0x539259u, wall, sizeof(wall));    // CALL target (insn 1)
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
            printf("__SHIM__ PARTIAL phase=entry_block_executed insns=%u halted=%s halt_va=0x%X\n",
                   r.insns, i386_halt_name(r.halt), r.halt_va);
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
    // First real EXECUTION of game-binary bytes. insn 1 (CALL rel32) must
    // push the return addr and branch to the real VA 0x539259, then HALT at
    // the decoder-coverage wall (0x83, uncovered).
    printf("\n[cpu] Run A — execute from entry 0x%X (real entry block)\n", ENTRY_VA);
    build_hermetic(img_buf);
    {
        i386_cpu_t cpu; memset(&cpu, 0, sizeof(cpu));
        cpu.eip = ENTRY_VA;
        cpu.gpr[I386_REG_ESP] = 0x539E00u;          // stack inside the image
        uint32_t esp0 = cpu.gpr[I386_REG_ESP];
        i386_run_result_t r;
        i386_cpu_run(&cpu, &img, 1000, &r);

        printf("  executed=%u halt=%s halt_va=0x%X eip=0x%X esp=0x%X\n",
               r.insns, i386_halt_name(r.halt), r.halt_va, cpu.eip, cpu.gpr[I386_REG_ESP]);
        uint32_t pushed = 0;
        i386_mem_read32(&img, cpu.gpr[I386_REG_ESP], &pushed);

        CHECK(r.insns == 1,                 "exactly 1 real instruction executed (the CALL)");
        CHECK(cpu.gpr[I386_REG_ESP] == esp0 - 4, "CALL decremented esp by 4");
        CHECK(pushed == 0x5388ABu,          "CALL pushed return addr 0x5388AB");
        CHECK(cpu.eip == 0x539259u,         "CALL set eip to real target 0x539259");
        CHECK(r.halt == I386_HALT_UNKNOWN,  "halted at decoder-coverage wall (UNKNOWN)");
        CHECK(r.halt_va == 0x539259u,       "wall VA == 0x539259");

        if (!emitted) {
            printf("__SHIM__ PARTIAL phase=entry_block_executed insns=%u halted=%s halt_va=0x%X\n",
                   r.insns, i386_halt_name(r.halt), r.halt_va);
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
