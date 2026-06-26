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
