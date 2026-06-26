// native/i386_decode.c — minimal i386 instruction decoder.
//
// Phase E2 (Track DM). Read-only — no execution.
// Spec ref: Intel SDM Vol. 2 — public Microsoft/Intel documentation.
// own1: no Wine / CrossOver / GPTK / Whisky / ReactOS source.
//
// Encoding overview (32-bit mode, default operand size = 32):
//   [prefix?]* opcode [modrm? [sib?] [disp?]] [imm?]
//
// Prefix bytes (one byte each, in any order, max ~4 typically):
//   0x66 = operand-size override (32→16)
//   0x67 = address-size override (32→16)
//   0xF2 = REPNZ
//   0xF3 = REP / REPZ
//   0xF0 = LOCK
//   0x2E/0x36/0x3E/0x26/0x64/0x65 = segment overrides (CS/SS/DS/ES/FS/GS)
//
// We accept and skip prefixes. We honor 0x66 in the few places it
// matters for decoding length (e.g. MOV r/m, imm).

#include "i386_decode.h"
#include <stdio.h>
#include <string.h>

static const char *REG32_NAMES[8] = {
    "eax", "ecx", "edx", "ebx", "esp", "ebp", "esi", "edi"
};

const char *i386_reg32_name(int r) {
    if (r < 0 || r > 7) return "?";
    return REG32_NAMES[r];
}

const char *i386_op_name(i386_op_t op) {
    switch (op) {
        case I386_OP_PUSH_RM:   return "push";
        case I386_OP_INC_RM:    return "inc";
        case I386_OP_DEC_RM:    return "dec";
        case I386_OP_NOP:       return "nop";
        case I386_OP_MOV_R_IMM: return "mov";
        case I386_OP_MOV_RM_R:  return "mov";
        case I386_OP_MOV_R_RM:  return "mov";
        case I386_OP_PUSH_R:    return "push";
        case I386_OP_PUSH_IMM:  return "push";
        case I386_OP_POP_R:     return "pop";
        case I386_OP_CALL_REL:  return "call";
        case I386_OP_CALL_RM:   return "call";
        case I386_OP_RET:       return "ret";
        case I386_OP_JMP_REL:   return "jmp";
        case I386_OP_JMP_RM:    return "jmp";
        case I386_OP_JCC:       return "j_cc";
        case I386_OP_INT:       return "int";
        case I386_OP_LEA:       return "lea";
        case I386_OP_XOR_RM_R:
        case I386_OP_XOR_R_RM:  return "xor";
        case I386_OP_ADD_RM_R:
        case I386_OP_ADD_R_RM:  return "add";
        case I386_OP_SUB_RM_R:
        case I386_OP_SUB_R_RM:  return "sub";
        case I386_OP_CMP_RM_R:
        case I386_OP_CMP_R_RM:  return "cmp";
        case I386_OP_TEST_RM_R: return "test";
        case I386_OP_INC_R:     return "inc";
        case I386_OP_DEC_R:     return "dec";
        case I386_OP_HLT:       return "hlt";
        case I386_OP_ADD_RM_IMM: return "add";
        case I386_OP_OR_RM_IMM:  return "or";
        case I386_OP_ADC_RM_IMM: return "adc";
        case I386_OP_SBB_RM_IMM: return "sbb";
        case I386_OP_AND_RM_IMM: return "and";
        case I386_OP_SUB_RM_IMM: return "sub";
        case I386_OP_XOR_RM_IMM: return "xor";
        case I386_OP_CMP_RM_IMM: return "cmp";
        case I386_OP_GRP1_RM8_IMM8: return "grp1b";
        case I386_OP_MOV_RM_IMM: return "mov";
        case I386_OP_MOV_RM8_IMM8: return "mov";
        case I386_OP_MOV_RM8_R8: return "mov";
        case I386_OP_MOV_R8_RM8: return "mov";
        case I386_OP_TEST_RM8_R8: return "test";
        case I386_OP_OR_RM_R:
        case I386_OP_OR_R_RM:   return "or";
        case I386_OP_AND_RM_R:
        case I386_OP_AND_R_RM:  return "and";
        case I386_OP_PREFIX_ONLY: return "(prefix-only)";
        case I386_OP_UNKNOWN:
        default:                return "(unknown)";
    }
}

static int decode_prefixes(const uint8_t *p, size_t avail, uint16_t *prefixes_out) {
    int n = 0;
    uint16_t pre = 0;
    while ((size_t)n < avail && n < 8) {
        uint8_t b = p[n];
        switch (b) {
            case 0x66: pre |= I386_PREFIX_OPSZ;   n++; continue;
            case 0x67: pre |= I386_PREFIX_ADDRSZ; n++; continue;
            case 0xF2: pre |= I386_PREFIX_REPNZ;  n++; continue;
            case 0xF3: pre |= I386_PREFIX_REPZ;   n++; continue;
            case 0xF0: pre |= I386_PREFIX_LOCK;   n++; continue;
            case 0x2E: pre |= I386_PREFIX_CS;     n++; continue;
            case 0x36: pre |= I386_PREFIX_SS;     n++; continue;
            case 0x3E: pre |= I386_PREFIX_DS;     n++; continue;
            case 0x26: pre |= I386_PREFIX_ES;     n++; continue;
            case 0x64: pre |= I386_PREFIX_FS;     n++; continue;
            case 0x65: pre |= I386_PREFIX_GS;     n++; continue;
            default:                              break;
        }
        break;
    }
    *prefixes_out = pre;
    return n;
}

// ModR/M decode helpers — we record raw bytes + reg/disp; do not
// resolve effective address yet (interpreter will).
static int decode_modrm_disp(const uint8_t *p, size_t avail,
                             uint8_t *modrm_out, uint8_t *sib_out,
                             int32_t *disp_out, int *has_disp_out) {
    *has_disp_out = 0;
    *disp_out = 0;
    *sib_out = 0;
    if (avail < 1) return 0;
    uint8_t modrm = p[0];
    *modrm_out = modrm;
    int n = 1;
    int mod = (modrm >> 6) & 3;
    int rm  = modrm & 7;
    int has_sib = (mod != 3 && rm == 4);
    if (has_sib) {
        if ((size_t)n >= avail) return 0;
        *sib_out = p[n];
        n++;
    }
    int disp_size = 0;
    if (mod == 0) {
        if (rm == 5) disp_size = 4;          // [disp32]
        else if (has_sib && (*sib_out & 7) == 5) disp_size = 4;  // SIB base=5 + mod=00 → disp32
    } else if (mod == 1) {
        disp_size = 1;
    } else if (mod == 2) {
        disp_size = 4;
    }
    if (disp_size > 0) {
        if ((size_t)(n + disp_size) > avail) return 0;
        if (disp_size == 1) {
            *disp_out = (int32_t)(int8_t)p[n];
        } else {
            uint32_t u = (uint32_t)p[n]
                       | ((uint32_t)p[n+1] << 8)
                       | ((uint32_t)p[n+2] << 16)
                       | ((uint32_t)p[n+3] << 24);
            *disp_out = (int32_t)u;
        }
        *has_disp_out = 1;
        n += disp_size;
    }
    return n;
}

static int32_t rd_s32(const uint8_t *p) {
    return (int32_t)((uint32_t)p[0]
                   | ((uint32_t)p[1] << 8)
                   | ((uint32_t)p[2] << 16)
                   | ((uint32_t)p[3] << 24));
}

static int16_t rd_s16(const uint8_t *p) {
    return (int16_t)((uint16_t)p[0] | ((uint16_t)p[1] << 8));
}

int i386_decode_one(const uint8_t *code, size_t buf_len, size_t off,
                    uint32_t eip_base, i386_insn_t *out) {
    if (off >= buf_len) return 0;
    memset(out, 0, sizeof(*out));
    out->eip = eip_base + (uint32_t)off;
    out->reg_a = -1;
    out->reg_b = -1;

    const uint8_t *p = code + off;
    size_t avail = buf_len - off;

    int n = decode_prefixes(p, avail, &out->prefixes);
    p += n; avail -= (size_t)n;
    if (avail == 0) {
        out->op = I386_OP_PREFIX_ONLY;
        out->len = (uint8_t)n;
        memcpy(out->bytes, code + off, (size_t)n);
        return n;
    }

    uint8_t op = p[0];
    int taken = n + 1;

    // 1-byte opcode dispatch
    if (op == 0x90) {
        out->op = I386_OP_NOP;
    } else if (op == 0xC3) {
        out->op = I386_OP_RET;
    } else if (op == 0xC2) {
        if (avail < 3) return 0;
        out->op = I386_OP_RET;
        out->imm = rd_s16(p + 1);
        out->has_imm = 1;
        taken += 2;
    } else if (op == 0xCC) {
        out->op = I386_OP_INT;
        out->imm = 3;
        out->has_imm = 1;
    } else if (op == 0xCD) {
        if (avail < 2) return 0;
        out->op = I386_OP_INT;
        out->imm = p[1];
        out->has_imm = 1;
        taken += 1;
    } else if (op == 0xF4) {
        out->op = I386_OP_HLT;
    } else if (op >= 0xB8 && op <= 0xBF) {
        // mov r32, imm32
        if (avail < 5) return 0;
        out->op = I386_OP_MOV_R_IMM;
        out->reg_a = (int8_t)(op - 0xB8);
        out->imm = rd_s32(p + 1);
        out->has_imm = 1;
        taken += 4;
    } else if (op >= 0x50 && op <= 0x57) {
        out->op = I386_OP_PUSH_R;
        out->reg_a = (int8_t)(op - 0x50);
    } else if (op >= 0x58 && op <= 0x5F) {
        out->op = I386_OP_POP_R;
        out->reg_a = (int8_t)(op - 0x58);
    } else if (op >= 0x40 && op <= 0x47) {
        out->op = I386_OP_INC_R;
        out->reg_a = (int8_t)(op - 0x40);
    } else if (op >= 0x48 && op <= 0x4F) {
        out->op = I386_OP_DEC_R;
        out->reg_a = (int8_t)(op - 0x48);
    } else if (op == 0x68) {
        if (avail < 5) return 0;
        out->op = I386_OP_PUSH_IMM;
        out->imm = rd_s32(p + 1);
        out->has_imm = 1;
        taken += 4;
    } else if (op == 0x6A) {
        if (avail < 2) return 0;
        out->op = I386_OP_PUSH_IMM;
        out->imm = (int32_t)(int8_t)p[1];
        out->has_imm = 1;
        taken += 1;
    } else if (op == 0xE8) {
        if (avail < 5) return 0;
        out->op = I386_OP_CALL_REL;
        out->imm = rd_s32(p + 1);
        out->has_imm = 1;
        taken += 4;
    } else if (op == 0xE9) {
        if (avail < 5) return 0;
        out->op = I386_OP_JMP_REL;
        out->imm = rd_s32(p + 1);
        out->has_imm = 1;
        taken += 4;
    } else if (op == 0xEB) {
        if (avail < 2) return 0;
        out->op = I386_OP_JMP_REL;
        out->imm = (int32_t)(int8_t)p[1];
        out->has_imm = 1;
        taken += 1;
    } else if (op >= 0x70 && op <= 0x7F) {
        if (avail < 2) return 0;
        out->op = I386_OP_JCC;
        out->imm = (int32_t)(int8_t)p[1];
        out->reg_a = (int8_t)(op - 0x70);  // condition encoding (0..15)
        out->has_imm = 1;
        taken += 1;
    } else if (op == 0x0F) {
        // 2-byte opcode prefix — we recognize Jcc rel32 (80..8F) only.
        if (avail < 6) return 0;
        uint8_t op2 = p[1];
        if (op2 >= 0x80 && op2 <= 0x8F) {
            out->op = I386_OP_JCC;
            out->reg_a = (int8_t)(op2 - 0x80);
            out->imm = rd_s32(p + 2);
            out->has_imm = 1;
            taken += 5;
        } else {
            out->op = I386_OP_UNKNOWN;
            taken = n + 1;  // skip 0x0F only
        }
    } else if (op == 0x89 || op == 0x8B || op == 0x8D
            || op == 0x01 || op == 0x03
            || op == 0x29 || op == 0x2B
            || op == 0x31 || op == 0x33
            || op == 0x39 || op == 0x3B
            || op == 0x85
            || op == 0x88 || op == 0x8A || op == 0x84   // E5 r3: byte mov + test
            || op == 0x09 || op == 0x0B                 // E5 r3: or
            || op == 0x21 || op == 0x23) {              // E5 r3: and
        // Common /r ModR/M instructions.
        int m_n = decode_modrm_disp(p + 1, avail - 1,
                                    &out->modrm, &out->sib,
                                    &out->disp, &out->has_disp);
        if (m_n == 0) return 0;
        // reg_a = the /r reg field (bits 5..3 of ModR/M).
        out->reg_a = (int8_t)((out->modrm >> 3) & 7);
        // reg_b = if mod==3, the r/m register (bits 2..0); else -1
        if (((out->modrm >> 6) & 3) == 3) {
            out->reg_b = (int8_t)(out->modrm & 7);
        }
        switch (op) {
            case 0x89: out->op = I386_OP_MOV_RM_R; break;
            case 0x8B: out->op = I386_OP_MOV_R_RM; break;
            case 0x8D: out->op = I386_OP_LEA;      break;
            case 0x01: out->op = I386_OP_ADD_RM_R; break;
            case 0x03: out->op = I386_OP_ADD_R_RM; break;
            case 0x29: out->op = I386_OP_SUB_RM_R; break;
            case 0x2B: out->op = I386_OP_SUB_R_RM; break;
            case 0x31: out->op = I386_OP_XOR_RM_R; break;
            case 0x33: out->op = I386_OP_XOR_R_RM; break;
            case 0x39: out->op = I386_OP_CMP_RM_R; break;
            case 0x3B: out->op = I386_OP_CMP_R_RM; break;
            case 0x85: out->op = I386_OP_TEST_RM_R; break;
            case 0x88: out->op = I386_OP_MOV_RM8_R8; break;
            case 0x8A: out->op = I386_OP_MOV_R8_RM8; break;
            case 0x84: out->op = I386_OP_TEST_RM8_R8; break;
            case 0x09: out->op = I386_OP_OR_RM_R;  break;
            case 0x0B: out->op = I386_OP_OR_R_RM;  break;
            case 0x21: out->op = I386_OP_AND_RM_R; break;
            case 0x23: out->op = I386_OP_AND_R_RM; break;
        }
        taken += m_n;
    } else if (op == 0x80 || op == 0x81 || op == 0x83) {
        // Group-1 r/m, imm (add/or/adc/sbb/and/sub/xor/cmp by ModR/M.reg).
        //   0x80 = r/m8, imm8   0x81 = r/m32, imm32   0x83 = r/m32, imm8(sx)
        // Intel SDM Vol.2 — own1, no Wine.
        int m_n = decode_modrm_disp(p + 1, avail - 1,
                                    &out->modrm, &out->sib,
                                    &out->disp, &out->has_disp);
        if (m_n == 0) return 0;
        int reg_field = (out->modrm >> 3) & 7;
        // reg_a carries the r/m base register (interpreter reads modrm directly).
        out->reg_a = (int8_t)(out->modrm & 7);
        int imm_size = (op == 0x81) ? 4 : 1;
        size_t imm_off = (size_t)(1 + m_n);
        if (imm_off + (size_t)imm_size > avail) return 0;
        if (imm_size == 1) {
            out->imm = (int32_t)(int8_t)p[imm_off];   // sign-extended imm8
        } else {
            out->imm = rd_s32(p + imm_off);
        }
        out->has_imm = 1;
        if (op == 0x80) {
            out->op = I386_OP_GRP1_RM8_IMM8;          // byte width — not executed
        } else {
            switch (reg_field) {
                case 0: out->op = I386_OP_ADD_RM_IMM; break;
                case 1: out->op = I386_OP_OR_RM_IMM;  break;
                case 2: out->op = I386_OP_ADC_RM_IMM; break;
                case 3: out->op = I386_OP_SBB_RM_IMM; break;
                case 4: out->op = I386_OP_AND_RM_IMM; break;
                case 5: out->op = I386_OP_SUB_RM_IMM; break;
                case 6: out->op = I386_OP_XOR_RM_IMM; break;
                case 7: out->op = I386_OP_CMP_RM_IMM; break;
            }
        }
        taken += m_n + imm_size;
    } else if (op == 0xC7 || op == 0xC6) {
        // MOV r/m, imm — group-11 /0 (other reg fields are undefined → UNKNOWN).
        //   0xC7 = r/m32, imm32   0xC6 = r/m8, imm8
        int m_n = decode_modrm_disp(p + 1, avail - 1,
                                    &out->modrm, &out->sib,
                                    &out->disp, &out->has_disp);
        if (m_n == 0) return 0;
        int reg_field = (out->modrm >> 3) & 7;
        out->reg_a = (int8_t)(out->modrm & 7);
        if (reg_field != 0) {
            out->op = I386_OP_UNKNOWN;                // only /0 is MOV
        } else {
            int imm_size = (op == 0xC7) ? 4 : 1;
            size_t imm_off = (size_t)(1 + m_n);
            if (imm_off + (size_t)imm_size > avail) return 0;
            if (imm_size == 1) {
                out->imm = (int32_t)(int8_t)p[imm_off];
            } else {
                out->imm = rd_s32(p + imm_off);
            }
            out->has_imm = 1;
            out->op = (op == 0xC7) ? I386_OP_MOV_RM_IMM : I386_OP_MOV_RM8_IMM8;
            taken += imm_size;
        }
        taken += m_n;
    } else if (op == 0xFF) {
        // Group 5 — distinguish CALL r/m32 (/2) and JMP r/m32 (/4) by ModR/M.reg
        int m_n = decode_modrm_disp(p + 1, avail - 1,
                                    &out->modrm, &out->sib,
                                    &out->disp, &out->has_disp);
        if (m_n == 0) return 0;
        int reg_field = (out->modrm >> 3) & 7;
        if (reg_field == 0)      out->op = I386_OP_INC_RM;
        else if (reg_field == 1) out->op = I386_OP_DEC_RM;
        else if (reg_field == 2) out->op = I386_OP_CALL_RM;
        else if (reg_field == 4) out->op = I386_OP_JMP_RM;
        else if (reg_field == 6) out->op = I386_OP_PUSH_RM;
        else                     out->op = I386_OP_UNKNOWN;
        // For group-5: reg_a holds r/m base register (whether mod==3 register-direct
        // or mod==1/2 with displacement). Disassembly distinguishes via mod field.
        out->reg_a = (int8_t)(out->modrm & 7);
        taken += m_n;
    } else {
        // Unknown — flag and consume only the opcode byte (not prefixes
        // again; len = total prefixes + 1).
        out->op = I386_OP_UNKNOWN;
    }

    if ((size_t)taken > 15) {
        // Intel max insn is 15 bytes; cap.
        return 0;
    }
    out->len = (uint8_t)taken;
    memcpy(out->bytes, code + off, (size_t)taken);
    return taken;
}

void i386_format_insn(const i386_insn_t *insn, char *buf, size_t buf_size) {
    char hex[64];
    int hp = 0;
    for (int i = 0; i < insn->len && hp + 3 < (int)sizeof(hex); i++) {
        hp += snprintf(hex + hp, sizeof(hex) - (size_t)hp, "%02X", insn->bytes[i]);
        if (i + 1 < insn->len) {
            if (hp + 1 < (int)sizeof(hex)) hex[hp++] = ' ';
            hex[hp] = 0;
        }
    }
    hex[hp] = 0;

    char operands[64];
    operands[0] = 0;
    int oh = 0;
    switch (insn->op) {
        case I386_OP_MOV_R_IMM:
            oh = snprintf(operands, sizeof(operands), "%s, 0x%X",
                          i386_reg32_name(insn->reg_a),
                          (unsigned)insn->imm);
            break;
        case I386_OP_PUSH_R:
        case I386_OP_POP_R:
        case I386_OP_INC_R:
        case I386_OP_DEC_R:
            oh = snprintf(operands, sizeof(operands), "%s",
                          i386_reg32_name(insn->reg_a));
            break;
        case I386_OP_PUSH_IMM:
            oh = snprintf(operands, sizeof(operands), "0x%X", (unsigned)insn->imm);
            break;
        case I386_OP_CALL_REL:
        case I386_OP_JMP_REL:
            oh = snprintf(operands, sizeof(operands), "0x%X",
                          (unsigned)(insn->eip + insn->len + (uint32_t)insn->imm));
            break;
        case I386_OP_JCC:
            oh = snprintf(operands, sizeof(operands), "cc%X 0x%X",
                          insn->reg_a & 0xF,
                          (unsigned)(insn->eip + insn->len + (uint32_t)insn->imm));
            break;
        case I386_OP_RET:
            if (insn->has_imm) oh = snprintf(operands, sizeof(operands), "0x%X", (unsigned)insn->imm);
            break;
        case I386_OP_INT:
            oh = snprintf(operands, sizeof(operands), "0x%X", (unsigned)insn->imm);
            break;
        case I386_OP_MOV_RM_R:
        case I386_OP_ADD_RM_R:
        case I386_OP_SUB_RM_R:
        case I386_OP_XOR_RM_R:
        case I386_OP_CMP_RM_R:
        case I386_OP_TEST_RM_R:
            if (insn->reg_b >= 0) {
                oh = snprintf(operands, sizeof(operands), "%s, %s",
                              i386_reg32_name(insn->reg_b),
                              i386_reg32_name(insn->reg_a));
            } else {
                oh = snprintf(operands, sizeof(operands), "[modrm=0x%02X], %s",
                              insn->modrm,
                              i386_reg32_name(insn->reg_a));
            }
            break;
        case I386_OP_MOV_R_RM:
        case I386_OP_ADD_R_RM:
        case I386_OP_SUB_R_RM:
        case I386_OP_XOR_R_RM:
        case I386_OP_CMP_R_RM:
        case I386_OP_LEA:
            if (insn->reg_b >= 0) {
                oh = snprintf(operands, sizeof(operands), "%s, %s",
                              i386_reg32_name(insn->reg_a),
                              i386_reg32_name(insn->reg_b));
            } else {
                oh = snprintf(operands, sizeof(operands), "%s, [modrm=0x%02X]",
                              i386_reg32_name(insn->reg_a),
                              insn->modrm);
            }
            break;
        case I386_OP_CALL_RM:
        case I386_OP_JMP_RM:
        case I386_OP_PUSH_RM:
        case I386_OP_INC_RM:
        case I386_OP_DEC_RM:
            if (insn->reg_a >= 0 && ((insn->modrm >> 6) & 3) == 3) {
                oh = snprintf(operands, sizeof(operands), "%s",
                              i386_reg32_name(insn->reg_a));
            } else if (insn->has_disp) {
                int base = insn->modrm & 7;
                oh = snprintf(operands, sizeof(operands), "[%s%+d]",
                              i386_reg32_name(base), (int)insn->disp);
            } else {
                oh = snprintf(operands, sizeof(operands), "[modrm=0x%02X]", insn->modrm);
            }
            break;
        default:
            break;
    }
    (void)oh;

    snprintf(buf, buf_size, "%08X  %-24s  %-6s %s",
             insn->eip, hex, i386_op_name(insn->op), operands);
}
