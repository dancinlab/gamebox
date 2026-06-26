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
// verbatim image). r3 execution advanced PAST the r2 1-instruction wall,
// THROUGH the prologue, and HALTED at the first IAT-style indirect CALL
// `FF 15 [0x538000]` @0x539274 — the E4 kernel32 boundary.
//
// E5 r4 — CROSS that boundary. The interpreter BINDS the IAT slot to a native
// kernel32 shim: on `FF 15 [slot]` it looks the slot VA up in a registry, and
// if bound, dispatches to the native C stub, places its result in EAX, and
// CONTINUES.
//
// E5 r5 — DRAIN more CRT-startup imports + close the 0xC1 shift gap. The
// prologue now runs FIVE bound kernel32 shims in a row: GetCurrentThreadId
// (r4), GetCurrentProcessId, GetSystemTimeAsFileTime (the first BUFFER-WRITING
// stub — reads the caller's pushed pointer arg and writes an 8-byte FILETIME
// into image memory), QueryPerformanceCounter (buffer write, → 1), GetTickCount;
// between them a `shl edx,4` exercises the freshly-decoded group-2 shift
// (0xC1 /4) with full CF/OF/ZF/SF flags.
//
// E5 r6 — DRAIN 3 more CRT-security imports, CLOSE the 0F B6/B7/BE/BF MOVZX/MOVSX
// decoder gap, and model a BOUNDED-SYNTHETIC TEB. After GetTickCount the prologue
// now binds IsProcessorFeaturePresent (1-arg → 1), InitializeSListHead (buffer-
// zeroes an 8-byte SLIST header), and GetModuleHandleW (→ the loaded image base),
// runs a `movzx ecx,al` + `movsx edx,cl` (the freshly-decoded two-byte extends),
// then reads a synthetic TEB self-pointer via `mov ecx, fs:[0x18]` (the FS
// segment-override redirects to teb_base) and walks TEB→PEB→ImageBaseAddress.
// Execution advances PAST the r5 wall (insns 18 → 30) and halts at a NINTH,
// UNregistered IAT call @0x5392D4 (slot 0x538020, the next import to bind, r7).
// The B/D walls now use a `0F A2` (cpuid) sentinel — the next genuinely-
// uncovered decoder gap (0F B6 movzx is no longer a wall).
// own1: binding the program's OWN imports to native impls is LOADING (what
// every PE loader does), NOT a bypass; the TEB here is OUR OWN synthetic block
// (a few fields), NOT the OS TEB and NOT a protection structure. kernel32 is the
// OS API, not DRM/Warden. Our own bytes, Intel SDM semantics, native shims — no
// Wine, no protection.

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
//   0x539274  FF 15 00 80 53 00     call [0x538000]       ; slot 0 → GetCurrentThreadId (r4)
//   0x53927A  89 C2                 mov  edx, eax         ; edx = returned TID
//   0x53927C  FF 15 04 80 53 00     call [0x538004]       ; slot 4 → GetCurrentProcessId (r5)
//   0x539282  68 00 9F 53 00        push 0x539F00         ; &FILETIME buffer (a pushed ptr arg)
//   0x539287  FF 15 08 80 53 00     call [0x538008]       ; slot 8 → GetSystemTimeAsFileTime
//                                                         ;   (r5 buffer-writing stub: 8 bytes
//                                                         ;    into [0x539F00])
//   0x53928D  68 10 9F 53 00        push 0x539F10         ; &LARGE_INTEGER buffer
//   0x539292  FF 15 0C 80 53 00     call [0x53800C]       ; slot C → QueryPerformanceCounter
//                                                         ;   (buffer write 8B → [0x539F10], → 1)
//   0x539298  C1 E2 04              shl  edx, 4           ; group-2 SHIFT (r5): edx = TID<<4
//   0x53929B  FF 15 10 80 53 00     call [0x538010]       ; slot 10 → GetTickCount (r5)
// E5 r6 continuation — drain 3 more CRT-security imports, exercise MOVZX/MOVSX,
// and read a BOUNDED-SYNTHETIC TEB→PEB→ImageBase chain via an fs: override:
//   0x5392A1  6A 0A                 push 0x0A             ; arg for next call
//   0x5392A3  FF 15 14 80 53 00     call [0x538014]       ; slot 14 → IsProcessorFeaturePresent (r6, →1)
//   0x5392A9  0F B6 C8              movzx ecx, al         ; 0F B6 (r6 decoder) → ecx=1
//   0x5392AC  0F BE D1              movsx edx, cl         ; 0F BE (r6 decoder) → edx=1
//   0x5392AF  68 20 9F 53 00        push 0x539F20         ; &SLIST head
//   0x5392B4  FF 15 18 80 53 00     call [0x538018]       ; slot 18 → InitializeSListHead (r6, zero 8B)
//   0x5392BA  6A 00                 push 0                ; GetModuleHandleW(NULL)
//   0x5392BC  FF 15 1C 80 53 00     call [0x53801C]       ; slot 1C → GetModuleHandleW (r6, → image base)
//   0x5392C2  64 8B 0D 18 00 00 00  mov ecx, fs:[0x18]    ; synthetic TEB self (fs override → teb_base)
//   0x5392C9  8B 51 30              mov edx, [ecx+0x30]   ; [TEB+0x30] → synthetic PEB
//   0x5392CC  8B 42 08              mov eax, [edx+8]      ; [PEB+8]    → ImageBaseAddress (= image base)
//   0x5392CF  68 30 9F 53 00        push 0x539F30         ; &STARTUPINFOW
//   0x5392D4  FF 15 20 80 53 00     call [0x538020]       ; slot 20 → UNBOUND (the new r7 wall)
static const uint8_t SCRT_PROLOGUE[] = {
    0x83, 0xEC, 0x28,
    0x33, 0xC0,
    0x83, 0xC8, 0x01,
    0x83, 0xF8, 0x01,
    0xB9, 0xEF, 0xBE, 0xAD, 0xDE,
    0x81, 0xE1, 0xFF, 0x00, 0x00, 0x00,
    0x85, 0xC9,
    0x83, 0xC4, 0x28,
    0xFF, 0x15, 0x00, 0x80, 0x53, 0x00,   // 0x539274 call [0x538000] → GetCurrentThreadId
    0x89, 0xC2,                           // 0x53927A mov edx, eax
    0xFF, 0x15, 0x04, 0x80, 0x53, 0x00,   // 0x53927C call [0x538004] → GetCurrentProcessId
    0x68, 0x00, 0x9F, 0x53, 0x00,         // 0x539282 push 0x539F00  (&filetime)
    0xFF, 0x15, 0x08, 0x80, 0x53, 0x00,   // 0x539287 call [0x538008] → GetSystemTimeAsFileTime
    0x68, 0x10, 0x9F, 0x53, 0x00,         // 0x53928D push 0x539F10  (&perfcount)
    0xFF, 0x15, 0x0C, 0x80, 0x53, 0x00,   // 0x539292 call [0x53800C] → QueryPerformanceCounter
    0xC1, 0xE2, 0x04,                     // 0x539298 shl edx, 4   (group-2 shift)
    0xFF, 0x15, 0x10, 0x80, 0x53, 0x00,   // 0x53929B call [0x538010] → GetTickCount
    // ── E5 r6 continuation ──
    0x6A, 0x0A,                           // 0x5392A1 push 0x0A   (IsProcessorFeaturePresent arg)
    0xFF, 0x15, 0x14, 0x80, 0x53, 0x00,   // 0x5392A3 call [0x538014] → IsProcessorFeaturePresent
    0x0F, 0xB6, 0xC8,                     // 0x5392A9 movzx ecx, al   (0F B6 — r6 decoder gap closed)
    0x0F, 0xBE, 0xD1,                     // 0x5392AC movsx edx, cl   (0F BE — r6)
    0x68, 0x20, 0x9F, 0x53, 0x00,         // 0x5392AF push 0x539F20  (&SLIST head)
    0xFF, 0x15, 0x18, 0x80, 0x53, 0x00,   // 0x5392B4 call [0x538018] → InitializeSListHead (buffer-zero)
    0x6A, 0x00,                           // 0x5392BA push 0       (GetModuleHandleW NULL)
    0xFF, 0x15, 0x1C, 0x80, 0x53, 0x00,   // 0x5392BC call [0x53801C] → GetModuleHandleW (→ image base)
    0x64, 0x8B, 0x0D, 0x18, 0x00, 0x00, 0x00, // 0x5392C2 mov ecx, fs:[0x18]  (synthetic TEB self)
    0x8B, 0x51, 0x30,                     // 0x5392C9 mov edx, [ecx+0x30]      ([TEB+0x30] → synthetic PEB)
    0x8B, 0x42, 0x08,                     // 0x5392CC mov eax, [edx+8]         ([PEB+8] → ImageBaseAddress)
    0x68, 0x30, 0x9F, 0x53, 0x00,         // 0x5392CF push 0x539F30  (&STARTUPINFOW)
    0xFF, 0x15, 0x20, 0x80, 0x53, 0x00,   // 0x5392D4 call [0x538020] → UNBOUND (the new r7 wall)
};

// The native kernel32 imports bound across IAT slots 0x538000..0x53801C (r4-r6).
// own1: loading the OS API surface to native impls, not a protection bypass.
// The buffer-writing trio (slots 8 / C / 18) take one stdcall pointer arg
// (4 bytes, callee-popped) and write into image memory; IsProcessorFeaturePresent
// (14) / GetModuleHandleW (1C) take 1 arg too; the rest are 0-arg.
static const i386_import_t K32_IMPORTS[] = {
    { 0x538000u, "GetCurrentThreadId",        i386_shim_GetCurrentThreadId,        0 },
    { 0x538004u, "GetCurrentProcessId",       i386_shim_GetCurrentProcessId,       0 },
    { 0x538008u, "GetSystemTimeAsFileTime",   i386_shim_GetSystemTimeAsFileTime,   4 },
    { 0x53800Cu, "QueryPerformanceCounter",   i386_shim_QueryPerformanceCounter,   4 },
    { 0x538010u, "GetTickCount",              i386_shim_GetTickCount,              0 },
    { 0x538014u, "IsProcessorFeaturePresent", i386_shim_IsProcessorFeaturePresent, 4 },
    { 0x538018u, "InitializeSListHead",       i386_shim_InitializeSListHead,       4 },
    { 0x53801Cu, "GetModuleHandleW",          i386_shim_GetModuleHandleW,          4 },
};
static const i386_iat_t K32_IAT = { K32_IMPORTS, 8 };
#define SHIM_TID  0x00001A2Bu   // GetCurrentThreadId stub return
#define SHIM_PID  0x00000D04u   // GetCurrentProcessId stub return
#define SHIM_TICK 0x0001D4C0u   // GetTickCount stub return
#define FT_BUF    0x539F00u     // FILETIME scratch (above the stack base, in-image)
#define QPC_BUF   0x539F10u     // LARGE_INTEGER scratch
#define SLIST_BUF 0x539F20u     // SLIST_HEADER scratch (InitializeSListHead zeroes 8B)
// ── BOUNDED-SYNTHETIC TEB/PEB (E5 r6) — OUR own block, NOT the OS TEB ────────
// A few fields, just enough for the security-cookie / CRT-startup chain to read
// fs:[0x18] (TEB self) → [TEB+0x30] (PEB) → [PEB+8] (ImageBaseAddress). own1:
// synthetic, labeled, not a protection structure.
#define TEB_BASE  0x539C00u     // synthetic TEB linear base
#define PEB_BASE  0x539C80u     // synthetic PEB linear base

// Build the hermetic flat image: entry block + the __scrt_common_main
// prologue at the insn-1 CALL target + the synthetic TEB/PEB + still-uncovered
// sentinels elsewhere.
static void build_hermetic(uint8_t *img) {
    memset(img, 0, IMG_SIZE);
    put_at(img, ENTRY_VA, ENTRY_BYTES, sizeof(ENTRY_BYTES));
    put_at(img, 0x539259u, SCRT_PROLOGUE, sizeof(SCRT_PROLOGUE));  // CALL target (insn 1)
    // Synthetic TEB/PEB fields (own1: our own block, labeled, not the OS TEB).
    i386_image_t m = { img, IMG_BASE, IMG_SIZE, 0 };
    i386_mem_write32(&m, TEB_BASE + 0x18, TEB_BASE);   // TEB self-pointer (NtCurrentTeb shape)
    i386_mem_write32(&m, TEB_BASE + 0x30, PEB_BASE);   // TEB→PEB
    i386_mem_write32(&m, PEB_BASE + 0x08, IMG_BASE);   // PEB→ImageBaseAddress
    // 0F A2 = CPUID — still uncovered after r6 (0F B6/B7/BE/BF movzx/movsx now
    // decode+execute). The 0x0F two-byte map recognizes Jcc (80..8F) + the
    // movzx/movsx family only, so 0F A2 → I386_OP_UNKNOWN. Keeps the B/D walls
    // honest and names the next real decoder gap (cpuid, ubiquitous in CRT) for r7.
    uint8_t wall[] = { 0x0F, 0xA2 };               // representative `cpuid`
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
            cpu.iat = &K32_IAT;                                 // native kernel32 binding
            cpu.teb_base = TEB_BASE;                            // synthetic TEB (own1: our block)
            i386_run_result_t r;
            i386_cpu_run(&cpu, &pe, 100000, &r);
            printf("__SHIM__ PARTIAL phase=e4_crt_security_teb insns=%u bound=%u last=%s halt_va=0x%X halt=%s halt_op=%s unbound_slot=0x%X\n",
                   r.insns, r.imports_bound, r.last_import ? r.last_import : "-", r.halt_va,
                   i386_halt_name(r.halt), i386_op_name(r.halt_op), r.import_slot);
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

    // === Run A — MILESTONE: 8 CRT imports + MOVZX/MOVSX + synthetic TEB =====
    // E5 r6 — execution runs the entry CALL (insn 1) → __scrt_common_main
    // prologue (sub/xor/or/cmp/mov/and/test/add — 8 insns, with EFLAGS) → binds
    // FIVE kernel32 imports (GetCurrentThreadId..GetTickCount, with a buffer-
    // writing FILETIME/QPC pair + a shl) → then the r6 continuation: push (19) →
    // IsProcessorFeaturePresent (20) → movzx ecx,al (21) → movsx edx,cl (22) →
    // push (23) → InitializeSListHead (24, BUFFER-ZERO [SLIST_BUF]) → push (25) →
    // GetModuleHandleW (26, → image base) → mov ecx,fs:[0x18] (27, SYNTHETIC TEB
    // self) → mov edx,[ecx+0x30] (28, → synthetic PEB) → mov eax,[edx+8] (29, →
    // ImageBaseAddress) → push (30) → HALTS at the NINTH, UNregistered IAT call
    // @0x5392D4 (UNBOUND_IMPORT — slot 0x538020, the next import, r7).
    printf("\n[cpu] Run A — 8 CRT imports + MOVZX/MOVSX + synthetic TEB→PEB→ImageBase chain\n");
    build_hermetic(img_buf);
    {
        i386_cpu_t cpu; memset(&cpu, 0, sizeof(cpu));
        cpu.eip = ENTRY_VA;
        cpu.gpr[I386_REG_ESP] = 0x539E00u;          // stack inside the image
        cpu.iat = &K32_IAT;                         // register the native kernel32 binding
        cpu.teb_base = TEB_BASE;                     // synthetic TEB (own1: our block, not OS TEB)
        uint32_t esp0 = cpu.gpr[I386_REG_ESP];
        i386_mem_write32(&img, SLIST_BUF + 0, 0xFFFFFFFFu);  // pre-dirty → prove the zeroing
        i386_mem_write32(&img, SLIST_BUF + 4, 0xFFFFFFFFu);
        i386_run_result_t r;
        i386_cpu_run(&cpu, &img, 1000, &r);

        printf("  executed=%u halt=%s halt_va=0x%X eip=0x%X esp=0x%X eax=0x%X ecx=0x%X edx=0x%X bound=%u(%s) slot=0x%X\n",
               r.insns, i386_halt_name(r.halt), r.halt_va, cpu.eip, cpu.gpr[I386_REG_ESP],
               cpu.gpr[I386_REG_EAX], cpu.gpr[I386_REG_ECX], cpu.gpr[I386_REG_EDX],
               r.imports_bound, r.last_import ? r.last_import : "-", r.import_slot);
        uint32_t entry_ret = 0;
        i386_mem_read32(&img, cpu.gpr[I386_REG_ESP] + 4, &entry_ret);  // [esp+4] (under final push)
        uint32_t ft_lo = 0, ft_hi = 0, qpc_lo = 0, qpc_hi = 0, sl_lo = 0, sl_hi = 0;
        i386_mem_read32(&img, FT_BUF + 0, &ft_lo);
        i386_mem_read32(&img, FT_BUF + 4, &ft_hi);
        i386_mem_read32(&img, QPC_BUF + 0, &qpc_lo);
        i386_mem_read32(&img, QPC_BUF + 4, &qpc_hi);
        i386_mem_read32(&img, SLIST_BUF + 0, &sl_lo);
        i386_mem_read32(&img, SLIST_BUF + 4, &sl_hi);

        CHECK(r.insns == 30,                "30 insns (18 r5 + push/IPFP/movzx/movsx/push/ISLH/push/GMHW/3×mov/push)");
        CHECK(r.insns > 18,                 "advanced PAST the r5 18-insn / 0x5392A1 wall");
        CHECK(entry_ret == 0x5388ABu,       "entry CALL return addr 0x5388AB intact under the stdcall pops");
        CHECK(cpu.gpr[I386_REG_ESP] == esp0 - 8, "esp net == esp0-8 (entry retaddr + final unpopped push)");
        CHECK(cpu.gpr[I386_REG_EAX] == IMG_BASE, "[PEB+8] → eax = ImageBaseAddress (synthetic TEB chain)");
        CHECK(cpu.gpr[I386_REG_ECX] == TEB_BASE, "mov ecx,fs:[0x18] → ecx = synthetic TEB self");
        CHECK(cpu.gpr[I386_REG_EDX] == PEB_BASE, "mov edx,[ecx+0x30] → edx = synthetic PEB");
        CHECK(ft_lo == 0xC3D4E5F6u && ft_hi == 0x01D7A1B2u,
                                            "GetSystemTimeAsFileTime wrote 8B FILETIME via the pushed ptr");
        CHECK(qpc_lo == 0x12345678u && qpc_hi == 0x00000000u,
                                            "QueryPerformanceCounter wrote 8B counter via the pushed ptr");
        CHECK(sl_lo == 0u && sl_hi == 0u,   "InitializeSListHead zeroed the 8B SLIST header (was 0xFFFFFFFF)");
        CHECK(r.imports_bound == 8,              "eight IAT calls dispatched to native shims");
        CHECK(r.last_import && strcmp(r.last_import, "GetModuleHandleW") == 0,
                                                 "most-recent bound import == GetModuleHandleW");
        CHECK(r.halt == I386_HALT_UNBOUND_IMPORT, "halt: ninth IAT call is UNbound (r7 boundary)");
        CHECK(r.halt_op == I386_OP_CALL_RM,      "wall op == CALL_RM (FF 15 [disp32])");
        CHECK(r.halt_va == 0x5392D4u,            "wall VA == 0x5392D4 (the ninth IAT call)");
        CHECK(r.import_slot == 0x538020u,        "unbound slot reported == 0x538020 (next import, r7)");
        (void)SHIM_PID; (void)SHIM_TID; (void)SHIM_TICK;

        if (!emitted) {
            printf("__SHIM__ PARTIAL phase=e4_crt_security_teb insns=%u bound=%u last=%s halt_va=0x%X halt=%s halt_op=%s unbound_slot=0x%X\n",
                   r.insns, r.imports_bound, r.last_import ? r.last_import : "-", r.halt_va,
                   i386_halt_name(r.halt), i386_op_name(r.halt_op), r.import_slot);
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
