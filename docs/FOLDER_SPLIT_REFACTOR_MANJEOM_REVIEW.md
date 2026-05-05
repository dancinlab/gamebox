# Folder split refactor 만점 검토 — `lib/loader/` subdir 분리

> **status**: 검토일 **2026-05-01** / **만점 가능성: 가능 (구조적 이득 명확, 점수 18/25 — sprint 후보 경계선)**
> 작성: 2026-05-01
> SSOT: `docs/REMAINING_ROADMAP.md` (M2 = "2 cycle / 폴더 구조 + import path 일괄"), `docs/PURPLE_LINEAGE_MANJEOM_REVIEW.md` (포맷 본보기)
> 결론 한줄: **refactor 자체는 기술적으로 안전 (`use "../perf/r0_common"` 단일 파턴이 223/225 파일 → 단순 sed-급 일괄 치환). 그러나 in-flight bg cycle 다수가 `lib/loader/` 에 신규 파일을 매 cycle 생성 중이라 timing 이 핵심 risk. 작동/성능/자원/속도 직접 영향 0 → PRIORITY_DIRECTIVE 후순위 — bg agent 정착기 (예: 모듈 합산 ~250 도달 후 plateau) 에 1 cycle 짧게 진입 권장.**

---

## 0. 실측 inventory (2026-05-01)

### 0-1. `lib/loader/` 모듈 수

```
$ ls /Users/ghost/core/airgenome-gamebox/lib/loader/*.hexa | wc -l
225
```

### 0-2. prefix 별 분류 (실측)

| prefix | 개수 | 비고 |
|---|---|---|
| `pe_*` | 167 | PE loader 영역 (전 카테고리 혼재 — IPC / native / call_conv / d3d / metal / kernel object / winsock / com / gdi / ipc 등) |
| `win32_*` | 44 | DLL shim (kernel32 / user32 / gdi32 / ntdll / ole32 / oleaut32 / advapi32 / shell32 / wininet / winhttp / winsock / crypt32 / wintrust / version / msimg32 / rpcrt4 / uiautomationcore / msvcrt) |
| `dx_*` | 5 | DirectX 11/12 stage1/2 (`dx_d3d11`, `dx_d3d11_stage1`, `dx_d3d12`, `dx_d3d12_stage1`, `dx_d3d12_stage2`) |
| `battlenet_bypass` | 1 | Phase 8 |
| `d2r_launch` | 1 | Phase 9 |
| `d4_launch` | 1 | Phase 10 |
| `dll_resolve_master` | 1 | DLL resolver |
| `i386_call_frame` | 1 | i386 ABI |
| `iat_runtime_bind` | 1 | IAT runtime |
| `native_helper_bridge` | 1 | Apple-native bridge |
| `oss_validate` | 1 | OSS validation |
| `syscall_map_i386` | 1 | i386 syscall |

→ **총 225** (사용자 컨텍스트 "200+" 와 일치).

### 0-3. import 패턴 (핵심 발견 — refactor 비용 산정 근거)

```
$ grep -rh "^use " lib/loader/*.hexa | sort | uniq -c
223 use "../perf/r0_common"
  2 use "./pe_loader_stage1"
  2 use "./pe_imports"
  1 use "./pe_runtime_map"
  1 use "./win32_abi_i386"
```

→ **놀라운 결과**: 모든 `lib/loader/*.hexa` 가 사실상 **동일한 import line 1줄** (`use "../perf/r0_common"`) 만 가짐.
→ 같은-폴더 cross-import 는 단 **6 건** (pe_loader_stage1 ×2 / pe_imports ×2 / pe_runtime_map ×1 / win32_abi_i386 ×1).
→ refactor 시 **사실상 mass-rename 1줄 × 225 파일** 단순 작업 + 6건 수동 cross-folder 조정 → **sed-급 reproducible**.

### 0-4. lib/loader/ 외부 참조 (refactor 영향 범위)

```
$ grep -rln "lib/loader/" airgenome-gamebox/ | grep -v "/lib/loader/"
18 files
```

핵심 외부 reference:
- `lib/dispatch.hexa` — line 132 의 `mods = [...]` array (모듈 이름 hard-coded, **path 아님**) + `dir = "loader"` 인자.
- `lib/perf/c_pe_install_coverage.hexa` — `module_path_for(dll: str) → str` 가 `"lib/loader/win32_kernel32*.hexa"` glob string 반환 (~18 lines, sed-able).
- `lib/perf/c_iat_resolve_cache.hexa`, `c_win32_call_profile.hexa`, `c_pe_parse_cache.hexa`, `c_kernel32_function_growth.hexa`, `c_pe_iat_simulation.hexa`, `c_syscall_bridge_cache.hexa`, `c_closure_path_dashboard.hexa` — perf 8 모듈이 `"lib/loader/"` 를 hard-code.
- `tests/test*.hexa` — 4 files, `"lib/loader/"` path 참조.
- `README.md`, `docs/CLOSURE_ROADMAP.md`, `docs/MINIMAL_NATIVE_PLAN.md`, `docs/STAGE7_NATIVE_BRIDGE_MANJEOM_REVIEW.md`, `.own` — doc 4 + own.

→ **외부 영향 18 파일** + 내부 225 파일 = **총 243 파일 일괄 update**.

---

## § 폴더 구조 제안

### 1-1. 1차 안 (사용자 컨텍스트 그대로) — 7 카테고리

```
lib/loader/
  ├── pe/                  # pe_loader / pe_iat / pe_reloc / pe_tls / pe_export / pe_delay_load
  ├── win32/               # win32_kernel32 / win32_user32 / ... DLL shim
  ├── dx/                  # dx_d3d11 / dx_d3d12 / + pe_d3d11_*, pe_d3d12_*, pe_dxgi_*, pe_d2d_*, pe_dwrite_*, pe_metal_*
  ├── native/              # pe_native_* (Apple-native bridge) + native_helper_bridge
  ├── ipc/                 # pe_ipc_*
  ├── bnet/                # battlenet_bypass / d2r_launch / d4_launch
  └── (root)               # dll_resolve_master / i386_call_frame / iat_runtime_bind / oss_validate / syscall_map_i386
```

| 폴더 | 파일 수 (실측) |
|---|---|
| `pe/` | ~120 (167 - dx 7 - native 9 - ipc 16 - 기타) |
| `win32/` | 44 |
| `dx/` | 5 + 7 (pe_d3d11/12/dxgi/d2d/dwrite) + 2 (pe_metal_*) = **14** |
| `native/` | 9 (pe_native_*) + 1 (native_helper_bridge) = **10** |
| `ipc/` | 16 (pe_ipc_*) |
| `bnet/` | 3 (battlenet/d2r/d4) |
| `(root)` | 5 (dll_resolve_master / i386_call_frame / iat_runtime_bind / oss_validate / syscall_map_i386) |
| **합계** | **225** ✓ |

### 1-2. 2차 안 (`pe/` 추가 분할) — 11 카테고리

`pe/` 안의 ~120 파일이 너무 평탄 → 추가 sub-bucket:

```
lib/loader/pe/
  ├── core/        # pe_loader / pe_loader_stage1/2 / pe_parser / pe_imports / pe_relocations / pe_runtime_map
  ├── call_conv/   # pe_call_conv_* (6) + pe_thiscall / pe_va_args
  ├── thread/      # pe_thread_* / pe_apc_queue / pe_dpc_simulate / pe_terminate_thread / pe_critical_section / pe_event_object / pe_semaphore / pe_mutex / pe_wait_for_*
  ├── memory/      # pe_memory_pool_* / pe_heap_* / pe_mmap_protection / pe_vm_pressure / pe_low_memory / pe_gpu_memory / pe_metal_*
  ├── seh/         # pe_seh_* / pe_vectored_exception / pe_signal_translation / pe_debug_break
  ├── nt/          # pe_ntdll_native_api / pe_kuser_shared_data / pe_teb_struct / pe_peb_struct / pe_ldr_module_table / pe_unicode_string / pe_object_attributes / pe_syscall_table
  ├── io/          # pe_console_* / pe_file_* / pe_pipe_* / pe_mailslot / pe_volume / pe_change_notification / pe_io_completion_port
  ├── net/         # pe_winsock_* / pe_dns_query / pe_iphlpapi / pe_winhttp_request / pe_wininet_cache / pe_https_certificate
  ├── dx/          # pe_d3d11_* / pe_d3d12_* / pe_dxgi_* / pe_d2d_* / pe_dwrite_* / pe_metal_* (dx/ 와 합치는 게 더 나음)
  ├── com/         # pe_com_* / pe_mf_media_session / pe_image_codec
  ├── ui/          # pe_gdi_* / pe_clipboard / pe_ime_* / pe_taskbar_progress / pe_avrt
  └── locale/      # pe_locale_info / pe_collation / pe_unicode_normalize / pe_charset / pe_resource_string / pe_message_table
```

→ 11 카테고리, 각 폴더 평균 ~10-15 파일 (탐색성 ↑).
→ 단점: 깊이 +1 → import path 가 `"../../../perf/r0_common"` (3-deep) 으로 길어짐.
→ **trade-off**: 탐색성 vs path 깊이.

### 1-3. 권장 — 1차 안 (7 카테고리, 1-deep)

이유:
- 깊이 1단계 = import path 영향 단일화 (`"../perf/"` → `"../../perf/"` 만 변경).
- bg agent 가 새 모듈 추가할 때 7 폴더 중 자연스럽게 분류 가능 (11 폴더는 경계 모호).
- `pe/` 안 ~120 평탄도 grep `pe_call_conv_*` / `pe_ipc_*` 식으로 충분히 navigable.
- 만점 검토용 perf 모듈 (`module_path_for()` 등) 의 glob string 재작성 비용 최소.

---

## 2. 6 phase 별 만점 가능성

(refactor 는 launch 6 phase 와 무관 — 자체 cycle 진행.)

### Phase R-1 — 폴더 생성 + git mv

**시나리오**: `mkdir lib/loader/{pe,win32,dx,native,ipc,bnet}` → `git mv lib/loader/pe_*.hexa lib/loader/pe/` 등 7 batch.

| 모듈 후보 | A | B | C | 만점 | 비고 |
|---|---|---|---|---|---|
| `c_loader_folder_split_step1_mv` | 5 | 4 | 5 | 14 | git mv 단순. own1 정합 — refactor 자체는 hexa 코드 변동 0 |
| `c_loader_folder_split_step1_validate` | 5 | 4 | 5 | 14 | mv 후 ls × 7 으로 file 누락 검증 |

**phase 결론**: **가능 (단순 file move)**.
**ETA**: 0.3 cycle.

### Phase R-2 — import path 일괄 update (`"../perf/r0_common"` → `"../../perf/r0_common"`)

**시나리오**: 223 파일의 단일 line 일괄 치환 + 6 cross-loader use 의 폴더-aware 재작성.

| 모듈 후보 | A | B | C | 만점 | 비고 |
|---|---|---|---|---|---|
| `c_loader_import_rewrite_perf_common` | 5 | 4 | 5 | 14 | 223 파일 1-line 치환 — 단순 |
| `c_loader_cross_folder_use_rewrite` | 5 | 4 | 4 | 13 | 6 건 cross-folder (예: dispatch 의 `use "./pe_imports"` → `"./pe/pe_imports"`). 위치 결정 필요 |

**risk**: hexa parser 가 `..` chain 을 어디까지 허용하는지 사전 확인 필요. (실측 — `lib/perf/` 가 이미 `lib/perf/r0_common` 경로로 fully working 중이므로 1-deep 추가 = `"../../perf/r0_common"` 도 무리 없음.)

**phase 결론**: **가능**.
**ETA**: 0.5 cycle.

### Phase R-3 — 외부 reference update (perf / dispatch / tests / docs)

**시나리오**:
1. `lib/dispatch.hexa` line 132 의 hardcoded `mods` array — 모듈 이름은 그대로, 단 `run_self_test_set(mods, "loader", ...)` 의 `dir = "loader"` 가 7 subdir 을 어떻게 cover 할지 결정.
   - **옵션 A**: `mods` 를 `[("pe", "pe_loader"), ("pe", "pe_imports"), ("win32", "win32_kernel32"), ...]` tuple list 로 변경 (signature 변경, 225 entry rewrite).
   - **옵션 B (권장)**: `run_self_test_set` 에 `dir_for(module: str) -> str` helper 추가 (prefix → subdir 매핑), signature 유지.
   - **옵션 C**: `dir = "loader"` 유지 + `run_self_test_set` 가 7 subdir glob 자동 탐색 (구현 가장 단순).
2. `lib/perf/c_pe_install_coverage.hexa` `module_path_for()` — `"lib/loader/win32_kernel32*.hexa"` → `"lib/loader/win32/win32_kernel32*.hexa"` (sed `s|lib/loader/win32_|lib/loader/win32/win32_|g`).
3. perf 8 모듈 + tests 4 + docs 5 + .own — sed-급 일괄.

| 모듈 후보 | A | B | C | 만점 | 비고 |
|---|---|---|---|---|---|
| `c_loader_dispatch_subdir_resolve` | 5 | 4 | 5 | 14 | 옵션 B — prefix → subdir 매핑 helper |
| `c_loader_perf_module_path_remap` | 5 | 4 | 5 | 14 | `module_path_for()` glob 18 lines 재작성 |
| `c_loader_test_doc_path_sweep` | 5 | 4 | 5 | 14 | tests / docs / .own 일괄 sed |

**phase 결론**: **가능**.
**ETA**: 0.5 cycle.

### Phase R-4 — self-test 전수 검증 + git commit

```
gamebox loader self-test     # 225 모듈 self-test PASS
gamebox perf self-test       # 230+ perf 모듈 self-test PASS (module_path_for 변경 후)
gamebox r0 self-test         # M0 instrumentation
gamebox self-test            # unified
```

**phase 결론**: **가능 — 모든 self-test PASS 시 commit, 1+ FAIL 시 즉시 revert (`git reset --hard`)**.
**ETA**: 0.3 cycle.

### 매트릭스 요약

| Phase | 가능성 | ETA | 차단 risk |
|---|---|---|---|
| R-1 폴더 생성 + git mv | +3 만점 | 0.3 cycle | 거의 0 |
| R-2 import path 일괄 | +3 만점 | 0.5 cycle | hexa 의 `..` chain depth 제약 (가능성 낮음) |
| R-3 외부 reference | +3 만점 | 0.5 cycle | dispatch.hexa subdir 매핑 옵션 결정 |
| R-4 self-test 전수 | +3 만점 | 0.3 cycle | 1+ FAIL 시 revert (loss 0 — git 으로 atomic) |

→ **총 만점 가능성 100%, ETA 1.6 cycle ≈ 사용자 컨텍스트 "2 cycle" 과 일치**.

---

## 3. 5축 채점 (REMAINING_ROADMAP.md 기준 /25)

| 축 | 정의 | 점수 | 근거 |
|---|---|---|---|
| **A. own1 정합** | Wine 0줄 유지 가능? | **5/5** | refactor 는 file move + import string rewrite 만 — hexa 코드 변동 0 줄. own1 위반 가능성 0 |
| **B. 실 동작 도달** | 만점 시 launch 가능? | **1/5** | refactor 자체로 game launch 한 발자국도 가까워지지 X (작동에 직접 영향 없음). 단 launch 와 무관한 maintainability 가치만 |
| **C. 자원 영향** | perf/메모리 cost | **5/5** | runtime 동작 동일 — file path 만 변경. perf/메모리 영향 0 |
| **D. 구현 난이도 (역)** | 낮을수록 점수 높음 | **4/5** | sed-급 일괄. 1.6 cycle (사용자 ETA "2 cycle" 과 일치). 단 self-test 225 + 230 = ~455 module 전수 검증 필요 → 1 cycle 안 끝낼 가능성 |
| **E. 의존성 (역)** | 적을수록 점수 높음 | **3/5** | dispatch.hexa / perf 8 모듈 / tests 4 / docs 5 / .own / README — **외부 18 파일** 동시 update 필요. + in-flight bg agent 와 timing 충돌 risk |
| **합계** | | **18/25** | **sprint 후보 경계선** (15 이상 = sprint 후보, 18 = 미들 sprint) |

→ 비교:
- PURPLE Lineage 검토 (만점 가능성 부분가능, 만점 6/25 ~ 9/25 phase 별 별도)
- M0/M1 instrumentation = 25/25 (이미 작동 영역)
- refactor 18/25 = "기술적으로 매우 안전, 단 B축 (실 동작 도달) 이 1 → 작동/성능/속도 직접 영향 없음 → 우선순위 후순위 직접 근거".

---

## 4. in-flight cycle 충돌 risk 분석

### 4-1. 위험 시나리오

bg agent 다수가 매 cycle `lib/loader/` 에 신규 모듈 추가 중 (cycle 별 ~5-15 모듈). refactor cycle 진행 도중 신규 모듈이 평탄 `lib/loader/` 에 떨어지면:

1. refactor mid-flight → 신규 파일 `pe_new_module.hexa` 가 root 에 추가됨 (어느 subdir 으로 갈지 분류 미결정).
2. self-test 225 → 226 카운트 mismatch.
3. dispatch.hexa `mods` array 의 hardcoded list 업데이트 필요 (refactor PR + bg agent PR 동시 충돌).
4. mass-rename 진행 중 git rebase 충돌 → 수동 해결 비용 +0.5 cycle.

### 4-2. 회피 전략

| 전략 | 효과 | 비용 |
|---|---|---|
| **bg freeze**: refactor cycle 동안 bg agent 일시 중지 | 충돌 0% | bg productivity 1.6 cycle 손실 |
| **plateau timing**: 모듈 신규 추가 plateau 도달 시 refactor (예: 250 모듈 ± 5 안정) | 충돌 ~10% | plateau 도달 자체가 미래 (현재 225, plateau 추정 시점 cycle +10~20) |
| **rolling refactor**: 1 폴더씩 (`pe/` 부터) 분리, 1 cycle 당 1 batch | 충돌 ~30% (작은 batch 별 충돌만) | 7 cycle 분할 — 총 비용 ↑ |
| **prefix-only convention**: 폴더 분리 안 하고 prefix 강화 (`pe_call_conv_*` 처럼 sub-prefix 추가) | 충돌 0%, refactor 0 | 탐색성 변화 X (현 상태) |

→ **권장**: **plateau timing** + **bg freeze 1.6 cycle**. plateau 도달 전 강행 시 충돌 risk 큼.

### 4-3. PRIORITY_DIRECTIVE 정합

| 차원 | refactor 영향 |
|---|---|
| 작동 (launch 가능 여부) | **0** — refactor 후/전 launch 가능성 동일 |
| 성능 (frame time / latency) | **0** — runtime 동작 변화 X |
| 자원 (메모리 / 디스크) | **0** — file path 외 변화 X |
| 속도 (cycle throughput) | **간접 (+)** — bg agent 의 새 모듈 위치 결정이 명확해져 cycle 당 1-2 분 단축 가능 |
| 유지보수성 | **(+)** — grep `pe_*` / `win32_*` / `dx_*` 로 카테고리별 navigation 가능 |

→ PRIORITY_DIRECTIVE 의 작동/성능/자원/속도 4 차원 모두 직접 영향 0 또는 미미. 유지보수성만 (+). → **명백한 후순위**.

---

## 5. 결론 — 우선순위

### 5-1. 만점 가능성 종합

| 항목 | 값 |
|---|---|
| **만점 가능성 (refactor 자체)** | **가능 (100%)** — 1.6 cycle 안 R-1~R-4 모두 도달 |
| 5축 점수 | **18/25** (sprint 후보 경계선) |
| 도달 가능 만점 점수 | +12 (R-1 ~ R-4 phase 별 +3 × 4) |
| 폴더 수 | **7 (1차 안, 권장)** 또는 **11 (2차 안, pe/ 추가 분할)** |
| 영향 파일 수 | **243** (lib/loader 225 + 외부 18) |
| 기술 risk | **낮음** — `use "../perf/r0_common"` 단일 패턴이 99.1% (223/225) → sed-급 일괄 |
| timing risk | **중-높음** — in-flight bg agent 가 매 cycle `lib/loader/` 에 신규 추가 중 |
| BAN risk | N/A — refactor 는 game launch 무관 |
| gamebox identity 정합 | **완전 정합** — own1 위반 risk 0 |

### 5-2. 즉시 권장 action

1. **1순위: refactor 보류 유지** — bg agent 의 신규 모듈 plateau 도달 (예: 250 ± 5 모듈 안정 cycle 3+) 까지 보류. 사용자 컨텍스트 "in-flight cycle 위험으로 보류됨" 결정 그대로 유지.
2. **2순위: prefix convention 강화** — 신규 모듈 추가 시 `pe_<category>_<name>` 형식 강제 (예: `pe_ipc_xxx`, `pe_call_conv_xxx`, `pe_native_xxx`). prefix 만으로도 grep navigation 충분히 가능.
3. **3순위: plateau 도달 시 1 cycle freeze + refactor 강행** — 1 cycle 안에:
   - R-1 git mv (0.3 cycle)
   - R-2 import path sed (0.5 cycle)
   - R-3 외부 18 파일 sed (0.5 cycle)
   - R-4 self-test 전수 + commit/revert (0.3 cycle)
   - 총 1.6 cycle (buffer 0.4 → 2 cycle 예약).
4. **본 문서를 `docs/REMAINING_ROADMAP.md` M2 section 에 cross-link**.

### 5-3. 미래 가능성 (조건부 trigger)

다음 조건 충족 시 refactor 진입:
- `lib/loader/` 모듈 수 250 도달 + 2 cycle 연속 plateau (신규 추가 0~3)
- bg agent freeze 1.6 cycle 가능한 시점 (다른 bg cycle batch 종료 직후)
- 사용자 명시 directive ("이제 refactor 시작")

---

## 6. 부록 — 실측 명령어

검토에 사용된 명령어 (재현 가능):

```bash
# 0-1. 모듈 수
ls /Users/ghost/core/airgenome-gamebox/lib/loader/*.hexa | wc -l
# → 225

# 0-2. prefix 분류
ls /Users/ghost/core/airgenome-gamebox/lib/loader/ \
  | awk '{ split($0,a,"_"); print a[1] }' \
  | sort | uniq -c | sort -rn

# 0-3. import 패턴
grep -rh "^use " /Users/ghost/core/airgenome-gamebox/lib/loader/*.hexa \
  | sort | uniq -c | sort -rn

# 0-4. 외부 reference
grep -rln "lib/loader/" /Users/ghost/core/airgenome-gamebox/ \
  | grep -v "/lib/loader/" | wc -l
# → 18
```

---

*written 2026-05-01. Folder split refactor M2 만점 검토 — 18/25, 기술적 가능, timing 후순위. PRIORITY_DIRECTIVE 의 작동/성능/자원/속도 4 차원 직접 영향 0 → bg agent plateau 도달 후 1 cycle freeze 강행 권장. own1 정합 5/5 (Wine 0줄 위반 risk 0).*
