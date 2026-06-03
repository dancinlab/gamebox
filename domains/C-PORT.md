# C-PORT — current state
@title: 🦀→💎 C-PORT — "native C → hexa 이식"

@goal: Drive gamebox's native x86-decoder / PE-shim C to hexa-native wherever no OS-ABI dependency forces C; RUNEQ-gated (byte-identical output vs the C baseline). Irreducible FFI/loader syscalls stay C.

Live authored scope: native/*.c — 4 files, 869 LOC. Classified into Tier A (PORTABLE), Tier B (TEST harness), Tier C (IRREDUCIBLE FFI/platform). See `.verdicts/c-port/INVENTORY.txt`.

- [x] Milestone-1 — inventory + classify live native/*.c (per-file tier + call surface + aggregate LOC)
- [x] Milestone-2 — Tier-A leaf port: decode_modrm_disp (ModR/M+SIB+disp, ~43 LOC) + i386_reg32_name → `native/i386_decode.hexa`, RUNEQ PORT-EQ (768/768 byte-identical vs C). Verdict `.verdicts/c-port/M2-decode_modrm_disp.txt`.
- [x] Milestone-3 — Tier-A leaf batch: decode_prefixes (C 72-95) + i386_op_name (C 34-70) + rd_s32/rd_s16 (C 143-152), ~71 LOC → `native/i386_decode.hexa`, RUNEQ PORT-EQ (1069/1069 byte-identical vs C, sha256 8e6acbb8). Per-leaf verdicts `.verdicts/c-port/M3-{decode_prefixes,i386_op_name,rd_s32_rd_s16}.txt`.
- [x] Milestone-4 — Tier-A stateful unit: i386_decode_one (C 154-303, ~150 LOC) — prefix run → full 1-byte/2-byte opcode dispatch → ModR/M+SIB+disp → i386_insn_t field set. Composes the M2/M3 leaves → `native/i386_decode.hexa` (12-tuple decode return). RUNEQ PORT-EQ (1042/1042 byte-identical vs C over a 5-sub-corpus instruction-byte sweep: all 256 opcodes + ModR/M·0x8B + group-5·0xFF + 0x0F two-byte + prefix/short-buffer edges; sha256 b2853d0a). Verdict `.verdicts/c-port/M4-decode_one.txt`. (i386_format_insn = libc snprintf pretty-printer, out of scope — left for follow-up.)
- [ ] Tier-A residual: i386_format_insn (C 305-end) disassembly-string formatter → hexa, RUNEQ on the rendered string (libc snprintf parity)
- [ ] pe_parse.c split: hexa-native pe_parse_buf(bytes) Tier-A core + thin C/FFI file-map ingress (Tier C)
- [ ] Tier-B retire: drop i386_decode_test.c once hexa decode reaches RUNEQ parity
- [ ] Tier-C documented-stay: pe_to_macho_shim.c MAP_JIT/mprotect loader remains C (or hexa FFI)
