# C-PORT — log

Append-only history sister of `C-PORT.md`. Each entry starts with `## <ISO timestamp> — <header>` (newest on top); body = `- [x]` (done) / `- [ ]` (pending) checkbox tasks.

## 2026-06-03 — Milestone-1: live native/*.c inventory + classification

- [x] Confirmed live authored .c set: 4 files, 869 LOC (`find native -name '*.c'` + `wc -l`).
- [x] Inspected each file's actual call surface (no fabricated counts — g63 honest).
- [x] Classified: i386_decode.c → A (pure decode tables, libc-format only) · pe_parse.c → C (open/mmap/munmap file ingress) · pe_to_macho_shim.c → C (MAP_JIT/mprotect Mach-O loader) · i386_decode_test.c → B (main()+mmap disasm test driver).
- [x] Aggregate: Tier-A 437 · Tier-B 113 · Tier-C 319 · total 869.
- [x] Wrote classification to `.verdicts/c-port/INVENTORY.txt` (per-file tier + calls + aggregate).
- [x] Registered C-PORT row in `DOMAINS.tape` + tree edge under GAMEBOX-NATIVE.
- [ ] Next: Tier-A port of i386_decode.c → hexa, RUNEQ-gated vs C baseline.
