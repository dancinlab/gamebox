# closure 100% 도달 검증 plan (2026-05-01)

> **status**: cycle 90 sprint 후 작성. closure 96% (shim phase 5-6 + IAT walk + helper IPC) → 100% (실 D2R first frame) 위한 검증 plan.

## 1. 100% 정의 (4 axis 기준)

### A. own1 (Apple-only stack) /5
- Apple SDK + macOS syscall + Apple D3DMetal + Rosetta 2 + DIY hexa shim 만 사용
- Wine / CrossOver / GPTk / Whisky / community-Win32 0줄
- 검증: `grep -rE 'wine|reactos|crossover|whisky|gptk' lib/ native/ docs/ | wc -l` (예상 0)

### B. Runtime (실 launch) /5
- D2R first frame 까지 PE binary 가 실 실행
- 검증: airgenome dispatch launch tool/d2r/D2R.exe → first frame screenshot

### C. 데이터재해석 (perf measurement) /5
- frametime / cold launch / GPU util / thermal 모두 측정
- 검증: lib/perf 의 D-list/R-list 만점 컴포넌트 모두 실 데이터 수집

### D. closure path B percent /1
- 100% = 1.0 (binary)

→ Total **/16 axis**, 100% = 16/16.

## 2. 단계별 검증 metric

### closure 96% (현재) — phase 1-8 도달
| metric | 측정값 | PASS 기준 |
|---|---|---|
| airgenome dispatch install | i386 PE32, 16 DLL, 435 fn, 74% coverage | ✓ install 완료 |
| airgenome dispatch launch | shim phase 1-8 ✓ | ✓ exit 0 |
| helper IPC dlopen 3종 | libSystem/Foundation/Metal handle 받음 | ✓ |
| native binary build | airgenome_helper + pe_to_macho_shim 모두 codesign + valid | ✓ |
| native binary entitlement | 4 keys (jit/unsigned-mem/no-lib-validate/dyld-env) | ✓ |
| 587+ hexa modules self-test | 100% PASS (own1 정합) | ✓ |

### closure 97-98% (cycle 90 sprint 후) — phase 9-13 추적기 추가
| metric | 측정값 | PASS 기준 |
|---|---|---|
| pe_section_mmap_real_invoke self-test | PASS | ✓ |
| pe_iat_dlsym_routing_real self-test | PASS | ✓ |
| pe_tls_fs_segment_alloc self-test | PASS | ✓ |
| pe_entry_invoke_phase self-test | PASS | ✓ |
| pe_metal_device_weak_link_real self-test | PASS | ✓ |
| WIN32_TO_POSIX_SYMBOL_MAP.md 카테고리 수 | ≥14 | ≥10 |

### closure 99% (R2 phase 2 통합 후) — 실 PE 본체 매핑 + IAT routing
| metric | 측정값 | PASS 기준 |
|---|---|---|
| shim phase 9 mmap 실 호출 | helper IPC kind=2 → /tmp/d2r_text.bin 매핑 | mmap addr != MAP_FAILED |
| shim phase 10 dlsym 실 routing | KERNEL32!GetCurrentThreadId → pthread_mach_thread_np | 매핑 성공률 ≥80% |
| shim phase 11 fake FS | FS:[0x18] / 0x30 액세스 시 정상 응답 | signal handler 적중 ≥100% |
| shim phase 12 entry call | entry_rva 호출 후 SIGSEGV 첫 trap 까지 도달 | trap 발생 ≥1회 |

### closure 100% (실 D2R first frame) — 최종
| metric | 측정값 | PASS 기준 |
|---|---|---|
| D2R first frame screenshot | image 파일 존재 | image 검증 |
| frame ms (cold launch) | < 60s | < 60s |
| frametime (steady state) | < 33ms | < 16ms (60fps target) |
| memory footprint | < 4 GB | < 8 GB |
| GPU util | > 30% | > 10% |
| 5분 stability | crash 없음 | 0 crash |

## 3. 실측 검증 명령

### 96% 검증 (현재)
```bash
cd ~/core/airgenome-gamebox

# 1. native binary 빌드
cd native && ./build.sh && cd ..

# 2. 검증
./entry launch tool/bench/Battle.net-Setup.exe
# 기대: phase 1-8 ✓, helper IPC dlopen 3종 PASS

# 3. self-test set
hexa run lib/dispatch.hexa loader self-test 2>&1 | tail -5
# 기대: ALL PASS (1 pre-existing FAIL win32_abi_i386 만 — fix됨)

# 4. own1 grep
grep -rE 'wine|reactos|crossover|whisky|gptk' lib/ native/ docs/ \
  | grep -v 'enforcement\|forbid\|허용 X\|위반' | wc -l
# 기대: 0
```

### 97-98% 검증 (cycle 90 후)
```bash
# phase 9-13 추적기 self-test
for m in pe_section_mmap_real_invoke pe_iat_dlsym_routing_real pe_tls_fs_segment_alloc pe_entry_invoke_phase pe_metal_device_weak_link_real; do
  hexa run lib/loader/$m.hexa self-test
done
# 기대: 5 PASS
```

### 99% 검증 (R2 phase 2 통합 후)
```bash
# shim 확장 phase 9-13
./entry launch tool/bench/Battle.net-Setup.exe --verbose
# 기대: phase 9-12 ✓ (실패 가능 — 정상)

# IAT routing 확인
cat ~/.airgenome/gamebox/persist/pe_iat_dlsym_routing_real/routes.tsv | wc -l
# 기대: ≥300 (Battle.net Setup 의 74% covered = ~322 함수)
```

### 100% 검증 (실 D2R first frame)
```bash
# D2R install (Battle.net Agent 통한 실 install)
./entry install tool/d2r/D2R.exe

# D2R launch
./entry launch ~/Library/Application\ Support/Battle.net/D2R/D2R.exe

# screenshot 검증
ls ~/.airgenome/gamebox/screenshot/d2r_first_frame.png
# 기대: 파일 존재 + non-zero size
```

## 4. 실패 시 대응

### phase 9 mmap 실패
- helper IPC kind=2 응답 errno 확인
- MAP_JIT entitlement 적용 여부 (codesign --verify)
- 16KB page alignment 위반 가능

### phase 10 dlsym 실패
- WIN32_TO_POSIX_SYMBOL_MAP.md 의 매핑 table 정확성 확인
- 매핑 미존재 함수 — own2 stub 주소로 fallback patch

### phase 12 entry call SIGSEGV
- signal handler ucontext_t 의 fault address 분석
- 미매핑 IAT slot 에 stub address 동적 patch

### phase 13 Metal device 실패
- helper IPC kind=7 응답 errno 확인
- MTLCreateSystemDefaultDevice 실패 → CAMetalLayer fallback

## 5. v4 trigger
- closure 100% 도달 시 SCORING_FINAL.md v4 작성
- 만점 ★ 추가 후보 정확화
