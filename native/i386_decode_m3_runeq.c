// native/i386_decode_m3_runeq.c — RUNEQ harness for the C baseline (M3).
//
// C-PORT M3. Drives the REAL static decode_prefixes() + rd_s32() +
// rd_s16() and the public i386_op_name() from i386_decode.c (via
// #include so the static symbols are visible) over the exact same
// deterministic corpus as native/i386_decode.hexa's runeq_dump_m3(),
// emitting byte-identical tab-separated lines. Diffing this output
// against the hexa output IS the RUNEQ gate.
//
// Build:  cc -std=c11 -o /tmp/m3_runeq native/i386_decode_m3_runeq.c
// Run:    /tmp/m3_runeq

#include <stdint.h>
#include <stddef.h>
#include <stdio.h>

// Pull in the real decoder (static decode_prefixes / rd_s32 / rd_s16
// + public i386_op_name).
#include "i386_decode.c"

int main(void) {
    // P: decode_prefixes over each byte alone, avail=1.
    for (int b = 0; b < 256; b++) {
        uint8_t bytes[4] = { (uint8_t)b, 0x90, 0x90, 0x90 };
        uint16_t pre = 0;
        int n = decode_prefixes(bytes, 1, &pre);
        printf("P\t%d\t%d\t%d\n", b, n, (int)pre);
    }
    // P2: each byte then a non-prefix opcode 0x90, avail=4.
    for (int b = 0; b < 256; b++) {
        uint8_t bytes[4] = { (uint8_t)b, 0x90, 0x00, 0x00 };
        uint16_t pre = 0;
        int n = decode_prefixes(bytes, 4, &pre);
        printf("P2\t%d\t%d\t%d\n", b, n, (int)pre);
    }
    // P3: a run of the SAME prefix byte 9 times, avail=9 (n<8 cap).
    int pkinds[11] = { 0x66, 0x67, 0xF2, 0xF3, 0xF0, 0x2E, 0x36, 0x3E, 0x26, 0x64, 0x65 };
    for (int ki = 0; ki < 11; ki++) {
        uint8_t pk = (uint8_t)pkinds[ki];
        uint8_t bytes[9] = { pk, pk, pk, pk, pk, pk, pk, pk, pk };
        uint16_t pre = 0;
        int n = decode_prefixes(bytes, 9, &pre);
        printf("P3\t%d\t%d\t%d\n", pkinds[ki], n, (int)pre);
    }
    // O: op enum 0..33 (33 > max valid → default "(unknown)").
    for (int op = 0; op < 34; op++) {
        printf("O\t%d\t%s\n", op, i386_op_name((i386_op_t)op));
    }
    // R32: rd_s32 over a sweep of the high byte (sign bit), low 3 fixed.
    for (int hb = 0; hb < 256; hb++) {
        uint8_t bytes[4] = { 0x11, 0x22, 0x33, (uint8_t)hb };
        int32_t v = rd_s32(bytes);
        printf("R32\t%d\t%d\n", hb, (int)v);
    }
    // R16: rd_s16 over a sweep of the high byte (sign bit).
    for (int hb = 0; hb < 256; hb++) {
        uint8_t bytes[2] = { 0x44, (uint8_t)hb };
        int16_t v = rd_s16(bytes);
        printf("R16\t%d\t%d\n", hb, (int)v);
    }
    return 0;
}
