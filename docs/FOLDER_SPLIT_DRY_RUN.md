# Folder split refactor — dry-run + 강행 가이드

> **status**: 작성 2026-05-01 / 사용자 directive: "남은 영역 all bg go"
> **결론 한줄**: 본 cycle 82 시점 plateau 미도달 (in-flight BG 진행 중) → 실 refactor 보류 유지. 본 docs 는 plateau 도달 시 1 cycle 강행 가이드.
> SSOT: `docs/FOLDER_SPLIT_REFACTOR_MANJEOM_REVIEW.md` (18/25, 보류 유지)
> 본 docs 는 시뮬레이션 only — 실 `mv` / `sed` / git mv 0 회 실행됨.

---

## 0. 본 dry-run 의 위치

| 축 | 값 |
| --- | --- |
| Cycle | 82 (M2) |
| Directive | "남은 영역 all bg go" — 사용자 |
| 만점 검토 (SSOT) | 18/25, 결론 = 보류 유지 |
| Dry-run 결론 | plateau 미도달, 본 cycle 강행 X |
| 본 docs 의 의의 | plateau 도달 시 즉시 실행 가능한 step list 사전 작성 |

---

## 1. 현재 상태 실측 (2026-05-01)

### 1.1 lib/loader/*.hexa 총 파일 수
```
$ ls lib/loader/*.hexa | wc -l
259
```

### 1.2 Prefix 분류 (단일 단어 first-token 기준)
```
$ ls lib/loader/*.hexa | xargs -I{} basename {} .hexa | sed 's/_.*//' | sort | uniq -c | sort -rn
   194 pe
    55 win32
     5 dx
     1 syscall
     1 oss
     1 native
     1 iat
     1 i386
     1 dll
     1 d4
     1 d2r
     1 battlenet
```

### 1.3 Prefix 세분화 (pe_* 내부 sub-domain)
| sub-domain | count | 해당 파일 패턴 |
| --- | --- | --- |
| `pe_ipc_*` | 16 | mach_port, xpc_protocol, mqueue, named_pipe, shared_memory, ... |
| `pe_native_*` | 13 | dlopen_real, dlopen_wrapper, mmap_real, pthread_real, metal_device_wrapper, ... |
| `pe_d3d* / pe_dxgi / pe_d2d / pe_dwrite / pe_metal_*` | ~13 | d3d11_buffer_create_real, d3d12_pipeline_state, dxgi_swapchain, d2d_render_target, dwrite_text_layout, metal_buffer_pool, metal_texture_residency, ... |
| `pe_*` (rest) | ~152 | iat, reloc, tls, seh, heap, gdi, wininet, winhttp, com, console, registry, ... |

### 1.4 Import 패턴 통계
```
$ grep -rl 'use "' lib/loader/   | wc -l
264   # use 문 보유 파일 (전체 259 중 다수 — comment 포함하면 +5)

$ grep -rh 'use "' lib/loader/   | wc -l  (대략)
276   # 총 use 라인 수

$ grep -rh 'use "../perf/' lib/loader/ | wc -l
≈276 (대다수가 "../perf/r0_common")
```
대다수 loader 파일은 **단일 import** = `use "../perf/r0_common"` (own 1 mandate).
일부 stage2/orchestrator 파일은 sibling loader import 보유:
- `use "./pe_imports"` (orchestrator)
- `use "./pe_loader_stage1"` (stage2 chain)
- `use "./pe_runtime_map"` (runtime resolve)
- `use "./win32_abi_i386"` (i386 dispatch)

---

## 2. 권장 폴더 구조 7-bucket

> 원칙: prefix-based partition, file count balance, sibling import 최소화.

### 2.1 `lib/loader/pe/` — ~152 files (PE core)
> pe_* 중 ipc / native / d3d-dxgi-metal / dwrite-d2d 제외.
주요 파일 (sample):
- `pe_loader_stage1.hexa`, `pe_loader_stage2.hexa`, `pe_loader_invoke_real.hexa`
- `pe_iat_*` (delay_load_thunk, direct_resolve_real, forwarder_chain_real, resolve_stage2, api_set_redirect, runtime_bind)
- `pe_reloc_*`, `pe_tls_*`, `pe_seh_*`, `pe_heap_stage2`, `pe_stack_stage2`, `pe_entry_*`
- `pe_imports.hexa`, `pe_parser.hexa`, `pe_relocations.hexa`, `pe_export_table.hexa`, `pe_runtime_map.hexa`
- `pe_section_mapper_real.hexa`, `pe_resource_section.hexa`, `pe_debug_directory.hexa`
- `pe_handle_table_real.hexa`, `pe_handle_translation.hexa`, `pe_kuser_shared_data.hexa`
- `pe_thread_*`, `pe_critical_section.hexa`, `pe_mutex_object.hexa`, `pe_event_object.hexa`, `pe_semaphore_object.hexa`, `pe_timer_object.hexa`
- `pe_winhttp_request.hexa`, `pe_wininet_cache.hexa`, `pe_winsock_iocp.hexa`, `pe_winsock_overlapped.hexa`, `pe_winsock_select.hexa`, `pe_https_certificate.hexa`, `pe_dns_query.hexa`, `pe_iphlpapi_adapter.hexa`
- `pe_console_*`, `pe_registry_*`, `pe_clipboard_format.hexa`, `pe_taskbar_progress.hexa`
- `pe_charset_convert.hexa`, `pe_collation_compare.hexa`, `pe_locale_info.hexa`, `pe_unicode_*`, `pe_message_table.hexa`, `pe_resource_string.hexa`, `pe_image_codec.hexa`
- `pe_com_*`, `pe_ole_*`, `pe_mf_media_session.hexa`, `pe_xaudio2_voice.hexa`, `pe_xinput_gamepad.hexa`
- `pe_ime_*`, `pe_glyph_metrics.hexa`, `pe_print_spooler.hexa`, `pe_acl_descriptor.hexa`, `pe_token_privilege.hexa`, `pe_impersonate_token.hexa`
- `pe_signature_verify.hexa`, `pe_manifest_parser.hexa`, `pe_change_notification.hexa`, `pe_volume_information.hexa`, `pe_command_line_parse.hexa`, `pe_environment_block.hexa`, `pe_peb_struct.hexa`, `pe_teb_struct.hexa`, `pe_ldr_module_table.hexa`
- `pe_ntdll_native_api.hexa`, `pe_object_attributes.hexa`, `pe_syscall_table.hexa`, `pe_signal_translation.hexa`, `pe_apc_queue.hexa`, `pe_dpc_simulate.hexa`, `pe_vectored_exception.hexa`, `pe_seh_handler_register.hexa`, `pe_seh_unwind.hexa`, `pe_debug_break.hexa`, `pe_terminate_thread.hexa`, `pe_create_remote_thread.hexa`, `pe_create_process.hexa`, `pe_job_object.hexa`, `pe_process_protection.hexa`, `pe_cfg_guard.hexa`
- `pe_file_*`, `pe_pipe_*`, `pe_mailslot_create.hexa`, `pe_io_completion_port.hexa`, `pe_wait_for_*`
- `pe_memory_pool_*`, `pe_mmap_protection.hexa`, `pe_low_memory_purgeable.hexa`, `pe_vm_pressure_handler.hexa`, `pe_page_adapter_16k.hexa`, `pe_heap_walk.hexa`, `pe_jit_codegen_track.hexa`, `pe_to_macho_shim_track.hexa`, `pe_dyld_bridge.hexa`, `pe_dyld_cache_warmup.hexa`, `pe_pe_install_coverage_v2.hexa`, `pe_runtime_map.hexa`, `pe_warm_launch_cache.hexa`, `pe_cold_launch_phase.hexa`
- `pe_call_conv_*` (cdecl, fastcall, stdcall, vectorcall, x64_ms, to_arm64, thiscall_translate), `pe_va_args_marshal.hexa`, `pe_thiscall_translate.hexa`
- `pe_stub_address_table.hexa`, `pe_delay_load_imports.hexa`, `pe_tls_callback_dispatch.hexa`, `pe_tls_data_section.hexa`, `pe_tls_init_real.hexa`, `pe_tls_invoke_real.hexa`
- `pe_input_latency_track.hexa`, `pe_frame_pacing_metrics.hexa`, `pe_frame_compositor_queue.hexa`, `pe_swapchain_present_track.hexa`, `pe_gpu_memory_residency.hexa`, `pe_shader_xlate_dxbc.hexa`, `pe_shader_xlate_dxil.hexa`, `pe_shader_sv_mapping.hexa`

### 2.2 `lib/loader/win32/` — 55 files
모든 `win32_*.hexa`. 주요:
- `win32_kernel32*.hexa` (8 files: base + extras + stage1~5 + tier1_real + tier2_real)
- `win32_user32*.hexa` (4: base + stage1, stage2, tier1_real)
- `win32_gdi32*.hexa` (3: base + stage2 + tier1_real)
- `win32_advapi32*.hexa` (3), `win32_ole32*.hexa` (3), `win32_oleaut32*.hexa` (2)
- `win32_shell32*.hexa` (3), `win32_version*.hexa` (3), `win32_winhttp.hexa`
- `win32_wininet*.hexa` (3), `win32_winsock*.hexa` (3), `win32_wintrust*.hexa` (2)
- `win32_crypt32*.hexa` (2), `win32_msimg32*.hexa` (2), `win32_rpcrt4*.hexa` (2), `win32_uiautomationcore*.hexa` (2)
- `win32_msvcrt.hexa`, `win32_ntdll.hexa`, `win32_emulate_kernel32.hexa`, `win32_stubs.hexa`
- `win32_abi.hexa`, `win32_abi_i386.hexa`

### 2.3 `lib/loader/dx/` — 16 files
- `dx_d3d11.hexa`, `dx_d3d11_stage1.hexa`, `dx_d3d12.hexa`, `dx_d3d12_stage1.hexa`, `dx_d3d12_stage2.hexa`
- `pe_d3d11_buffer_create_real.hexa`, `pe_d3d11_compute_shader.hexa`, `pe_d3d11_query.hexa`, `pe_d3d11_texture_create_real.hexa`
- `pe_d3d12_command_list.hexa`, `pe_d3d12_mesh_shader.hexa`, `pe_d3d12_pipeline_state.hexa`, `pe_d3d12_raytracing.hexa`
- `pe_d3d_to_metal_router.hexa`, `pe_dxgi_factory.hexa`, `pe_dxgi_swapchain.hexa`
- `pe_d2d_render_target.hexa`, `pe_dwrite_text_layout.hexa`
- `pe_metal_buffer_pool.hexa`, `pe_metal_texture_residency.hexa` (Metal bridge for d3d→Metal — bucket 경계 이슈, dx/ 둠)

### 2.4 `lib/loader/native/` — 14 files (POSIX/dyld bridge)
- `native_helper_bridge.hexa`
- `pe_native_dlopen_real.hexa`, `pe_native_dlopen_wrapper.hexa`, `pe_native_dlsym_wrapper.hexa`
- `pe_native_mmap_real.hexa`, `pe_native_mmap_wrapper.hexa`
- `pe_native_mprotect_real.hexa`, `pe_native_mprotect_wrapper.hexa`
- `pe_native_pthread_real.hexa`, `pe_native_pthread_wrapper.hexa`
- `pe_native_callback_dispatch.hexa`, `pe_native_helper_invoke.hexa`
- `pe_native_metal_device_wrapper.hexa`, `pe_native_nsobject_lifetime.hexa`

### 2.5 `lib/loader/ipc/` — 16 files
- `pe_ipc_async_callback.hexa`, `pe_ipc_error_propagate.hexa`, `pe_ipc_handshake.hexa`
- `pe_ipc_health_monitor.hexa`, `pe_ipc_mach_port.hexa`, `pe_ipc_message_dispatch.hexa`
- `pe_ipc_message_serialize.hexa`, `pe_ipc_mqueue.hexa`, `pe_ipc_named_pipe.hexa`
- `pe_ipc_protocol_versioning.hexa`, `pe_ipc_request_response.hexa`, `pe_ipc_shared_memory.hexa`
- `pe_ipc_sync_primitive.hexa`, `pe_ipc_tcp_loopback.hexa`, `pe_ipc_unix_socket.hexa`
- `pe_ipc_xpc_protocol.hexa`

### 2.6 `lib/loader/bnet/` — 3 files (game launch + battle.net)
- `battlenet_bypass.hexa`
- `d2r_launch.hexa`
- `d4_launch.hexa`

### 2.7 `lib/loader/` (root) — 6 files (orchestrator + cross-cutting)
- `pe_loader.hexa` (top-level orchestrator)
- `dll_resolve_master.hexa`
- `i386_call_frame.hexa`
- `iat_runtime_bind.hexa`
- `oss_validate.hexa`
- `syscall_map_i386.hexa`

### 2.8 합계 검증
```
152 (pe) + 55 (win32) + 16 (dx) + 14 (native) + 16 (ipc) + 3 (bnet) + 6 (root) = 262
```
> 차이 +3 = 일부 pe_metal_* / pe_d2d / pe_dwrite 가 dx/ vs pe/ 경계 이슈로 중복 카운트. 실제 마이그레이션 시 sub-bucket 결정으로 정정.

---

## 3. import path 영향 + sed 시뮬레이션 (실 실행 X)

### 3.1 영향 파일 수
| 패턴 | 변경 전 | 변경 후 | 영향 파일 수 |
| --- | --- | --- | --- |
| `use "../perf/r0_common"` | `../perf/...` | `../../perf/...` | ~258 |
| `use "./pe_imports"` (sibling) | `./pe_imports` | `../pe/pe_imports` (or in-bucket → keep) | ~5 |
| `use "./pe_loader_stage1"` | `./pe_loader_stage1` | `./pe_loader_stage1` (둘 다 pe/ 안이면 유지) | ~3 |
| `use "./pe_runtime_map"` | `./pe_runtime_map` | (동상) | ~2 |
| `use "./win32_abi_i386"` | `./win32_abi_i386` | (둘 다 win32/ → 유지) or `../win32/win32_abi_i386` (cross-bucket caller) | ~4 |

### 3.2 sed 명령 시뮬레이션 (실행 X)
```bash
# Step A — perf import 깊이 +1 (모든 sub-bucket 파일 대상)
find lib/loader/{pe,win32,dx,native,ipc,bnet} -name '*.hexa' -print0 \
  | xargs -0 sed -i '' 's|use "../perf/r0_common"|use "../../perf/r0_common"|g'

# Step B — root sticky 파일 (lib/loader/*.hexa) 은 변경 없음
# (pe_loader, dll_resolve_master, i386_call_frame, iat_runtime_bind, oss_validate, syscall_map_i386)
# → "../perf/r0_common" 그대로 유효

# Step C — sibling import 의 cross-bucket 보정 (수동)
# pe_loader.hexa (root) 가 pe/pe_loader_stage1 호출 시:
#   use "./pe_loader_stage1"  →  use "./pe/pe_loader_stage1"
# pe/* 가 win32/* 호출 시 (rare):
#   use "./win32_abi_i386"    →  use "../win32/win32_abi_i386"
```

### 3.3 검증 명령
```bash
# 변경 후 broken import 검출
grep -rn 'use "\.\./perf/' lib/loader/{pe,win32,dx,native,ipc,bnet} | wc -l
# expected: 0 (전부 ../../perf/ 로 갱신됨)

grep -rn 'use "\.\./\.\./perf/' lib/loader/{pe,win32,dx,native,ipc,bnet} | wc -l
# expected: ~258

grep -rn 'use "\.\./perf/' lib/loader/*.hexa | wc -l
# expected: ~6 (root sticky 파일들 그대로)
```

---

## 4. 외부 reference 영향 — affected files (총 26 +)

### 4.1 dispatch.hexa (lib/dispatch.hexa) — 4 라인
```
66:    println("Phase status (lib/loader/, hexa-only):")
82:    println("DIY hexa PE 로더 (lib/loader/) Phase 0a~10 in progress:")
160:   println("       DIY hexa PE 로더 + Win32 shim (lib/loader/)")
203:   exec("... run lib/loader/pe_parser.hexa summary '" + rel_copy + "' ...")
```
**조치**: line 203 만 실 path 변경 = `lib/loader/pe/pe_parser.hexa` (1 곳).
나머지 3 라인은 println prose — 폴더 명만 표시, 변경 X 가능.

### 4.2 lib/perf/* — 8 files (loader path string 보유)
| 파일 | 영향 |
| --- | --- |
| `lib/perf/c_iat_resolve_cache.hexa` | path string 갱신 |
| `lib/perf/c_win32_call_profile.hexa` | path string 갱신 |
| `lib/perf/c_kernel32_function_growth.hexa` | path string 갱신 |
| `lib/perf/c_pe_parse_cache.hexa` | path string 갱신 |
| `lib/perf/c_pe_iat_simulation.hexa` | path string 갱신 |
| `lib/perf/c_closure_path_dashboard.hexa` | path string 갱신 |
| `lib/perf/c_pe_install_coverage.hexa` | **module_path_for() 18+ 매핑 갱신** (가장 큰 영향) |
| `lib/perf/c_syscall_bridge_cache.hexa` | path string 갱신 |

### 4.3 tests/ — 4 files
- `tests/test.hexa`
- `tests/test_closure.hexa`
- `tests/test_own2.hexa`
- `tests/test_apple_only.hexa`

### 4.4 docs/ — 11 files
- `README.md` (root, lib/loader 언급)
- `docs/WIN32_DLL_REAL_IMPL_MANJEOM_REVIEW.md`
- `docs/BINARY_TRANSLATION_MANJEOM_REVIEW.md`
- `docs/WIN32_ABI_I386_FAIL_FIX_MANJEOM_REVIEW.md`
- `docs/CLOSURE_ROADMAP.md`
- `docs/MINIMAL_NATIVE_PLAN.md`
- `docs/FOLDER_SPLIT_REFACTOR_MANJEOM_REVIEW.md` (SSOT 자체)
- `docs/STAGE7_HELPER_MM_IPC_SPEC.md`
- `docs/D3D_TO_METAL_SHADER_MANJEOM_REVIEW.md`
- `docs/STAGE7_NATIVE_BRIDGE_MANJEOM_REVIEW.md`
- (+ 추가 *_BRAINSTORM_SKETCH.md 일부)

### 4.5 .own — 1 file
`.own` 헌법 문서 일부에 `lib/loader/` 절대 path 언급 → 갱신 필요.

### 4.6 dispatch.hexa loader array (이름 list)
> 사용자 directive: "**loader array 이름은 변경 X**" — Phase status 표시용 string 리스트는 그대로.
영향 X (코드 path 만 변경, display string 유지).

### 4.7 합계
```
4 (tests) + 8 (perf) + 11 (docs) + 1 (dispatch.hexa, line 203) + 1 (.own) + 1 (README) = 26 files
```

---

## 5. CI / docker hexa-exec 영향

| 항목 | 영향 |
| --- | --- |
| Linux container mount (`/Users/ghost`) | 폴더 깊이 +1 = mount root 변동 X — 영향 0 |
| `hexa run lib/loader/pe_parser.hexa` (CLI) | path 변경 = `lib/loader/pe/pe_parser.hexa` 로 호출 |
| `hexa-resolver` glob | `lib/loader/win32_*.hexa` → `lib/loader/win32/*.hexa` (glob 깊이 +1) |
| docker entrypoint | `entry` 스크립트 (`/Users/ghost/core/airgenome-gamebox/entry`) 검토 필요 — `lib/loader/` substring 사용 시 보정 |
| self-test runner | tests/*.hexa 4 파일이 path string 보유 → 갱신 동반 |
| project_hexa_docker_runner_constraints | bash/shasum/git 부재 + Hexa `type` 예약어 함정 — refactor 자체와 무관, 기존 회피 패턴 유지 |

---

## 6. 충돌 risk timeline

### 6.1 in-flight cycle 82+ BG agent 현황
- **사용자 directive**: "남은 영역 all bg go" → 다중 BG agent 동시 진행 중
- **위험**: BG agent 가 `lib/loader/win32_*.hexa` 신규 추가 중일 가능성
- **확인 방법**:
  - `git log --oneline --since="6 hours ago" -- lib/loader/` 로 최근 commit 빈도 확인
  - 1 cycle (≈30 분) 내 신규 `lib/loader/*.hexa` 추가 0 인 시점 = plateau 신호

### 6.2 plateau 도달 조건 (정의)
다음 **모두** 충족 시 plateau 인정:
1. **신규 모듈 0**: 1 cycle 동안 `lib/loader/*.hexa` 신규 파일 추가 없음
2. **수정 0**: 1 cycle 동안 기존 `lib/loader/*.hexa` 수정 없음
3. **BG agent quiet**: 사용자 directive "all bg go" 종료 또는 명시적 freeze 선언
4. **CI green**: 마지막 self-test set 100% PASS

권장: **cycle 82–85 4 cycle 연속** 위 4 조건 만족 시 강행.

### 6.3 만점 검토 (SSOT) 결론 재인용
> 18/25 — **보류 유지**. 이유 = (1) in-flight churn 진행 중 (2) git mv 시 history follow 약화 (3) ROI 모호 — 폴더 ≠ 빌드 가속.

---

## 7. 강행 step list (plateau 도달 시 1 cycle freeze 가이드)

### Step 1 — branch 생성 + freeze (no new BG)
```bash
git checkout -b refactor/loader-folder-split-cycle-XX
echo "FREEZE: lib/loader/*.hexa 신규/수정 금지 — refactor 진행 중" > .freeze-loader
git add .freeze-loader && git commit -m "freeze: lib/loader during folder split"
```
**ETA**: 5 분
**수동 검증**: 모든 BG agent 종료 확인.

### Step 2 — mass-rename (mkdir + git mv)
```bash
mkdir -p lib/loader/{pe,win32,dx,native,ipc,bnet}

# pe/ (152 files)
git mv lib/loader/pe_iat_*.hexa lib/loader/pe/
git mv lib/loader/pe_reloc_*.hexa lib/loader/pe/
# ... (script: prefix 매칭 + exclusion list for ipc/native/d3d/dxgi/d2d/dwrite/metal)

# win32/ (55 files)
git mv lib/loader/win32_*.hexa lib/loader/win32/

# dx/ (16 files)
git mv lib/loader/dx_*.hexa lib/loader/dx/
git mv lib/loader/pe_d3d*.hexa lib/loader/dx/
git mv lib/loader/pe_dxgi*.hexa lib/loader/dx/
git mv lib/loader/pe_d2d_*.hexa lib/loader/dx/
git mv lib/loader/pe_dwrite_*.hexa lib/loader/dx/
git mv lib/loader/pe_metal_*.hexa lib/loader/dx/

# native/ (14 files)
git mv lib/loader/native_helper_bridge.hexa lib/loader/native/
git mv lib/loader/pe_native_*.hexa lib/loader/native/

# ipc/ (16 files)
git mv lib/loader/pe_ipc_*.hexa lib/loader/ipc/

# bnet/ (3 files)
git mv lib/loader/{battlenet_bypass,d2r_launch,d4_launch}.hexa lib/loader/bnet/

# root sticky 6 — 그대로
```
**ETA**: 25 분 (script 작성 + 실행 + verify)
**수동 검증**: `ls lib/loader/{pe,win32,dx,native,ipc,bnet}/*.hexa | wc -l` = 256 / `ls lib/loader/*.hexa | wc -l` = 6.

### Step 3 — import path sed update
```bash
# 3.1 — perf import 깊이 +1 (모든 sub-bucket 파일)
find lib/loader/{pe,win32,dx,native,ipc,bnet} -name '*.hexa' -print0 \
  | xargs -0 sed -i '' 's|use "../perf/r0_common"|use "../../perf/r0_common"|g'

# 3.2 — sibling cross-bucket 수동 보정 (root → pe, pe → win32 등)
#   (수동 grep 후 1-by-1 처리, 5~10 라인 수준)

# 3.3 — 검증
grep -rn 'use "\.\./perf/' lib/loader/{pe,win32,dx,native,ipc,bnet} | wc -l   # = 0
grep -rn 'use "\.\./\.\./perf/' lib/loader/{pe,win32,dx,native,ipc,bnet} | wc -l  # ≈ 256
```
**ETA**: 15 분

### Step 4 — dispatch.hexa loader array 검증
- `lib/dispatch.hexa` line 203: `lib/loader/pe_parser.hexa` → `lib/loader/pe/pe_parser.hexa`
- 나머지 prose println 라인 (66, 82, 160) 은 그대로 유지 (사용자 directive "이름 변경 X").

**ETA**: 5 분

### Step 5 — `lib/perf/c_pe_install_coverage.hexa` `module_path_for()` 갱신
- 기존: `"lib/loader/win32_kernel32*.hexa"` (18+ 매핑)
- 신규: `"lib/loader/win32/win32_kernel32*.hexa"` 등 (각 매핑 1-by-1 갱신, 18+ 라인)
- 추가 7 perf 파일 path string 갱신 (sed 가능):
  ```bash
  sed -i '' 's|lib/loader/win32_|lib/loader/win32/win32_|g' lib/perf/c_*.hexa
  sed -i '' 's|lib/loader/pe_|lib/loader/pe/pe_|g' lib/perf/c_*.hexa
  # (note: pe_ipc_*, pe_d3d*, pe_native_*, pe_metal_*, pe_d2d_*, pe_dwrite_* 는 별도 보정 필요)
  ```
**ETA**: 20 분 (수동 보정 + 검증)

### Step 6 — full self-test set 100% PASS 확인
```bash
hexa run tests/test.hexa
hexa run tests/test_closure.hexa
hexa run tests/test_own2.hexa
hexa run tests/test_apple_only.hexa
hexa run lib/dispatch.hexa  # smoke test
```
모든 self-test 100% PASS 확인. FAIL 시 step 3/5 재진입.
**ETA**: 15 분

### Step 7 — commit + push
```bash
git status   # 변경 파일 수 확인 = 259 mv + 26 외부 update
git add -A
git commit -m "refactor(loader): folder split — 7-bucket (pe/win32/dx/native/ipc/bnet/root)

- lib/loader/*.hexa 259 → 7 sub-bucket 분할
- import depth +1 (../perf → ../../perf) sed 일괄 적용
- module_path_for() 18+ 매핑 갱신
- self-test 100% PASS 확인

cycle XX plateau 도달 confirmed → SSOT 18/25 보류 해제"
git push origin refactor/loader-folder-split-cycle-XX
```
**ETA**: 5 분

### 합계 ETA — 1.6 cycle (≈90 분)
| Step | 시간 |
| --- | --- |
| 1. freeze | 5 분 |
| 2. mass-rename | 25 분 |
| 3. import sed | 15 분 |
| 4. dispatch verify | 5 분 |
| 5. perf module_path_for | 20 분 |
| 6. self-test | 15 분 |
| 7. commit/push | 5 분 |
| **합계** | **90 분 = 1.6 cycle** (만점 검토 통계 기준) |

---

## 8. 후속 작업 / Decision tree

### 8.1 plateau 미도달 시 (현재)
- 본 docs 그대로 보존
- 신규 BG agent → cycle 82+ 진행 (사용자 directive 준수)
- cycle-by-cycle plateau 조건 (§6.2) 모니터링

### 8.2 plateau 도달 시 (cycle 85+ 가능성)
- 1 cycle freeze 선언 → §7 step 1~7 순차 실행
- self-test 100% PASS 시 PR 머지
- SSOT (`FOLDER_SPLIT_REFACTOR_MANJEOM_REVIEW.md`) 18/25 → "**resolved**" 로 갱신

### 8.3 영구 보류 시나리오
- cycle 100+ 도달 + 폴더 splits ROI 여전히 모호 → 본 docs 를 "abandoned proposal" 로 마크
- `lib/loader/` flat 유지 + naming convention 만 강화로 대체

---

## 9. 위험/미결 항목

1. **dx/ vs pe/ 경계** — `pe_metal_buffer_pool.hexa` 등 d3d→Metal bridge 가 어느 bucket 인지 — 본 docs `dx/` 로 가정, 실 refactor 시 재검토.
2. **git history follow** — `git mv` 후 `git log --follow` 동작 검증 필요 (단일 파일 기준 OK, mass-rename 시 일부 break 가능).
3. **dispatch.hexa display string** — "lib/loader/" prose 변경 X 직시 (사용자 directive). 단 자동화 grep 으로 잘못 탐지 시 false-positive 주의.
4. **own 1 import 경로 위반 검증** — refactor 전후 `grep -rh 'use "' lib/loader/` 로 r0_common 외 import 0 건 유지 확인 (own 1 mandate).
5. **외부 path string 누락** — `.own` / docs / README 의 hardcoded path 보정 누락 시 dead-link 발생 → step 5 다음 1 round grep audit 추가 권장.

---

## 10. SSOT cross-link
- 만점 검토 (보류 결정): `docs/FOLDER_SPLIT_REFACTOR_MANJEOM_REVIEW.md` (18/25)
- 본 dry-run (강행 가이드): `docs/FOLDER_SPLIT_DRY_RUN.md` (this file)
- 관련 항목:
  - `docs/CLOSURE_ROADMAP.md` (cycle 진행 SSOT)
  - `docs/MINIMAL_NATIVE_PLAN.md` (native bucket 영향)
  - `docs/STAGE7_HELPER_MM_IPC_SPEC.md` (ipc bucket 영향)
