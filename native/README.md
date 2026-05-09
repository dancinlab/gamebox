# `native/` — pe_to_macho_shim (PE → Mach-O bridge)

> Native binary that bootstraps PE32 / PE32+ executables on Apple
> Silicon macOS. Written from public Apple SDK + Microsoft PE-COFF
> spec. No Wine / CrossOver / Whisky / GPTK / ReactOS source.
>
> Phasing per `docs/PE_TO_MACHO_SHIM_PLAN.ai.md`.

## Current phase: E1 (harness scaffold + PE mmap)

The shim:
- Reads the PE header (DOS magic, NT signature, COFF, optional header).
- mmaps each section into our address space — `.text` via `MAP_JIT`,
  others via plain anonymous memory with section permissions.
- Honest-emits `__SHIM__ PARTIAL pe_to_macho_shim phase=mmap_done ...`.
- **Does not** execute any i386 code. The mmapped `.text` is left in
  RW state; future phases flip it to RX and start interpreting.

## Build

```
cd native && ./build.sh
```

Requires: macOS 14+ on Apple Silicon, Xcode command-line tools (clang +
codesign).

## Run

```
./pe_to_macho_shim <path-to-pe.exe>
```

Example:
```
./pe_to_macho_shim ~/Downloads/Battle.net-Setup.exe
```

Expected output (E1):
```
[shim] gamebox pe_to_macho_shim version=0.1.0-e1-harness-scaffold
[shim] phase=E1 (harness scaffold + PE mmap; no i386 execution)
[shim] own1: Wine 0 / hexa+C-from-spec only
[shim.parse] path=...
[shim.parse] machine=0x14C (332=i386, 8664=AMD64)
[shim.parse] sections=N
...
__SHIM__ PARTIAL pe_to_macho_shim phase=parse machine=0x14C ...
__SHIM__ PARTIAL pe_to_macho_shim phase=section_map idx=0 ...
__SHIM__ PARTIAL pe_to_macho_shim phase=mmap_done sections_mapped=N ...
__SHIM__ PARTIAL pe_to_macho_shim phase=e1_done validated_manjeom=0 ...
```

Exit 0 on E1 success.

## What this does NOT do (yet)

E1 deliberately stops after mmap. It does NOT:
- Decode i386 instructions.
- Execute any code from the mapped PE.
- Resolve the IAT.
- Forward Win32 API calls.
- Open any window.

Each of those is a future phase (E2..E5+) per the plan doc. Until E5,
`validated_manjeom` carries 0.

## Files

| File | Purpose |
|---|---|
| `pe_to_macho_shim.c` | main entry, mmap orchestration, honest emit |
| `pe_parse.h` / `pe_parse.c` | PE header reader (PE-COFF spec) |
| `entitlements.plist` | JIT + library-validation entitlements |
| `build.sh` | clang + ad-hoc codesign script |

## own1 audit

- No `#include <wine/...>`.
- No copy-paste from CrossOver / Whisky / GPTK / ReactOS / Wine.
- All Win32 type names typed by hand from Microsoft PE-COFF spec
  (which is public; this is the same source ReactOS would cite if
  ReactOS were starting from scratch).
- Apple SDK only via system headers.
