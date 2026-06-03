# C-PORT — current state
@title: 🦀→💎 C-PORT — "native C → hexa 이식"

@goal: Drive gamebox's native x86-decoder / PE-shim C to hexa-native wherever no OS-ABI dependency forces C; RUNEQ-gated (byte-identical output vs the C baseline). Irreducible FFI/loader syscalls stay C.

Live authored scope: native/*.c — 4 files, 869 LOC. Classified into Tier A (PORTABLE), Tier B (TEST harness), Tier C (IRREDUCIBLE FFI/platform). See `.verdicts/c-port/INVENTORY.txt`.

- [x] Milestone-1 — inventory + classify live native/*.c (per-file tier + call surface + aggregate LOC)
- [ ] Tier-A port: i386_decode.c (437 LOC) → hexa-native, RUNEQ vs C over real PE .text corpus
- [ ] pe_parse.c split: hexa-native pe_parse_buf(bytes) Tier-A core + thin C/FFI file-map ingress (Tier C)
- [ ] Tier-B retire: drop i386_decode_test.c once hexa decode reaches RUNEQ parity
- [ ] Tier-C documented-stay: pe_to_macho_shim.c MAP_JIT/mprotect loader remains C (or hexa FFI)
