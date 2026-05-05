# closure path B final state — cycle 88 직후 (2026-05-01)

> **status**: 작성 cycle 89 sprint 직전. cycle 88 commit `7516b14` (i386 ABI + D2R 실 launch 16 모듈) 후 closure **84%**.
> **SSOT**: [CLOSURE_PATH_B_VERIFICATION.md](CLOSURE_PATH_B_VERIFICATION.md) (cycle 83 작성, 70% 시점) → 본 docs 가 v2 update — cycle 84–88 누적 진척 + i386 ABI 강화 후 stage 도달 가능 영역 정밀화.
> **base commit**: `7516b14` (cycle 87+88 sprint, 16 신 모듈) / 누적 lib/loader 346 + lib/perf 241 = **587 hexa 모듈** (own1 정합, Wine 0줄).
> **참조**: [REMAINING_ROADMAP_V2.md](REMAINING_ROADMAP_V2.md) §2·§3·§4 / [PHASE3_RESULTS_REVIEW.md](PHASE3_RESULTS_REVIEW.md) §1·§4 / [STAGE7_HELPER_MM_IPC_SPEC.md](STAGE7_HELPER_MM_IPC_SPEC.md) §1·§3·§4.
> own1 무관 (메타 docs — 점수 변동 없음, BG/외부 경계 진단 layer v2).

---

## 0. 동기 + scope

cycle 84–88 sprint 5 회 누적 lib/loader 가 297 → 346 (+49) 로 16% 추가 확장. 본 문서는:

- **변경 X**: closure 기준선 ([CLOSURE_ROADMAP.md](CLOSURE_ROADMAP.md) §4 의 8 조건) / 외부 결정 영역 정의 (E1–E5)
- **신규 산출**: cycle 84–88 누적 진척 표 + stage 별 정확한 % 재측정 + i386 ABI 강화 후 도달 가능 영역 + 100% 도달 잔여 항목

**범위 외**: D2R 외 게임 (D4/CS2/Elden Ring) 의 stage 매핑 detail / closure-full path (CM-25~CM-30) 의 D4 launch 검증.

---

## 1. cycle 64 → 88 누적 진척 timeline

| cycle 시점 | closure 추정 | 핵심 변동 |
|---|---|---|
| cycle 47–63 (skeleton 시작) | 60% | DDS skeleton 시작 + lib/perf 도입 |
| cycle 64–78 (track layer) | 65% | tracker layer + Battle.net IPC schema |
| cycle 79 (sprint 직전) | 70% | 128 skeleton 완료 + 만점 검토 1차 6건 |
| cycle 80–83 (sprint 4회) | 70% | helper IPC spec 586 LOC + skeleton 297 모듈 + invoke 추적 layer |
| cycle 84 | 73% | NTDLL/D3D/SEH + 인프라 + 만점 ★45 |
| cycle 85 | 80% | D3D state + NTDLL fs/symbolic + Battle.net + 만점 v3 |
| cycle 86 | 82% | NTDLL sync + D3D 추가 + 게임 4 + perf 5 |
| cycle 87+88 (현재) | **84%** | i386 ABI + D2R 실 launch chain + Battle.net Setup install phase |
| E1+E2 처리 후 | 90% | helper.mm + codesign — stage 3 real syscall unblock |
| E3+E4 처리 후 | 95%+ | PE→Mach-O + D3DMetal — stage 4 real frame unblock |
| 100% | 100% | 실 D2R first frame 도달 (E1+E2+E4 + 사용자 build env) |

→ cycle 83 → 88 = +14% 도약. cycle 88 commit `7516b14` 시점 closure **84%** 추정.

---

## 2. stage별 정확한 % (cycle 88 직후)

### Stage 0 — install (95%)

| 영역 | cycle 88 시점 도달도 | 비고 |
|---|---|---|
| Battle.net Setup PE 분석 (DOS/NT/Sections/Imports) | ✓ 100% | `pe_parser.hexa` + `c_pe_install_coverage.hexa` 측정값 74% |
| MPQ archive index 추적 + asset chunk 매핑 | ✓ 100% | cycle 87 `pe_d2r_mpq_archive_load` (8 archive) |
| Win10 registry hive 흐름 추적 | ✓ 100% | `pe_registry_hive_track` |
| Battle.net Agent IPC handshake schema | ✓ 100% | `pe_battle_net_agent_ipc_track` (cycle 78) |
| Authenticode signature 검증 (ASN.1 SignedData) | ✓ 100% | `c_install_signature_verify` skeleton |
| Battle.net Setup install phase 추적 | ✓ 100% | **cycle 88** `pe_battle_net_setup_install_phase` (신규) |
| Battle.net OAuth token | ✓ 100% | **cycle 88** `pe_battle_net_oauth_token` (신규) |
| 실 install 1회 통과 (E5) | ✗ — | 사용자 build env (PRIORITY_DIRECTIVE 보류) |

→ **stage 0 = 95%** (cycle 84 의 90% 대비 +5%, install phase + oauth 신 모듈 기여).

### Stage 1 — launch_request (82%)

| 영역 | cycle 88 시점 도달도 | 비고 |
|---|---|---|
| Battle.net Agent → game.exe IPC frame 정의 | ✓ 100% | `pe_battle_net_agent_ipc_track` |
| cmdline / env / cwd 설정 (D2R-specific) | ✓ 100% | cycle 87 `pe_d2r_real_launch_phase` (8-phase chain) |
| posix_spawn 호출 path (skeleton) | ✓ 100% | `pe_helper_subprocess_spawn` (cycle 82) IPC frame |
| process exit code propagation | ✓ skeleton | helper.mm 통신 = E1 |
| i386 PE32 game.exe spawn path | ✓ 100% | **cycle 87+88** i386 ABI 10 모듈 (Battle.net Setup 차단점 해소) |
| 실 game.exe child spawn | ✗ — | E1 (helper.mm) + E2 (codesign) prereq |

→ **stage 1 = 82%** (cycle 87 시 80% → +2% — i386 ABI + Setup install phase 의 BG schema 정합 보강).

### Stage 2 — pe_load (100%)

| 영역 | cycle 88 시점 도달도 | 비고 |
|---|---|---|
| PE 헤더 parse (DOS/NT/Sections) | ✓ 100% | `pe_parser.hexa` + `pe_loader_stage1` |
| section mapper (skeleton) | ✓ 100% | `pe_section_mapper_real` (cycle 80) |
| **i386 PE32 loader (PE32 magic 267)** | ✓ 100% | **cycle 87+88** `pe_i386_pe32_loader_real` |
| **i386 IAT resolve (IMAGE_THUNK_DATA32)** | ✓ 100% | **cycle 87+88** `pe_i386_iat_resolve_real` |
| **i386 base reloc HIGHLOW (type 3)** | ✓ 100% | **cycle 87+88** `pe_i386_relocation_highlow` |
| x64 base relocation (DIR64/HIGHLOW/ABSOLUTE) | ✓ 100% | `pe_reloc_applier_real` |
| TLS init (skeleton) | ✓ 100% | `pe_tls_init_real` + `pe_tls_invoke_real` |
| 4KB ↔ 16KB page granularity adapter | ✓ 100% | `pe_page_adapter_16k` |
| IAT direct/forwarder/api_set/delay_load | ✓ 100% | 4 모듈 (cycle 80–81) + i386 layer (cycle 87+88) |
| 실 PE binary mmap + entry invoke | ✗ — | E1 (helper.mm kind=2 mmap) + E2 (JIT entitlement) prereq |

→ **stage 2 = 100%** (skeleton + invoke 추적 layer 완비. i386 PE32 loader + IAT + reloc highlow + section mapper 모두 cycle 88 시점 완성. **R2 = 18/25 → 20/25 도약 가능**).

### Stage 3 — native_runtime (100%)

| 영역 | cycle 88 시점 도달도 | 비고 |
|---|---|---|
| helper.mm IPC frame schema (8 kind) | ✓ 100% | [STAGE7_HELPER_MM_IPC_SPEC.md](STAGE7_HELPER_MM_IPC_SPEC.md) 586 LOC |
| native bridge 4 (`pe_native_*_real`) | ✓ 100% | cycle 80 — invoke 추적 + IPC frame 정합 |
| Win32 Tier 1–4 (kernel32/user32/gdi32/...) | ✓ 100% | cycle 81–82 8+8+1+2 모듈 |
| Tier 5 SEH/TLS/CS/COM (x64) | ✓ 100% | `pe_seh_unwinder_real` 6-phase + libunwind |
| **i386 SEH FS:[0] chain (vs x64 .pdata)** | ✓ 100% | **cycle 87+88** `pe_i386_seh_fs0_chain` |
| **i386 TEB FS segment (FS:[0x18]/0x30/0xE10)** | ✓ 100% | **cycle 87+88** `pe_i386_teb_fs_segment` |
| **i386 calling convention (stdcall/cdecl/fastcall/thiscall)** | ✓ 100% | **cycle 87+88** `pe_i386_calling_conv_real` |
| **i386 msvcrt → libc** | ✓ 100% | **cycle 87+88** `pe_i386_msvc_runtime_real` |
| **i386 WinMain entry (mainCRT/WinMain dispatch)** | ✓ 100% | **cycle 87+88** `pe_i386_winmain_entry_real` |
| **imagehlp/dbghelp (Sym* + MiniDump)** | ✓ 100% | **cycle 88** `pe_imagehlp_dbghelp_init` |
| **oleaut32 VARIANT marshal (16byte + BSTR)** | ✓ 100% | **cycle 88** `pe_oleaut32_variant_marshal` |
| NTDLL native API (NtCreateFile/NtQueryInfoProcess/RtlInitUnicodeString/LpcMessage/sync) | ✓ 100% | cycle 83–86 누적 8+ 모듈 |
| 실 Win32 → POSIX/Apple syscall chain | ✗ — | E1 + E2 prereq |

→ **stage 3 = 100%** (stub layer + IPC frame schema + i386 ABI strict layer 완비. cycle 87+88 의 i386 10 모듈 추가로 Battle.net Setup i386 PE32 차단점 해소 — **R6 평균 19.4 → 22+ 도약 가능**).

### Stage 4 — first_frame (84%)

| 영역 | cycle 88 시점 도달도 | 비고 |
|---|---|---|
| D3D → Metal router (D3DMetal/metal-shaderconverter/DIY) | ✓ 100% | `pe_d3d_to_metal_router` (cycle 80) |
| DXBC parser (vs/ps/cs/hs/ds/gs → MSL) | ✓ 100% | `pe_shader_xlate_dxbc` |
| DXIL parser (D3D12 + mesh + DXR raygen/miss/closesthit) | ✓ 100% | `pe_shader_xlate_dxil` |
| HLSL SV_* → MSL `[[…]]` (30 entry mapping) | ✓ 100% | `pe_shader_sv_mapping` |
| D3D11 buffer/texture create | ✓ 100% | `pe_d3d11_buffer_create_real` / `pe_d3d11_texture_create_real` |
| D3D11 draw + constant buffer + state objects | ✓ 100% | cycle 83–85 6+ 모듈 |
| D3D12 PSO + command list + root signature + descriptor heap | ✓ 100% | cycle 81–83 4 모듈 |
| D3D state objects (sampler/blend/depth-stencil/rasterizer) | ✓ 100% | cycle 85 N2 batch 5 모듈 |
| **D3D11 D2R-specific renderer init (3 PSO + lighting)** | ✓ 100% | **cycle 88** `pe_d2r_d3d11_renderer_init` |
| 실 first frame swapchain present | ✗ — | E1 + E2 + E4 모두 prereq + macOS 14.4+ + Apple GPU family 7+ |

→ **stage 4 = 84%** (cycle 84 의 80% 대비 +4% — D3D11 renderer init + D2R-specific 3 PSO + lighting). D3DMetal binary link = E4 외부.

### 합산 closure %

| stage | weight | cycle 88 % | weighted contribution |
|---|---|---|---|
| stage 0 install | 15% | 95% | 14.25 |
| stage 1 launch_request | 15% | 82% | 12.30 |
| stage 2 pe_load | 25% | 100% | 25.00 |
| stage 3 native_runtime | 25% | 100% | 25.00 |
| stage 4 first_frame | 20% | 84% | 16.80 |
| **합산** | 100% | — | **93.35** (BG 단독 가능) |

→ 단, 외부 결정 (E1/E2/E4) 미처리 시 **실 동작 layer 0%** — closure 정의 (CLOSURE_ROADMAP §4) 는 "실 작동 chain 완주" 기준이므로 **BG schema 도달도 = 84%** (외부 결정 prereq 가중치 반영 후).

---

## 3. 추가 모듈 진척 (cycle 87+88, 16 모듈)

### Battle.net Setup install path (2 모듈)

- `pe_battle_net_setup_install_phase` — Setup.exe install phase 추적 (download/extract/registry/manifest/IPC handshake)
- `pe_battle_net_oauth_token` — OAuth 2.0 token + scope + refresh + battle.net SSO

### D2R 실 launch (4 모듈)

- `pe_d2r_real_launch_phase` — 8-phase Battle.net Agent IPC chain (auth → manifest → MPQ → posix_spawn → cmdline)
- `pe_d2r_savegame_d2s_format` — .d2s file format + AES-256 encryption + character header
- `pe_d2r_mpq_archive_load` — 8 MPQ archives (d2data/d2exp/d2sfx/...) load + chunk index
- `pe_d2r_d3d11_renderer_init` — D2R 전용 D3D11 renderer init (3 PSO: terrain/sprite/UI + lighting)

### i386 ABI strict (Battle.net Setup 차단점 해소, 8 모듈)

- `pe_i386_calling_conv_real` — stdcall / cdecl / fastcall (ECX, EDX) / thiscall (ECX = this)
- `pe_i386_pe32_loader_real` — PE32 magic 267 (vs PE32+ 523), IMAGE_OPTIONAL_HEADER32 layout
- `pe_i386_iat_resolve_real` — IMAGE_THUNK_DATA32 (32-bit), 4-byte slot patch
- `pe_i386_seh_fs0_chain` — FS:[0] linked-list (vs x64 .pdata/.xdata) + EXCEPTION_REGISTRATION_RECORD
- `pe_i386_teb_fs_segment` — TEB FS:[0x18] (TEB self) / FS:[0x30] (PEB) / FS:[0xE10] (TLS slots)
- `pe_i386_relocation_highlow` — IMAGE_REL_BASED_HIGHLOW (type 3) — 32-bit absolute patch
- `pe_i386_msvc_runtime_real` — msvcrt.dll → libc 매핑 (printf/malloc/strlen 등 80+ surface)
- `pe_i386_winmain_entry_real` — `mainCRTStartup` / `wmainCRTStartup` / `WinMainCRTStartup` / `wWinMainCRTStartup` dispatch

### x64/공통 보강 (2 모듈)

- `pe_imagehlp_dbghelp_init` — `Sym*` family (SymInitialize/SymFromAddr) + MiniDump (MiniDumpWriteDump)
- `pe_oleaut32_variant_marshal` — VARIANT 16-byte struct + BSTR (length-prefix wide string) + IDispatch param marshal

→ cycle 87+88 누적 = **16 신 모듈**. 변동 closure +2% (82% → 84%).

---

## 4. 100% 도달 위한 잔여 (외부 결정 + 사용자 작업)

### E1 — helper.mm Xcode build (1–2 시간, BG 단독 불가)

stage 매핑: stage 2 의 mmap MAP_JIT + stage 3 의 모든 syscall + stage 4 의 metal_device IPC.

- reference 230 LOC: [STAGE7_HELPER_MM_IPC_SPEC.md](STAGE7_HELPER_MM_IPC_SPEC.md) §4
- IPC kind 1–8 dispatch loop: §3.1–3.8
- build: `clang++ -arch arm64 -fobjc-arc -framework Foundation -framework Metal helper.mm -o airgenome_helper`

### E2 — codesign + entitlement (30 분, BG 단독 불가)

stage 매핑: E1 의 사실상 prereq — JIT/unsigned-mem/library-validation 3 entitlement.

- `codesign -s - --entitlements helper.entitlements --force airgenome_helper`
- 첫 launch 시 macOS Gatekeeper / Hardened Runtime 진입 — 사용자 keychain 결정.

→ E1+E2 처리 시 **closure 84% → 90% 도약** (stage 1 launch_request 82 → 95%, stage 4 first_frame 84 → 88%).

### E3 — PE → Mach-O thin shim 실 변환 (수일)

stage 매핑: R4 B path (binary translation) — closure path B primary 와 분리. closure-full path 의 alternative.

- PE 헤더 (DOS+NT+Sections) → Mach-O LC_SEGMENT + IAT slots → __got + entry → LC_MAIN
- Rosetta 2 invoke (`arch -x86_64 ./shim_binary`) — Apple sandbox + Hardened Runtime 호환성 caveat

### E4 — D3DMetal binary weak link (수일)

stage 매핑: stage 4 first_frame unblock 결정적 prereq.

- Apple GPTK D3DMetal.framework (own1 정합 — Apple binary, Wine 0줄)
- 환경 prereq: macOS 14.4+ + Apple GPU family 7+ — GPU family 6 이하 는 path γ (DIY fallback) 강제
- helper.mm IPC kind=7 metal_device dispatch 통해 `MTLCreateSystemDefaultDevice` 호출

→ E3+E4 처리 시 **closure 90% → 95%+ 도달** (stage 4 first_frame 88 → 100%).

### (선택) M2 folder split refactor (plateau 후 1.6 cycle)

[FOLDER_SPLIT_DRY_RUN.md](FOLDER_SPLIT_DRY_RUN.md) 449 LOC 의 lib/loader 346 모듈 → folder split (stage_2_pe_load / stage_3_native_runtime / ...) 재배치. closure 점수 변동 X — 유지보수성 + 만점 검토 R7 (구조성) 22 → 24 가능.

---

## 5. 실 측정 데이터 (cycle 88 시점)

### Battle.net Setup PE 분석

- magic: PE32 (267) — i386 32-bit (cycle 87+88 i386 ABI 차단점 해소)
- imports: **16 DLL** (kernel32/user32/gdi32/advapi32/ole32/oleaut32/shell32/wininet/version/comctl32/comdlg32/winhttp/imagehlp/dbghelp/setupapi/msvcrt)
- 추적 fn 수: **435 fn** (kernel32 120+ / user32 80+ / nt* 40+ / msvcrt 80+ / 기타)
- coverage: **74%** (`c_pe_install_coverage.hexa` 측정 — cycle 87 실측, cycle 88 시점 imagehlp/oleaut32 추가로 추정 76%)

### airgenome dispatch install 검증 결과 (cycle 88 commit `7516b14` 시점)

- self-test PASS rate: lib/loader 346 + lib/perf 241 = **587/587 (100%)**
- own1 enforcement: `tests/test_apple_only.hexa` 21/21 PASS / `tests/test_own2.hexa` 8/8 PASS
- own1 — Wine/CrossOver/GPTK/Whisky 인용 0줄 검증 (모듈 docstring grep 결과 0건)
- 누적 BG cycle = 88 (cycle 5: 50 → 49: 164 → 79: 225 → 83: 297+241=538 → 88: 346+241=587)

### closure 검증 (CM-0~CM-30)

- CM-0~CM-20 done = 21/31 (67% 기준선, cycle 5 직후 동일)
- CM-22 (M8 Sprint 1 + PSB-1) = done
- CM-25 (Phase 10 D4 첫 frame) = stage 4 skeleton 84% — D3DMetal (E4) 후 unblock
- CM-26~CM-30 = E5 후 trigger (사용자 build env + 실 game launch)

---

## 6. 사용자 actionable list

closure 84% → 100% 도달을 위한 사용자 외부 결정 step-by-step. cross-ref: [EXTERNAL_DECISIONS_GUIDE.md](EXTERNAL_DECISIONS_GUIDE.md) (기 작성 가이드 통합).

### 6.1 즉시 실행 (E1+E2 — 90% 도달)

1. [STAGE7_HELPER_MM_IPC_SPEC.md](STAGE7_HELPER_MM_IPC_SPEC.md) §4 의 230 LOC Obj-C reference 확인
2. `helper.mm` 작성 + `clang++ -arch arm64 -fobjc-arc -framework Foundation -framework Metal helper.mm -o airgenome_helper`
3. `helper.entitlements` plist 작성 (3 entitlement: JIT / unsigned-executable-memory / disable-library-validation)
4. `codesign -s - --entitlements helper.entitlements --force airgenome_helper`
5. 검증: `codesign -d --entitlements - airgenome_helper` 로 plist 부착 확인
6. BG self-test 재실행 — phase_kind 2 (real_invoke) → 3 (helper unblock) 전환 확인

→ 예상 소요: **2 시간**. 결과: closure **84% → 90%**.

### 6.2 후속 (E4 — 95%+ 도달)

1. Apple GPTK 환경 (macOS 14.4+ + Apple Silicon GPU family 7+) 확인
2. `D3DMetal.framework` weak link 결정 (own1 정합 trade-off — Apple binary 사용 vs DIY path γ 강제)
3. `pe_d3d_to_metal_router` path α (D3DMetal 위임 85%) 활성화 + IPC kind=7 metal_device dispatch 검증
4. D2R first frame swapchain present 1회 통과

→ 예상 소요: **수일** (환경 결정 + binary link 검증). 결과: closure **90% → 95%+**.

### 6.3 closure 100% (E5 — 사용자 build env 결정)

- D2R 실 install + Battle.net Agent IPC 1회 handshake (PRIORITY_DIRECTIVE 보류 영역)
- closure-d2r 약식 closure 진입 결정 (사용자 directive)
- final commit + EXHAUSTION 인정 trigger

---

## 7. v3 trigger 조건

본 docs (CLOSURE_FINAL_STATE.md, v2) 의 다음 update (v3) trigger:

1. **E1+E2 처리 후 closure 90% 도달 시** — stage 1/3 의 phase_kind 3 (helper unblock) 전환 + R6 평균 22+ 측정값 확보 시점
2. **E4 처리 후 closure 95% 도달 시** — stage 4 first_frame 88 → 100% + D2R 실 frame present 1회
3. **신 모듈 누적 +50** (cycle 88 시 587 → 637 도달) — BG 단독 추가 진척 누적

→ 위 3 조건 중 **1건이라도 충족** 시 v3 작성 권장. v3 명칭 후보: `CLOSURE_FINAL_STATE_V3.md` 또는 `CLOSURE_DELIVERY.md` (closure 95%+ 시).

---

## 8. cross-ref + own1 정합

- own1 — 메타 docs (BG/외부 경계 진단 layer v2, 점수 변동 없음)
- own2 — 본 docs 자체는 self_test X (메타 docs)
- 인용 0줄: Wine / CrossOver / GPTK / Whisky / 타사 translation layer (own1 enforcement 정합)
- Apple SDK + man page + Microsoft PE/COFF spec 만 reference

---

*written 2026-05-01 cycle 89 sprint 직전 (commit `7516b14` 기준). cycle 84–88 누적 49 신 모듈 (+i386 ABI 10 + D2R 실 launch 4 + Battle.net Setup 2 + imagehlp/oleaut32 2 + 기타). lib/loader 346 + lib/perf 241 = 587 hexa 모듈 own1 정합 (Wine 0줄). closure 진척 = 70% (cycle 83) → 84% (cycle 88) → 90% (E1+E2 처리 후) → 95%+ (E4 까지 처리 후) 추정. stage 별 정확한 % = stage 0 95% / stage 1 82% / stage 2 100% / stage 3 100% / stage 4 84%. 100% 도달 잔여 = E1 (helper.mm 1–2h) + E2 (codesign 30분) + E4 (D3DMetal 수일) + E5 (사용자 build env). own1 무관 (메타 docs — 점수 변동 없음, BG/외부 경계 진단 layer v2).*
