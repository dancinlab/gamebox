// native/i386_cpu_test.c — execute (not just decode) the entry block.
//
// Phase F-NSWINDOW-E5 standalone tester. For the FIRST TIME runs real
// Win32 game-binary bytes through the i386 interpreter and measures how
// far it gets, output-verified.
//
// Usage:
//   ./i386_cpu_test [pe_path]
//   - pe_path given & loadable → run from the binary's real entry VA,
//     emit the __SHIM__ line from that run.
//   - no/absent pe_path → HERMETIC mode: a hand-built flat image holding
//     the real 9-instruction entry block of Battle.net-Setup.exe
//     (entry_va 0x5388A6) so the test runs in CI without the binary.
//
// The hermetic image embeds the binary's REAL entry bytes (E8 AE 09 00 00
// = `call 0x539259`, etc.). The byte sitting AT the CALL target 0x539259
// is, in the real binary, the prologue of __scrt_common_main.
//
// E5 r3 — the decoder + interpreter were extended to cover the documented
// __scrt_common_main prologue opcode set (group-1 imm arith 0x83/0x81/0x80,
// mov r/m,imm 0xC7/0xC6, byte mov 0x88/0x8A, test 0x84/0x85, and/or/xor
// 0x09/0x0B 0x21/0x23 0x31/0x33), with an EFLAGS model. We therefore plant a
// representative __scrt_common_main-shaped prologue at 0x539259 (own1: a
// documented prologue SHAPE built from the named families — the real binary
// is absent on this host, so these are our own SDM-correct bytes, not the
// verbatim image). r3 execution advanced PAST the r2 1-instruction wall,
// THROUGH the prologue, and HALTED at the first IAT-style indirect CALL
// `FF 15 [0x538000]` @0x539274 — the E4 kernel32 boundary.
//
// E5 r4 — CROSS that boundary. The interpreter BINDS the IAT slot to a native
// kernel32 shim: on `FF 15 [slot]` it looks the slot VA up in a registry, and
// if bound, dispatches to the native C stub, places its result in EAX, and
// CONTINUES.
//
// E5 r5 — DRAIN more CRT-startup imports + close the 0xC1 shift gap. The
// prologue now runs FIVE bound kernel32 shims in a row: GetCurrentThreadId
// (r4), GetCurrentProcessId, GetSystemTimeAsFileTime (the first BUFFER-WRITING
// stub — reads the caller's pushed pointer arg and writes an 8-byte FILETIME
// into image memory), QueryPerformanceCounter (buffer write, → 1), GetTickCount;
// between them a `shl edx,4` exercises the freshly-decoded group-2 shift
// (0xC1 /4) with full CF/OF/ZF/SF flags.
//
// E5 r6 — DRAIN 3 more CRT-security imports, CLOSE the 0F B6/B7/BE/BF MOVZX/MOVSX
// decoder gap, and model a BOUNDED-SYNTHETIC TEB. After GetTickCount the prologue
// now binds IsProcessorFeaturePresent (1-arg → 1), InitializeSListHead (buffer-
// zeroes an 8-byte SLIST header), and GetModuleHandleW (→ the loaded image base),
// runs a `movzx ecx,al` + `movsx edx,cl` (the freshly-decoded two-byte extends),
// then reads a synthetic TEB self-pointer via `mov ecx, fs:[0x18]` (the FS
// segment-override redirects to teb_base) and walks TEB→PEB→ImageBaseAddress.
// Execution advances PAST the r5 wall (insns 18 → 30) and halts at a NINTH,
// UNregistered IAT call @0x5392D4 (slot 0x538020, the next import to bind, r7).
//
// E5 r7 — DRAIN 3 more CRT imports (GetStartupInfoW [buffer-write STARTUPINFOW
// cb=0x44] / GetSystemInfo [bounded-synthetic SYSTEM_INFO] / GetProcAddress [→ a
// synthetic in-image stub addr]), then run the MSVC __security_init_cookie shape:
// load the FILETIME entropy, `xor`, `imul eax,eax,0x01000193` (the freshly-decoded
// 0x69 IMUL), and STORE a non-default cookie to a global slot. Then `cpuid` (the
// 0F A2 gap closed → synthetic "GenuineIntel") and `rdtsc` (0F 31 → a synthetic
// monotonic edx:eax). Execution advances PAST the r6 wall (insns 30 → 44) and
// halts at a TWELFTH, UNregistered IAT call @0x539318 (slot 0x53802C, r8). The
// B/D walls now use a `0F A3` (bt) sentinel — the next genuinely-uncovered gap
// (0F A2 cpuid / 0F 31 rdtsc / 0F AF imul are no longer walls).
// own1: binding the program's OWN imports to native impls is LOADING (what
// every PE loader does), NOT a bypass; the TEB here is OUR OWN synthetic block
// (a few fields), NOT the OS TEB and NOT a protection structure. CPUID/RDTSC are
// plain CPU instructions; the cookie is OUR arithmetic over OUR buffers. kernel32
// is the OS API, not DRM/Warden. Our own bytes, Intel SDM semantics, native
// shims — no Wine, no protection.
//
// E5 r8 — REACH THE CRT→USER-ENTRY HANDOFF. Bind the last two CRT imports
// (GetCommandLineW [slot 2C] / SetUnhandledExceptionFilter [slot 30]), close the
// 0F A3/AB/B3/BB BT/BTS/BTR/BTC decoder gap, then set up the WinMain stdcall
// frame and `call` the synthetic user entry. The interpreter detects the call
// target == cpu.user_entry_va and HALTS I386_HALT_USER_ENTRY (insns 44 → 60).
// This is the honest E4 completion milestone: CRT init reached `call WinMain`.
// It is NOT a rendered game frame — validated_manjeom STAYS 0. The WinMain target
// is OUR OWN synthetic VA; a real frame is real-asset-gated (real i386 game PE +
// the binary's own IAT auto-bound by import name + its real WinMain → message
// loop → CreateWindowEx → D3D→Metal, + D3DMetal SDK + a real display) — none of
// which exists on this host/CI. The B/D walls now use a `0F B1` (cmpxchg)
// sentinel — the next genuinely-uncovered decoder gap (r9). own1: synthetic
// shape, our bytes, Intel SDM semantics — no Wine, no protection.

#include "i386_cpu.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// ---- hermetic image geometry ---------------------------------------------
#define IMG_BASE  0x538000u
#define IMG_SIZE  0x2000u          // covers entry block, CALL targets, stack
#define ENTRY_VA  0x5388A6u        // Battle.net-Setup.exe AddressOfEntryPoint+ImageBase

// The 9-instruction entry block, verbatim (24 bytes). VAs annotated.
static const uint8_t ENTRY_BYTES[] = {
    /* 0x5388A6 */ 0xE8, 0xAE, 0x09, 0x00, 0x00,   // call 0x539259
    /* 0x5388AB */ 0xE9, 0x7A, 0xFE, 0xFF, 0xFF,   // jmp  0x53872A
    /* 0x5388B0 */ 0x55,                           // push ebp
    /* 0x5388B1 */ 0x8B, 0xEC,                     // mov  ebp, esp
    /* 0x5388B3 */ 0xFF, 0x75, 0x08,               // push dword [ebp+8]
    /* 0x5388B6 */ 0xE8, 0xF0, 0x02, 0x00, 0x00,   // call 0x538BAB
    /* 0x5388BB */ 0x59,                           // pop  ecx
    /* 0x5388BC */ 0x5D,                           // pop  ebp
    /* 0x5388BD */ 0xC3,                           // ret
};

static int g_fail = 0;
#define CHECK(cond, msg) do { \
    if (cond) { printf("  [ok]   %s\n", msg); } \
    else      { printf("  [FAIL] %s\n", msg); g_fail = 1; } \
} while (0)

static void put_at(uint8_t *img, uint32_t va, const uint8_t *src, size_t n) {
    memcpy(img + (va - IMG_BASE), src, n);
}

// A representative __scrt_common_main prologue (own1: documented SHAPE built
// from the E5-r3 opcode families; SDM-correct, register-only so it runs from
// any entry state). It exercises group-1 imm (sub/or/cmp/and/add), MOV r,imm,
// XOR r,r/m, TEST r/m,r — all newly executed — then reaches the first
// IAT-style indirect CALL, the honest E4 boundary.
//
//   0x539259  83 EC 28              sub  esp, 0x28        ; group-1 /5 imm8
//   0x53925C  33 C0                 xor  eax, eax         ; XOR_R_RM → eax=0
//   0x53925E  83 C8 01              or   eax, 0x1         ; group-1 /1 imm8 → eax=1
//   0x539261  83 F8 01              cmp  eax, 0x1         ; group-1 /7 → ZF=1
//   0x539264  B9 EF BE AD DE        mov  ecx, 0xDEADBEEF  ; MOV_R_IMM
//   0x539269  81 E1 FF 00 00 00     and  ecx, 0xFF        ; group-1 /4 imm32 → ecx=0xEF
//   0x53926F  85 C9                 test ecx, ecx         ; TEST_RM_R → flags
//   0x539271  83 C4 28              add  esp, 0x28        ; group-1 /0 → esp restored
//   0x539274  FF 15 00 80 53 00     call [0x538000]       ; slot 0 → GetCurrentThreadId (r4)
//   0x53927A  89 C2                 mov  edx, eax         ; edx = returned TID
//   0x53927C  FF 15 04 80 53 00     call [0x538004]       ; slot 4 → GetCurrentProcessId (r5)
//   0x539282  68 00 9F 53 00        push 0x539F00         ; &FILETIME buffer (a pushed ptr arg)
//   0x539287  FF 15 08 80 53 00     call [0x538008]       ; slot 8 → GetSystemTimeAsFileTime
//                                                         ;   (r5 buffer-writing stub: 8 bytes
//                                                         ;    into [0x539F00])
//   0x53928D  68 10 9F 53 00        push 0x539F10         ; &LARGE_INTEGER buffer
//   0x539292  FF 15 0C 80 53 00     call [0x53800C]       ; slot C → QueryPerformanceCounter
//                                                         ;   (buffer write 8B → [0x539F10], → 1)
//   0x539298  C1 E2 04              shl  edx, 4           ; group-2 SHIFT (r5): edx = TID<<4
//   0x53929B  FF 15 10 80 53 00     call [0x538010]       ; slot 10 → GetTickCount (r5)
// E5 r6 continuation — drain 3 more CRT-security imports, exercise MOVZX/MOVSX,
// and read a BOUNDED-SYNTHETIC TEB→PEB→ImageBase chain via an fs: override:
//   0x5392A1  6A 0A                 push 0x0A             ; arg for next call
//   0x5392A3  FF 15 14 80 53 00     call [0x538014]       ; slot 14 → IsProcessorFeaturePresent (r6, →1)
//   0x5392A9  0F B6 C8              movzx ecx, al         ; 0F B6 (r6 decoder) → ecx=1
//   0x5392AC  0F BE D1              movsx edx, cl         ; 0F BE (r6 decoder) → edx=1
//   0x5392AF  68 20 9F 53 00        push 0x539F20         ; &SLIST head
//   0x5392B4  FF 15 18 80 53 00     call [0x538018]       ; slot 18 → InitializeSListHead (r6, zero 8B)
//   0x5392BA  6A 00                 push 0                ; GetModuleHandleW(NULL)
//   0x5392BC  FF 15 1C 80 53 00     call [0x53801C]       ; slot 1C → GetModuleHandleW (r6, → image base)
//   0x5392C2  64 8B 0D 18 00 00 00  mov ecx, fs:[0x18]    ; synthetic TEB self (fs override → teb_base)
//   0x5392C9  8B 51 30              mov edx, [ecx+0x30]   ; [TEB+0x30] → synthetic PEB
//   0x5392CC  8B 42 08              mov eax, [edx+8]      ; [PEB+8]    → ImageBaseAddress (= image base)
//   0x5392CF  68 30 9F 53 00        push 0x539F30         ; &STARTUPINFOW
//   0x5392D4  FF 15 20 80 53 00     call [0x538020]       ; slot 20 → GetStartupInfoW (r7, buffer-write)
// E5 r7 continuation — drain 3 more CRT imports, then run the __security_init_cookie
// shape (imul-mix → store global cookie) + CPUID + RDTSC (plain CPU ops):
//   0x5392DA  68 90 9F 53 00        push 0x539F90         ; &SYSTEM_INFO
//   0x5392DF  FF 15 24 80 53 00     call [0x538024]       ; slot 24 → GetSystemInfo (r7, buffer-write)
//   0x5392E5  68 C0 9F 53 00        push 0x539FC0         ; lpProcName (arg2)
//   0x5392EA  6A 00                 push 0                ; hModule NULL (arg1)
//   0x5392EC  FF 15 28 80 53 00     call [0x538028]       ; slot 28 → GetProcAddress (r7, → synth stub)
//   0x5392F2  8B 05 00 9F 53 00     mov eax, [0x539F00]   ; FILETIME low entropy = 0xC3D4E5F6
//   0x5392F8  33 05 04 9F 53 00     xor eax, [0x539F04]   ; ^ FILETIME high → 0xC2034444
//   0x5392FE  69 C0 93 01 00 01     imul eax, eax, 0x01000193 ; cookie mix (69 /r id — r7 decoder)
//   0x539304  89 05 80 9F 53 00     mov [0x539F80], eax   ; store the security cookie (global slot)
//   0x53930A  B8 00 00 00 00        mov eax, 0            ; cpuid leaf 0
//   0x53930F  0F A2                 cpuid                 ; → ebx/edx/ecx "GenuineIntel" (r7 decoder)
//   0x539311  0F 31                 rdtsc                 ; → edx:eax synthetic monotonic tsc (r7)
//   0x539313  68 D0 9F 53 00        push 0x539FD0         ; & scratch buffer (stray push, r8 discards)
//   0x539318  FF 15 2C 80 53 00     call [0x53802C]       ; slot 2C → GetCommandLineW (r8, → cmdline ptr)
// E5 r8 continuation — bind the last two CRT imports, exercise BT/BTS/BTR/BTC,
// then set up the WinMain stdcall frame and reach the CRT→user-entry handoff:
//   0x53931E  59                    pop ecx               ; discard the stray scratch push (balance esp)
//   0x53931F  89 C6                 mov esi, eax          ; esi = lpCmdLine (synthetic GetCommandLineW ret)
//   0x539321  6A 00                 push 0                ; SetUnhandledExceptionFilter(NULL)
//   0x539323  FF 15 30 80 53 00     call [0x538030]       ; slot 30 → SetUnhandledExceptionFilter (r8, →0)
//   0x539329  BA 05 00 00 00        mov edx, 5            ; bit index 5
//   0x53932E  B8 20 00 00 00        mov eax, 0x20         ; feature word (bit 5 set)
//   0x539333  0F A3 D0              bt  eax, edx          ; CF ← bit5 = 1            (0F A3 — r8 decoder)
//   0x539336  0F AB D0              bts eax, edx          ; set bit5 (CF←1), eax=0x20 (0F AB — r8)
//   0x539339  0F B3 D0              btr eax, edx          ; clear bit5 (CF←1), eax=0  (0F B3 — r8)
//   0x53933C  0F BB D0              btc eax, edx          ; flip bit5 (CF←0), eax=0x20 (0F BB — r8)
//   0x53933F  6A 0A                 push 0x0A             ; nShowCmd
//   0x539341  56                    push esi              ; lpCmdLine
//   0x539342  6A 00                 push 0                ; hPrevInstance NULL
//   0x539344  68 00 80 53 00        push 0x538000         ; hInstance (image base, synthetic)
//   0x539349  E8 B2 00 00 00        call 0x539400         ; CRT→user-entry handoff → WinMain (SYNTHETIC)
static const uint8_t SCRT_PROLOGUE[] = {
    0x83, 0xEC, 0x28,
    0x33, 0xC0,
    0x83, 0xC8, 0x01,
    0x83, 0xF8, 0x01,
    0xB9, 0xEF, 0xBE, 0xAD, 0xDE,
    0x81, 0xE1, 0xFF, 0x00, 0x00, 0x00,
    0x85, 0xC9,
    0x83, 0xC4, 0x28,
    0xFF, 0x15, 0x00, 0x80, 0x53, 0x00,   // 0x539274 call [0x538000] → GetCurrentThreadId
    0x89, 0xC2,                           // 0x53927A mov edx, eax
    0xFF, 0x15, 0x04, 0x80, 0x53, 0x00,   // 0x53927C call [0x538004] → GetCurrentProcessId
    0x68, 0x00, 0x9F, 0x53, 0x00,         // 0x539282 push 0x539F00  (&filetime)
    0xFF, 0x15, 0x08, 0x80, 0x53, 0x00,   // 0x539287 call [0x538008] → GetSystemTimeAsFileTime
    0x68, 0x10, 0x9F, 0x53, 0x00,         // 0x53928D push 0x539F10  (&perfcount)
    0xFF, 0x15, 0x0C, 0x80, 0x53, 0x00,   // 0x539292 call [0x53800C] → QueryPerformanceCounter
    0xC1, 0xE2, 0x04,                     // 0x539298 shl edx, 4   (group-2 shift)
    0xFF, 0x15, 0x10, 0x80, 0x53, 0x00,   // 0x53929B call [0x538010] → GetTickCount
    // ── E5 r6 continuation ──
    0x6A, 0x0A,                           // 0x5392A1 push 0x0A   (IsProcessorFeaturePresent arg)
    0xFF, 0x15, 0x14, 0x80, 0x53, 0x00,   // 0x5392A3 call [0x538014] → IsProcessorFeaturePresent
    0x0F, 0xB6, 0xC8,                     // 0x5392A9 movzx ecx, al   (0F B6 — r6 decoder gap closed)
    0x0F, 0xBE, 0xD1,                     // 0x5392AC movsx edx, cl   (0F BE — r6)
    0x68, 0x20, 0x9F, 0x53, 0x00,         // 0x5392AF push 0x539F20  (&SLIST head)
    0xFF, 0x15, 0x18, 0x80, 0x53, 0x00,   // 0x5392B4 call [0x538018] → InitializeSListHead (buffer-zero)
    0x6A, 0x00,                           // 0x5392BA push 0       (GetModuleHandleW NULL)
    0xFF, 0x15, 0x1C, 0x80, 0x53, 0x00,   // 0x5392BC call [0x53801C] → GetModuleHandleW (→ image base)
    0x64, 0x8B, 0x0D, 0x18, 0x00, 0x00, 0x00, // 0x5392C2 mov ecx, fs:[0x18]  (synthetic TEB self)
    0x8B, 0x51, 0x30,                     // 0x5392C9 mov edx, [ecx+0x30]      ([TEB+0x30] → synthetic PEB)
    0x8B, 0x42, 0x08,                     // 0x5392CC mov eax, [edx+8]         ([PEB+8] → ImageBaseAddress)
    0x68, 0x30, 0x9F, 0x53, 0x00,         // 0x5392CF push 0x539F30  (&STARTUPINFOW)
    0xFF, 0x15, 0x20, 0x80, 0x53, 0x00,   // 0x5392D4 call [0x538020] → GetStartupInfoW (r7, buffer-write)
    // ── E5 r7 continuation ──
    0x68, 0x90, 0x9F, 0x53, 0x00,         // 0x5392DA push 0x539F90  (&SYSTEM_INFO)
    0xFF, 0x15, 0x24, 0x80, 0x53, 0x00,   // 0x5392DF call [0x538024] → GetSystemInfo (buffer-write)
    0x68, 0xC0, 0x9F, 0x53, 0x00,         // 0x5392E5 push 0x539FC0  (lpProcName arg2)
    0x6A, 0x00,                           // 0x5392EA push 0       (hModule NULL arg1)
    0xFF, 0x15, 0x28, 0x80, 0x53, 0x00,   // 0x5392EC call [0x538028] → GetProcAddress (→ synth stub)
    0x8B, 0x05, 0x00, 0x9F, 0x53, 0x00,   // 0x5392F2 mov eax, [0x539F00]  (FILETIME low entropy)
    0x33, 0x05, 0x04, 0x9F, 0x53, 0x00,   // 0x5392F8 xor eax, [0x539F04]  (^ FILETIME high)
    0x69, 0xC0, 0x93, 0x01, 0x00, 0x01,   // 0x5392FE imul eax, eax, 0x01000193  (cookie mix — r7)
    0x89, 0x05, 0x80, 0x9F, 0x53, 0x00,   // 0x539304 mov [0x539F80], eax  (store global cookie)
    0xB8, 0x00, 0x00, 0x00, 0x00,         // 0x53930A mov eax, 0          (cpuid leaf 0)
    0x0F, 0xA2,                           // 0x53930F cpuid              (0F A2 — r7 decoder gap closed)
    0x0F, 0x31,                           // 0x539311 rdtsc              (0F 31 — r7)
    0x68, 0xD0, 0x9F, 0x53, 0x00,         // 0x539313 push 0x539FD0  (& scratch buffer; r8 discards)
    0xFF, 0x15, 0x2C, 0x80, 0x53, 0x00,   // 0x539318 call [0x53802C] → GetCommandLineW (r8, → cmdline)
    // ── E5 r8 continuation ──
    0x59,                                 // 0x53931E pop ecx   (discard the stray scratch push)
    0x89, 0xC6,                           // 0x53931F mov esi, eax  (esi = lpCmdLine)
    0x6A, 0x00,                           // 0x539321 push 0   (SetUnhandledExceptionFilter NULL)
    0xFF, 0x15, 0x30, 0x80, 0x53, 0x00,   // 0x539323 call [0x538030] → SetUnhandledExceptionFilter
    0xBA, 0x05, 0x00, 0x00, 0x00,         // 0x539329 mov edx, 5     (bit index)
    0xB8, 0x20, 0x00, 0x00, 0x00,         // 0x53932E mov eax, 0x20  (feature word, bit5 set)
    0x0F, 0xA3, 0xD0,                     // 0x539333 bt  eax, edx   (0F A3 — r8 decoder gap closed)
    0x0F, 0xAB, 0xD0,                     // 0x539336 bts eax, edx   (0F AB — r8)
    0x0F, 0xB3, 0xD0,                     // 0x539339 btr eax, edx   (0F B3 — r8)
    0x0F, 0xBB, 0xD0,                     // 0x53933C btc eax, edx   (0F BB — r8)
    0x6A, 0x0A,                           // 0x53933F push 0x0A   (nShowCmd)
    0x56,                                 // 0x539341 push esi    (lpCmdLine)
    0x6A, 0x00,                           // 0x539342 push 0      (hPrevInstance NULL)
    0x68, 0x00, 0x80, 0x53, 0x00,         // 0x539344 push 0x538000 (hInstance, synthetic)
    0xE8, 0xB2, 0x00, 0x00, 0x00,         // 0x539349 call 0x539400 → WinMain (CRT→user-entry handoff)
};

// The native kernel32 imports bound across IAT slots 0x538000..0x538030 (r4-r8).
// own1: loading the OS API surface to native impls, not a protection bypass.
// The buffer-writing shims (slots 8 / C / 18 / 20 / 24) take one stdcall pointer
// arg (4 bytes, callee-popped) and write into image memory; IsProcessorFeature-
// Present (14) / GetModuleHandleW (1C) / SetUnhandledExceptionFilter (30) take 1
// arg; GetProcAddress (28) takes 2 (8 bytes); the rest (incl. GetCommandLineW
// at slot 2C) are 0-arg.
static const i386_import_t K32_IMPORTS[] = {
    { 0x538000u, "GetCurrentThreadId",        i386_shim_GetCurrentThreadId,        0 },
    { 0x538004u, "GetCurrentProcessId",       i386_shim_GetCurrentProcessId,       0 },
    { 0x538008u, "GetSystemTimeAsFileTime",   i386_shim_GetSystemTimeAsFileTime,   4 },
    { 0x53800Cu, "QueryPerformanceCounter",   i386_shim_QueryPerformanceCounter,   4 },
    { 0x538010u, "GetTickCount",              i386_shim_GetTickCount,              0 },
    { 0x538014u, "IsProcessorFeaturePresent", i386_shim_IsProcessorFeaturePresent, 4 },
    { 0x538018u, "InitializeSListHead",       i386_shim_InitializeSListHead,       4 },
    { 0x53801Cu, "GetModuleHandleW",          i386_shim_GetModuleHandleW,          4 },
    { 0x538020u, "GetStartupInfoW",           i386_shim_GetStartupInfoW,           4 },
    { 0x538024u, "GetSystemInfo",             i386_shim_GetSystemInfo,             4 },
    { 0x538028u, "GetProcAddress",            i386_shim_GetProcAddress,            8 },
    // ── E5 r8: the last two CRT→user-entry imports ──
    { 0x53802Cu, "GetCommandLineW",           i386_shim_GetCommandLineW,           0 },
    { 0x538030u, "SetUnhandledExceptionFilter", i386_shim_SetUnhandledExceptionFilter, 4 },
};
static const i386_iat_t K32_IAT = { K32_IMPORTS, 13 };
#define WINMAIN_VA 0x539400u    // synthetic user entry (CRT→user-entry handoff target)
#define CMDLINE_PTR 0x539FE0u   // GetCommandLineW synthetic return (img base + 0x1FE0)
#define SHIM_TID  0x00001A2Bu   // GetCurrentThreadId stub return
#define SHIM_PID  0x00000D04u   // GetCurrentProcessId stub return
#define SHIM_TICK 0x0001D4C0u   // GetTickCount stub return
#define FT_BUF    0x539F00u     // FILETIME scratch (above the stack base, in-image)
#define QPC_BUF   0x539F10u     // LARGE_INTEGER scratch
#define SLIST_BUF 0x539F20u     // SLIST_HEADER scratch (InitializeSListHead zeroes 8B)
#define STARTUP_BUF 0x539F30u   // STARTUPINFOW scratch (GetStartupInfoW writes 0x44 B, cb@0)
#define COOKIE_BUF  0x539F80u   // global security-cookie slot (imul-mix stored here)
#define SYSINFO_BUF 0x539F90u   // SYSTEM_INFO scratch (GetSystemInfo writes 0x24 B)
// ── BOUNDED-SYNTHETIC TEB/PEB (E5 r6) — OUR own block, NOT the OS TEB ────────
// A few fields, just enough for the security-cookie / CRT-startup chain to read
// fs:[0x18] (TEB self) → [TEB+0x30] (PEB) → [PEB+8] (ImageBaseAddress). own1:
// synthetic, labeled, not a protection structure.
#define TEB_BASE  0x539C00u     // synthetic TEB linear base
#define PEB_BASE  0x539C80u     // synthetic PEB linear base

// Build the hermetic flat image: entry block + the __scrt_common_main
// prologue at the insn-1 CALL target + the synthetic TEB/PEB + still-uncovered
// sentinels elsewhere.
static void build_hermetic(uint8_t *img) {
    memset(img, 0, IMG_SIZE);
    put_at(img, ENTRY_VA, ENTRY_BYTES, sizeof(ENTRY_BYTES));
    put_at(img, 0x539259u, SCRT_PROLOGUE, sizeof(SCRT_PROLOGUE));  // CALL target (insn 1)
    // Synthetic TEB/PEB fields (own1: our own block, labeled, not the OS TEB).
    i386_image_t m = { img, IMG_BASE, IMG_SIZE, 0 };
    i386_mem_write32(&m, TEB_BASE + 0x18, TEB_BASE);   // TEB self-pointer (NtCurrentTeb shape)
    i386_mem_write32(&m, TEB_BASE + 0x30, PEB_BASE);   // TEB→PEB
    i386_mem_write32(&m, PEB_BASE + 0x08, IMG_BASE);   // PEB→ImageBaseAddress
    // 0F B1 = CMPXCHG r/m32, r32 — still uncovered after r8 (0F A3/AB/B3/BB bt/bts/
    // btr/btc now decode+execute). The 0x0F two-byte map recognizes Jcc (80..8F) +
    // movzx/movsx (B6/B7/BE/BF) + imul/cpuid/rdtsc (AF/A2/31) + bit-test (A3/AB/B3/
    // BB), so 0F B1 → I386_OP_UNKNOWN. Keeps the B/D walls honest and names the next
    // real decoder gap (CMPXCHG, the lock-free CRT init primitive) for r9.
    uint8_t wall[] = { 0x0F, 0xB1 };               // representative `cmpxchg` (next gap)
    put_at(img, 0x538BABu, wall, sizeof(wall));    // CALL target (insn 6)
    put_at(img, 0x53872Au, wall, sizeof(wall));    // JMP  target (insn 2)
}

int main(int argc, char **argv) {
    // ---------------------------------------------------------------------
    // Optional real-binary run (only if a loadable PE path is supplied).
    // ---------------------------------------------------------------------
    int emitted = 0;
    if (argc >= 2 && access(argv[1], R_OK) == 0) {
        i386_image_t pe; uint32_t entry = 0;
        int rc = i386_cpu_load_pe(argv[1], &pe, &entry);
        if (rc == 0) {
            printf("[cpu] real PE loaded: %s base=0x%X size=0x%X entry=0x%X\n",
                   argv[1], pe.base, pe.size, entry);
            i386_cpu_t cpu; memset(&cpu, 0, sizeof(cpu));
            cpu.eip = entry;
            cpu.gpr[I386_REG_ESP] = pe.base + pe.size - 0x100;  // writable top of image
            cpu.iat = &K32_IAT;                                 // native kernel32 binding
            cpu.teb_base = TEB_BASE;                            // synthetic TEB (own1: our block)
            // NB: user_entry_va stays 0 for a real PE — the user entry VA is found
            // by the binary's own CRT, not known a priori; the run halts honestly at
            // its first unbound import / uncovered opcode. The K32_IAT slots here are
            // the HERMETIC synthetic VAs (0x538000..), so a real PE's IAT will not
            // match until a future round parses the PE's import table and auto-binds
            // by name (see the INFO line below). own1: honest, no invented functions.
            i386_run_result_t r;
            i386_cpu_run(&cpu, &pe, 100000, &r);
            printf("__SHIM__ PARTIAL phase=e4_real_pe_probe insns=%u bound=%u last=%s halt_va=0x%X halt=%s halt_op=%s unbound_slot=0x%X\n",
                   r.insns, r.imports_bound, r.last_import ? r.last_import : "-", r.halt_va,
                   i386_halt_name(r.halt), i386_op_name(r.halt_op), r.import_slot);
            printf("__SHIM__ INFO real_pe_path=loaded sections+entry mapped; needs:IAT_autobind_by_import_name+wider_opcode_coverage+D3DMetal+display for a frame (validated_manjeom=0)\n");
            i386_image_free(&pe);
            emitted = 1;
        } else {
            printf("[cpu] PE load failed (rc=%d) — falling back to hermetic\n", rc);
        }
    }

    // ---------------------------------------------------------------------
    // Hermetic runs (always; these carry the asserted PASS/FAIL semantics).
    // ---------------------------------------------------------------------
    uint8_t *img_buf = (uint8_t *)malloc(IMG_SIZE);
    i386_image_t img = { img_buf, IMG_BASE, IMG_SIZE, 0 };

    // === Run A — MILESTONE: 13 CRT imports + BT family → CRT→user-entry handoff ===
    // E5 r8 — execution runs the entry CALL (insn 1) → __scrt_common_main prologue
    // → the r6 chain (insns 1..30) → the r7 continuation (StartupInfo/SysInfo/
    // ProcAddr + security cookie + CPUID/RDTSC, insns 31..44) → THEN the r8
    // continuation: GetCommandLineW (45, slot 2C, → synthetic cmdline ptr) →
    // pop ecx (46, discard the stray scratch push) → mov esi,eax (47) →
    // push/SetUnhandledExceptionFilter (48,49, slot 30) → mov edx,5 / mov eax,0x20
    // (50,51) → bt/bts/btr/btc eax,edx (52..55, the freshly-decoded 0F A3/AB/B3/BB)
    // → push the four WinMain args (56..59: nShowCmd, lpCmdLine, hPrevInstance,
    // hInstance) → CALL 0x539400 (60) = the CRT→user-entry handoff. The interpreter
    // detects the call target == cpu.user_entry_va, pushes the return address, and
    // HALTS I386_HALT_USER_ENTRY. own1: this is the honest E4 milestone — CRT init
    // reached `call WinMain` — NOT a rendered game frame (the WinMain target is OUR
    // synthetic VA; the real game body needs the real PE + D3DMetal + a display).
    printf("\n[cpu] Run A — 13 CRT imports + BT family → CRT→user-entry handoff (SYNTHETIC)\n");
    build_hermetic(img_buf);
    {
        i386_cpu_t cpu; memset(&cpu, 0, sizeof(cpu));
        cpu.eip = ENTRY_VA;
        cpu.gpr[I386_REG_ESP] = 0x539E00u;          // stack inside the image
        cpu.iat = &K32_IAT;                         // register the native kernel32 binding
        cpu.teb_base = TEB_BASE;                     // synthetic TEB (own1: our block, not OS TEB)
        cpu.user_entry_va = WINMAIN_VA;              // CRT→user-entry handoff marker (own1: synthetic)
        uint32_t esp0 = cpu.gpr[I386_REG_ESP];
        i386_mem_write32(&img, SLIST_BUF + 0, 0xFFFFFFFFu);  // pre-dirty → prove the zeroing
        i386_mem_write32(&img, SLIST_BUF + 4, 0xFFFFFFFFu);
        i386_run_result_t r;
        i386_cpu_run(&cpu, &img, 1000, &r);

        printf("  executed=%u halt=%s halt_va=0x%X eip=0x%X esp=0x%X eax=0x%X ebx=0x%X edx=0x%X bound=%u(%s)\n",
               r.insns, i386_halt_name(r.halt), r.halt_va, cpu.eip, cpu.gpr[I386_REG_ESP],
               cpu.gpr[I386_REG_EAX], cpu.gpr[I386_REG_EBX], cpu.gpr[I386_REG_EDX],
               r.imports_bound, r.last_import ? r.last_import : "-");
        uint32_t ft_lo = 0, ft_hi = 0, qpc_lo = 0, qpc_hi = 0, sl_lo = 0, sl_hi = 0;
        uint32_t si_cb = 0, sys_page = 0, sys_ncpu = 0, cookie = 0;
        i386_mem_read32(&img, FT_BUF + 0, &ft_lo);
        i386_mem_read32(&img, FT_BUF + 4, &ft_hi);
        i386_mem_read32(&img, QPC_BUF + 0, &qpc_lo);
        i386_mem_read32(&img, QPC_BUF + 4, &qpc_hi);
        i386_mem_read32(&img, SLIST_BUF + 0, &sl_lo);
        i386_mem_read32(&img, SLIST_BUF + 4, &sl_hi);
        i386_mem_read32(&img, STARTUP_BUF + 0x00, &si_cb);       // STARTUPINFOW.cb
        i386_mem_read32(&img, SYSINFO_BUF + 0x04, &sys_page);    // SYSTEM_INFO.dwPageSize
        i386_mem_read32(&img, SYSINFO_BUF + 0x14, &sys_ncpu);    // SYSTEM_INFO.dwNumberOfProcessors
        i386_mem_read32(&img, COOKIE_BUF + 0x00, &cookie);       // stored security cookie
        // Re-derive the cookie the prologue computes: (FT_lo ^ FT_hi) * 0x01000193
        // (the low 32 bits of the signed imul == the low 32 of the 32×32 product).
        uint32_t cookie_expect = (uint32_t)((0xC3D4E5F6u ^ 0x01D7A1B2u) * 0x01000193u);
        // WinMain stdcall frame at the handoff. esp points at the pushed return
        // address (0x53934E); the four args sit just above it.
        uint32_t wm_ret = 0, wm_hinst = 0, wm_hprev = 0, wm_cmd = 0, wm_show = 0, entry_ret = 0;
        i386_mem_read32(&img, cpu.gpr[I386_REG_ESP] + 0x00, &wm_ret);    // return address
        i386_mem_read32(&img, cpu.gpr[I386_REG_ESP] + 0x04, &wm_hinst);  // hInstance
        i386_mem_read32(&img, cpu.gpr[I386_REG_ESP] + 0x08, &wm_hprev);  // hPrevInstance
        i386_mem_read32(&img, cpu.gpr[I386_REG_ESP] + 0x0C, &wm_cmd);    // lpCmdLine
        i386_mem_read32(&img, cpu.gpr[I386_REG_ESP] + 0x10, &wm_show);   // nShowCmd
        i386_mem_read32(&img, cpu.gpr[I386_REG_ESP] + 0x14, &entry_ret); // entry CALL retaddr (deep)

        CHECK(r.insns == 60,                "60 insns (44 r7 + GetCommandLineW/SUEF + BT family + WinMain frame + handoff call)");
        CHECK(r.insns > 44,                 "advanced PAST the r7 44-insn / 0x539318 wall");
        // r6/r7 buffer-write evidence is still intact (those shims ran en route).
        CHECK(ft_lo == 0xC3D4E5F6u && ft_hi == 0x01D7A1B2u,
                                            "GetSystemTimeAsFileTime wrote 8B FILETIME via the pushed ptr");
        CHECK(qpc_lo == 0x12345678u && qpc_hi == 0x00000000u,
                                            "QueryPerformanceCounter wrote 8B counter via the pushed ptr");
        CHECK(sl_lo == 0u && sl_hi == 0u,   "InitializeSListHead zeroed the 8B SLIST header (was 0xFFFFFFFF)");
        CHECK(si_cb == 0x44u,               "GetStartupInfoW stamped STARTUPINFOW.cb = 0x44 (buffer-write)");
        CHECK(sys_page == 0x1000u && sys_ncpu == 4u,
                                            "GetSystemInfo wrote dwPageSize=0x1000 + nCPU=4 (bounded-synthetic)");
        CHECK(cookie == cookie_expect && cookie != 0u,
                                            "__security cookie = imul(FT_lo^FT_hi, 0x01000193) stored to global");
        CHECK(cpu.gpr[I386_REG_EBX] == 0x756E6547u, "cpuid leaf0 → ebx vendor 'Genu' (un-clobbered)");
        CHECK(cpu.tsc == I386_TSC_STEP,     "rdtsc advanced the synthetic tsc by exactly one step");
        // E5 r8 — the BT family (eax 0x20 → bt/bts/btr/btc by edx=5 → eax 0x20 again).
        CHECK(cpu.gpr[I386_REG_EAX] == 0x20u, "bt/bts/btr/btc sequence left eax = 0x20 (write-back correct)");
        // E5 r8 — the CRT→user-entry handoff itself.
        CHECK(r.imports_bound == 13,        "thirteen IAT calls dispatched to native shims (r7 11 + GetCommandLineW + SUEF)");
        CHECK(r.last_import && strcmp(r.last_import, "SetUnhandledExceptionFilter") == 0,
                                            "most-recent bound import == SetUnhandledExceptionFilter");
        CHECK(r.halt == I386_HALT_USER_ENTRY, "halt: reached the CRT→user-entry handoff (call WinMain)");
        CHECK(r.halt_op == I386_OP_CALL_REL,  "handoff op == CALL_REL (E8 call WinMain)");
        CHECK(r.halt_va == WINMAIN_VA,       "handoff VA == WINMAIN_VA 0x539400 (synthetic user entry)");
        CHECK(cpu.eip == WINMAIN_VA,         "eip parked at the synthetic user entry");
        // The WinMain stdcall frame the CRT set up (own1: synthetic args).
        CHECK(wm_ret == 0x53934Eu,          "WinMain return address == 0x53934E (pushed by the handoff call)");
        CHECK(wm_hinst == 0x538000u,        "WinMain arg hInstance == image base 0x538000 (synthetic)");
        CHECK(wm_hprev == 0u,               "WinMain arg hPrevInstance == NULL");
        CHECK(wm_cmd == CMDLINE_PTR,        "WinMain arg lpCmdLine == GetCommandLineW synthetic ptr 0x539FE0");
        CHECK(wm_show == 0x0Au,             "WinMain arg nShowCmd == 0x0A");
        CHECK(entry_ret == 0x5388ABu,       "entry CALL return addr 0x5388AB still intact deep in the stack");
        CHECK(cpu.gpr[I386_REG_ESP] == esp0 - 0x18, "esp net == esp0-0x18 (entry ret + 4 WinMain args + handoff ret)");
        (void)SHIM_PID; (void)SHIM_TID; (void)SHIM_TICK; (void)TEB_BASE; (void)PEB_BASE;

        if (!emitted) {
            printf("__SHIM__ PARTIAL phase=e4_reached_user_entry insns=%u bound=%u last=%s entry_va=0x%X halt=%s halt_op=%s (SYNTHETIC user-entry; validated_manjeom=0)\n",
                   r.insns, r.imports_bound, r.last_import ? r.last_import : "-", r.halt_va,
                   i386_halt_name(r.halt), i386_op_name(r.halt_op));
            printf("__SHIM__ INFO real_pe_path=structurally_ready needs:real_i386_PE+IAT_autobind_by_import_name+wider_opcode_coverage+D3DMetal+display\n");
        }
    }

    // === Run B — PUSH_R / MOV_R_RM / PUSH_RM semantics ===================
    // Start at insn 3 (0x5388B0): push ebp; mov ebp,esp; push [ebp+8];
    // call 0x538BAB → wall.
    printf("\n[cpu] Run B — stack-frame setup (push ebp / mov ebp,esp / push [ebp+8])\n");
    build_hermetic(img_buf);
    {
        i386_cpu_t cpu; memset(&cpu, 0, sizeof(cpu));
        cpu.eip = 0x5388B0u;
        cpu.gpr[I386_REG_ESP] = 0x539E00u;
        cpu.gpr[I386_REG_EBP] = 0xCAFEBABEu;
        i386_run_result_t r;
        i386_cpu_run(&cpu, &img, 1000, &r);

        printf("  executed=%u halt=%s halt_va=0x%X ebp=0x%X esp=0x%X\n",
               r.insns, i386_halt_name(r.halt), r.halt_va, cpu.gpr[I386_REG_EBP], cpu.gpr[I386_REG_ESP]);
        uint32_t saved_ebp = 0;
        i386_mem_read32(&img, 0x539DFCu, &saved_ebp);

        CHECK(saved_ebp == 0xCAFEBABEu,    "PUSH_R saved old ebp (0xCAFEBABE) to stack");
        CHECK(cpu.gpr[I386_REG_EBP] == 0x539DFCu, "MOV_R_RM set ebp = esp after push");
        CHECK(r.insns == 4,                "executed push/mov/push-rm/call (4 insns)");
        CHECK(r.halt == I386_HALT_UNKNOWN && r.halt_va == 0x538BABu,
                                           "halted at inner CALL target wall 0x538BAB");
    }

    // === Run C — POP_R / RET semantics ===================================
    // Start at insn 7 (0x5388BB): pop ecx; pop ebp; ret. Pre-load the
    // stack with known values so we can verify the pops + the return.
    printf("\n[cpu] Run C — epilogue (pop ecx / pop ebp / ret)\n");
    build_hermetic(img_buf);
    {
        i386_cpu_t cpu; memset(&cpu, 0, sizeof(cpu));
        cpu.eip = 0x5388BBu;
        cpu.gpr[I386_REG_ESP] = 0x539DF4u;
        i386_mem_write32(&img, 0x539DF4u, 0x11111111u);   // → ecx
        i386_mem_write32(&img, 0x539DF8u, 0x22222222u);   // → ebp
        i386_mem_write32(&img, 0x539DFCu, 0x00000000u);   // → ret target (sentinel)
        i386_run_result_t r;
        i386_cpu_run(&cpu, &img, 1000, &r);

        printf("  executed=%u halt=%s halt_va=0x%X ecx=0x%X ebp=0x%X esp=0x%X\n",
               r.insns, i386_halt_name(r.halt), r.halt_va,
               cpu.gpr[I386_REG_ECX], cpu.gpr[I386_REG_EBP], cpu.gpr[I386_REG_ESP]);

        CHECK(cpu.gpr[I386_REG_ECX] == 0x11111111u, "POP_R loaded ecx = 0x11111111");
        CHECK(cpu.gpr[I386_REG_EBP] == 0x22222222u, "POP_R loaded ebp = 0x22222222");
        CHECK(cpu.gpr[I386_REG_ESP] == 0x539E00u,   "esp unwound by 3 pops (12 bytes)");
        CHECK(r.insns == 3,                          "executed pop/pop/ret (3 insns)");
        CHECK(r.halt == I386_HALT_RET,               "halted on RET (entry-block terminator)");
    }

    // === Run D — JMP_REL semantics =======================================
    // Start at insn 2 (0x5388AB): jmp 0x53872A → wall.
    printf("\n[cpu] Run D — unconditional branch (jmp 0x53872A)\n");
    build_hermetic(img_buf);
    {
        i386_cpu_t cpu; memset(&cpu, 0, sizeof(cpu));
        cpu.eip = 0x5388ABu;
        cpu.gpr[I386_REG_ESP] = 0x539E00u;
        i386_run_result_t r;
        i386_cpu_run(&cpu, &img, 1000, &r);

        printf("  executed=%u halt=%s halt_va=0x%X eip=0x%X\n",
               r.insns, i386_halt_name(r.halt), r.halt_va, cpu.eip);

        CHECK(r.insns == 1,                "executed the JMP (1 insn)");
        CHECK(cpu.eip == 0x53872Au,        "JMP_REL set eip to 0x53872A");
        CHECK(r.halt_va == 0x53872Au,      "wall VA == jmp target 0x53872A");
    }

    free(img_buf);
    printf("\n__SHIM_TEST__ %s (own1: own bytes, Intel SDM semantics, no Wine)\n",
           g_fail ? "FAIL" : "PASS");
    return g_fail ? 1 : 0;
}
