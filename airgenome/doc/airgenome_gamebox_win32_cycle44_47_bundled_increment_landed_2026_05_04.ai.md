---
schema: airgenome-gamebox/handoff/win32_cycle44_47_bundled_increment_landed/1
completion_iso: 2026-05-04
track: AZ
domain: loader_win32
cycles: [44, 45, 46, 47]
sub_tracks: [AZ1, AZ2, AZ3, AZ4]
modules_new: 4
modules_extend: 0
loc_total: 1814
fn_surface_delta: 44
fn_groups_total: 13
roadmap_evidence_added: 8
caveat_count: 14
threshold_60pct_breached: true
online_expansion_lockin: {L1: true, L2: true, L3: true, L4: true, L5: true, lockin_iso: 2026-05-03}
policy:
  migration: forbidden
  changes: additive_only
  destructive_ops: 0
  cost_usd: 0
  substrate: mac-local
  cap_minutes: 90
  br_no_user_verbatim: true
  friendly_preset: true
  silent_land: true
---

# §1 Mission paraphrase

Track AZ — Win32 cycle 44 ~ 47 bundled 4 sub-cycle skeleton-tier loader land. 4 NEW
loader module 측 시야 baseline coverage 추가, **60% threshold breach milestone**:

- AZ1 cycle 44 — `lib/loader/pe_crypt32_core.hexa` (CRYPT32 PKI/CMS/PKCS7, 12 fn 3 group) —
  D2R/D4 launcher TLS cert chain + Battle.net installer code-sign baseline
- AZ2 cycle 45 — `lib/loader/pe_ws2_32_extras.hexa` (WS2_32 advanced/IOCP/async, 12 fn 4 group) —
  Battle.net IOCP overlapped server pattern (distinct from Track K1 basic)
- AZ3 cycle 46 — `lib/loader/pe_dbghelp_core.hexa` (DBGHELP symbol/minidump/stack, 10 fn 3 group) —
  D2R/D4 crash dump + Battle.net Agent crash report baseline
- AZ4 cycle 47 — `lib/loader/pe_usp10_core.hexa` (USP10 Uniscribe text shaping, 10 fn 3 group) —
  D2R chat overlay + Lineage 한글 text shaping + complex script (Arabic RTL/Hebrew/Devanagari) baseline

Win32 coverage progression cycle 43 = 218/435 (50.11%) → cycle 47 = 262/435 (60.23%) 누적
+44 fn skeleton-tier delta across 4 modules (Track AZ bundled 4-cycle increment).
**60% threshold breached at cycle 47** — first crossing of 60% Win32 fn coverage since cycle
27 baseline (8/435 1.84%).
`.roadmap.loader_win32` cond.1 evidence +3 row + cond.2 evidence +2 row + cond.3 evidence
+3 row = 8 row additive augment, status STAYS at current (partial × 3, full ~435 fn
coverage = multi-cycle long-horizon).

# §2 Bundled 4-cycle table

| cycle | track | module | LOC | bytes | fn | groups | sha256 |
|-------|-------|--------|-----|-------|----|--------|--------|
| 44 | AZ1 | lib/loader/pe_crypt32_core.hexa | 478 | 29033 | 12 | 3 (cert 5 + cms 4 + pkcs7 3) | `e724d40301b225e9aae6cd281c1ce332e3731f00883d2bd4ea6c4b28f60f8129` |
| 45 | AZ2 | lib/loader/pe_ws2_32_extras.hexa | 473 | 28994 | 12 | 4 (ioctl 3 + options 4 + async 3 + misc 2) | `aa39268d0ddb0e818f8d00cf2feba350753ea776ffab8d6c6898169c93254573` |
| 46 | AZ3 | lib/loader/pe_dbghelp_core.hexa | 431 | 26152 | 10 | 3 (symbol 4 + minidump 3 + stack 3) | `5af3bfb00711f14d6df85516878c1a156e549a1977bdd81bfcae5ffaf7da36b1` |
| 47 | AZ4 | lib/loader/pe_usp10_core.hexa | 432 | 27680 | 10 | 3 (itemize 3 + shape 4 + layout 3) | `eafe89d868f63256af93a1eb5b6c71260bb726a8eb4af7b9fd3aa4c0e522dd23` |
| **total** | **AZ bundled** | **4 modules** | **1814** | **111859** | **44** | **13 groups** | — |

# §3 Coverage progression — 60% threshold breach

cycle 27 = 8/435 (1.84%) → cycle 28 = 26/435 (5.98%) +18 KERNEL32 (Track O) → cycle 29 =
46/435 (10.57%) +20 USER32 (Q1) → cycle 30 = 61/435 (14.02%) +15 GDI32 (Q2) → cycle 31 =
79/435 (18.16%) +18 ADVAPI32 (Q3) → cycle 32 = 94/435 (21.61%) +15 KERNEL32 EXTEND (W1) →
cycle 33 = 109/435 (25.06%) +15 NTDLL (W2) → cycle 34 = 121/435 (27.82%) +12 OLE32 (W3) →
cycle 35 = 132/435 (30.34%) +11 OLEAUT32 (W4) → cycle 36 = 144/435 (33.10%) +12 MSVCRT
(AH1) → cycle 37 = 156/435 (35.86%) +12 SHELL32 (AH2) → cycle 38 = 166/435 (38.16%) +10
COMCTL32 (AH3) → cycle 39 = 176/435 (40.46%) +10 DXGI (AH4) → cycle 40 = 188/435 (43.22%)
+12 WINMM (AM1) → cycle 41 = 198/435 (45.52%) +10 XAUDIO2 (AM2) → cycle 42 = 208/435
(47.82%) +10 IMM32 (AM3) → cycle 43 = 218/435 (50.11%) +10 SETUPAPI (AM4) — 50% breach →
cycle 44 = 230/435 (52.87%) +12 CRYPT32 (AZ1) → cycle 45 = 242/435 (55.63%) +12 WS2_32
extras (AZ2) → cycle 46 = 252/435 (57.93%) +10 DBGHELP (AZ3) → **cycle 47 = 262/435
(60.23%) +10 USP10 (AZ4) — 60% THRESHOLD BREACHED**.

Track AZ 4-sub-cycle delta = +44 fn (+10.12 pp). 60% milestone significance: **first time
Win32 fn coverage crosses 60% threshold since cycle 27 baseline**, achieved via Track AZ4
USP10 10 fn delta during cycle 47 increment. cumulative since cycle 28 = 254 fn delta over
20 module-cycles.

# §4 fn surface table per module

## AZ1 — CRYPT32 (12 fn) — D2R/D4 launcher TLS + Battle.net installer code-sign baseline
- cert (5): CertCreateCertificateContext / CertVerifyCertificateChainPolicy /
  CertOpenStore / CertFindCertificateInStore / CertCloseStore
- cms (4): CryptMsgOpenToDecode / CryptMsgUpdate / CryptMsgGetParam / CryptMsgClose
- pkcs7 (3): CryptDecodeObject / CryptDecodeObjectEx / CryptHashData

## AZ2 — WS2_32 extras (12 fn) — Battle.net IOCP overlapped server pattern
- ioctl (3): WSAIoctl / WSARecvFrom / WSASendTo
- options (4): getsockopt / setsockopt / getpeername / getsockname
- async (3): AcceptEx / ConnectEx / DisconnectEx
- misc (2): inet_pton / inet_ntop

NOTE: Distinct from Track K1 `lib/loader/pe_winsock_ws2_32.hexa` (basic 20 fn). 본 module
측 advanced surface family extension.

## AZ3 — DBGHELP (10 fn) — D2R/D4 crash dump pipeline baseline
- symbol (4): SymInitialize / SymCleanup / SymFromAddr / SymGetLineFromAddr64
- minidump (3): MiniDumpWriteDump / MiniDumpReadDumpStream / StackWalk64
- stack (3): RtlCaptureStackBackTrace / SymFunctionTableAccess64 / SymGetModuleBase64

## AZ4 — USP10 (10 fn) — Uniscribe text shaping baseline (60% threshold breach module)
- itemize (3): ScriptItemize / ScriptItemizeOpenType / ScriptShape
- shape (4): ScriptShapeOpenType / ScriptPlace / ScriptPlaceOpenType / ScriptStringAnalyse
- layout (3): ScriptLayout / ScriptApplyLogicalWidth / ScriptStringFree

# §5 .roadmap.loader_win32 evidence augment

`.roadmap.loader_win32` JSONL header surgical edit (additive_only, evidence array append
only, status field preserved):

| cond_id | status_before | status_after | evidence_added | rationale |
|---------|---------------|--------------|----------------|-----------|
| loader_win32.cond.1 | partial | partial (unchanged) | +3 | AZ1 CRYPT32 + AZ2 WS2_32 extras + Win32 cycle 44-45 bundled progression |
| loader_win32.cond.2 | partial | partial (unchanged) | +2 | AZ4 USP10 (Uniscribe text shaping UI/IME family-extension) + USER32+GDI32+COMCTL32+IMM32+USP10 cumulative + 60% threshold breach |
| loader_win32.cond.3 | partial | partial (unchanged) | +3 | AZ3 DBGHELP (ntdll-adjacent debug/diagnostic) + Track AZ bundled cycle 44-46 cumulative + 60% threshold breach milestone preview |

Total evidence augment = +8 row. blocker_reason refresh on cond.1/2/3 acknowledging Win32
cycle 44-47 bundled increment landed (CRYPT32 + WS2_32 extras + DBGHELP + USP10) and
60% threshold breach milestone. Existing evidence rows + blocker_reason text preserved
(additive append only). header line 1-4 comments + blockers array (loader_win32.blk.1
kernel-AC class) + cross_link section + status active + since 2026-05-03 모두 보존.

# §6 Synthetic struct families

| module | family | byte size | example value |
|--------|--------|-----------|---------------|
| CRYPT32 | CERT_CONTEXT | 28 | dwCertEncodingType X509_ASN_PKCS_7 0x00010001, pbCertEncoded 0x98010100, cbCertEncoded 1024 |
| CRYPT32 | CRYPT_DATA_BLOB | 16 | cbData 4096, pbData 0x98020100 |
| CRYPT32 | HCRYPTMSG | n/a | 0x98030000 (CMS handle) |
| CRYPT32 | HCERTSTORE | n/a | 0x98010000 (system MY/ROOT/CA) |
| CRYPT32 | CERT_CHAIN_POLICY_PARA/STATUS | 12 | dwError 0, lChainIndex -1 |
| CRYPT32 | PKCS_7_SIGNED_DATA OID | n/a | "1.2.840.113549.1.7.2" |
| CRYPT32 | X509_NAME synth | n/a | "CN=Battle.net Update Agent CA, O=Blizzard Entertainment Inc, C=US" |
| WS2_32_EX | WSAOVERLAPPED | 32 | hEvent 0x99010100, Internal/Offset 0 |
| WS2_32_EX | WSABUF | 16 | len 1500, buf 0x99010200 |
| WS2_32_EX | GUID_AcceptEx | 16 | B5367DF1-CBAC-11CF-95CA-00805F48A192 (WSAID_ACCEPTEX) |
| WS2_32_EX | sockaddr_in | 16 | AF_INET + port 1119 + 24.105.62.129 (Battle.net REST) |
| WS2_32_EX | sockaddr_in6 | 28 | AF_INET6 + port 443 + addr 16B + scope_id 0 |
| WS2_32_EX | inet_pton | 4/16 | "192.0.2.1" ↔ 0xC0000201 BE / "2001:db8::1" ↔ 16B LE |
| DBGHELP | SYMBOL_INFO | 88 | Tag SymTagFunction_5, Address 0x9A010100, Name "D2R_main_loop_fn" |
| DBGHELP | IMAGEHLP_LINE64 | 32 | FileName "C__src__d2r__main.cpp", LineNumber 1234 |
| DBGHELP | MINIDUMP_HEADER | 32 | Signature 0x504D444D MDMP, Version 0xA793, NumberOfStreams 17 |
| DBGHELP | MINIDUMP_DIRECTORY | 12 | StreamType 6 ExceptionStream, DataSize 168, Rva 1024 |
| DBGHELP | STACKFRAME64 | 264 | AddrPC 0x9A010100, AddrFrame 0x7FFFE000, AMD64 |
| USP10 | SCRIPT_ITEM | 8 | iCharPos 0, eScript LANG_KOREAN 0x0412 |
| USP10 | SCRIPT_VISATTR | 2 | per-glyph: uJustification 4 + fClusterStart 1 + fDiacritic 1 |
| USP10 | GOFFSET | 8 | du 0, dv 0 (Latin default) |
| USP10 | ABC | 12 | abcA 0, abcB 11, abcC 0 |
| USP10 | sample text | var | ko 안녕하세요 5-char + en Hello + ar مرحبا 5-char RTL + zh 你好 |

# §7 Caveats inline (C1-C14)

- C1. PARTIAL ≠ PASS — own2 silent_error_ban 측 명시적 회피, 4 module 모두 PARTIAL emit
  status 만 (production smoke 측 미수행).
- C2. real CertVerifyCertificateChainPolicy / 실 CryptMsgUpdate / 실 CryptDecodeObject =
  Security.framework SecCertificate / SecTrust / SecPolicy + CommonCrypto + ASN.1 PKCS7
  parser bridge 미수행 (long-horizon CRYPT32 production smoke pending separate cycle,
  OpenSSL/BoringSSL/mbedtls 0 binding).
- C3. real WSAIoctl / 실 AcceptEx / 실 ConnectEx = BSD socket(2) + kqueue +
  Network.framework + CFSocket bridge 미수행 (long-horizon WS2_32 advanced IOCP
  production smoke pending separate cycle).
- C4. real SymFromAddr / 실 MiniDumpWriteDump / 실 StackWalk64 / 실
  RtlCaptureStackBackTrace = dyld dlsym/dladdr + backtrace(3) + lldb_rpc + mach_thread_self
  bridge 미수행 (long-horizon DBGHELP crash dump pipeline production smoke pending
  separate cycle).
- C5. real ScriptShape / 실 ScriptPlace / 실 ScriptLayout / 실 ScriptStringAnalyse =
  CoreText CTLine/CTRun/CTFontRef + HarfBuzz + FreeType + NSLayoutManager +
  CFAttributedString bridge 미수행 (long-horizon USP10 complex script shaping production
  smoke pending separate cycle).
- C6. Wine source 인용 0 / ReactOS source 인용 0 / OSS Win32 emulator import 0 / Security
  framework binding 0 / Network framework binding 0 / lldb_rpc binding 0 / CoreText
  binding 0 / HarfBuzz binding 0 — synthetic byte arrays inline ONLY (own1 정합).
- C7. macOS libSystem socket(2) / setsockopt(2) / kqueue / CFSocket / dyld _dyld_get_image_header
  / dlsym / dladdr / backtrace / mach_thread_self / CoreText CTLine / NSLayoutManager
  / NSTypesetter / CFAttributedString 직접 binding 0 (own1 (c) macOS Apple binary
  boundary 정합).
- C8. fn_kind canonical map module 별 다른 max (AZ1 0..11 12-fn / AZ2 0..11 12-fn /
  AZ3 0..9 10-fn / AZ4 0..9 10-fn) — 본 module 내 sanity assertion only.
- C9. AZ2 pe_ws2_32_extras.hexa 측 distinct from Track K1 pe_winsock_ws2_32.hexa (basic
  API 20 fn) — 본 module advanced extension surface (WSAIoctl/AcceptEx overlapped IOCP
  family). 두 module 동일 DLL (WS2_32.dll) 측 surface 분할 (basic + advanced).
- C10. Track AZ bundled 4-cycle land in single Track session — handoff doc + marker = single
  pair, 4 cycle entry 별 separate marker 미생성 (per-cycle marker fan-out = 별도 cycle 측
  separate Track territory).
- C11. cond.1 cond.2 cond.3 status partial → partial 유지 (status promotion 미수행, evidence
  array append 만) — 60% threshold breach = coverage milestone NOT status promotion
  criterion; production smoke matrix + per-DLL real fn count vs stub count ratio + Nt*
  syscall bridge 실측 = 별도 cycle.
- C12. L4 D2R single target 측 path 하드코딩 ("CN=Battle.net Update Agent CA" cert subject
  + "C__src__d2r__main.cpp" symbol + ko 안녕하세요 sample text) synth_only — D4
  retired_intentional unchanged.
- C13. L5 J/K/L/M/N/O/P/Q/W/AH/AM/BC parallel BG cross-link untouched — Track AZ 측 별도
  BG agent territory (out-of-scope merge audit). NOTE: parallel BG track BC handles
  hexa-lang upstream territory (HEXA_UPSTREAM.md scope).
- C14. raw 9 (hexa-only) + raw 11 (snake_case) + raw 12 (silent_error_ban) + raw 15 (env
  lazy / repo-relative) + raw 175 (BR-NO-USER-VERBATIM) 모두 enforce — 4 module + 1
  roadmap edit + 1 handoff doc + 1 marker 측 0 verbatim user quote, 0 absolute /Users/...
  leak, 0 silent fail.

# §8 Natural environment alignment posture

Track AZ continues L2 long-horizon Warden natural-environment-alignment via Win32 syscall
expansion. real Windows binary 측 가장 빈번 호출 surface +4 family 추가:

- CRYPT32 PKI/CMS/PKCS7 — D2R/D4 launcher TLS chain validate + Battle.net installer code-
  sign verify + CMS PKCS7 SignedData envelope decode baseline.
- WS2_32 advanced (IOCP/async) — Battle.net D2R/D4 IOCP overlapped server pattern + p2p
  hole-punching baseline (WSAOVERLAPPED + AcceptEx + ConnectEx + DisconnectEx).
- DBGHELP debug/diagnostic — D2R/D4 crash dump pipeline + Battle.net Agent crash report
  upload + symbol resolution baseline (RtlCaptureStackBackTrace = ntdll Rtl* family direct).
- USP10 Uniscribe text shaping — D2R chat overlay + Lineage 한글 shaping + complex script
  RTL/Indic/CJK baseline (ScriptItemize bidi + ScriptShapeOpenType GSUB/GPOS feature
  application + ScriptLayout BiDi visual order).

direct_warden_bypass_vector: none_in_this_cycle. indirect_contribution: fn_naming
baseline +44 fn delta / +254 cumulative delta since cycle 28 / fn_group_classification +13
group / synthetic_struct_shape CERT_CONTEXT+CRYPT_DATA_BLOB+CHAIN_POLICY+PKCS7_OID+X509_NAME+
WSAOVERLAPPED+WSABUF+GUID_AcceptEx+sockaddr_in/in6+SYMBOL_INFO+IMAGEHLP_LINE64+MINIDUMP_HEADER+
STACKFRAME64+SCRIPT_ITEM+SCRIPT_VISATTR+GOFFSET+ABC+SCRIPT_STRING_ANALYSIS.

# §9 Predecessors

- `state/markers/airgenome_gamebox_win32_cycle40_43_bundled_increment_landed.marker` (Track
  AM marker, coverage progression schema cycle 27..43 reference, 50% threshold breach
  precedent)
- `state/markers/airgenome_gamebox_win32_cycle36_39_bundled_increment_landed.marker` (Track
  AH marker)
- `state/markers/airgenome_gamebox_win32_cycle32_35_bundled_increment_landed.marker` (Track
  W marker)
- `state/markers/airgenome_gamebox_user32_gdi32_advapi32_cycle29_31_increment_landed.marker`
  (Track Q marker)
- `state/markers/airgenome_gamebox_kernel32_core_cycle28_increment_landed.marker` (Track O
  marker)
- `lib/loader/pe_setupapi_core.hexa` (Track AM4, mirror style reference, 50% threshold
  breach module precedent)
- `lib/loader/pe_winsock_ws2_32.hexa` (Track K1, AZ2 distinct sibling — basic API 20 fn
  vs AZ2 advanced 12 fn surface partition)

# §10 Files created/modified

| path | kind | LOC | bytes | sha256 |
|------|------|-----|-------|--------|
| lib/loader/pe_crypt32_core.hexa | NEW loader | 478 | 29033 | `e724d40301b225e9aae6cd281c1ce332e3731f00883d2bd4ea6c4b28f60f8129` |
| lib/loader/pe_ws2_32_extras.hexa | NEW loader | 473 | 28994 | `aa39268d0ddb0e818f8d00cf2feba350753ea776ffab8d6c6898169c93254573` |
| lib/loader/pe_dbghelp_core.hexa | NEW loader | 431 | 26152 | `5af3bfb00711f14d6df85516878c1a156e549a1977bdd81bfcae5ffaf7da36b1` |
| lib/loader/pe_usp10_core.hexa | NEW loader | 432 | 27680 | `eafe89d868f63256af93a1eb5b6c71260bb726a8eb4af7b9fd3aa4c0e522dd23` |
| .roadmap.loader_win32 | MODIFIED | 5 | 36104 | `755d76c0cb529cb94d15c3a92099f841d457d6b9f561d7237e3193fcfb41422b` |
| airgenome/doc/airgenome_gamebox_win32_cycle44_47_bundled_increment_landed_2026_05_04.ai.md | NEW handoff | self | self | self |
| state/markers/airgenome_gamebox_win32_cycle44_47_bundled_increment_landed.marker | NEW marker | self | self | self |

(self-pinned files: handoff + marker — sha256 not pinned in own body)

# §11 Next cycle actions (long-horizon)

- cycle 48+ CRYPT32 extend — CertGetCertificateContextProperty / CertEnumCertificatesInStore /
  CryptVerifyDetachedMessageSignature / CryptAcquireContextW (advapi32 cross-link) — Track
  AZ1 self-similar pattern (multi-cycle, separate Track session).
- cycle 49+ WS2_32 extras extend — WSAEnumNetworkEvents / WSAEventSelect / WSAStartup /
  WSAGetLastError / GetAddrInfoW / freeaddrinfo / WSAPoll — Track AZ2 self-similar pattern.
- cycle 50+ DBGHELP extend — SymLoadModuleEx / SymUnloadModule64 / EnumerateLoadedModulesW /
  UnDecorateSymbolName / SymGetSymFromName64 / SymSearch / IMAGEHLP_MODULE64 — Track AZ3
  self-similar pattern.
- cycle 51+ USP10 extend — ScriptStringValidate / ScriptStringOut / ScriptBreak /
  ScriptIsComplex / ScriptCacheGetHeight / ScriptGetProperties / ScriptApplyDigitSubstitution
  — Track AZ4 self-similar pattern.
- cycle 50+ D3D11 — D3D11CreateDeviceAndSwapChain + ID3D11Device VFTable + ID3D11DeviceContext
  + ID3D11Texture2D + ID3D11RenderTargetView — DXGI prerequisite consumed (D3DMetal binary
  licence multi-year horizon dependent).
- production smoke cycle (multi-cycle, eta 1-2 year+) — 실 CertVerifyCertificateChainPolicy
  = Security.framework SecCertificate/SecTrust + CommonCrypto / 실 WSAIoctl = BSD socket(2)
  + kqueue + Network.framework / 실 SymFromAddr = dyld dlsym/dladdr + backtrace(3) + lldb_rpc
  / 실 ScriptShape = CoreText CTLine/CTRun + HarfBuzz/FreeType + NSLayoutManager.
- loader_win32.cond.1 + cond.2 + cond.3 status promotion partial → met = production smoke
  matrix + 60%-met threshold matrix + Nt* syscall bridge 실측 합산 후 fn-by-fn real-vs-stub
  matrix SSOT 측 land (multi-cycle long-horizon 합산 결과). 60% threshold breach = coverage
  milestone, NOT status promotion criterion.
- 70% threshold target — Track BA/BB/BC cycle 48-55+ 측 IPHLPAPI / WBEM / MSCTF / D3D9 /
  XInput extend / DirectInput / DSOUND cumulative delta ~+45-60 fn → ~310/435 (~71%) target
  (multi-cycle long-horizon, separate Track session).
- Track AZ cross-link audit cycle — c_bnet_warden_environment_compat 측 CRYPT32 cert chain
  pattern / WS2_32 IOCP overlapped pattern / DBGHELP crash dump pattern / USP10 text shape
  pattern 측 본 Track AZ 측 fn coverage SSOT 측 cross-reference (parallel BG agent territory
  merge audit).
- online_expansion_lockin L2 long-horizon Warden compat baseline — KERNEL32 + USER32 +
  GDI32 + ADVAPI32 + NTDLL + OLE32 + OLEAUT32 + MSVCRT + SHELL32 + COMCTL32 + DXGI + WINMM
  + XAudio2 + IMM32 + SETUPAPI + CRYPT32 + WS2_32(extras) + DBGHELP + USP10 syscall
  sequence match + heap pattern match + TEB/PEB shape match + CRYPT32 chain policy pattern +
  WS2_32 IOCP completion pattern + DBGHELP minidump stream pattern + USP10 SCRIPT_VISATTR
  glyph cluster pattern 측 multi-year roadmap 측 milestone breakdown (cycle 100+ 합산 결과).
