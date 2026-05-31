# GAMEBOX-LOADER — current state
@title: 🧱 GAMEBOX-LOADER — "PE 적재기"

@goal: hexa-only PE loader: map PE32/PE32+ images, i386/x64 decode, IAT resolve, base relocations, SEH chains, TLS callbacks, MSVC CRT init — with Wine=0 (no Wine dependency).

(edit me — describe current state in completed-form; no history, no changelog inside this file)
- [ ] PE32/PE32+ section mapper + base relocation applier (real mmap)
- [ ] i386 + x64 instruction decode + calling-convention translation to arm64
- [ ] IAT resolve (direct/forwarder/delay-load/hint fast-path) real binding
- [ ] SEH x64 .pdata unwinder + i386 FS:[0] chain dispatch
- [ ] TLS callback dispatch + MSVC CRT (initterm/security-cookie/SEH) bring-up
