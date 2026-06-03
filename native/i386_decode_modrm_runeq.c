// native/i386_decode_modrm_runeq.c — RUNEQ harness for the C baseline.
//
// C-PORT M2. Drives the REAL static decode_modrm_disp() from
// i386_decode.c (via #include so the static symbol is visible) over the
// exact same deterministic corpus as native/i386_decode.hexa's
// runeq_dump(), emitting byte-identical tab-separated lines. Diffing
// this output against the hexa output IS the RUNEQ gate.
//
// Build:  cc -std=c11 -o /tmp/modrm_runeq native/i386_decode_modrm_runeq.c
// Run:    /tmp/modrm_runeq

#include <stdint.h>
#include <stddef.h>
#include <stdio.h>

// Pull in the real decoder (gives us the static decode_modrm_disp).
#include "i386_decode.c"

int main(void) {
    // Corpus 1: every ModR/M byte 0..255 + fixed 5-byte tail, avail=6.
    for (int modrm = 0; modrm < 256; modrm++) {
        uint8_t bytes[6] = { (uint8_t)modrm, 0x25, 0x78, 0x9A, 0xBC, 0xFE };
        uint8_t m_out = 0, s_out = 0;
        int32_t d_out = 0;
        int h_out = 0;
        int n = decode_modrm_disp(bytes, 6, &m_out, &s_out, &d_out, &h_out);
        printf("%d\t%d\t%d\t%d\t%d\t%d\n",
               modrm, n, (int)m_out, (int)s_out, (int)d_out, h_out);
    }
    // Corpus 2: short buffer, avail=1.
    for (int modrm = 0; modrm < 256; modrm++) {
        uint8_t bytes[6] = { (uint8_t)modrm, 0, 0, 0, 0, 0 };
        uint8_t m_out = 0, s_out = 0;
        int32_t d_out = 0;
        int h_out = 0;
        int n = decode_modrm_disp(bytes, 1, &m_out, &s_out, &d_out, &h_out);
        printf("S1\t%d\t%d\t%d\t%d\t%d\t%d\n",
               modrm, n, (int)m_out, (int)s_out, (int)d_out, h_out);
    }
    // Corpus 3: mod=1 disp8 across all sign values (modrm=0x45).
    for (int db = 0; db < 256; db++) {
        uint8_t bytes[6] = { 0x45, (uint8_t)db, 0, 0, 0, 0 };
        uint8_t m_out = 0, s_out = 0;
        int32_t d_out = 0;
        int h_out = 0;
        int n = decode_modrm_disp(bytes, 6, &m_out, &s_out, &d_out, &h_out);
        printf("D8\t%d\t%d\t%d\n", db, n, (int)d_out);
    }
    return 0;
}
