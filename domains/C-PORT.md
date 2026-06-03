# C-PORT — current state
@title: 🦀→💎 C-PORT — "native C → hexa 이식"

@goal: Drive gamebox's native x86-decoder / PE-shim C to hexa-native wherever no OS-ABI dependency forces C; RUNEQ-gated (byte-identical output vs the C baseline). Irreducible FFI/loader syscalls stay C.

Live authored scope: native/*.c — 4 files, 869 LOC. Classified into Tier A (PORTABLE), Tier B (TEST harness), Tier C (IRREDUCIBLE FFI/platform). See `.verdicts/c-port/INVENTORY.txt`.

- [x] Milestone-1 — inventory + classify live native/*.c (per-file tier + call surface + aggregate LOC)
- [x] Milestone-2 — Tier-A leaf port: decode_modrm_disp (ModR/M+SIB+disp, ~43 LOC) + i386_reg32_name → `native/i386_decode.hexa`, RUNEQ PORT-EQ (768/768 byte-identical vs C). Verdict `.verdicts/c-port/M2-decode_modrm_disp.txt`.
- [ ] Tier-A port (full): rest of i386_decode.c (i386_decode_one + opcode dispatch + i386_format_insn) → hexa, RUNEQ vs C over real PE .text corpus
- [ ] pe_parse.c split: hexa-native pe_parse_buf(bytes) Tier-A core + thin C/FFI file-map ingress (Tier C)
- [ ] Tier-B retire: drop i386_decode_test.c once hexa decode reaches RUNEQ parity
- [ ] Tier-C documented-stay: pe_to_macho_shim.c MAP_JIT/mprotect loader remains C (or hexa FFI)
