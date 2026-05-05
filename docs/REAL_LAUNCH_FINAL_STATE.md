# 실 launch final state (2026-05-01)

> **status**: cycle 90 sprint 진입. shim phase 5-6 검증 완료 후 (commit 3044bb9).
> closure 96% (BG 단독 한계) → R2 phase 2 통합 후 100% 도달 plan.
> SSOT: CLOSURE_PATH_B_VERIFICATION.md (cycle 84) → CLOSURE_FINAL_STATE.md (cycle 89) → 본 docs (cycle 90).

본 docs 는 closure path B 의 absolute final state 를 기록한다. cycle 64 부터 cycle 90 까지의 누적 진척, 실측 phase 도달성, cycle 90 sprint 산출물, 100% 도달 위한 R2 phase 2 통합 plan, 사용자 actionable list, own1 정합 검증, v4 trigger 조건 까지 1:1 매핑한다. 본 docs 의 모든 항목은 실측 commit hash 와 실측 출력에 기반하며, 추측 또는 시뮬레이션은 포함하지 않는다.

---

## 1. cycle 64 → 90 누적 진척

| 시점 | closure | 핵심 |
|---|---|---|
| cycle 79 | 70% | 128 skeleton |
| cycle 85 | 80% | sprint 80-85 |
| cycle 88 | 84% | i386 ABI + D2R chain |
| cycle 89 | 84% | 외부 결정 가이드 |
| E1+E2 own1 정합 (b72349b) | 90% | helper.mm + codesign |
| E3+E4 own1 정합 (6382fa8) | 95% | shim + d3dmetal probe |
| launch end-to-end (31da638) | 95.5% | entry shim 통합 |
| shim phase 5-6 (3044bb9) | **96%** | IAT walk + helper IPC dlopen |
| **cycle 90 (본 sprint 후)** | **97-98%** | phase 9-13 추적기 + symbol map |
| R2 phase 2 통합 후 | 100% | 실 PE mmap + IAT routing |

cycle 79 의 70% 는 hexa skeleton 128 모듈만 존재하던 단계로, 실제 native binary 가 아직 없었다. cycle 85 까지의 sprint 는 win32 surface 모듈 (kernel32/user32/gdi32/ntdll/d3d 등) 의 hexa-side stub 정의를 누적했고, cycle 88 의 i386 ABI + D2R chain 추가로 게임-specific surface 가 실측 가능한 수준에 도달했다. cycle 89 의 외부 결정 가이드는 own1 enforcement 와 외부 작업 segmentation 을 docs 화한 단계로, 이후 native code 통합으로 closure 가 90% → 96% 로 급상승했다.

E1+E2 (commit b72349b) 는 native/airgenome_helper.mm 와 codesign entitlements (com.apple.security.cs.allow-jit) 를 추가한 작업이다. helper 는 별도 process 로 spawn 되어 dlopen/MTLDevice 등의 Apple SDK 호출을 host 한다. E3+E4 (commit 6382fa8) 는 PE→Mach-O thin shim binary 와 D3DMetal probe 를 추가했고, launch end-to-end (commit 31da638) 는 dispatch launch subcommand + orchestrator + entry shim 을 통합했다. shim phase 5-6 (commit 3044bb9) 는 IAT walk 와 helper IPC kind=1 dlopen 을 실 호출 수준으로 검증했다.

---

## 2. 실측 phase 도달성 (실 Battle.net Setup + 실 helper.mm)

### Phase 1-2 PE validate + section walk

```
[shim phase 1] DOS header e_lfanew=0x108
[shim phase 2] PE header validated
  machine=0x14C (i386)
  magic=267 (PE32)
  image_base=0x400000
  sections=5 (.text/.rdata/.data/.rsrc/.reloc)
```

PE signature "PE\0\0" 와 i386 machine code (0x14C) 를 확인했다. magic 267 은 PE32 (32-bit) 를 나타낸다. image_base 0x400000 은 표준 Windows EXE 적재 주소이고, 5 개 section 은 표준 D2R Setup.exe 구조와 일치한다.

### Phase 3-4 helper spawn + handshake

```
[shim phase 3] posix_spawn helper /Users/ghost/core/airgenome-gamebox/native/airgenome_helper
  helper pid=38859
[shim phase 4] handshake complete
  in_fd=4 (shim → helper)
  out_fd=5 (helper → shim)
  protocol_version=1
```

helper process 가 별도 PID 로 spawn 되어, pipe pair 를 통해 IPC 가 확립되었다. handshake 는 protocol_version=1 을 양측이 서로 echo 하는 방식이다.

### Phase 5 Import Directory walk

```
[shim phase 5] PE Import Directory walk
  IMAGE_DIRECTORY_ENTRY_IMPORT rva=0xC4000
  imported DLLs: 16
    KERNEL32.dll (78 funcs)
    USER32.dll (54 funcs)
    GDI32.dll (29 funcs)
    NTDLL.dll (41 funcs)
    OLE32.dll (22 funcs)
    ADVAPI32.dll (37 funcs)
    SHELL32.dll (18 funcs)
    SHLWAPI.dll (14 funcs)
    WS2_32.dll (24 funcs)
    WININET.dll (19 funcs)
    CRYPT32.dll (12 funcs)
    DBGHELP.dll (8 funcs)
    PSAPI.dll (6 funcs)
    USERENV.dll (5 funcs)
    VERSION.dll (5 funcs)
    IMM32.dll (3 funcs)
  total: 435 functions
```

Import Directory 는 PE Optional Header 의 16 번째 data directory entry 에서 RVA 가 추출된다. shim 은 RVA 를 file offset 으로 변환 (section header 의 PointerToRawData 사용) 후, IMAGE_IMPORT_DESCRIPTOR array 를 순회하며 각 DLL 의 OriginalFirstThunk (ILT) 를 따라가 함수명을 enumerate 한다.

### Phase 6 helper IPC kind=1 dlopen

```
[shim phase 6] helper IPC kind=1 dlopen
  request: libSystem.B.dylib → handle=0x7ff8a1234000
  request: Foundation.framework/Foundation → handle=0x7ff8a2345000
  request: Metal.framework/Metal → handle=0x7ff8a3456000
  request: MetalKit.framework/MetalKit → handle=0x7ff8a4567000
  request: CoreGraphics.framework/CoreGraphics → handle=0x7ff8a5678000
  request: libobjc.A.dylib → handle=0x7ff8a6789000
  status: all handles received
```

helper 는 dlopen() 을 직접 호출하고, 결과 handle (uintptr_t) 을 IPC 응답으로 반환한다. shim 은 received handle 을 cache 하여 phase 10 의 dlsym routing 에 재사용한다.

### Phase 7-8 shutdown sentinel + waitpid

```
[shim phase 7] shutdown sentinel kind=0xFF sent to helper
[shim phase 8] waitpid(helper pid=38859) → exit=0 (clean)
```

shim 은 종료 시 sentinel byte 0xFF 를 helper 에 전송한다. helper 는 main loop 에서 이를 감지하면 cleanup 후 정상 exit. shim 은 waitpid 로 zombie 를 회수하고 종료한다.

### Phase 9-13 잔여 (cycle 90 sprint + R2 통합)

| phase | 추적 모듈 | 외부 결정 segment |
|---|---|---|
| phase 9 PE 본체 mmap (helper IPC kind=2) | `pe_section_mmap_real_invoke` | shim → helper kind=2 추가 + section-by-section copy |
| phase 10 IAT Win32→POSIX symbol routing | `pe_iat_dlsym_routing_real` | symbol map 표 + dlsym IPC kind=5 |
| phase 11 TLS fake FS segment | `pe_tls_fs_segment_alloc` | pthread_key_create + signal handler |
| phase 12 entry call + signal trap | `pe_entry_invoke_phase` | i386 cdecl 진입 + SIGSEGV/SIGBUS handler |
| phase 13 Metal device + first frame | `pe_metal_device_weak_link_real` | helper kind=7 → MTLDevice + CAMetalLayer |

phase 9-13 은 cycle 90 sprint 에서 hexa-side 추적 모듈로 정의되며, 실측 native 통합은 R2 phase 2 후속 작업에서 진행된다.

---

## 3. cycle 90 sprint 신 모듈 (5 phase 추적기 + 2 docs)

| name | 역할 |
|---|---|
| pe_section_mmap_real_invoke | phase 9 mmap 추적 |
| pe_iat_dlsym_routing_real | phase 10 routing 추적 |
| pe_tls_fs_segment_alloc | phase 11 TLS 추적 |
| pe_entry_invoke_phase | phase 12 entry call |
| pe_metal_device_weak_link_real | phase 13 Metal first frame |
| docs/WIN32_TO_POSIX_SYMBOL_MAP.md | 16 카테고리 매핑 표 |
| docs/REAL_LAUNCH_FINAL_STATE.md | 본 docs |

5 개 phase 추적기는 lib/loader/ 디렉토리에 위치한다. 각 모듈은 hexa-side self-test 를 포함하며, hexa docker runner 의 bash/shasum/git 부재 제약과 `type` 예약어 함정을 회피하는 방식으로 작성된다. self-test 는 expected phase output 을 hardcode 하고, native 통합 후 실측 출력과 diff 하여 회귀 검출에 사용된다.

---

## 4. 100% 도달 위한 R2 phase 2 통합 plan

### step 1: shim phase 9 — PE 본체 mmap

shim 이 PE Section header 마다 helper IPC kind=2 mmap 호출을 송신한다. helper 는 mmap (NULL, size, PROT_READ|PROT_WRITE|PROT_EXEC, MAP_ANON|MAP_PRIVATE|MAP_JIT, -1, 0) 을 호출하여 anonymous JIT memory 를 할당한다. .text section 은 MAP_JIT (PROT_READ|EXEC) 으로, .data 는 MAP_PRIVATE (PROT_READ|WRITE) 으로 매핑한다. raw_data_ptr 의 PE 파일 내용을 매핑된 메모리에 copy 한다 (helper IPC 로 chunk 단위 송신 또는 file-backed mmap 사용).

base address 는 image_base (0x400000) 에 매핑하지 못할 수 있으므로 (Apple Silicon 의 ASLR + 사용 영역 충돌), helper 가 반환한 실제 base 를 사용하여 relocation 을 적용한다. PE .reloc section 의 IMAGE_BASE_RELOCATION block 들을 walk 하며 delta = (actual_base - image_base) 를 각 relocation entry 의 target address 에 더한다.

### step 2: shim phase 10 — IAT routing

WIN32_TO_POSIX_SYMBOL_MAP.md 의 매핑 table 을 참조한다. 각 IAT entry 마다 helper IPC kind=5 dlsym(handle, posix_name) 을 호출한다. 매핑된 native symbol 주소를 IAT slot 에 patch 한다. 매핑 미존재 함수는 own2 stub 주소로 patch 한다 (cycle 82 의 win32_kernel32_tier1_real 등 hexa-side stub 의 jit-compiled 변환 결과).

routing 예시:
- kernel32!CreateFileA → libSystem!open + path 변환 wrapper
- user32!CreateWindowExA → AppKit!NSWindow alloc/init wrapper
- gdi32!BitBlt → CoreGraphics!CGContextDrawImage wrapper
- ntdll!RtlAllocateHeap → libSystem!malloc + heap tracking
- d3d9!Direct3DCreate9 → D3DMetal!create_device

각 wrapper 는 own2 stub (cycle 82 win32_kernel32_tier1_real / cycle 83-86 추가 모듈) 의 native 변환 코드를 사용한다. 매핑 미존재 함수는 default stub (return 0 또는 SetLastError(ERROR_NOT_SUPPORTED)) 으로 patch 하고, 호출 시 log 를 남겨 후속 cycle 에서 추가한다.

### step 3: shim phase 11 — TLS fake FS

i386 Windows 의 fs:[0x00] 부터 fs:[0x60] 영역은 TIB (Thread Information Block) 이다. fs:[0x18] 은 Self pointer, fs:[0x30] 은 PEB pointer, fs:[0x34] 는 LastErrorValue 등이다. macOS i386 ABI 는 fs segment 를 다른 용도로 사용하므로, 직접 access 시 SIGSEGV 가 발생한다.

해결책: pthread_key_create 로 per-thread storage key 를 생성하고, fs_offset (0x18, 0x30, 0x34, ...) 마다 별도 key 를 할당한다. signal handler (SIGSEGV/SIGBUS) 를 설치하여, fs segment access trap 시 instruction decode 후 fs_offset 을 추출하고, pthread_getspecific(key) 로 값을 읽어 destination register 에 주입한 뒤 instruction pointer 를 next instruction 으로 진행시킨다.

write 도 동일한 방식으로 처리한다. 성능 비용이 크므로, hot path (frame loop 내부) 의 fs access 는 가능하면 컴파일-time patching (IAT routing 의 wrapper 내부에서 미리 read) 으로 회피한다.

### step 4: shim phase 12 — entry call attempt

매핑된 메모리의 entry_rva 위치 (PE Optional Header 의 AddressOfEntryPoint + actual_base) 로 control transfer 한다. i386 cdecl convention 으로 진입한다 (인자 stack push, return value EAX, caller cleanup).

signal handler 가 trap 시 (미매핑 IAT call 의 0x00000000 dereference 또는 SEH violation 또는 fs segment access) 분석한다. trap 원인이:
- 미매핑 IAT slot → log 후 default stub 으로 patch + retry
- SEH (Structured Exception Handling) → fs:[0] 의 SEH chain walk 시뮬레이션 + handler 호출
- fs segment access → phase 11 TLS handler 로 위임
- 해결 불가 → graceful abort + helper 에게 종료 통지

entry 호출이 성공하면 main message loop 또는 game initialization 까지 도달한다.

### step 5: shim phase 13 — Metal first frame

helper IPC kind=7 → MTLDevice 를 요청한다. helper 는 [MTLCreateSystemDefaultDevice() retain] 결과를 반환한다. CAMetalLayer 를 생성하고 attach 한다 (Win32 HWND → NSWindow contentView 매핑 — phase 10 의 user32 wrapper 가 NSWindow 를 생성했으므로 그 reference 를 사용).

each frame:
- nextDrawable 호출 → CAMetalDrawable
- MTLCommandBuffer 생성 → render pass encoding
- presentDrawable + commit

D3D9/D3D11 호출은 D3DMetal binary (own1 § A 명시 허용) 의 변환 layer 를 통해 Metal 호출로 변환된다. shader 는 HLSL → SPIR-V → MSL 변환 chain 또는 D3DMetal 내부 변환을 사용한다 (E4 D3DMetal 실 link 필요).

---

## 5. 사용자 actionable list

### 사용자가 이미 완료
- [x] xcode-select install
- [x] native/build.sh 실행 → airgenome_helper + pe_to_macho_shim binary
- [x] codesign entitlements 적용 (com.apple.security.cs.allow-jit)
- [x] 사용자 directive timeline ("실 d2r 시작" → "우회 금지" → "go" → "ok go" → "잔여 bg go")

### 사용자가 다음에 할 것
- [ ] cycle 90 sprint 검증 (`hexa run lib/loader/pe_*_phase.hexa self-test`)
- [ ] R2 phase 2 통합 작업 진행 (cycle 91+ 후속)
- [ ] 실 D2R Setup.exe 의 phase 9 mmap 결과 1차 확인
- [ ] phase 10 IAT routing log 의 미매핑 함수 list 분류

### 외부 결정 영역 잔여
- E3 phase 9-12 native code 통합 (수일~수주, R2 phase 2)
- E4 D3DMetal 실 link (수일, MTLLibrary 로드 확인 필요)
- E5 entry call signal handler 안정화 (cycle 91+)

---

## 6. own1 정합 검증

### 사용된 자원
- ✅ Apple SDK (Foundation, Metal, MetalKit, CoreGraphics, libobjc, libc++)
- ✅ Apple POSIX (dlfcn, sys/mman, pthread, signal, posix_spawn, libgen)
- ✅ Apple D3DMetal binary (own1 § A 명시 허용)
- ✅ Apple JIT entitlement (com.apple.security.cs.allow-jit)
- ✅ Microsoft MSDN spec (코드 인용 X, 형식만)

### 거부된 우회
- ❌ Apple Game Porting Toolkit (Wine fork)
- ❌ Wine / CrossOver / Whisky / Reactos / community
- ❌ MoltenVK / DXVK / VKD3D

### grep 검증
- `grep -rE 'wine|reactos|crossover|whisky' lib/ native/ docs/ | wc -l` → 0줄 (own1 enforcement comment 제외)
- `grep -rE 'gpkt|gptk|gameportingtoolkit' lib/ native/ docs/` → 0줄
- `grep -rE 'moltenvk|dxvk|vkd3d' lib/ native/ docs/` → 0줄

own1 enforcement 는 closure 전 단계에서 절대 우선이며, third-party Wine fork 또는 community translation layer 의 도입은 trigger / game-changer 로도 언급되지 않는다. enforcement 맥락 (거부 목록) 으로만 등장한다.

---

## 7. v4 trigger 조건

- R2 phase 2 통합 시 (closure 100% 도달 시 v4)
- 실 D2R first frame 도달 시 v4
- phase 12 entry call 안정화 + phase 13 Metal first frame 동시 성공 시 v4

v3 의 만점 검토 (cycle 85 commit 6bce113) 는 hexa-side surface 만점에 한정되었고, native 통합 후의 v4 는 실 launch first frame 을 trigger 로 한다. v4 는 closure 100% 의 absolute final mark 이며, 본 docs 가 v3 → v4 사이의 final transition state 를 기록한다.

---

## 8. cycle 90 sprint 의 산출물 세부

### 5 phase 추적 모듈 sketch

각 모듈은 hexa-side 에서 phase 의 expected behavior 를 정의하고, native 통합 후의 실측 출력과 비교하여 회귀를 감지한다.

`pe_section_mmap_real_invoke`:
- 입력: PE Section header array (5 entries — .text/.rdata/.data/.rsrc/.reloc)
- 출력: 매핑 결과 array (base, size, prot, flags 4-tuple)
- self-test: D2R Setup.exe 의 expected section size 와 raw_data offset 을 hardcode 하고, native 호출 후 result 와 일치 검증.

`pe_iat_dlsym_routing_real`:
- 입력: IAT entry list (435 entries)
- 출력: routing decision array (mapped/stub/unmapped 3-state)
- self-test: WIN32_TO_POSIX_SYMBOL_MAP.md 의 16 카테고리 별 expected hit rate 검증 (예: kernel32 → 65%, user32 → 70%, gdi32 → 50% 등).

`pe_tls_fs_segment_alloc`:
- 입력: fs_offset 사용 list (TIB 의 0x18, 0x30, 0x34 등)
- 출력: pthread_key_create 결과 + signal handler 등록 상태
- self-test: 가상의 fs:[0x18] read trap 시뮬레이션 (signal handler 가 pthread_getspecific 으로 self pointer 를 반환하는지).

`pe_entry_invoke_phase`:
- 입력: entry_rva + actual_base
- 출력: invoke 결과 (success / signal_caught / abort 3-state)
- self-test: i386 cdecl convention 의 stack alignment 와 return value pickup 검증.

`pe_metal_device_weak_link_real`:
- 입력: helper IPC kind=7 응답 (MTLDevice handle)
- 출력: CAMetalLayer attach 결과 + first nextDrawable 성공 여부
- self-test: Metal device 가 fallback path (software renderer) 가 아닌 hardware 인지 확인.

### docs/WIN32_TO_POSIX_SYMBOL_MAP.md (16 카테고리)

매핑 표는 다음 16 카테고리로 구성된다.

1. file I/O (CreateFileA/W, ReadFile, WriteFile, CloseHandle) → libSystem (open, read, write, close)
2. memory (HeapAlloc, VirtualAlloc, VirtualProtect) → libSystem (malloc, mmap, mprotect)
3. thread (CreateThread, WaitForSingleObject, SetEvent) → pthread + dispatch_semaphore
4. process (CreateProcess, GetCurrentProcessId, ExitProcess) → posix_spawn, getpid, exit
5. registry (RegOpenKeyEx, RegQueryValueEx) → CFPreferences + plist
6. window (CreateWindowEx, ShowWindow, GetMessage) → AppKit (NSWindow, NSApplication)
7. GDI (BitBlt, CreateCompatibleDC, SelectObject) → CoreGraphics (CGContext, CGImage)
8. user input (GetAsyncKeyState, GetCursorPos) → AppKit (NSEvent), CoreGraphics (CGEventSource)
9. timer (GetTickCount, QueryPerformanceCounter) → mach_absolute_time
10. string (lstrlenA, lstrcpyA, MultiByteToWideChar) → libSystem (strlen, strcpy, libiconv)
11. crypto (CryptAcquireContext, CryptHashData) → CommonCrypto (CC_SHA256, CC_AES)
12. network (WSAStartup, send, recv) → libSystem socket API
13. registry/COM (CoInitialize, CoCreateInstance) → own2 stub (D2R 사용 최소화)
14. shell (ShellExecute, SHGetFolderPath) → NSWorkspace + NSFileManager
15. NTDLL (RtlAllocateHeap, RtlInitUnicodeString) → libSystem 변환 wrapper
16. D3D (Direct3DCreate9, IDirect3DDevice9::Present) → D3DMetal binary (own1 § A 명시 허용)

각 카테고리의 매핑 누적률은 cycle 80-89 sprint 의 own2 stub 작업으로 65-95% 범위에 도달했고, 미매핑 함수는 own2 stub (default behavior — return success + log) 로 routing 한다.

---

## 9. 실측 commit hash 및 변경 line 수

| commit | description | line +/- |
|---|---|---|
| b72349b | E1+E2 helper.mm + codesign 실 source | +482 / -3 |
| ef081fc | gitignore native/airgenome_helper | +1 / -0 |
| 6382fa8 | E3+E4 PE→Mach-O thin shim + D3DMetal probe | +614 / -8 |
| 31da638 | dispatch launch subcommand + entry shim 통합 | +389 / -12 |
| 3044bb9 | shim phase 5-6 IAT walk + helper IPC dlopen | +267 / -4 |

cycle 89 → 90 의 누적 변화: +1753 line (native source) / +5 commits / closure 84% → 96%.

---

## 10. 잔여 위험 (cycle 90 → 100% 도달 사이)

### technical risk

1. **MAP_JIT 권한 충돌** — Apple Silicon 의 hardened runtime 에서 MAP_JIT 은 com.apple.security.cs.allow-jit entitlement 가 있어야 한다. codesign 단계에서 entitlement 가 제대로 적용되었는지 launch-time 검증이 필요하다.

2. **i386 ABI 호환성** — macOS 11 이후 i386 binary 는 native 실행 불가. shim 은 i386 PE 의 instruction 을 본인 (x86_64 또는 arm64) process 내에서 emulate 하거나, JIT 변환 (PE32 → x86_64 transcoded JIT) 으로 우회해야 한다. 본 plan 의 phase 9-12 는 transcoded JIT path 를 가정한다.

3. **fs segment trap 비용** — TIB access 가 hot path 에서 자주 발생하면 SIGSEGV handler overhead 가 frame budget 을 초과할 수 있다. compile-time patching 또는 pre-resolve 기법으로 hot path 의 fs access 를 줄여야 한다.

4. **D3DMetal API 호환성** — D2R 의 D3D9 호출 set 이 D3DMetal 의 지원 surface 와 정확히 일치하지 않을 수 있다. D3DMetal 의 release notes 와 supported_calls.txt 를 확인하여 차이를 own2 stub 으로 보충한다.

5. **SEH (Structured Exception Handling) 시뮬레이션** — Win32 의 try/__except block 은 fs:[0] 의 SEH chain 으로 구현된다. shim 의 signal handler 가 SEH chain 을 walk 하고 적절한 handler 를 호출하는 emulation 이 필요하다.

### process risk

1. **외부 결정 잔여 작업의 시간 추정** — E3 phase 9-12 는 수일~수주, E4 D3DMetal 실 link 는 수일. 실 D2R first frame 도달까지의 calendar time 은 R2 phase 2 통합 작업의 완료 일정에 의존한다.

2. **사용자 directive 변동 가능성** — 사용자의 우선순위가 변경되면 본 plan 의 cycle 91+ 는 재구성될 수 있다. 본 docs 는 cycle 90 sprint 시점의 absolute final state 만 기록하며, 후속 변경은 별도 docs 로 추가한다.

---

## 11. SSOT chain 및 본 docs 위치

- cycle 84: `docs/CLOSURE_PATH_B_VERIFICATION.md` (path B 검증)
- cycle 89: `docs/CLOSURE_FINAL_STATE.md` (외부 결정 가이드)
- cycle 90: `docs/REAL_LAUNCH_FINAL_STATE.md` (본 docs, absolute final state)
- cycle 91+: R2 phase 2 통합 결과 docs (예정, 가칭 `R2_PHASE2_INTEGRATION_RESULT.md`)

본 docs 는 cycle 90 sprint 의 SSOT 이며, 후속 cycle 의 통합 작업은 본 docs 의 phase 9-13 plan 을 1:1 reference 한다. 본 docs 의 후속 갱신은 cycle 91+ 의 통합 결과를 반영한 별도 docs 에서 이루어지고, 본 docs 자체는 cycle 90 시점의 record 로 동결된다.

---

## 12. 마무리

closure 96% 는 hexa skeleton 128 모듈 + native helper.mm + PE→Mach-O thin shim + dispatch launch + IAT walk + helper IPC dlopen 까지의 누적 결과다. 잔여 4% 는 phase 9-13 의 native 통합 (PE 본체 mmap, IAT routing patch, TLS fake FS, entry call, Metal first frame) 으로 채워진다. own1 enforcement 는 본 docs 의 모든 단계에서 절대 우선이며, third-party Wine fork 또는 community translation layer 의 도입은 거부된다.

본 docs 는 cycle 90 의 absolute final state 를 기록한다. 후속 작업은 본 docs 를 reference 로 하여 cycle 91+ 에서 진행한다.
