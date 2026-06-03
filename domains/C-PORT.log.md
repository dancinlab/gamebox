# C-PORT — log

Append-only history sister of `C-PORT.md`. Each entry starts with `## <ISO timestamp> — <header>` (newest on top); body = `- [x]` (done) / `- [ ]` (pending) checkbox tasks.

## 2026-06-03 — Milestone-3: Tier-A leaf batch — decode_prefixes + i386_op_name + rd_s32/rd_s16 → hexa, RUNEQ PORT-EQ

- [x] Picked the next 4 PURE leaves from tier-A i386_decode.c (deterministic input→output, no I/O): `decode_prefixes` (C 72-95, ~24 LOC, prefix-byte run → (n, bitmask)), `i386_op_name` (C 34-70, ~37 LOC, op enum→mnemonic), `rd_s32` (C 143-148) + `rd_s16` (C 150-152, ~10 LOC, little-endian signed readers). ~71 LOC total.
- [x] Ported all 4 into `native/i386_decode.hexa` (extends the M2 file). Prefix bits as constant fns matching the i386_decode.h #defines; op enum ordinals from header declaration order (0..31); rd_s32 reuses M2's sx32, rd_s16 sign-extends inline. self_test extended with 11 M3 assertions — PASS.
- [x] C RUNEQ harness `native/i386_decode_m3_runeq.c` — `#include`s i386_decode.c to drive the real static decode_prefixes/rd_s32/rd_s16 + public i386_op_name over an identical deterministic corpus (REUSE of the M2 #include-the-.c pattern).
- [x] RUNEQ corpus 1069 rows: P (256, each byte avail=1) + P2 (256, byte+opcode avail=4) + P3 (11, 9-long prefix run, n<8 cap) + O (34, op enum 0..33 incl. 2 OOB→default) + R32 (256 sign sweep) + R16 (256 sign sweep). Tab-sep.
- [x] RUNEQ: byte-identical 1069/1069 on first run (no DIFFER — these leaves have no write-before-fail trap like M2's). sha256 match 8e6acbb8... Per-leaf verdicts `.verdicts/c-port/M3-decode_prefixes.txt`, `M3-i386_op_name.txt`, `M3-rd_s32_rd_s16.txt`.
- [ ] Next: port `i386_decode_one` + `i386_format_insn` (the remaining stateful Tier-A unit), RUNEQ over a real PE .text corpus.

## 2026-06-03 — Milestone-2: Tier-A leaf port — decode_modrm_disp → hexa, RUNEQ PORT-EQ

- [x] Picked the smallest substantive PURE leaf from tier-A i386_decode.c: `decode_modrm_disp` (C 99-141, ~43 LOC) — ModR/M + SIB + displacement field extractor; pure bytes→fields, no I/O. Bundled trivial `i386_reg32_name` (C 25-32) lookup leaf.
- [x] Ported to `native/i386_decode.hexa` (first .hexa under native/). 64-bit `int` arith; disp values manually sign-extended (sx8/sx32) to match C int32_t; failure paths return n=0.
- [x] C RUNEQ harness `native/i386_decode_modrm_runeq.c` — `#include`s i386_decode.c to drive the real static fn over an identical deterministic corpus.
- [x] RUNEQ corpus: all 256 ModR/M bytes (avail=6) + all 256 short-buffer (avail=1) + all 256 disp8 sign values = 768 rows; tab-sep (n modrm sib disp has_disp).
- [x] First run DIFFERED (real, honest): avail=1 + rm==4 rows — C writes *modrm_out BEFORE the SIB-avail check, so returns n=0 with modrm SET. Fixed hexa failure tuples to `(0, modrm, ...)` to match the write-before-fail output-param contract.
- [x] Re-run: byte-identical 768/768, sha256 match (b4388479...). Verdict `.verdicts/c-port/M2-decode_modrm_disp.txt`. self_test PASS.
- [ ] Next: port the full 1-byte/2-byte opcode dispatch + i386_decode_one + i386_format_insn (rest of the 437-LOC Tier-A unit), RUNEQ over real PE .text corpus.

## 2026-06-03 — Milestone-1: live native/*.c inventory + classification

- [x] Confirmed live authored .c set: 4 files, 869 LOC (`find native -name '*.c'` + `wc -l`).
- [x] Inspected each file's actual call surface (no fabricated counts — g63 honest).
- [x] Classified: i386_decode.c → A (pure decode tables, libc-format only) · pe_parse.c → C (open/mmap/munmap file ingress) · pe_to_macho_shim.c → C (MAP_JIT/mprotect Mach-O loader) · i386_decode_test.c → B (main()+mmap disasm test driver).
- [x] Aggregate: Tier-A 437 · Tier-B 113 · Tier-C 319 · total 869.
- [x] Wrote classification to `.verdicts/c-port/INVENTORY.txt` (per-file tier + calls + aggregate).
- [x] Registered C-PORT row in `DOMAINS.tape` + tree edge under GAMEBOX-NATIVE.
- [ ] Next: Tier-A port of i386_decode.c → hexa, RUNEQ-gated vs C baseline.
