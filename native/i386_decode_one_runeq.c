// native/i386_decode_one_runeq.c — RUNEQ harness for the C baseline (M4).
//
// C-PORT M4. Drives the REAL public i386_decode_one() from i386_decode.c
// (via #include, the same pattern as the M2/M3 harnesses) over the exact
// same deterministic instruction-byte corpus as native/i386_decode.hexa's
// runeq_dump_m4(), emitting byte-identical tab-separated lines. Diffing
// this output against the hexa output IS the RUNEQ gate (g5/g63).
//
// Row format (struct-field order, omitting eip + bytes[]):
//   tag  off  taken  op  len  prefixes  reg_a  reg_b  imm  has_imm
//        has_disp  disp  modrm  sib
//
// Build:  cc -std=c11 -o /tmp/m4_runeq native/i386_decode_one_runeq.c
// Run:    /tmp/m4_runeq

#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

// Pull in the real decoder (public i386_decode_one + the static leaves
// it composes).
#include "i386_decode.c"

static void emit(const char *tag, size_t off, const uint8_t *code, size_t buf_len) {
    i386_insn_t insn;
    // The hexa port starts from the post-memset(0)+reg_a/reg_b=-1 struct
    // state and mirrors C's partial writes on every `return 0` path. To
    // compare apples-to-apples we pre-seed the same baseline here so that
    // when i386_decode_one returns 0 early (before it would memset), the
    // dumped fields are the documented post-init state. i386_decode_one
    // itself memsets on the non-trivial path; the only path that returns
    // WITHOUT memset is `off >= buf_len`, which the hexa models as the
    // all-default tuple — matched by this pre-seed.
    memset(&insn, 0, sizeof(insn));
    insn.reg_a = -1;
    insn.reg_b = -1;
    int taken = i386_decode_one(code, buf_len, off, 0, &insn);
    printf("%s\t%zu\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n",
           tag, off, taken,
           (int)insn.op, (int)insn.len, (int)insn.prefixes,
           (int)insn.reg_a, (int)insn.reg_b, (int)insn.imm, (int)insn.has_imm,
           (int)insn.has_disp, (int)insn.disp, (int)insn.modrm, (int)insn.sib);
}

int main(void) {
    // Sub-corpus A — every opcode byte 0..255 as a lone insn, fixed
    // 14-byte tail, buf_len=15, off=0.
    for (int o = 0; o < 256; o++) {
        uint8_t code[15] = { (uint8_t)o, 0xC4, 0x37, 0x89, 0xAB, 0xCD, 0xEF,
                             0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xF0 };
        emit("A", 0, code, 15);
    }
    // Sub-corpus B — MOV r,r/m (0x8B) over every ModR/M byte, fixed tail.
    for (int mr = 0; mr < 256; mr++) {
        uint8_t code[15] = { 0x8B, (uint8_t)mr, 0x25, 0x78, 0x9A, 0xBC, 0xFE,
                             0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88 };
        emit("B", 0, code, 15);
    }
    // Sub-corpus C — group-5 (0xFF) over every ModR/M byte.
    for (int mr = 0; mr < 256; mr++) {
        uint8_t code[15] = { 0xFF, (uint8_t)mr, 0x25, 0x78, 0x9A, 0xBC, 0xFE,
                             0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88 };
        emit("C", 0, code, 15);
    }
    // Sub-corpus D — 0x0F two-byte: every second byte 0..255.
    for (int o = 0; o < 256; o++) {
        uint8_t code[15] = { 0x0F, (uint8_t)o, 0x78, 0x9A, 0xBC, 0xFE,
                             0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99 };
        emit("D", 0, code, 15);
    }
    // Sub-corpus E — prefix composition + prefix-only + short-buffer.
    int pk[11] = { 0x66, 0x67, 0xF2, 0xF3, 0xF0, 0x2E, 0x36, 0x3E, 0x26, 0x64, 0x65 };
    for (int ki = 0; ki < 11; ki++) {
        uint8_t code[15] = { (uint8_t)pk[ki], 0x8B, 0xC1, 0,0,0,0,0,0,0,0,0,0,0,0 };
        emit("E", 0, code, 15);
    }
    {
        uint8_t cpo[15] = { 0x66, 0x67, 0xF0, 0,0,0,0,0,0,0,0,0,0,0,0 };
        emit("E", 0, cpo, 3);
    }
    {
        uint8_t s1[15] = { 0xB8, 0x11, 0x22, 0,0,0,0,0,0,0,0,0,0,0,0 };
        emit("E", 0, s1, 3);
    }
    {
        uint8_t s2[15] = { 0xE8, 0x11, 0,0,0,0,0,0,0,0,0,0,0,0,0 };
        emit("E", 0, s2, 2);
    }
    {
        uint8_t s3[15] = { 0xC2, 0x11, 0,0,0,0,0,0,0,0,0,0,0,0,0 };
        emit("E", 0, s3, 2);
    }
    {
        uint8_t s4[15] = { 0xCD, 0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
        emit("E", 0, s4, 1);
    }
    {
        uint8_t s5[15] = { 0x8B, 0x85, 0x11, 0x22, 0,0,0,0,0,0,0,0,0,0,0 };
        emit("E", 0, s5, 4);
    }
    {
        uint8_t s6[15] = { 0x90, 0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
        emit("E", 5, s6, 3);
    }
    return 0;
}
