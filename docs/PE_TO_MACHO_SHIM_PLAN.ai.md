// docs/PE_TO_MACHO_SHIM_PLAN.ai.md
# `native/pe_to_macho_shim` — Implementation Plan (Brainstorm + Phasing)

> Plan for closing the last hard gate to `validated_manjeom > 0`: the
> native PE → Mach-O shim that bridges hexa-side PE loader output to
> actual code execution on Apple Silicon while preserving own1 (Wine 0).
>
> Trigger: 2026-05-09 user directive — "native pe_to_macho_shim 새로
> 작성 ... 진행 계획 브레인스토밍 후 md save → 바로 all bg go".
>
> Scope of this doc: the **plan**. First-phase implementation lands
> immediately in Track DL (the shim harness scaffold + JIT entitlement
> + PE-mmap + PARTIAL emit). Later phases (i386 decoder, JIT translator,
> Win32 API forwards) are tracked as future work.

---

## §1 Hard constraints

1. **own1 invariant carries** — Wine / CrossOver / Whisky / GPTK source
   = 0 lines. No `#include` of any community-PE-loader header.
2. **Apple SDK only** — clang + libc + Mach-O loader + Hypervisor /
   posix_spawn / mmap (MAP_JIT) / pthread_jit_write_protect_np / Metal /
   AppKit / Foundation. Public SDK only.
3. **Apple Silicon target** — arm64 macOS 14+. Rosetta 2 unavailable
   for our purposes (it only handles x86_64, **not i386**, and we can't
   shell out to it from a hardened-runtime app to load arbitrary i386).
4. **Honest emit per own2** — every shim phase emits PASS / PARTIAL /
   FAIL with explicit reason. No silent_swallow.
5. **No DRM / Warden / anti-cheat bypass** — we don't touch those
   paths. The shim aims for "first NSWindow paint" only. Full game
   integrity protection out of scope.

---

## §2 The architecture problem

Battle.net-Setup.exe is **i386 (32-bit Intel)**. macOS dropped 32-bit
user-mode in Catalina (10.15). Apple Silicon never had 32-bit Intel
support. Rosetta 2 covers x86_64 only.

**There is no Apple-blessed path to run i386 code on Apple Silicon.**

Every path requires us to **emulate or translate** i386 in software:

| Approach | Pros | Cons |
|---|---|---|
| Software interpreter (decode + dispatch) | Simplest. Pure-C. Portable. | Slow (~1% native). Probably enough for installer GUI but not gameplay. |
| JIT binary translator (i386 → arm64 basic-block recompile) | ~10-30% native; closer to playable. | Big project (need decoder, lifter, codegen, exception model, self-modifying-code handling). |
| Static lifter (i386 .text → LLVM IR → arm64 dylib at install time) | Native speed once lifted. One-shot cost. | Indirect jumps / runtime-generated code force JIT fallback anyway. SEH/TLS callback timing tricky. |
| Hypervisor.framework virt | None — Apple Silicon Hypervisor.framework supports arm64-EL1 guests only. **No x86 vCPU.** | N/A. |

**Decision:** start with **software interpreter** (slow but minimum
viable for first-NSWindow), keep JIT translator as future
optimization track. Static lifter is too brittle for arbitrary PE
inputs (especially packed installers).

This pushes "validated_manjeom > 0" out further than the original
CLOSURE_ROADMAP §5 estimates — which were drafted assuming Apple
shipped first-class PE/i386 support (they didn't and won't).

---

## §3 Phase decomposition (E1..E∞)

Each phase has: name / scope / deliverables / honest-tier on completion /
estimated session-equivalents (1 session ≈ ~hours of focused work).

### E1 — shim harness scaffold + PE mmap (this session, Track DL)

**Scope:** the pe_to_macho_shim binary exists, codesigns with JIT
entitlement, reads PE, mmaps sections into JIT-protected memory, does
**not** attempt any i386 execution. Reports PARTIAL with phase reached.

**Deliverables:**
- `native/pe_to_macho_shim.c` — main entry, PE header parse, mmap with
  MAP_JIT for .text, R for .rdata/.rsrc/.reloc, RW for .data.
- `native/entitlements.plist` — `com.apple.security.cs.allow-jit`,
  `com.apple.security.cs.disable-library-validation` (we mmap user-supplied
  i386 code into our address space).
- `native/build.sh` — clang -arch arm64, codesign with entitlements via
  ad-hoc cert (user's local dev cert — no Apple Developer ID required).
- `lib/loader/pe_to_macho_shim_track.hexa` — extend to record real
  invocation result + emit PARTIAL with phase counter.

**Honest tier on completion:** skeleton-tier-real-execution-confirmed
sub-tier extends to the **shim harness** (not the i386 code). Emits
`__SHIM__ PARTIAL pe_to_macho phase=mmap_done sections_mapped=5`.

**`validated_manjeom` change:** none. Still 0.

**Session estimate:** 1 session.

### E2 — own-rolled minimal i386 decoder (read-only)

**Scope:** decode i386 instructions starting from entry point. Print
disassembly for the first ~256 instructions. No execution. Intel SDM
volume 2 as reference. Handle only: MOV / PUSH / POP / CALL / RET /
JMP / JCC / ADD / SUB / XOR / AND / OR / CMP / TEST / LEA / NOP / INT /
prefix bytes (0x66 / 0x67 / 0xF2 / 0xF3) / ModR/M / SIB / Imm encoding.

**Deliverables:**
- `native/i386_decode.c` + `.h` — instruction decoder, returns
  `i386_insn { op, prefixes, operands[], length }`.
- `native/i386_decode_test.c` — disassemble Battle.net-Setup.exe entry
  block, print first 256 insns.

**Honest tier on completion:** still skeleton-tier. Decoder PASS sets
`__I386_DECODE__ PASS first_256_insns`. No execution emit.

**Session estimate:** 2-3 sessions (just to cover the long-tail of
encoding edge cases; ModR/M alone is fiddly).

### E3 — minimal i386 interpreter (subset, sufficient for kernel32 startup)

**Scope:** execute decoded instructions against a software CPU state
struct (32 GPRs + EFLAGS + segment regs + EIP + FPU stack stub). Stop
on first IAT call (we can't satisfy IAT yet). Subset: ~30 most-common
ops covering Microsoft x86 ABI startup sequence (mainCRTStartup style).

**Deliverables:**
- `native/i386_cpu.c` — CPU state, fetch-decode-execute loop.
- `native/i386_memory.c` — virtual memory accessor that maps
  PE-virtual-addresses to mmapped offsets.
- Hook on indirect call → IAT entry → emit PARTIAL `iat_call_<dll>_<func>`
  and break (don't continue).

**Honest tier:** PARTIAL `__SHIM__ PARTIAL pe_to_macho phase=first_iat_call
api=KERNEL32!GetVersionExW`. Concrete API name observed.

**Session estimate:** 3-5 sessions (interpretation correctness +
ABI invariants; SEH and TLS callbacks deferred).

### E4 — Win32 API shim (kernel32 startup-only subset)

**Scope:** when interpreter hits IAT call, dispatch to a C function
that mirrors the Win32 contract for that specific function. Initial
set: GetVersionExW, GetModuleHandleW, GetProcAddress, GetCommandLineW,
ExitProcess, HeapCreate, HeapAlloc, HeapFree, GetStdHandle,
GetSystemTimeAsFileTime, QueryPerformanceCounter, TlsAlloc,
TlsGetValue, TlsSetValue, IsDebuggerPresent. Maybe 20-30 fns.

**Deliverables:**
- `native/win32_kernel32_shim.c` — C implementations, return realistic
  values that satisfy startup CRT.
- `native/teb_peb.c` — fake TEB/PEB structures at known FS-relative
  addresses (via memory map; we don't have FS segment register, so
  emulate via interpreter-side trap).

**Honest tier:** skeleton-tier extends — first IAT call resolved,
returns to interpreter, advances. Emits per-call PARTIAL.

**Session estimate:** 5-8 sessions.

### E5 — Win32 API shim (user32 + gdi32 startup-only subset for first dialog)

**Scope:** RegisterClassExW, CreateWindowExW, ShowWindow, GetMessageW,
DispatchMessageW, PeekMessageW, DefWindowProcW. CreateWindowExW
forwards to NSWindow (Cocoa AppKit) via Objective-C runtime call from C.

**Deliverables:**
- `native/win32_user32_shim.c` — message-loop emulation.
- `native/cocoa_window_bridge.m` — Objective-C-flavored C bridging
  NSWindow create/show/event-loop pump. We **write** this from scratch
  using public AppKit headers; no Wine winex11/winemac code.

**Honest tier:** PARTIAL → "first NSWindow create" — this is the gate
to validated_manjeom = 1. Emits `__SHIM__ PARTIAL phase=first_nswindow
title="Battle.net-Setup"`.

**Session estimate:** 5-8 sessions.

**At end of E5: `validated_manjeom = 1` candidate** (skeleton tier
crosses to validated tier for one specific surface — first NSWindow
paint of the installer). CM-26 of CLOSURE_ROADMAP §5 first promotion
becomes possible.

### E6+ — beyond first dialog (out of immediate scope)

To ship a runnable Battle.net + D2R, we still need:
- DirectX 11 → Metal command translation (D3DMetal harness)
- WinSock → POSIX socket forwards
- WinHTTP → URLSession bridge
- Crypto (CryptAcquireContext, etc.) → CommonCrypto / SecKey
- File I/O / registry / process spawn
- DLL loader for transitively-loaded DLLs (D3D11.dll etc shipped in
  Battle.net Setup's archive, not host system)
- TLS callbacks (Battle.net Setup almost certainly uses them for
  anti-debug)
- SEH for graceful CRT exception path

Each is 5-20 sessions. Total to "Battle.net Setup first install dialog
clickable": ~60-100 sessions of focused work. To "actual D2R first
gameplay frame": likely 200-400 sessions.

The point of this plan is **not** to do all that. The point is to
land the first scaffold (E1) **now** so future cycles have a place to
attach.

---

## §4 First-phase deliverable structure (Track DL — this session)

```
native/
├── build.sh                    # clang -arch arm64 + codesign --entitlements
├── entitlements.plist          # JIT + library-validation-disable
├── pe_to_macho_shim.c          # main entry — argv[1] = pe path
├── pe_parse.h / pe_parse.c     # PE header read (mirrors hexa parser)
└── README.md                   # what this is, how to build, what it does/doesn't
```

Behavior:
- `./pe_to_macho_shim <pe.exe>` → reads PE, mmaps sections, prints
  layout, emits `__SHIM__ PARTIAL pe_to_macho_shim phase=mmap_done
  sections_mapped=N entry_va=0xHEX path=<pe.exe>`, exits 0.
- Failure modes (file not found / not PE / mmap fail) emit
  `__SHIM__ FAIL pe_to_macho_shim phase=<X> reason=<Y>` and exit 1.

The shim is **pre-execution** — we mmap with MAP_JIT (so JIT
entitlement is justified) but never actually jump into the mapped i386
code. The next phase (E2/E3) flips that switch.

Hexa side (`pe_real_launch_orchestrator launch <pe>`) gets updated to:
1. Resolve `gamebox_root + "/native/pe_to_macho_shim"`.
2. Spawn it with the PE path argv.
3. Capture stdout, parse the `__SHIM__` lines, emit honest hexa-side
   PARTIAL with the phase reached.
4. Return phase reached as exit code.

This wires the existing skeleton-tier orchestrator to a **real native
binary** for the first time, even though the binary itself doesn't yet
execute i386.

---

## §5 own1 audit checklist (per file landed)

For each file in `native/`, before commit:

- [ ] No `#include <wine/...>` / `#include "wine/..."`.
- [ ] No copy from CrossOver / Whisky / GPTK source trees.
- [ ] No copy from ReactOS source.
- [ ] All Win32 type definitions (HANDLE, DWORD, etc.) typed by hand
      from public Microsoft documentation, not headered in.
- [ ] All AppKit / Foundation / Metal usage via public framework
      headers shipped with macOS SDK.
- [ ] Comments cite specs (Intel SDM, Microsoft PE-COFF spec,
      AppKit reference) — never wine source.

The `tests/test_apple_only.hexa` wine-grep currently scans `lib/` /
`entry/` / `tests/`. Extend to also scan `native/` for the same
patterns. (Defer the test extension to a follow-up cycle if it
introduces new failures; flag in DL doc.)

---

## §6 Risks + mitigation

| Risk | Impact | Mitigation |
|---|---|---|
| User has no codesign cert at all | shim won't run on hardened-runtime macOS | use `codesign --sign - --entitlements ...` (ad-hoc); works for local dev. |
| MAP_JIT requires entitlement; entitlement requires codesign; codesign with entitlements on ad-hoc fails on some macOS versions | shim mmap failure | start with `MAP_JIT \| MAP_PRIVATE \| MAP_ANON` in pure-arm64 path; fall back to plain `mmap(... PROT_NONE then mprotect(... PROT_EXEC))` if JIT denied. Emit FAIL with diagnostic if both fail. |
| 32-bit i386 in PE, but our shim is arm64 | can't dlopen, can't direct-call | this is exactly *why* we eventually need the interpreter (E3). E1 doesn't try; just maps. |
| Apple SDK changes break codesign / MAP_JIT contract in future macOS | shim breaks | pin to macOS 14+; add CI check (out of scope this session). |
| Battle.net Setup has anti-debug / TLS callbacks that detect interpretation | even E5 fails to reach NSWindow | accept; at that point we've reached the anti-cheat-style boundary that own1 says we don't bypass. Stop. Emit honest. |
| User runs shim with hardened-runtime claude / gamebox process | parent process attempts MAP_JIT inheritance issues | the shim is its own process (spawned by orchestrator), so its own entitlements apply. |
| Project grows into a clone of Wine | violates own1 spirit | every Win32 API impl must cite Microsoft spec, **not** Wine source. Honest declaration: "we're writing a Win32 reimplementation" without copying — like ReactOS but starting from spec instead of from Wine. Ack: this is years of work; that's why E1 only lands here. |

---

## §7 What "all bg go" means for this session

Per user directive 2026-05-09 ("브레인스토밍 후 md save → 바로 all bg
go"): land **E1 only** (the harness scaffold), in this session, in the
foreground. "all bg go" here = full-throttle within this turn, not
literally background-task. Future phases E2-E5 are out-of-session.

Concretely:

1. Save this plan as `docs/PE_TO_MACHO_SHIM_PLAN.ai.md` (this file).
2. Create `native/` directory with the 4 source/build files.
3. Build the shim.
4. Test it on `~/Downloads/Battle.net-Setup.exe`.
5. Update `pe_real_launch_orchestrator.hexa` to honest-emit the shim's
   actual stdout instead of just running it blind.
6. Verify unified test 9/9 still PASS.
7. Commit as Track DL.

E2+ stays as planned milestones in this doc, **not implemented this
session.**

---

## §8 Naming + commit convention

- Track ID: **DL** — "DL" mnemonics: "Dynamic Link" (PE→Mach-O bridge)
  + alphabet ordering after DK.
- Commit prefix: `feat+code+docs+marker:`
- Marker: `state/markers/native_pe_to_macho_shim_e1_harness_scaffold_track_dl_landed.marker`

---

## §9 Honest disclaimer

This plan acknowledges that **doing the full job is a multi-year
multi-person effort**. The plan exists so that the work can be staged
honestly: each phase emits a real signal at completion, and the carry
of `validated_manjeom = 0` is preserved exactly until E5 lands a real
NSWindow from Battle.net-Setup.exe code paths.

There are no shortcuts. Wine exists because building this is hard. We
don't use Wine because own1. We accept the cost.

E1 lands now.

---

*written 2026-05-09 (pre-Track DL). Revision 1.*
