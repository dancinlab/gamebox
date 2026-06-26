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
        default:                    return "?";
    }
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

// Compute the effective address for a memory-form ModR/M (mod != 3).
// Supports the [reg+disp] / [disp32] forms the entry block uses; SIB is
// not yet modeled (returns 0 → caller halts UNSUPPORTED).
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
    *out = addr;
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
    res->insns   = 0;
    res->halt    = I386_HALT_NONE;
    res->halt_va = cpu->eip;
    res->halt_op = I386_OP_UNKNOWN;

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
                cpu->eip = eip + insn.len + (uint32_t)insn.imm;   // same formula as the disassembler
                res->insns++;
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
