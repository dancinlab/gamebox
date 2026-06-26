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
// not modeled) → honest UNSUPPORTED.
//
// Coverage (E5 r4/r5 — cross the E4 kernel32 boundary):
//   r4: indirect IAT CALL/JMP (FF /2 · FF /4 [disp32]) dispatched to a native
//       kernel32 shim registry (own1: loading our OWN imports, not a bypass).
//   r5: group-2 shift/rotate (C1 /r ib · D1 /r · D3 /r — SHL/SHR/SAL/SAR + ROL/
//       ROR with CF/OF/ZF/SF/PF; RCL/RCR carry-chain not modeled → UNSUPPORTED),
//       plus four more CRT-startup shims including the first BUFFER-WRITING stub
//       (GetSystemTimeAsFileTime — reads a pushed pointer arg, writes 8 bytes
//       into image memory). Anything else halts honestly (UNKNOWN / UNSUPPORTED
//       / UNBOUND_IMPORT) with the wall VA.
//
// Coverage (E5 r6 — CRT security/startup + MOVZX/MOVSX + synthetic TEB):
//   MOVZX/MOVSX r32, r/m8|r/m16 (0F B6/B7/BE/BF — zero/sign-extend, no EFLAGS) ·
//   three more kernel32 shims (IsProcessorFeaturePresent / InitializeSListHead
//   [buffer-zero] / GetModuleHandleW → image base) · a BOUNDED-SYNTHETIC TEB:
//   an FS segment-override (0x64) on a memory operand redirects the effective
//   address into a few-field synthetic TEB/PEB (teb_base; self@+0x18, PEB@+0x30,
//   ImageBase@PEB+8) so the security-cookie / CRT-startup `fs:[..]` chain runs.
//   own1: synthetic block, NOT the OS TEB and NOT a protection structure.

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

struct i386_image;   // fwd (memory model — defined below)

// ── E4 kernel32 IAT binding (E5 r4) ─────────────────────────────────────────
// A native import stub: invoked when execution reaches an indirect IAT call
// `FF 15 [slot_va]` whose slot is registered. It reads its arguments off the
// emulated stack (Win32 stdcall: arg1 at [esp], arg2 at [esp+4], …; there is
// NO return address on top because we intercept AT the call, before it would
// be pushed) and returns the EAX value the call yields.
//
// own1: this is a PE LOADER binding its OWN process's imports to native
// implementations — exactly what every loader does. kernel32 is the OS API
// surface, NOT a protection. Binding GetCurrentThreadId etc. to a native shim
// is LOADING, not bypassing. No DRM / Warden / anti-cheat is touched.
typedef struct i386_cpu i386_cpu_t;
typedef uint32_t (*i386_import_stub_fn)(i386_cpu_t *cpu, const struct i386_image *img);

typedef struct {
    uint32_t            slot_va;    // IAT slot VA == the disp32 of `FF 15 [disp32]`
    const char         *name;      // imported symbol (e.g. "GetCurrentThreadId")
    i386_import_stub_fn fn;        // native shim
    uint32_t            arg_bytes; // stdcall callee-pop bytes (0 for 0-arg / cdecl)
} i386_import_t;

typedef struct {
    const i386_import_t *imports;
    uint32_t             count;
} i386_iat_t;

struct i386_cpu {
    uint32_t gpr[8];
    uint32_t eip;
    uint32_t eflags;
    const i386_iat_t *iat;   // optional import registry (NULL → no bindings)
    // ── E5 r6 SYNTHETIC TEB (NOT the real OS TEB) ───────────────────────────
    // Linear base of a BOUNDED-SYNTHETIC Thread Environment Block modeled in
    // image memory, so the MSVC security-cookie / CRT-startup prologue can read
    // `fs:[disp]` (e.g. fs:[0x18] = TEB self, [TEB+0x30] = PEB, [PEB+8] =
    // ImageBaseAddress). When a memory operand carries the FS segment-override
    // prefix, the interpreter redirects its effective address to teb_base+disp.
    // own1: this is OUR synthetic block (a few fields), NOT the OS-supplied TEB
    // and NOT a protection structure — just enough for the cookie math to run.
    // 0 → no TEB modeled (an fs: access then halts OOB honestly).
    uint32_t teb_base;
};

// Native kernel32 shim — GetCurrentThreadId (WINAPI/stdcall, 0 args, → DWORD).
// own1: returns a deterministic plausible thread id. This is the OS API
// surface bound to a native impl (loading), not a protection bypass.
uint32_t i386_shim_GetCurrentThreadId(i386_cpu_t *cpu, const struct i386_image *img);

// More CRT-startup kernel32 shims (E5 r5). own1: native re-impls of the OS API
// surface bound to the program's OWN imports — loading, not a bypass.
//   GetCurrentProcessId / GetTickCount   — 0-arg, deterministic DWORD.
//   GetSystemTimeAsFileTime(LPFILETIME)  — 1 stdcall ptr arg; WRITES an 8-byte
//       FILETIME into image memory at the pushed pointer (first buffer-writing
//       shim — exercises stub→image writes + a real pushed pointer arg).
//   QueryPerformanceCounter(LARGE_INTEGER*) — 1 ptr arg; writes 8 bytes, → BOOL.
uint32_t i386_shim_GetCurrentProcessId(i386_cpu_t *cpu, const struct i386_image *img);
uint32_t i386_shim_GetTickCount(i386_cpu_t *cpu, const struct i386_image *img);
uint32_t i386_shim_GetSystemTimeAsFileTime(i386_cpu_t *cpu, const struct i386_image *img);
uint32_t i386_shim_QueryPerformanceCounter(i386_cpu_t *cpu, const struct i386_image *img);

// Further CRT-security/startup kernel32 shims (E5 r6). own1: native re-impls of
// the OS API surface bound to the program's OWN imports — loading, not a bypass.
//   IsProcessorFeaturePresent(DWORD)  — 1-arg, returns BOOL 1 (feature present).
//   InitializeSListHead(PSLIST_HEADER)— 1 ptr arg, void; zeroes the 8-byte
//       SLIST header in image memory at the pushed pointer (buffer-write).
//   GetModuleHandleW(LPCWSTR)         — 1 ptr arg, returns HMODULE == the loaded
//       module's image base (the loader knows its own base).
uint32_t i386_shim_IsProcessorFeaturePresent(i386_cpu_t *cpu, const struct i386_image *img);
uint32_t i386_shim_InitializeSListHead(i386_cpu_t *cpu, const struct i386_image *img);
uint32_t i386_shim_GetModuleHandleW(i386_cpu_t *cpu, const struct i386_image *img);

// Look up an IAT slot VA in the registry. Returns NULL if `iat` is NULL or
// the slot is not bound (→ the caller halts UNBOUND_IMPORT honestly).
const i386_import_t *i386_iat_lookup(const i386_iat_t *iat, uint32_t slot_va);

// Flat memory image: a single contiguous host buffer covering the VA
// window [base, base+size). VA→host is host + (va - base). Simple and
// low-risk; the PE loader fills it section-by-section (see
// i386_cpu_load_pe), and the hermetic test fills it by hand.
struct i386_image {
    uint8_t *host;
    uint32_t base;   // VA of host[0]
    uint32_t size;   // bytes mapped
    int      owns;   // free(host) on destroy if set
};
typedef struct i386_image i386_image_t;

// Why the run loop stopped.
typedef enum {
    I386_HALT_NONE = 0,
    I386_HALT_RET,         // executed RET (entry-block terminator / stack unwind)
    I386_HALT_UNKNOWN,     // decoder returned I386_OP_UNKNOWN (decoder-coverage wall)
    I386_HALT_UNSUPPORTED, // decoded op the interpreter does not yet execute
    I386_HALT_OOB,         // eip / mem access outside the mapped image
    I386_HALT_TRUNC,       // decode_one returned 0 (truncated)
    I386_HALT_GUARD,       // hit the max-step guard
    I386_HALT_UNBOUND_IMPORT, // indirect IAT call FF 15 [slot] to an UNregistered
                              // kernel32 import — the next import to bind (own1:
                              // honest stop, we never invent the function)
} i386_halt_t;

typedef struct {
    uint32_t   insns;          // instructions fully executed
    i386_halt_t halt;
    uint32_t   halt_va;        // VA where execution stopped
    i386_op_t  halt_op;        // op at the wall (meaningful for UNKNOWN/UNSUPPORTED)
    uint32_t   imports_bound;  // count of IAT calls dispatched to a native shim
    const char *last_import;   // name of the most recent bound import (NULL if none)
    uint32_t   import_slot;    // slot VA at an UNBOUND_IMPORT halt (else 0)
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
