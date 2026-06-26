// native/i386_decode.h — minimal i386 instruction decoder.
//
// Phase E2 (Track DM): read-only decoder. Decode → struct, no execute.
// Spec ref: Intel SDM Vol. 2 (Instruction Set Reference) — public.
// own1: written from Intel SDM, no Wine source.
//
// Coverage (initial): MOV / PUSH / POP / CALL / RET / JMP / JCC / NOP /
// INT / prefix bytes (0x66 / 0x67 / 0xF2 / 0xF3) / ModR/M / SIB / Imm.
// More opcodes added as future cycles need them.

#ifndef GAMEBOX_I386_DECODE_H
#define GAMEBOX_I386_DECODE_H

#include <stdint.h>
#include <stddef.h>

// Operation enumeration. Only what the decoder currently recognizes.
// Unknown / unimplemented opcodes are reported as I386_OP_UNKNOWN; the
// caller can decide whether to error out or skip.
typedef enum {
    I386_OP_UNKNOWN = 0,
    I386_OP_NOP,
    I386_OP_MOV_R_IMM,        // B8+rd id  / B0+rb ib  (mov reg, imm)
    I386_OP_MOV_RM_R,         // 89 /r     (mov r/m32, r32)
    I386_OP_MOV_R_RM,         // 8B /r     (mov r32, r/m32)
    I386_OP_PUSH_R,           // 50+rd
    I386_OP_PUSH_IMM,         // 68 id  / 6A ib
    I386_OP_POP_R,            // 58+rd
    I386_OP_CALL_REL,         // E8 cd
    I386_OP_CALL_RM,          // FF /2
    I386_OP_RET,              // C3 / C2 iw
    I386_OP_JMP_REL,          // EB cb / E9 cd
    I386_OP_JMP_RM,           // FF /4
    I386_OP_PUSH_RM,          // FF /6
    I386_OP_INC_RM,           // FF /0
    I386_OP_DEC_RM,           // FF /1
    I386_OP_JCC,              // 70..7F cb / 0F 80..8F cd
    I386_OP_INT,              // CD ib  /  CC
    I386_OP_LEA,              // 8D /r
    I386_OP_XOR_RM_R,         // 31 /r
    I386_OP_XOR_R_RM,         // 33 /r
    I386_OP_ADD_RM_R,         // 01 /r
    I386_OP_ADD_R_RM,         // 03 /r
    I386_OP_SUB_RM_R,         // 29 /r
    I386_OP_SUB_R_RM,         // 2B /r
    I386_OP_CMP_RM_R,         // 39 /r
    I386_OP_CMP_R_RM,         // 3B /r
    I386_OP_TEST_RM_R,        // 85 /r
    I386_OP_INC_R,            // 40+rd
    I386_OP_DEC_R,            // 48+rd
    I386_OP_HLT,              // F4
    I386_OP_PREFIX_ONLY,      // standalone prefix run with no opcode (not a valid program; flag)
    // ── E5 r3 additions (ordinals 32+; appended so existing values stay) ──
    // Group-1 r/m32, imm (0x81 /r id, 0x83 /r ib sign-extended). The
    // sub-operation is selected by ModR/M.reg (0..7) → these 8 enums.
    I386_OP_ADD_RM_IMM,       // 81/83 /0 — add r/m32, imm
    I386_OP_OR_RM_IMM,        // 81/83 /1 — or  r/m32, imm
    I386_OP_ADC_RM_IMM,       // 81/83 /2 — adc r/m32, imm
    I386_OP_SBB_RM_IMM,       // 81/83 /3 — sbb r/m32, imm
    I386_OP_AND_RM_IMM,       // 81/83 /4 — and r/m32, imm
    I386_OP_SUB_RM_IMM,       // 81/83 /5 — sub r/m32, imm
    I386_OP_XOR_RM_IMM,       // 81/83 /6 — xor r/m32, imm
    I386_OP_CMP_RM_IMM,       // 81/83 /7 — cmp r/m32, imm
    I386_OP_GRP1_RM8_IMM8,    // 80 /r ib — byte group-1 (width not modeled; UNSUPPORTED)
    I386_OP_MOV_RM_IMM,       // C7 /0 id — mov r/m32, imm32
    I386_OP_MOV_RM8_IMM8,     // C6 /0 ib — mov r/m8, imm8 (byte; UNSUPPORTED)
    I386_OP_MOV_RM8_R8,       // 88 /r    — mov r/m8, r8 (byte; UNSUPPORTED)
    I386_OP_MOV_R8_RM8,       // 8A /r    — mov r8, r/m8 (byte; UNSUPPORTED)
    I386_OP_TEST_RM8_R8,      // 84 /r    — test r/m8, r8 (byte; UNSUPPORTED)
    I386_OP_OR_RM_R,          // 09 /r    — or  r/m32, r32
    I386_OP_OR_R_RM,          // 0B /r    — or  r32, r/m32
    I386_OP_AND_RM_R,         // 21 /r    — and r/m32, r32
    I386_OP_AND_R_RM,         // 23 /r    — and r32, r/m32
} i386_op_t;

#define I386_PREFIX_OPSZ   0x01u
#define I386_PREFIX_ADDRSZ 0x02u
#define I386_PREFIX_REPNZ  0x04u
#define I386_PREFIX_REPZ   0x08u
#define I386_PREFIX_LOCK   0x10u
#define I386_PREFIX_CS     0x20u
#define I386_PREFIX_SS     0x40u
#define I386_PREFIX_DS     0x80u
#define I386_PREFIX_ES     0x100u
#define I386_PREFIX_FS     0x200u
#define I386_PREFIX_GS     0x400u

// One decoded instruction. `len` = total bytes consumed including
// prefixes. `eip` = address (in PE virtual address space) where this
// instruction starts.
typedef struct {
    uint32_t eip;
    uint8_t  bytes[16];  // raw bytes copy (Intel max insn = 15 bytes; +1 slack)
    uint8_t  len;
    uint16_t prefixes;   // bitmask of I386_PREFIX_*
    i386_op_t op;
    int8_t   reg_a;      // primary register (0=EAX, 1=ECX, 2=EDX, 3=EBX, 4=ESP, 5=EBP, 6=ESI, 7=EDI; -1 if none)
    int8_t   reg_b;      // secondary register or -1
    int32_t  imm;        // immediate value (sign-extended for cb/iw/id)
    int      has_imm;
    int      has_disp;
    int32_t  disp;
    uint8_t  modrm;
    uint8_t  sib;
} i386_insn_t;

// Decode a single instruction starting at `code[off]`. Returns total
// bytes consumed (>0) on success. Returns 0 if buffer too short or
// invalid. On unknown opcode, returns >0 with op = I386_OP_UNKNOWN and
// len = 1 (skip one byte).
//
// `eip_base` is the PE-virtual address of `code[0]`. `out->eip` is set
// to `eip_base + off`.
int i386_decode_one(const uint8_t *code, size_t buf_len, size_t off,
                    uint32_t eip_base, i386_insn_t *out);

// Pretty-print a decoded instruction in disassembly style.
// `buf_size` should be at least 96.
void i386_format_insn(const i386_insn_t *insn, char *buf, size_t buf_size);

// Register name lookup (for disassembly).
const char *i386_reg32_name(int r);
const char *i386_op_name(i386_op_t op);

#endif // GAMEBOX_I386_DECODE_H
