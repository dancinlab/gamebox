// native/i386_cpu.h — minimal i386 interpreter (EXECUTE, not just decode).
//
// Phase F-NSWINDOW-E5 (Track DM). Builds on the E2 read-only decoder
// (i386_decode.{c,h}) by adding a register file, a flat VA→host memory
// model, and a fetch→decode→EXECUTE→advance loop. This is the first
// gamebox component that *runs* (mutates state from) real Win32 PE bytes.
//
// Spec ref: Intel SDM Vol. 1 (3.4 registers, 6.3 stack) + Vol. 2
// (instruction semantics). own1: pure Intel SDM semantics over the
// program's OWN bytes. No Wine / CrossOver / Box86 / QEMU / ReactOS
// source. Executing a PE we loaded ourselves != circumventing protection.
//
// Coverage (E5 first batch — matches the entry-block opcodes):
//   PUSH r32 (50+rd) · POP r32 (58+rd) · PUSH imm (68/6A) ·
//   PUSH r/m32 (FF /6, [reg+disp]) · MOV r32,r/m32 (8B) ·
//   MOV r/m32,r32 (89) · CALL rel32 (E8) · JMP rel (E9/EB) · RET (C3/C2).
//
// Coverage (E5 r3 — the __scrt_common_main prologue set, with an EFLAGS
// model — CF/PF/AF/ZF/SF/OF set per SDM ADD/SUB/CMP/logic):
//   MOV r32,imm32 (B8+rd) · MOV r/m32,imm32 (C7 /0) · LEA (8D) ·
//   INC/DEC r32 (40+/48+, CF-preserving) ·
//   ALU r/m,r + r,r/m for ADD/SUB/CMP/XOR/OR/AND + TEST (01/03/29/2B/39/3B/
//     31/33/09/0B/21/23/85) ·
//   group-1 r/m32,imm  ADD/OR/ADC/SBB/AND/SUB/XOR/CMP (81 id / 83 ib).
// Byte-width forms (80, C6, 88, 8A, 84) decode but are NOT executed (width
// not modeled) → honest UNSUPPORTED. Indirect IAT CALL (FF /2 [disp32]) is
// decoded (CALL_RM) but deliberately NOT executed — the E4 kernel32 boundary
// (own1: no IAT resolution / no DRM interaction). Anything else halts
// honestly (UNKNOWN / UNSUPPORTED) with the wall VA.

#ifndef GAMEBOX_I386_CPU_H
#define GAMEBOX_I386_CPU_H

#include <stdint.h>
#include <stddef.h>
#include "i386_decode.h"

// GPR indices (x86 convention). esp = gpr[4], ebp = gpr[5].
enum {
    I386_REG_EAX = 0, I386_REG_ECX = 1, I386_REG_EDX = 2, I386_REG_EBX = 3,
    I386_REG_ESP = 4, I386_REG_EBP = 5, I386_REG_ESI = 6, I386_REG_EDI = 7,
};

typedef struct {
    uint32_t gpr[8];
    uint32_t eip;
    uint32_t eflags;
} i386_cpu_t;

// Flat memory image: a single contiguous host buffer covering the VA
// window [base, base+size). VA→host is host + (va - base). Simple and
// low-risk; the PE loader fills it section-by-section (see
// i386_cpu_load_pe), and the hermetic test fills it by hand.
typedef struct {
    uint8_t *host;
    uint32_t base;   // VA of host[0]
    uint32_t size;   // bytes mapped
    int      owns;   // free(host) on destroy if set
} i386_image_t;

// Why the run loop stopped.
typedef enum {
    I386_HALT_NONE = 0,
    I386_HALT_RET,         // executed RET (entry-block terminator / stack unwind)
    I386_HALT_UNKNOWN,     // decoder returned I386_OP_UNKNOWN (decoder-coverage wall)
    I386_HALT_UNSUPPORTED, // decoded op the interpreter does not yet execute
    I386_HALT_OOB,         // eip / mem access outside the mapped image
    I386_HALT_TRUNC,       // decode_one returned 0 (truncated)
    I386_HALT_GUARD,       // hit the max-step guard
} i386_halt_t;

typedef struct {
    uint32_t   insns;    // instructions fully executed
    i386_halt_t halt;
    uint32_t   halt_va;  // VA where execution stopped
    i386_op_t  halt_op;  // op at the wall (meaningful for UNKNOWN/UNSUPPORTED)
} i386_run_result_t;

// Little-endian 32-bit memory accessors. Return 1 on success, 0 if the
// access falls outside [base, base+size) (caller halts OOB).
int  i386_mem_read32(const i386_image_t *img, uint32_t va, uint32_t *out);
int  i386_mem_write32(const i386_image_t *img, uint32_t va, uint32_t val);

// Run from cpu->eip until a halt condition. Pure SDM semantics over
// img's bytes; mutates cpu + img memory. Never executes outside img.
void i386_cpu_run(i386_cpu_t *cpu, const i386_image_t *img,
                  uint32_t max_steps, i386_run_result_t *res);

// Build a flat image from a real PE32 file (sections copied to their
// virtual addresses; whole [image_base, image_base+size_of_image) is
// allocated and zeroed so the stack region is writable). Returns 0 on
// success and sets *entry_va. own1: read-only PE parse, no Wine.
int  i386_cpu_load_pe(const char *path, i386_image_t *out, uint32_t *entry_va);

void i386_image_free(i386_image_t *img);

const char *i386_halt_name(i386_halt_t h);

#endif // GAMEBOX_I386_CPU_H
