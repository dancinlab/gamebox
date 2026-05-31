# GAMEBOX-NATIVE — current state
@title: 🔩 GAMEBOX-NATIVE — "C 받침대"

@goal: Thin native C helper layer the hexa stack calls into: i386 instruction decode, PE parse, and PE→Mach-O shim — built with entitlements for JIT/mmap.

(edit me — describe current state in completed-form; no history, no changelog inside this file)
- [ ] i386_decode.c — i386 instruction decode helper + test
- [ ] pe_parse.c — PE header/section/import parse helper
- [ ] pe_to_macho_shim.c — PE→Mach-O bridge
- [ ] build.sh + entitlements.plist (JIT/mmap/weak-link Metal)
