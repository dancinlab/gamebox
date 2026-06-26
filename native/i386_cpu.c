// native/i386_cpu.c — minimal i386 interpreter implementation.
//
// Phase F-NSWINDOW-E5 (Track DM). See i386_cpu.h for the contract.
// own1: Intel SDM semantics over the program's own bytes only.

#include "i386_cpu.h"
#include "pe_parse.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/mman.h>

const char *i386_halt_name(i386_halt_t h) {
    switch (h) {
        case I386_HALT_NONE:        return "none";
        case I386_HALT_RET:         return "ret";
        case I386_HALT_UNKNOWN:     return "unknown";
        case I386_HALT_UNSUPPORTED: return "unsupported";
        case I386_HALT_OOB:         return "oob";
        case I386_HALT_TRUNC:       return "trunc";
        case I386_HALT_GUARD:       return "guard";
        case I386_HALT_UNBOUND_IMPORT: return "unbound_import";
        case I386_HALT_USER_ENTRY:  return "user_entry";
        default:                    return "?";
    }
}

// ── E4 kernel32 native shim layer (E5 r4) ───────────────────────────────────
// own1: these are native re-implementations of the OS API surface, bound by
// the loader to the program's OWN imports — what every PE loader does. They
// are NOT a protection bypass (kernel32 is the platform API, not DRM/Warden).
// Values are deterministic + plausible and documented as STUBS (not the real
// kernel call), so the run is honest about being a loader probe, not a frame.

uint32_t i386_shim_GetCurrentThreadId(i386_cpu_t *cpu, const struct i386_image *img) {
    (void)cpu; (void)img;
    return 0x00001A2Bu;   // stub TID — deterministic; not the real OS thread id
}

// GetCurrentProcessId (WINAPI/stdcall, 0 args, → DWORD). own1: a deterministic
// plausible PID; the OS API surface bound to a native impl, not a bypass.
uint32_t i386_shim_GetCurrentProcessId(i386_cpu_t *cpu, const struct i386_image *img) {
    (void)cpu; (void)img;
    return 0x00000D04u;   // stub PID (3332) — deterministic; not the real OS pid
}

// GetTickCount (WINAPI/stdcall, 0 args, → DWORD ms since boot). own1: stub.
uint32_t i386_shim_GetTickCount(i386_cpu_t *cpu, const struct i386_image *img) {
    (void)cpu; (void)img;
    return 0x0001D4C0u;   // stub tick = 120000 ms — deterministic, not real uptime
}

// GetSystemTimeAsFileTime(LPFILETIME lpft) — WINAPI/stdcall, 1 pointer arg,
// returns void. This is the first BUFFER-WRITING shim: it reads the caller's
// pushed pointer arg off the emulated stack ([esp], since we intercept BEFORE
// the call's return address would be pushed) and writes an 8-byte FILETIME
// (dwLowDateTime, dwHighDateTime) into image memory at that pointer.
// own1: a CRT-startup OS-API call (time, not DRM/Warden); a native impl writing
// a deterministic timestamp into the program's OWN buffer — loading, not bypass.
uint32_t i386_shim_GetSystemTimeAsFileTime(i386_cpu_t *cpu, const struct i386_image *img) {
    uint32_t pft;                                  // LPFILETIME arg
    if (!i386_mem_read32(img, cpu->gpr[I386_REG_ESP], &pft)) return 0;
    // Deterministic FILETIME (100ns ticks since 1601). Plausible, fixed.
    i386_mem_write32(img, pft + 0, 0xC3D4E5F6u);   // dwLowDateTime
    i386_mem_write32(img, pft + 4, 0x01D7A1B2u);   // dwHighDateTime
    return 0;                                       // void → EAX irrelevant
}

// QueryPerformanceCounter(LARGE_INTEGER *lpPerformanceCount) — WINAPI/stdcall,
// 1 pointer arg, returns BOOL (nonzero = success). Buffer-writing: writes an
// 8-byte counter into image memory at the pushed pointer ([esp]). own1: stub.
uint32_t i386_shim_QueryPerformanceCounter(i386_cpu_t *cpu, const struct i386_image *img) {
    uint32_t plc;                                  // LARGE_INTEGER* arg
    if (!i386_mem_read32(img, cpu->gpr[I386_REG_ESP], &plc)) return 0;
    i386_mem_write32(img, plc + 0, 0x12345678u);   // QuadPart low  (deterministic)
    i386_mem_write32(img, plc + 4, 0x00000000u);   // QuadPart high
    return 1;                                       // BOOL success
}

// IsProcessorFeaturePresent(DWORD ProcessorFeature) — WINAPI/stdcall, 1 arg,
// returns BOOL. own1: deterministically claim the queried feature is present.
// The CRT startup probes this (e.g. PF_XMMI64_INSTRUCTIONS) to pick code paths;
// it is a capability query on the OS API surface, not a protection mechanism.
uint32_t i386_shim_IsProcessorFeaturePresent(i386_cpu_t *cpu, const struct i386_image *img) {
    (void)cpu; (void)img;
    return 1;                                       // feature present (deterministic)
}

// InitializeSListHead(PSLIST_HEADER ListHead) — WINAPI/stdcall, 1 ptr arg,
// returns void. Buffer-writing: zeroes the 8-byte SLIST header in image memory
// at the pushed pointer ([esp]). own1: a CRT-startup OS-API call initializing
// the program's OWN list head — loading, not a bypass.
uint32_t i386_shim_InitializeSListHead(i386_cpu_t *cpu, const struct i386_image *img) {
    uint32_t phead;                                // PSLIST_HEADER arg
    if (!i386_mem_read32(img, cpu->gpr[I386_REG_ESP], &phead)) return 0;
    i386_mem_write32(img, phead + 0, 0u);          // zero the 8-byte (x86) header
    i386_mem_write32(img, phead + 4, 0u);
    return 0;                                       // void → EAX irrelevant
}

// GetModuleHandleW(LPCWSTR lpModuleName) — WINAPI/stdcall, 1 ptr arg, returns
// HMODULE. own1: with NULL (the common startup call) this returns the calling
// module's base — which the LOADER knows exactly: the image base. Not a bypass.
uint32_t i386_shim_GetModuleHandleW(i386_cpu_t *cpu, const struct i386_image *img) {
    (void)cpu;
    return img->base;                               // HMODULE == loaded image base
}

// GetStartupInfoW(LPSTARTUPINFOW lpStartupInfo) — WINAPI/stdcall, 1 ptr arg,
// returns void. Buffer-writing: zeroes a STARTUPINFOW (0x44 bytes on x86) in
// image memory at the pushed pointer ([esp]) and stamps cb = 0x44 at offset 0.
// own1: a CRT-startup OS-API call filling the program's OWN buffer — loading.
uint32_t i386_shim_GetStartupInfoW(i386_cpu_t *cpu, const struct i386_image *img) {
    uint32_t psi;                                   // LPSTARTUPINFOW arg
    if (!i386_mem_read32(img, cpu->gpr[I386_REG_ESP], &psi)) return 0;
    for (uint32_t o = 0; o < 0x44u; o += 4) i386_mem_write32(img, psi + o, 0u);
    i386_mem_write32(img, psi + 0x00u, 0x44u);      // cb = sizeof(STARTUPINFOW) (x86)
    return 0;                                       // void → EAX irrelevant
}

// GetSystemInfo(LPSYSTEM_INFO lpSystemInfo) — WINAPI/stdcall, 1 ptr arg, void.
// Buffer-writing: a BOUNDED-SYNTHETIC SYSTEM_INFO (0x24 bytes on x86) — a few
// plausible fields (page size / processor count / type), the rest zeroed.
// own1: native re-impl of the OS API filling the program's OWN buffer — loading.
uint32_t i386_shim_GetSystemInfo(i386_cpu_t *cpu, const struct i386_image *img) {
    uint32_t psi;                                   // LPSYSTEM_INFO arg
    if (!i386_mem_read32(img, cpu->gpr[I386_REG_ESP], &psi)) return 0;
    for (uint32_t o = 0; o < 0x24u; o += 4) i386_mem_write32(img, psi + o, 0u);
    i386_mem_write32(img, psi + 0x00u, 0x00000000u);// wProcessorArchitecture = INTEL(0)
    i386_mem_write32(img, psi + 0x04u, 0x00001000u);// dwPageSize = 4096
    i386_mem_write32(img, psi + 0x14u, 0x00000004u);// dwNumberOfProcessors = 4 (synthetic)
    i386_mem_write32(img, psi + 0x18u, 0x000006F0u);// dwProcessorType (synthetic)
    return 0;                                       // void → EAX irrelevant
}

// GetProcAddress(HMODULE hModule, LPCSTR lpProcName) — WINAPI/stdcall, 2 args,
// returns FARPROC. own1: the LOADER hands back a SYNTHETIC in-image stub address
// (we never resolve to a real OS export here); a later round can bind it. Not a
// bypass — this is import resolution, exactly what a loader does.
uint32_t i386_shim_GetProcAddress(i386_cpu_t *cpu, const struct i386_image *img) {
    uint32_t hmod = 0, pname = 0;
    i386_mem_read32(img, cpu->gpr[I386_REG_ESP] + 0, &hmod);   // arg1 hModule
    i386_mem_read32(img, cpu->gpr[I386_REG_ESP] + 4, &pname);  // arg2 lpProcName
    (void)hmod; (void)pname;
    return img->base + 0x1FF0u;                     // synthetic in-image stub addr
}

// GetCommandLineW() — WINAPI/stdcall, 0 args, returns LPWSTR. own1: the LOADER
// hands the program a SYNTHETIC in-image command-line pointer (its own argv
// blob). The CRT-startup `__scrt_common_main_seh` reads this before invoking the
// user entry. Not a bypass — this is the program's OWN command line.
uint32_t i386_shim_GetCommandLineW(i386_cpu_t *cpu, const struct i386_image *img) {
    (void)cpu;
    return img->base + 0x1FE0u;                     // synthetic in-image cmdline ptr
}

// SetUnhandledExceptionFilter(LPTOP_LEVEL_EXCEPTION_FILTER) — WINAPI/stdcall, 1
// ptr arg, returns the PREVIOUS top-level filter. own1: a CRT-startup OS-API
// call installing the program's OWN handler; we return NULL (none installed).
// An exception-handler registration, NOT a protection / anti-debug mechanism.
uint32_t i386_shim_SetUnhandledExceptionFilter(i386_cpu_t *cpu, const struct i386_image *img) {
    uint32_t pfn = 0;
    i386_mem_read32(img, cpu->gpr[I386_REG_ESP], &pfn);   // arg (unused — no real SEH)
    (void)pfn;
    return 0;                                       // previous filter == NULL
}

const i386_import_t *i386_iat_lookup(const i386_iat_t *iat, uint32_t slot_va) {
    if (!iat || !iat->imports) return NULL;
    for (uint32_t i = 0; i < iat->count; i++) {
        if (iat->imports[i].slot_va == slot_va) return &iat->imports[i];
    }
    return NULL;
}

int i386_mem_read32(const i386_image_t *img, uint32_t va, uint32_t *out) {
    if (va < img->base || (uint64_t)va + 4 > (uint64_t)img->base + img->size) return 0;
    const uint8_t *p = img->host + (va - img->base);
    *out = (uint32_t)p[0] | ((uint32_t)p[1] << 8)
         | ((uint32_t)p[2] << 16) | ((uint32_t)p[3] << 24);
    return 1;
}

int i386_mem_write32(const i386_image_t *img, uint32_t va, uint32_t val) {
    if (va < img->base || (uint64_t)va + 4 > (uint64_t)img->base + img->size) return 0;
    uint8_t *p = img->host + (va - img->base);
    p[0] = (uint8_t)(val);
    p[1] = (uint8_t)(val >> 8);
    p[2] = (uint8_t)(val >> 16);
    p[3] = (uint8_t)(val >> 24);
    return 1;
}

// Byte / word little-endian reads (E5 r6 — for MOVZX/MOVSX r/m8|r/m16 sources).
static int mem_read8(const i386_image_t *img, uint32_t va, uint8_t *out) {
    if (va < img->base || (uint64_t)va + 1 > (uint64_t)img->base + img->size) return 0;
    *out = img->host[va - img->base];
    return 1;
}
static int mem_read16(const i386_image_t *img, uint32_t va, uint16_t *out) {
    if (va < img->base || (uint64_t)va + 2 > (uint64_t)img->base + img->size) return 0;
    const uint8_t *p = img->host + (va - img->base);
    *out = (uint16_t)((uint16_t)p[0] | ((uint16_t)p[1] << 8));
    return 1;
}

// Compute the effective address for a memory-form ModR/M (mod != 3).
// Supports the [reg+disp] / [disp32] forms the entry block uses; SIB is
// not yet modeled (returns 0 → caller halts UNSUPPORTED). E5 r6: an FS
// segment-override prefix redirects the address into the synthetic TEB
// (own1: our own block, not the OS TEB — see i386_cpu.h teb_base).
static int i386_ea(const i386_cpu_t *cpu, const i386_insn_t *insn, uint32_t *out) {
    int mod = (insn->modrm >> 6) & 3;
    int rm  = insn->modrm & 7;
    if (mod == 3) return 0;   // register-direct, not a memory operand
    if (rm == 4)  return 0;   // SIB byte present — not modeled yet
    uint32_t addr;
    if (mod == 0 && rm == 5) {
        addr = (uint32_t)insn->disp;                  // [disp32]
    } else {
        addr = cpu->gpr[rm] + (insn->has_disp ? (uint32_t)insn->disp : 0);
    }
    if (insn->prefixes & I386_PREFIX_FS) {
        addr += cpu->teb_base;                        // fs:[disp] → synthetic TEB
    }
    *out = addr;
    return 1;
}

// Read an 8-bit r/m operand. Register-direct uses the x86 byte-register map
// (rm 0..3 = low byte of eax/ecx/edx/ebx; rm 4..7 = high byte ah/ch/dh/bh).
static int rm_get8(const i386_cpu_t *cpu, const i386_image_t *img,
                   const i386_insn_t *insn, uint8_t *val, i386_halt_t *why) {
    int mod = (insn->modrm >> 6) & 3;
    if (mod == 3) {
        int rm = insn->modrm & 7;
        if (rm < 4) *val = (uint8_t)(cpu->gpr[rm] & 0xFFu);
        else        *val = (uint8_t)((cpu->gpr[rm - 4] >> 8) & 0xFFu);
        return 1;
    }
    uint32_t a;
    if (!i386_ea(cpu, insn, &a)) { *why = I386_HALT_UNSUPPORTED; return 0; }
    if (!mem_read8(img, a, val)) { *why = I386_HALT_OOB; return 0; }
    return 1;
}

// Read a 16-bit r/m operand. Register-direct = low word of gpr[rm].
static int rm_get16(const i386_cpu_t *cpu, const i386_image_t *img,
                    const i386_insn_t *insn, uint16_t *val, i386_halt_t *why) {
    int mod = (insn->modrm >> 6) & 3;
    if (mod == 3) { *val = (uint16_t)(cpu->gpr[insn->modrm & 7] & 0xFFFFu); return 1; }
    uint32_t a;
    if (!i386_ea(cpu, insn, &a)) { *why = I386_HALT_UNSUPPORTED; return 0; }
    if (!mem_read16(img, a, val)) { *why = I386_HALT_OOB; return 0; }
    return 1;
}

// ── EFLAGS model (E5 r3) — Intel SDM Vol.1 §3.4.3. Only the arithmetic /
// logic status flags we can compute exactly from 32-bit operands. ──────────
#define EFL_CF 0x00000001u
#define EFL_PF 0x00000004u
#define EFL_AF 0x00000010u
#define EFL_ZF 0x00000040u
#define EFL_SF 0x00000080u
#define EFL_OF 0x00000800u

static void set_flag(uint32_t *f, uint32_t mask, int on) {
    if (on) *f |= mask; else *f &= ~mask;
}

// Set SF/ZF/PF from a 32-bit result (PF reflects the low byte, per SDM).
static void set_szp32(uint32_t *f, uint32_t res) {
    set_flag(f, EFL_ZF, res == 0);
    set_flag(f, EFL_SF, (res >> 31) & 1u);
    uint8_t b = (uint8_t)res; int ones = 0;
    for (int i = 0; i < 8; i++) ones += (b >> i) & 1;
    set_flag(f, EFL_PF, (ones & 1) == 0);
}

// a + b + cin → result, with CF/OF/AF/SF/ZF/PF set per SDM ADD/ADC.
static uint32_t alu_add(uint32_t *f, uint32_t a, uint32_t b, uint32_t cin) {
    uint64_t r = (uint64_t)a + (uint64_t)b + (uint64_t)cin;
    uint32_t res = (uint32_t)r;
    set_szp32(f, res);
    set_flag(f, EFL_CF, (int)((r >> 32) & 1u));
    set_flag(f, EFL_AF, (int)(((a ^ b ^ res) >> 4) & 1u));
    set_flag(f, EFL_OF, (int)(((~(a ^ b) & (a ^ res)) >> 31) & 1u));
    return res;
}

// a - b - bin → result, with CF(borrow)/OF/AF/SF/ZF/PF set per SDM SUB/SBB/CMP.
static uint32_t alu_sub(uint32_t *f, uint32_t a, uint32_t b, uint32_t bin) {
    uint64_t r = (uint64_t)a - (uint64_t)b - (uint64_t)bin;
    uint32_t res = (uint32_t)r;
    set_szp32(f, res);
    set_flag(f, EFL_CF, (int)((r >> 32) & 1u));   // borrow out
    set_flag(f, EFL_AF, (int)(((a ^ b ^ res) >> 4) & 1u));
    set_flag(f, EFL_OF, (int)((((a ^ b) & (a ^ res)) >> 31) & 1u));
    return res;
}

// Logical result → SF/ZF/PF set; CF/OF cleared; AF cleared (SDM: undefined).
static uint32_t alu_logic(uint32_t *f, uint32_t res) {
    set_szp32(f, res);
    set_flag(f, EFL_CF, 0);
    set_flag(f, EFL_OF, 0);
    set_flag(f, EFL_AF, 0);
    return res;
}

// Read the 32-bit r/m operand (register-direct or [mem]). Returns 1 on
// success; on failure sets *why (UNSUPPORTED for un-modeled EA, OOB for a
// memory access outside the image) and returns 0.
static int rm_get32(const i386_cpu_t *cpu, const i386_image_t *img,
                    const i386_insn_t *insn, uint32_t *val, i386_halt_t *why) {
    int mod = (insn->modrm >> 6) & 3;
    if (mod == 3) { *val = cpu->gpr[insn->modrm & 7]; return 1; }
    uint32_t a;
    if (!i386_ea(cpu, insn, &a)) { *why = I386_HALT_UNSUPPORTED; return 0; }
    if (!i386_mem_read32(img, a, val)) { *why = I386_HALT_OOB; return 0; }
    return 1;
}

// Write the 32-bit r/m operand (register-direct or [mem]). Same failure
// contract as rm_get32.
static int rm_set32(i386_cpu_t *cpu, const i386_image_t *img,
                    const i386_insn_t *insn, uint32_t val, i386_halt_t *why) {
    int mod = (insn->modrm >> 6) & 3;
    if (mod == 3) { cpu->gpr[insn->modrm & 7] = val; return 1; }
    uint32_t a;
    if (!i386_ea(cpu, insn, &a)) { *why = I386_HALT_UNSUPPORTED; return 0; }
    if (!i386_mem_write32(img, a, val)) { *why = I386_HALT_OOB; return 0; }
    return 1;
}

void i386_cpu_run(i386_cpu_t *cpu, const i386_image_t *img,
                  uint32_t max_steps, i386_run_result_t *res) {
    res->insns         = 0;
    res->halt          = I386_HALT_NONE;
    res->halt_va       = cpu->eip;
    res->halt_op       = I386_OP_UNKNOWN;
    res->imports_bound = 0;
    res->last_import   = NULL;
    res->import_slot   = 0;

    for (uint32_t step = 0; step < max_steps; step++) {
        uint32_t eip = cpu->eip;

        // FETCH — eip must be inside the mapped image.
        if (eip < img->base || eip >= img->base + img->size) {
            res->halt = I386_HALT_OOB; res->halt_va = eip; return;
        }
        size_t off = (size_t)(eip - img->base);
        i386_insn_t insn;
        int n = i386_decode_one(img->host, img->size, off, img->base, &insn);
        if (n == 0) { res->halt = I386_HALT_TRUNC; res->halt_va = eip; return; }
        if (insn.op == I386_OP_UNKNOWN) {
            // The decoder-coverage wall. Honest stop — record the VA.
            res->halt = I386_HALT_UNKNOWN; res->halt_va = eip; res->halt_op = insn.op; return;
        }

        int mod = (insn.modrm >> 6) & 3;

        // EXECUTE.
        switch (insn.op) {
            case I386_OP_PUSH_R: {
                cpu->gpr[I386_REG_ESP] -= 4;
                if (!i386_mem_write32(img, cpu->gpr[I386_REG_ESP], cpu->gpr[insn.reg_a])) {
                    res->halt = I386_HALT_OOB; res->halt_va = eip; return;
                }
                break;
            }
            case I386_OP_POP_R: {
                uint32_t v;
                if (!i386_mem_read32(img, cpu->gpr[I386_REG_ESP], &v)) {
                    res->halt = I386_HALT_OOB; res->halt_va = eip; return;
                }
                cpu->gpr[insn.reg_a] = v;
                cpu->gpr[I386_REG_ESP] += 4;
                break;
            }
            case I386_OP_PUSH_IMM: {
                cpu->gpr[I386_REG_ESP] -= 4;
                if (!i386_mem_write32(img, cpu->gpr[I386_REG_ESP], (uint32_t)insn.imm)) {
                    res->halt = I386_HALT_OOB; res->halt_va = eip; return;
                }
                break;
            }
            case I386_OP_PUSH_RM: {
                uint32_t v;
                if (mod == 3) {
                    v = cpu->gpr[insn.reg_a];   // group-5 decoder stores base reg in reg_a
                } else {
                    uint32_t addr;
                    if (!i386_ea(cpu, &insn, &addr)) {
                        res->halt = I386_HALT_UNSUPPORTED; res->halt_va = eip; res->halt_op = insn.op; return;
                    }
                    if (!i386_mem_read32(img, addr, &v)) {
                        res->halt = I386_HALT_OOB; res->halt_va = eip; return;
                    }
                }
                cpu->gpr[I386_REG_ESP] -= 4;
                if (!i386_mem_write32(img, cpu->gpr[I386_REG_ESP], v)) {
                    res->halt = I386_HALT_OOB; res->halt_va = eip; return;
                }
                break;
            }
            case I386_OP_MOV_R_RM: {            // 8B /r : mov r32(reg field), r/m32
                int dst = (insn.modrm >> 3) & 7;
                uint32_t v;
                if (mod == 3) {
                    v = cpu->gpr[insn.modrm & 7];
                } else {
                    uint32_t addr;
                    if (!i386_ea(cpu, &insn, &addr)) {
                        res->halt = I386_HALT_UNSUPPORTED; res->halt_va = eip; res->halt_op = insn.op; return;
                    }
                    if (!i386_mem_read32(img, addr, &v)) {
                        res->halt = I386_HALT_OOB; res->halt_va = eip; return;
                    }
                }
                cpu->gpr[dst] = v;
                break;
            }
            case I386_OP_MOV_RM_R: {            // 89 /r : mov r/m32, r32(reg field)
                int src = (insn.modrm >> 3) & 7;
                uint32_t v = cpu->gpr[src];
                if (mod == 3) {
                    cpu->gpr[insn.modrm & 7] = v;
                } else {
                    uint32_t addr;
                    if (!i386_ea(cpu, &insn, &addr)) {
                        res->halt = I386_HALT_UNSUPPORTED; res->halt_va = eip; res->halt_op = insn.op; return;
                    }
                    if (!i386_mem_write32(img, addr, v)) {
                        res->halt = I386_HALT_OOB; res->halt_va = eip; return;
                    }
                }
                break;
            }
            case I386_OP_CALL_REL: {            // E8 cd
                uint32_t ret = eip + insn.len;
                cpu->gpr[I386_REG_ESP] -= 4;
                if (!i386_mem_write32(img, cpu->gpr[I386_REG_ESP], ret)) {
                    res->halt = I386_HALT_OOB; res->halt_va = eip; return;
                }
                uint32_t target = eip + insn.len + (uint32_t)insn.imm;  // disassembler formula
                cpu->eip = target;
                res->insns++;
                // CRT→user-entry handoff: this CALL transfers control to the
                // user entry (main/WinMain). Faithful "about to enter" — the
                // return address is already pushed. Honest E4 milestone, NOT a
                // rendered frame. own1: we stop AT the handoff; we do not run
                // the (absent) real game body.
                if (cpu->user_entry_va && target == cpu->user_entry_va) {
                    res->halt = I386_HALT_USER_ENTRY; res->halt_va = target;
                    res->halt_op = insn.op; return;
                }
                continue;                       // branch — skip linear advance
            }
            case I386_OP_JMP_REL: {             // E9 cd / EB cb
                cpu->eip = eip + insn.len + (uint32_t)insn.imm;
                res->insns++;
                continue;
            }
            case I386_OP_RET: {                 // C3 / C2 iw
                uint32_t ret;
                if (!i386_mem_read32(img, cpu->gpr[I386_REG_ESP], &ret)) {
                    res->halt = I386_HALT_OOB; res->halt_va = eip; return;
                }
                cpu->gpr[I386_REG_ESP] += 4;
                if (insn.has_imm) cpu->gpr[I386_REG_ESP] += (uint32_t)insn.imm;
                cpu->eip = ret;
                res->insns++;
                res->halt = I386_HALT_RET; res->halt_va = ret;   // entry-block terminator
                return;
            }
            case I386_OP_MOV_R_IMM: {          // B8+rd id : mov r32, imm32
                cpu->gpr[insn.reg_a] = (uint32_t)insn.imm;
                break;
            }
            case I386_OP_MOV_RM_IMM: {          // C7 /0 id : mov r/m32, imm32
                i386_halt_t why = I386_HALT_UNSUPPORTED;
                if (!rm_set32(cpu, img, &insn, (uint32_t)insn.imm, &why)) {
                    res->halt = why; res->halt_va = eip; res->halt_op = insn.op; return;
                }
                break;
            }
            case I386_OP_LEA: {                 // 8D /r : lea r32, [mem]
                uint32_t addr;
                if (!i386_ea(cpu, &insn, &addr)) {
                    res->halt = I386_HALT_UNSUPPORTED; res->halt_va = eip; res->halt_op = insn.op; return;
                }
                cpu->gpr[(insn.modrm >> 3) & 7] = addr;
                break;
            }
            case I386_OP_INC_R: {               // 40+rd : inc r32 (CF preserved)
                uint32_t cf = cpu->eflags & EFL_CF;
                cpu->gpr[insn.reg_a] = alu_add(&cpu->eflags, cpu->gpr[insn.reg_a], 1, 0);
                cpu->eflags = (cpu->eflags & ~EFL_CF) | cf;
                break;
            }
            case I386_OP_DEC_R: {               // 48+rd : dec r32 (CF preserved)
                uint32_t cf = cpu->eflags & EFL_CF;
                cpu->gpr[insn.reg_a] = alu_sub(&cpu->eflags, cpu->gpr[insn.reg_a], 1, 0);
                cpu->eflags = (cpu->eflags & ~EFL_CF) | cf;
                break;
            }
            // ── ALU r/m,r and r,r/m (reg field = second operand) ──────────
            case I386_OP_ADD_RM_R: case I386_OP_ADD_R_RM:
            case I386_OP_SUB_RM_R: case I386_OP_SUB_R_RM:
            case I386_OP_CMP_RM_R: case I386_OP_CMP_R_RM:
            case I386_OP_XOR_RM_R: case I386_OP_XOR_R_RM:
            case I386_OP_OR_RM_R:  case I386_OP_OR_R_RM:
            case I386_OP_AND_RM_R: case I386_OP_AND_R_RM:
            case I386_OP_TEST_RM_R: {
                int reg = (insn.modrm >> 3) & 7;
                i386_halt_t why = I386_HALT_UNSUPPORTED;
                uint32_t rm_v;
                if (!rm_get32(cpu, img, &insn, &rm_v, &why)) {
                    res->halt = why; res->halt_va = eip; res->halt_op = insn.op; return;
                }
                uint32_t reg_v = cpu->gpr[reg];
                uint32_t out_v = 0; int wb_rm = 0, wb_reg = 0;
                switch (insn.op) {
                    case I386_OP_ADD_RM_R: out_v = alu_add(&cpu->eflags, rm_v, reg_v, 0); wb_rm = 1; break;
                    case I386_OP_ADD_R_RM: out_v = alu_add(&cpu->eflags, reg_v, rm_v, 0); wb_reg = 1; break;
                    case I386_OP_SUB_RM_R: out_v = alu_sub(&cpu->eflags, rm_v, reg_v, 0); wb_rm = 1; break;
                    case I386_OP_SUB_R_RM: out_v = alu_sub(&cpu->eflags, reg_v, rm_v, 0); wb_reg = 1; break;
                    case I386_OP_CMP_RM_R: (void)alu_sub(&cpu->eflags, rm_v, reg_v, 0); break;
                    case I386_OP_CMP_R_RM: (void)alu_sub(&cpu->eflags, reg_v, rm_v, 0); break;
                    case I386_OP_XOR_RM_R: out_v = alu_logic(&cpu->eflags, rm_v ^ reg_v); wb_rm = 1; break;
                    case I386_OP_XOR_R_RM: out_v = alu_logic(&cpu->eflags, reg_v ^ rm_v); wb_reg = 1; break;
                    case I386_OP_OR_RM_R:  out_v = alu_logic(&cpu->eflags, rm_v | reg_v); wb_rm = 1; break;
                    case I386_OP_OR_R_RM:  out_v = alu_logic(&cpu->eflags, reg_v | rm_v); wb_reg = 1; break;
                    case I386_OP_AND_RM_R: out_v = alu_logic(&cpu->eflags, rm_v & reg_v); wb_rm = 1; break;
                    case I386_OP_AND_R_RM: out_v = alu_logic(&cpu->eflags, reg_v & rm_v); wb_reg = 1; break;
                    case I386_OP_TEST_RM_R:(void)alu_logic(&cpu->eflags, rm_v & reg_v); break;
                    default: break;
                }
                if (wb_rm) {
                    if (!rm_set32(cpu, img, &insn, out_v, &why)) {
                        res->halt = why; res->halt_va = eip; res->halt_op = insn.op; return;
                    }
                } else if (wb_reg) {
                    cpu->gpr[reg] = out_v;
                }
                break;
            }
            // ── Group-1 r/m32, imm (0x81 id / 0x83 ib) ────────────────────
            case I386_OP_ADD_RM_IMM: case I386_OP_OR_RM_IMM:
            case I386_OP_ADC_RM_IMM: case I386_OP_SBB_RM_IMM:
            case I386_OP_AND_RM_IMM: case I386_OP_SUB_RM_IMM:
            case I386_OP_XOR_RM_IMM: case I386_OP_CMP_RM_IMM: {
                i386_halt_t why = I386_HALT_UNSUPPORTED;
                uint32_t rm_v;
                if (!rm_get32(cpu, img, &insn, &rm_v, &why)) {
                    res->halt = why; res->halt_va = eip; res->halt_op = insn.op; return;
                }
                uint32_t imm = (uint32_t)insn.imm;
                uint32_t cf = (cpu->eflags & EFL_CF) ? 1u : 0u;
                uint32_t out_v = 0; int wb = 1;
                switch (insn.op) {
                    case I386_OP_ADD_RM_IMM: out_v = alu_add(&cpu->eflags, rm_v, imm, 0);  break;
                    case I386_OP_ADC_RM_IMM: out_v = alu_add(&cpu->eflags, rm_v, imm, cf); break;
                    case I386_OP_SUB_RM_IMM: out_v = alu_sub(&cpu->eflags, rm_v, imm, 0);  break;
                    case I386_OP_SBB_RM_IMM: out_v = alu_sub(&cpu->eflags, rm_v, imm, cf); break;
                    case I386_OP_CMP_RM_IMM: (void)alu_sub(&cpu->eflags, rm_v, imm, 0); wb = 0; break;
                    case I386_OP_AND_RM_IMM: out_v = alu_logic(&cpu->eflags, rm_v & imm); break;
                    case I386_OP_OR_RM_IMM:  out_v = alu_logic(&cpu->eflags, rm_v | imm); break;
                    case I386_OP_XOR_RM_IMM: out_v = alu_logic(&cpu->eflags, rm_v ^ imm); break;
                    default: wb = 0; break;
                }
                if (wb) {
                    if (!rm_set32(cpu, img, &insn, out_v, &why)) {
                        res->halt = why; res->halt_va = eip; res->halt_op = insn.op; return;
                    }
                }
                break;
            }
            // ── Group-2 shift/rotate r/m32 (E5 r5) — C1 /r ib · D1 /r · D3 /r ─
            // Sub-op by ModR/M.reg; count by encoding (imm8 / 1 / CL), masked to
            // 5 bits per SDM Vol.2. SHL/SHR/SAL/SAR + ROL/ROR with CF/OF/ZF/SF/PF.
            // RCL/RCR (reg 2/3) carry-chain not modeled → honest UNSUPPORTED.
            case I386_OP_SHIFT_RM_IMM8:
            case I386_OP_SHIFT_RM_1:
            case I386_OP_SHIFT_RM_CL: {
                i386_halt_t why = I386_HALT_UNSUPPORTED;
                uint32_t v;
                if (!rm_get32(cpu, img, &insn, &v, &why)) {
                    res->halt = why; res->halt_va = eip; res->halt_op = insn.op; return;
                }
                uint32_t count = (insn.op == I386_OP_SHIFT_RM_CL)
                               ? (cpu->gpr[I386_REG_ECX] & 0xFFu)
                               : (uint32_t)insn.imm;
                count &= 0x1Fu;                      // SDM: 32-bit shift count masked to 5 bits
                int sub = (insn.modrm >> 3) & 7;     // group-2 operation selector
                if (count == 0) {
                    break;                           // SDM: count==0 → no change, no flags
                }
                uint32_t out_v;
                switch (sub) {
                    case 4: case 6: {                // SHL / SAL
                        uint32_t cf = (v >> (32 - count)) & 1u;
                        out_v = v << count;
                        set_szp32(&cpu->eflags, out_v);
                        set_flag(&cpu->eflags, EFL_CF, (int)cf);
                        if (count == 1)
                            set_flag(&cpu->eflags, EFL_OF, (int)(((out_v >> 31) & 1u) ^ cf));
                        break;
                    }
                    case 5: {                        // SHR (logical)
                        uint32_t cf = (v >> (count - 1)) & 1u;
                        out_v = v >> count;
                        set_szp32(&cpu->eflags, out_v);
                        set_flag(&cpu->eflags, EFL_CF, (int)cf);
                        if (count == 1)
                            set_flag(&cpu->eflags, EFL_OF, (int)((v >> 31) & 1u));
                        break;
                    }
                    case 7: {                        // SAR (arithmetic)
                        uint32_t cf = (v >> (count - 1)) & 1u;
                        out_v = (uint32_t)((int32_t)v >> count);
                        set_szp32(&cpu->eflags, out_v);
                        set_flag(&cpu->eflags, EFL_CF, (int)cf);
                        if (count == 1) set_flag(&cpu->eflags, EFL_OF, 0);
                        break;
                    }
                    case 0: {                        // ROL (rotates don't touch SZP)
                        out_v = (v << count) | (v >> (32 - count));
                        uint32_t cf = out_v & 1u;
                        set_flag(&cpu->eflags, EFL_CF, (int)cf);
                        if (count == 1)
                            set_flag(&cpu->eflags, EFL_OF, (int)(((out_v >> 31) & 1u) ^ cf));
                        break;
                    }
                    case 1: {                        // ROR
                        out_v = (v >> count) | (v << (32 - count));
                        uint32_t msb = (out_v >> 31) & 1u;
                        set_flag(&cpu->eflags, EFL_CF, (int)msb);
                        if (count == 1)
                            set_flag(&cpu->eflags, EFL_OF, (int)(msb ^ ((out_v >> 30) & 1u)));
                        break;
                    }
                    default:                          // RCL/RCR (2/3) — not modeled
                        res->halt = I386_HALT_UNSUPPORTED; res->halt_va = eip;
                        res->halt_op = insn.op; return;
                }
                if (!rm_set32(cpu, img, &insn, out_v, &why)) {
                    res->halt = why; res->halt_va = eip; res->halt_op = insn.op; return;
                }
                break;
            }
            // ── MOVZX / MOVSX r32, r/m8|r/m16 (E5 r6) — 0F B6/B7/BE/BF ──────
            // Zero/sign-extend a byte or word source into the 32-bit dst
            // register (ModR/M.reg). No EFLAGS change (Intel SDM Vol.2).
            case I386_OP_MOVZX_RM8:
            case I386_OP_MOVSX_RM8: {
                i386_halt_t why = I386_HALT_UNSUPPORTED;
                uint8_t b;
                if (!rm_get8(cpu, img, &insn, &b, &why)) {
                    res->halt = why; res->halt_va = eip; res->halt_op = insn.op; return;
                }
                uint32_t v = (insn.op == I386_OP_MOVSX_RM8)
                           ? (uint32_t)(int32_t)(int8_t)b   // sign-extend
                           : (uint32_t)b;                   // zero-extend
                cpu->gpr[(insn.modrm >> 3) & 7] = v;
                break;
            }
            case I386_OP_MOVZX_RM16:
            case I386_OP_MOVSX_RM16: {
                i386_halt_t why = I386_HALT_UNSUPPORTED;
                uint16_t w;
                if (!rm_get16(cpu, img, &insn, &w, &why)) {
                    res->halt = why; res->halt_va = eip; res->halt_op = insn.op; return;
                }
                uint32_t v = (insn.op == I386_OP_MOVSX_RM16)
                           ? (uint32_t)(int32_t)(int16_t)w  // sign-extend
                           : (uint32_t)w;                   // zero-extend
                cpu->gpr[(insn.modrm >> 3) & 7] = v;
                break;
            }
            // ── IMUL r32, r/m32[, imm] (E5 r7) — 0F AF · 69 /r id · 6B /r ib ─
            // 32-bit signed multiply. Low 32 bits of the product → dst register
            // (ModR/M.reg). CF=OF=1 if the full signed product does not fit in a
            // sign-extended 32-bit result (per SDM Vol.2 IMUL); SF/ZF/AF/PF
            // undefined (left unchanged). own1: a plain CPU multiply.
            case I386_OP_IMUL_R_RM:
            case I386_OP_IMUL_R_RM_IMM: {
                int dst = (insn.modrm >> 3) & 7;
                i386_halt_t why = I386_HALT_UNSUPPORTED;
                uint32_t rm_v;
                if (!rm_get32(cpu, img, &insn, &rm_v, &why)) {
                    res->halt = why; res->halt_va = eip; res->halt_op = insn.op; return;
                }
                int64_t a = (insn.op == I386_OP_IMUL_R_RM_IMM)
                          ? (int64_t)(int32_t)rm_v        // src1 = r/m32
                          : (int64_t)(int32_t)cpu->gpr[dst];
                int64_t b = (insn.op == I386_OP_IMUL_R_RM_IMM)
                          ? (int64_t)(int32_t)insn.imm    // src2 = imm
                          : (int64_t)(int32_t)rm_v;       // src2 = r/m32
                int64_t prod = a * b;
                uint32_t low = (uint32_t)prod;
                cpu->gpr[dst] = low;
                int ovf = (prod != (int64_t)(int32_t)low);
                set_flag(&cpu->eflags, EFL_CF, ovf);
                set_flag(&cpu->eflags, EFL_OF, ovf);
                break;
            }
            // ── CPUID (E5 r7) — 0F A2 — SYNTHETIC CPU-identity / feature set ─
            // Leaf in EAX selects the result. We return a plausible synthetic
            // set (vendor "GenuineIntel" + a max-leaf for leaf 0; family/feature
            // bits for leaf 1). own1: CPUID is a plain CPU instruction — the CRT
            // probes it to pick code paths; NOT a protection mechanism.
            case I386_OP_CPUID: {
                uint32_t leaf = cpu->gpr[I386_REG_EAX];
                if (leaf == 0) {
                    cpu->gpr[I386_REG_EAX] = 0x00000016u;   // max basic leaf (synthetic)
                    cpu->gpr[I386_REG_EBX] = 0x756E6547u;   // "Genu"
                    cpu->gpr[I386_REG_EDX] = 0x49656E69u;   // "ineI"
                    cpu->gpr[I386_REG_ECX] = 0x6C65746Eu;   // "ntel"  → "GenuineIntel"
                } else if (leaf == 1) {
                    cpu->gpr[I386_REG_EAX] = 0x000006F0u;   // family/model/stepping (synthetic)
                    cpu->gpr[I386_REG_EBX] = 0x00000000u;
                    cpu->gpr[I386_REG_ECX] = 0x00000201u;   // a couple ECX feature bits (synthetic)
                    cpu->gpr[I386_REG_EDX] = 0x078BFBFFu;   // common base EDX feature bits (synthetic)
                } else {
                    cpu->gpr[I386_REG_EAX] = 0u; cpu->gpr[I386_REG_EBX] = 0u;
                    cpu->gpr[I386_REG_ECX] = 0u; cpu->gpr[I386_REG_EDX] = 0u;
                }
                break;
            }
            // ── RDTSC (E5 r7) — 0F 31 — SYNTHETIC monotonic timestamp counter ─
            // Each rdtsc advances cpu->tsc by a fixed step and returns the running
            // 64-bit count in edx:eax. own1: a deterministic synthetic counter,
            // NOT the host TSC and NOT a protection clock.
            case I386_OP_RDTSC: {
                cpu->tsc += I386_TSC_STEP;
                cpu->gpr[I386_REG_EAX] = (uint32_t)cpu->tsc;
                cpu->gpr[I386_REG_EDX] = (uint32_t)(cpu->tsc >> 32);
                break;
            }
            // ── BT / BTS / BTR / BTC r/m32, r32 (E5 r8) — 0F A3/AB/B3/BB ─────
            // CF ← bit(r/m, index). The bit index is the ModR/M.reg register.
            // For a register-direct operand the index is masked mod 32 (SDM);
            // for a memory operand we model the same 32-bit-window form (bounded
            // — bit base + index/8 byte-stride not modeled). BTS/BTR/BTC write
            // the modified value back. own1: plain Intel SDM Vol.2 bit ops.
            case I386_OP_BT_RM_R:  case I386_OP_BTS_RM_R:
            case I386_OP_BTR_RM_R: case I386_OP_BTC_RM_R: {
                i386_halt_t why = I386_HALT_UNSUPPORTED;
                uint32_t v;
                if (!rm_get32(cpu, img, &insn, &v, &why)) {
                    res->halt = why; res->halt_va = eip; res->halt_op = insn.op; return;
                }
                uint32_t idx = cpu->gpr[(insn.modrm >> 3) & 7] & 0x1Fu;  // bit index
                uint32_t bit = (v >> idx) & 1u;
                set_flag(&cpu->eflags, EFL_CF, (int)bit);
                uint32_t out_v = v;
                int wb = 1;
                switch (insn.op) {
                    case I386_OP_BT_RM_R:  wb = 0;                       break;
                    case I386_OP_BTS_RM_R: out_v = v |  (1u << idx);     break;
                    case I386_OP_BTR_RM_R: out_v = v & ~(1u << idx);     break;
                    case I386_OP_BTC_RM_R: out_v = v ^  (1u << idx);     break;
                    default: break;
                }
                if (wb) {
                    if (!rm_set32(cpu, img, &insn, out_v, &why)) {
                        res->halt = why; res->halt_va = eip; res->halt_op = insn.op; return;
                    }
                }
                break;
            }
            // ── E4 kernel32 boundary — indirect IAT call / jump ───────────
            case I386_OP_CALL_RM:               // FF /2 [..]
            case I386_OP_JMP_RM: {              // FF /4 [..]
                // The canonical IAT thunk is the memory-indirect [disp32] form
                // (ModR/M mod=00, rm=101): `FF 15 [slot]` (call) / `FF 25 [slot]`
                // (tail jump). insn.disp holds the IAT slot VA. Other indirect
                // forms (register / [reg] vtable dispatch) are NOT imports →
                // honest UNSUPPORTED, unchanged from r3.
                int rm = insn.modrm & 7;
                if (!(mod == 0 && rm == 5)) {
                    res->halt = I386_HALT_UNSUPPORTED; res->halt_va = eip; res->halt_op = insn.op; return;
                }
                uint32_t slot_va = (uint32_t)insn.disp;
                const i386_import_t *imp = i386_iat_lookup(cpu->iat, slot_va);
                if (!imp) {
                    // Slot is an IAT thunk but no binding is registered — the
                    // next kernel32 import to implement. own1: we never invent
                    // the function; we stop and name the slot.
                    res->halt = I386_HALT_UNBOUND_IMPORT; res->halt_va = eip;
                    res->halt_op = insn.op; res->import_slot = slot_va; return;
                }
                // BIND: dispatch to the native shim, place the result in EAX
                // (Win32 ABI), pop stdcall callee-popped args, and continue.
                // own1: loading the OS API to a native impl, not a bypass.
                uint32_t eax = imp->fn(cpu, img);
                cpu->gpr[I386_REG_EAX] = eax;
                cpu->gpr[I386_REG_ESP] += imp->arg_bytes;   // stdcall callee-pop
                res->last_import = imp->name;
                res->imports_bound++;
                res->insns++;
                if (insn.op == I386_OP_JMP_RM) {
                    // Tail-call thunk: the import's RET returns to the ORIGINAL
                    // caller, so emulate that return (pop the return address).
                    uint32_t ret;
                    if (!i386_mem_read32(img, cpu->gpr[I386_REG_ESP], &ret)) {
                        res->halt = I386_HALT_OOB; res->halt_va = eip; return;
                    }
                    cpu->gpr[I386_REG_ESP] += 4;
                    cpu->eip = ret;
                } else {
                    cpu->eip = eip + insn.len;              // call: fall through
                }
                continue;
            }
            default: {
                // Decoded, but the interpreter does not execute it yet.
                res->halt = I386_HALT_UNSUPPORTED; res->halt_va = eip; res->halt_op = insn.op; return;
            }
        }

        // Linear advance for non-branch instructions.
        cpu->eip = eip + insn.len;
        res->insns++;
    }

    res->halt = I386_HALT_GUARD; res->halt_va = cpu->eip;
}

int i386_cpu_load_pe(const char *path, i386_image_t *out, uint32_t *entry_va) {
    memset(out, 0, sizeof(*out));

    pe_image_t pe;
    if (pe_parse_file(path, &pe) != 0 || !pe.ok) return -1;
    if (pe.machine != 0x14C) return -2;          // not i386
    if (pe.size_of_image == 0 || pe.size_of_image > (256u << 20)) return -3;

    uint8_t *flat = (uint8_t *)calloc(1, pe.size_of_image);
    if (!flat) return -4;

    int fd = open(path, O_RDONLY);
    if (fd < 0) { free(flat); return -5; }
    struct stat st;
    if (fstat(fd, &st) < 0) { close(fd); free(flat); return -6; }
    size_t fsz = (size_t)st.st_size;
    void *map = mmap(NULL, fsz, PROT_READ, MAP_PRIVATE, fd, 0);
    close(fd);
    if (map == MAP_FAILED) { free(flat); return -7; }

    // Copy each section's raw bytes to its virtual address in the flat image.
    for (uint16_t i = 0; i < pe.num_sections; i++) {
        const pe_section_t *s = &pe.sections[i];
        uint32_t vaddr = s->virtual_address;
        uint32_t rawsz = s->size_of_raw_data;
        uint32_t rawp  = s->pointer_to_raw_data;
        if (rawsz == 0) continue;
        if ((uint64_t)vaddr + rawsz > pe.size_of_image) continue;
        if ((uint64_t)rawp + rawsz > fsz) continue;
        memcpy(flat + vaddr, (const uint8_t *)map + rawp, rawsz);
    }
    munmap(map, fsz);

    out->host = flat;
    out->base = (uint32_t)pe.image_base;
    out->size = pe.size_of_image;
    out->owns = 1;
    if (entry_va) *entry_va = (uint32_t)(pe.image_base + pe.entry_point_rva);
    return 0;
}

void i386_image_free(i386_image_t *img) {
    if (img && img->owns && img->host) {
        free(img->host);
        img->host = NULL;
        img->owns = 0;
    }
}
