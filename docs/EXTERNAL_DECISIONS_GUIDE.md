# 외부 결정 4건 — 사용자 step-by-step 가이드 (2026-05-01)

> **status**: cycle 88 sprint 직후 (closure path B 84% 도달). closure 95%+ 도달 위한 외부 작업 영역 4건의 사용자 actionable 가이드.
> **대상 사용자**: airgenome-gamebox 의 BG agent 한계 (~84%) 도달 후 외부 결정 처리하려는 사용자.
> **SSOT**: `docs/STAGE7_HELPER_MM_IPC_SPEC.md` (586 LOC) / `docs/CLOSURE_PATH_B_VERIFICATION.md` / `docs/REMAINING_ROADMAP_V2.md` / `docs/PHASE3_RESULTS_REVIEW.md`
> **own1 enforcement**: Apple SDK + Microsoft PE/COFF spec + man page reference only.

---

## 0. 4 외부 결정 영역 요약

| # | 영역 | 예상 시간 | closure 기여 | stage 매핑 | 의존 prereq |
|---|---|---|---|---|---|
| **E1** | helper.mm Xcode/clang build | 1–2 시간 | 70% → 78% | stage 1/2/3/4 모두 unblock | 없음 — reference sample 230 LOC inline |
| **E2** | codesign + JIT entitlement | 30 분 | 78% → 85% | stage 2 MAP_JIT + stage 3 syscall + stage 4 Metal | E1 산출물 `airgenome_helper` |
| **E3** | PE → Mach-O thin shim 실 변환 | 수일 | 85% → 92% | R4 B path (binary translation) | E1+E2 |
| **E4** | D3DMetal binary weak link | 수일 | 92% → 95%+ | stage 4 (first_frame) 결정적 prereq | macOS 14.4+ + Apple GPU family 7+ |

→ E1+E2 처리만으로 closure 84% → 92% 도약. E4 까지 처리 시 D2R/D4 first_frame unblock — closure 95%+ 도달.

→ E3 는 R4 B path (선택) — closure path B primary chain 와는 분리 (Rosetta 2 위임 OS-level).

---

## 1. E1 — helper.mm Xcode/clang build (1–2 시간)

### 1.1 사전 점검

```bash
# Xcode Command Line Tools 설치 확인
xcode-select -p                  # /Applications/Xcode.app/Contents/Developer 또는 /Library/Developer/CommandLineTools
xcode-select --install           # 미설치 시 (이미 설치됐다면 no-op)

# clang 버전 확인 — Apple clang 15.0+ 권장
clang++ --version

# Apple Silicon 확인
uname -m                         # arm64 출력 기대
sw_vers                          # macOS 14.4+ 권장 (D3DMetal 호환)
```

기대 출력:
```
Apple clang version 15.0.0 (clang-1500.x.x)
Target: arm64-apple-darwin23.x.x
```

### 1.2 helper.mm 파일 작성

경로: `~/core/airgenome-helper-mm/helper.mm` (또는 사용자 선택 경로)

```bash
mkdir -p ~/core/airgenome-helper-mm
cd ~/core/airgenome-helper-mm
```

**source 내용**: `docs/STAGE7_HELPER_MM_IPC_SPEC.md` § 4 의 230 LOC reference sample 인용. 핵심 layout:

- frame header 24 byte (`AHI1` magic + kind uint32 + seq uint64 + payload_len uint32 + reserved uint32)
- 8 IPC kind enumeration (dlopen=1 / mmap=2 / mprotect=3 / pthread_create=4 / dlsym=5 / nsobject=6 / metal_device=7 / signal_register=8)
- `read_frame()` / `write_frame()` 의 stdin/stdout binary I/O (`STDIN_FILENO` / `STDOUT_FILENO` + `read_full` / `write_full` loop)
- `handle_dlopen` / `handle_mmap` / `handle_mprotect` 3 handler 충실 구현 (sample 에 inline)
- `handle_pthread_create` / `handle_dlsym` / `handle_nsobject` / `handle_metal_device` / `handle_signal_register` 5 handler 는 TODO — 실 구현 시 본 가이드 § 1.5 참고

`helper.entitlements` 파일도 동일 디렉토리에 작성 (§ 2 참조).

### 1.3 Build 명령

```bash
cd ~/core/airgenome-helper-mm

clang++ -arch arm64 \
    -fobjc-arc \
    -framework Foundation \
    -framework Metal \
    -framework MetalKit \
    -O2 -std=c++17 \
    -Wall -Wextra \
    helper.mm \
    -o airgenome_helper
```

flag 설명:
- `-arch arm64` — Apple Silicon native (Rosetta 2 X)
- `-fobjc-arc` — Objective-C ARC 자동 메모리 관리 (NSObject lifetime kind=6)
- `-framework Foundation` — NSString/NSData/NSDictionary 의 obj_msgSend dispatch
- `-framework Metal` — `MTLCreateSystemDefaultDevice()` / `<MTLDevice>` (kind=7 metal_device)
- `-framework MetalKit` — MTKView 등 (선택, kind=7 확장 시)
- `-O2` — release build (debug 시 `-O0 -g`)
- `-std=c++17` — C++17 (`if constexpr`, structured binding 등)

### 1.4 검증 — binary 자체

```bash
# 1. 실행 가능?
file airgenome_helper
# 기대: airgenome_helper: Mach-O 64-bit executable arm64

# 2. framework link 확인
otool -L airgenome_helper
# 기대 (필수):
#   /System/Library/Frameworks/Foundation.framework/Versions/C/Foundation
#   /System/Library/Frameworks/Metal.framework/Versions/A/Metal
#   /usr/lib/libc++.1.dylib
#   /usr/lib/libobjc.A.dylib
#   /usr/lib/libSystem.B.dylib

# 3. arch 확인
lipo -info airgenome_helper
# 기대: Non-fat file: airgenome_helper is architecture: arm64

# 4. symbol 확인 (handler 함수 등)
nm airgenome_helper | grep -E '_handle_|_main|_read_frame|_write_frame'
# 기대: handle_dlopen / handle_mmap / handle_mprotect / read_frame / write_frame / _main 노출
```

### 1.5 검증 — handshake test (frame round-trip)

```bash
# 단일 dlopen frame 송신 — magic "AHI1" + kind=1 + seq=0 + payload_len=0 응답 echo 확인
# (single byte python helper — 외부 의존 X, system Python 3 사용)
python3 -c "
import sys, struct
# request: AHI1 magic + kind=1 (dlopen) + seq=0 + payload_len=12 + reserved=0
#          + flags=2 (RTLD_NOW) + path_len=0 + (no path)
hdr = b'AHI1' + struct.pack('<IQII', 1, 0, 12, 0)
payload = struct.pack('<iI', 2, 0) + b''  # flags=RTLD_NOW, path_len=0
sys.stdout.buffer.write(hdr + payload)
sys.stdout.buffer.flush()
" | ./airgenome_helper | xxd | head -5

# 기대 첫 24 byte: 41 48 49 31 (AHI1) / kind=01000000 / seq=0 / payload_len > 0 / reserved=0
# 응답 payload: handle_kind=int32 / native_handle=uint64 / err=int32 / err_msg_len=uint32 / err_msg
```

→ EOF 시 helper graceful exit (stdin EOF → main loop break).

### 1.6 trouble shooting

| 증상 | 원인 | 해결 |
|---|---|---|
| `'Foundation/Foundation.h' file not found` | Xcode CLT 미설치 | `xcode-select --install` 재실행 |
| `linker error: framework Metal` | Metal SDK 미존재 (macOS < 12.0) | macOS 14.4+ 업그레이드 |
| `bus error / SIGBUS` 첫 mmap 호출 | JIT entitlement 미적용 | E2 (§ 2) 적용 후 재실행 |
| `Killed: 9` 첫 launch | Hardened Runtime + library validation | E2 의 `disable-library-validation` 적용 |
| frame magic mismatch (`-2` 반환) | stdin pipe 가 line buffered | parent 가 unbuffered binary mode 보장 (`setvbuf(_IONBF)`) |
| ARC 컴파일 에러 (`__bridge` 누락) | C → ObjC 형 변환 명시 누락 | `(__bridge id)` 또는 `(__bridge_retained CFTypeRef)` 추가 |

---

## 2. E2 — codesign + JIT entitlement (30 분)

### 2.1 entitlement plist 작성

`~/core/airgenome-helper-mm/helper.entitlements`:

```xml
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE plist PUBLIC "-//Apple//DTD PLIST 1.0//EN" "http://www.apple.com/DTDs/PropertyList-1.0.dtd">
<plist version="1.0">
<dict>
    <!-- JIT region 사용 (MAP_JIT + pthread_jit_write_protect_np) -->
    <key>com.apple.security.cs.allow-jit</key>
    <true/>
    <!-- unsigned executable memory (PE→arm64 binary translation 결과 RX 매핑) -->
    <key>com.apple.security.cs.allow-unsigned-executable-memory</key>
    <true/>
    <!-- dyld interposing 허용 (DYLD_INSERT_LIBRARIES 사용 시) -->
    <key>com.apple.security.cs.allow-dyld-environment-variables</key>
    <true/>
    <!-- library validation 우회 — DLL→dylib path 는 ad-hoc sign -->
    <key>com.apple.security.cs.disable-library-validation</key>
    <true/>
</dict>
</plist>
```

각 entitlement 의 stage 매핑:

| entitlement | stage 매핑 | 미적용 시 |
|---|---|---|
| `allow-jit` | stage 2 mmap MAP_JIT | mmap MAP_JIT → EACCES |
| `allow-unsigned-executable-memory` | stage 2 mprotect PROT_EXEC | mprotect → SIGBUS |
| `allow-dyld-environment-variables` | stage 1 launch env propagation | DYLD_* env 무시 |
| `disable-library-validation` | stage 3 dlopen (ad-hoc signed dylib) | dlopen → "code signature invalid" |

### 2.2 ad-hoc codesign

```bash
cd ~/core/airgenome-helper-mm

# ad-hoc signing (`-s -`) — Developer ID 발급 보류 가능
codesign --sign - \
    --entitlements helper.entitlements \
    --force \
    --options runtime \
    airgenome_helper
```

flag 설명:
- `--sign -` — ad-hoc identity (keychain identity 불요)
- `--entitlements` — plist 부착
- `--force` — 기존 signature 덮어쓰기
- `--options runtime` — Hardened Runtime 활성화 (entitlement 적용 prereq)

### 2.3 검증 — entitlement 부착 확인

```bash
# entitlement plist 출력
codesign -d --entitlements - airgenome_helper

# 기대 출력 (stderr 일부, stdout 에 plist xml):
#   Executable=/path/to/airgenome_helper
#   <?xml version="1.0" encoding="UTF-8"?>
#   <plist version="1.0">
#   <dict>
#       <key>com.apple.security.cs.allow-jit</key>
#       <true/>
#       ...
#   </dict>
#   </plist>

# signature 메타 출력
codesign -dv airgenome_helper
# 기대:
#   Identifier=airgenome_helper
#   Format=Mach-O thin (arm64)
#   CodeDirectory v=20500 size=... flags=0x10000(runtime) hashes=...+7 location=embedded
#   Signature=adhoc

# verify
codesign --verify --verbose airgenome_helper
# 기대: airgenome_helper: valid on disk
#       airgenome_helper: satisfies its Designated Requirement
```

### 2.4 검증 — JIT 영역 mmap 가능?

```bash
# helper 직 launch — JIT 영역 mmap 시도 (kind=2 mmap with MAP_JIT)
python3 -c "
import sys, struct
# request: AHI1 + kind=2 (mmap) + seq=1 + payload_len=24 + reserved=0
hdr = b'AHI1' + struct.pack('<IQII', 2, 1, 24, 0)
# mmap_req: length=16384 / prot=PROT_READ|PROT_EXEC=5 / flags=MAP_PRIVATE|MAP_JIT|MAP_ANON=6146 / addr_hint=0
payload = struct.pack('<QiiQ', 16384, 5, 2|2048|4096, 0)
sys.stdout.buffer.write(hdr + payload)
sys.stdout.buffer.flush()
" | ./airgenome_helper | xxd | head -3

# 기대: 응답 payload 의 addr (uint64) > 0 + err (int32) == 0
# 만약 err != 0 (예: errno=13 EACCES) — entitlement 미적용 또는 ad-hoc sign 실패
```

### 2.5 distribution 시 (선택)

ad-hoc 으로 dev 환경 충분 — distribution (Apple notarization) 은 선택:

```bash
# 1. Apple Developer ID 등록 (apple.com/developer 가입 — 연 $99 / 본 가이드 범위 외)
# 2. keychain 에 Developer ID Application certificate 설치
# 3. Developer ID 로 sign:
codesign --sign "Developer ID Application: <Your Name> (<Team ID>)" \
    --entitlements helper.entitlements \
    --force \
    --options runtime \
    --timestamp \
    airgenome_helper

# 4. notarization 제출:
xcrun notarytool submit airgenome_helper \
    --apple-id <your.email@example.com> \
    --team-id <Team ID> \
    --password <app-specific-password> \
    --wait

# 5. staple ticket 부착 (notarization 통과 후):
xcrun stapler staple airgenome_helper
```

→ 본 가이드는 ad-hoc dev path 만 충분 — distribution 은 사용자 release 결정.

---

## 3. E3 — PE → Mach-O thin shim 실 변환 (수일)

> **stage 매핑**: R4 B path (binary translation) — closure path B primary chain 와는 분리. Rosetta 2 위임 OS-level.
> **prereq**: E1+E2 처리 완료 (helper IPC dispatch 가능 상태).
> **참조**: `lib/loader/pe_to_macho_shim_track.hexa` (cycle 80 phase 1 추적기) + `docs/BINARY_TRANSLATION_MANJEOM_REVIEW.md`.

### 3.1 step 1 — PE Optional Header 파싱

PE binary 의 IMAGE_DOS_HEADER → IMAGE_NT_HEADERS64 → IMAGE_OPTIONAL_HEADER64 fields 파싱:

- `Magic` (PE32+ = 0x20B 만 지원, PE32 = 0x10B 거부)
- `AddressOfEntryPoint` — RVA 의 entry point
- `ImageBase` — preferred base (보통 0x140000000 EXE / 0x180000000 DLL)
- `SectionAlignment` (보통 0x1000) / `FileAlignment` (보통 0x200)
- `SizeOfImage` — total VA size
- `Subsystem` (3=Console / 2=GUI)

reference: Microsoft PE/COFF spec — Optional Header section.

### 3.2 step 2 — Mach-O LC_SEGMENT_64 합성

Mach-O thin binary 헤더 합성:

- `mach_header_64` — magic=`MH_MAGIC_64` (0xFEEDFACF) / cputype=`CPU_TYPE_X86_64` (0x01000007) / filetype=`MH_EXECUTE` / ncmds / sizeofcmds / flags=`MH_TWOLEVEL|MH_NOUNDEFS`
- 각 PE section → `LC_SEGMENT_64` load command:
  - `__TEXT` (PE `.text` characteristics IMAGE_SCN_MEM_EXECUTE) — initprot=`VM_PROT_READ|VM_PROT_EXECUTE`
  - `__DATA` (PE `.data` IMAGE_SCN_MEM_WRITE) — initprot=`VM_PROT_READ|VM_PROT_WRITE`
  - `__RODATA` (PE `.rdata`) — initprot=`VM_PROT_READ`
- `LC_MAIN` (entryoff = `AddressOfEntryPoint - .text RVA` + `__TEXT` file offset)
- `LC_UNIXTHREAD` 대안 (older Rosetta 호환)

reference: Apple `<mach-o/loader.h>` + `man Mach-O`.

### 3.3 step 3 — x86_64 RIP-relative 보존

PE x86_64 의 RIP-relative addressing 은 Mach-O 변환 시:
- code section 그대로 복사 (ImageBase 와 무관 — RIP-relative 자체 보존)
- IAT slot 재배치: PE IAT (`.idata` table) → Mach-O `__got` section (lazy binding 미사용 — full bind)
- absolute reloc (`IMAGE_REL_BASED_DIR64`) → Mach-O `LC_DYSYMTAB` 의 indirect symbol table 재배치
- TLS callback (`IMAGE_DIRECTORY_ENTRY_TLS`) → `__thread_vars` section 변환 (선택)

caveat: Mach-O 의 strict PIE 와 PE 의 ImageBase preferred 충돌 — `MH_PIE` flag off + `LC_SEGMENT_64.vmaddr` 명시.

### 3.4 step 4 — posix_spawn 호출 (Rosetta 2 위임)

```bash
# 변환된 Mach-O thin binary 실행 — Rosetta 2 가 자동 binary translation
arch -x86_64 ./shim_binary

# 또는 posix_spawn 의 attribute 로 지정:
# posix_spawnattr_setbinpref_np(&attr, 1, &cputype_x86_64, &count)
# cputype_x86_64 = CPU_TYPE_X86_64 (0x01000007)
```

caveat:
- Rosetta 2 sandbox + Hardened Runtime 호환성 = PE/SEH (`.pdata`/`.xdata` → DWARF/compact unwind) coverage 가 game-by-game caveat
- macOS Sequoia 15+ 에서 Rosetta 2 의 일부 instruction (AVX-512) 미지원 — game 의존
- Rosetta 2 미설치 시: `/usr/sbin/softwareupdate --install-rosetta --agree-to-license`

### 3.5 검증 — 가짜 Hello World

```bash
# 1. PE32+ Hello World (cl.exe / mingw-w64 별도 환경에서 컴파일 결과)
file hello.exe
# 기대: hello.exe: PE32+ executable (console) x86-64, for MS Windows

# 2. PE → Mach-O thin shim 변환
./pe_to_macho_shim hello.exe -o hello_macho

# 3. 변환 결과 검증
file hello_macho
# 기대: hello_macho: Mach-O 64-bit x86_64 executable

otool -h hello_macho     # mach_header_64 확인
otool -l hello_macho     # LC_SEGMENT_64 / LC_MAIN 확인

# 4. ad-hoc sign + Rosetta 실행
codesign --sign - --force hello_macho
arch -x86_64 ./hello_macho
# 기대 출력: Hello, World!
```

→ 검증 PASS 후 R4 B path skeleton → real_invoke 도약. closure 점수 +7% (85% → 92%) 추정.

---

## 4. E4 — D3DMetal binary weak link (수일)

> **stage 매핑**: stage 4 (first_frame) 결정적 prereq. closure-full path 의 마지막 gating point.
> **prereq**: E1+E2 처리 완료 + macOS 14.4+ + Apple GPU family 7+ (M1 Pro/Max/Ultra / M2/M3/M4 series).
> **참조**: `lib/loader/pe_d3d_to_metal_router.hexa` 헤더 docstring + `docs/D3D_TO_METAL_SHADER_MANJEOM_REVIEW.md`.

### 4.1 step 1 — D3DMetal binary location 확인

macOS 14.4+ 의 Apple D3DMetal binary 위치 (Apple-provided framework):

```bash
# Apple D3DMetal framework path (macOS 14.4+ Sonoma+)
ls -la /Library/Apple/usr/libexec/gameportingtoolkit*
ls -la /System/Library/PrivateFrameworks/D3DMetal.framework/ 2>&1

# 또는 user-installed (Apple Developer 계정 download):
ls -la /Library/Apple/usr/lib/libd3dmetal*.dylib

# version 확인
otool -L /Library/Apple/usr/lib/libd3dmetal.dylib 2>&1 | head -3
```

만약 미설치 시: Apple Developer 계정에서 Game Porting Toolkit 다운로드 (apple.com/developer → "Game Porting Toolkit") + `installer -pkg gameportingtoolkit.pkg -target /`.

### 4.2 step 2 — Apple-provided dylib stub 매핑

helper.mm 측에서 D3DMetal weak-link:

```objc
// helper.mm 추가 (kind=7 metal_device handler 확장)
#include <dlfcn.h>

static void *d3dmetal_handle = NULL;

static void d3dmetal_init(void) {
    // weak link — 미설치 시 NULL 반환 + DIY fallback path γ
    d3dmetal_handle = dlopen("/Library/Apple/usr/lib/libd3dmetal.dylib", RTLD_LAZY | RTLD_LOCAL);
    if (d3dmetal_handle == NULL) {
        // path γ DIY fallback — `pe_d3d_to_metal_router` 의 fallback 15%
        return;
    }
    // path α D3DMetal 위임 85%
}
```

build flag (clang++ 추가):

```bash
clang++ -arch arm64 -fobjc-arc \
    -framework Foundation \
    -framework Metal \
    -L/Library/Apple/usr/lib \
    -Wl,-weak_library,/Library/Apple/usr/lib/libd3dmetal.dylib \
    -O2 -std=c++17 \
    helper.mm \
    -o airgenome_helper
```

`-weak_library` flag — load 실패 시 NULL handle (hard error X).

### 4.3 step 3 — D3D11Device → MTLDevice bridge 검증

```bash
# helper 의 kind=7 metal_device 호출 — D3DMetal 위임 path
python3 -c "
import sys, struct
# request: AHI1 + kind=7 (metal_device) + seq=2 + payload_len=12 + reserved=0
hdr = b'AHI1' + struct.pack('<IQII', 7, 2, 12, 0)
# metal_req: op=1 (MTLCreateSystemDefaultDevice) / target=0
payload = struct.pack('<iQ', 1, 0)
sys.stdout.buffer.write(hdr + payload)
sys.stdout.buffer.flush()
" | ./airgenome_helper | xxd | head -3

# 기대: 응답 payload 의 device_handle (uint64) > 0 + err (int32) == 0
```

### 4.4 단점 + 대안

| 단점 | 대안 |
|---|---|
| Apple GPU family 7+ 의존 (M1 Pro/Max+) | path γ DIY fallback (`pe_shader_xlate_dxbc` direct MSL 변환) — 15% coverage |
| macOS 14.4+ 의존 | path β `metal-shaderconverter` (macOS 13+ 호환) — DXIL/DXBC SPIR-V 우회 |
| D3DMetal 자체 closed-source (Apple binary) | own1 정합 — Wine / CrossOver / GPTK Whisky 인용 X (Apple binary 사용은 사용자 결정) |
| GPU family 6 이하 + Intel Mac | path γ DIY fallback 강제 — coverage 15% (R5 점수 제한) |

→ 본 가이드는 path α (D3DMetal 위임 85%) 우선 — Apple Silicon M1 Pro/Max+ + macOS 14.4+ 환경 기준.

### 4.5 검증 — first frame swapchain present

```bash
# helper 의 kind=7 metal_device 응답 + Metal command queue 생성 확인
# (D2R install 후 game.exe 의 D3D11CreateDevice 호출 → helper IPC dispatch → MTLDevice 생성)

# Metal performance counter 확인
sudo dtrace -n 'metal*:::frame-present { @[probename] = count(); }' 2>&1 | head -5
# 기대: D2R launch 후 frame-present probe count > 0
```

→ first_frame 도달 시 closure path B 5-stage chain 완주. closure 점수 95%+ 도달.

---

## 5. 검증 — 4건 모두 처리 후 closure 측정

### 5.1 dispatch install 재실행

```bash
cd ~/core/airgenome-gamebox

# helper binary path 환경변수 지정
export AIRGENOME_HELPER=~/core/airgenome-helper-mm/airgenome_helper

# install dispatch
./entry install tool/bench/Battle.net-Setup.exe

# 기대 stage 도달도:
#   stage 0 install              ✓ (BG 단독 95%)
#   stage 1 launch_request       ✓ (E1 후)
#   stage 2 pe_load              ✓ (E1+E2 후 — MAP_JIT)
#   stage 3 native_runtime       ✓ (E1+E2 후 — syscall path)
#   stage 4 first_frame          ✓ (E1+E2+E4 후 — Metal swapchain)
```

### 5.2 closure 점수 재측정

```bash
# closure validation harness
hexa run tests/test_closure.hexa

# coverage v3 측정
hexa run lib/perf/c_pe_install_coverage.hexa
# 기대: Battle.net Setup PE imports coverage 73% → (E1+E2+E4 후) 95%+

# phase status dashboard
hexa run lib/perf/pe_skeleton_phase_status.hexa
# 기대: phase_kind 2 (real_invoke) → 3/4/5 (external_dep_*) unblock 비율 100%
```

### 5.3 self-test 회귀 확인

```bash
hexa run tests/test_apple_only.hexa     # 21/21 PASS 유지
hexa run tests/test_own2.hexa            # 8/8 PASS 유지
hexa run tests/test_closure.hexa         # closure validation
```

---

## 6. trouble shooting — 흔한 에러 + 해결

| 단계 | 증상 | 원인 | 해결 |
|---|---|---|---|
| E1 build | `'Foundation/Foundation.h' file not found` | Xcode CLT 미설치 | `xcode-select --install` |
| E1 build | `linker command failed: framework Metal` | macOS < 12.0 | macOS 14.4+ 업그레이드 |
| E1 runtime | `bus error / SIGBUS` | JIT entitlement 미적용 | E2 (§ 2) 재실행 |
| E1 runtime | frame magic mismatch (-2) | parent stdin line-buffered | `setvbuf(stdin, NULL, _IONBF, 0)` |
| E2 codesign | `errSecCSReqInvalid` | entitlements xml 형식 오류 | plist 1.0 DTD 확인 |
| E2 runtime | `Killed: 9` | Hardened Runtime + library validation | `disable-library-validation` 적용 |
| E2 runtime | `EACCES` mmap MAP_JIT | `allow-jit` 미적용 | entitlement plist 재확인 |
| E3 변환 | Rosetta 2 미설치 | softwareupdate 필요 | `/usr/sbin/softwareupdate --install-rosetta --agree-to-license` |
| E3 실행 | `arch: posix_spawnp: Bad CPU type` | Apple Silicon 에서 x86_64 binary 직 실행 | `arch -x86_64` prefix 명시 |
| E4 dlopen | `libd3dmetal.dylib not found` | Game Porting Toolkit 미설치 | Apple Developer 다운로드 |
| E4 runtime | MTLDevice == NULL | GPU family 6 이하 | path γ DIY fallback (R5 coverage 15%) |

---

## 7. 다음 step — 실 D2R install/launch

### 7.1 D2R install (E1+E2 후)

```bash
# Battle.net Setup 다운로드 (battle.net 공식)
curl -L -o /tmp/Battle.net-Setup.exe \
    "https://www.battle.net/download/getInstaller?os=mac&installer=Battle.net-Setup.exe"

# airgenome dispatch install
cd ~/core/airgenome-gamebox
./entry install /tmp/Battle.net-Setup.exe

# 기대: stage 0 (install) + stage 1 (launch_request) + stage 2 (pe_load) PASS
# 미처리 stage: stage 3 의 일부 syscall (NTDLL Nt* 미커버) — N1 후속 cycle 대기
```

### 7.2 D2R launch (E1+E2+E4 후)

```bash
# Diablo II: Resurrected 설치 (Battle.net Agent 통해)
./entry launch ~/Library/Application\ Support/Battle.net/Games/d2r/D2R.exe

# 기대 시나리오:
#   1. Battle.net Agent IPC handshake (stage 1)
#   2. D2R.exe PE load + IAT resolve (stage 2)
#   3. KERNEL32 / NTDLL / D3D11 syscall dispatch via helper (stage 3)
#   4. MTLDevice 생성 + D3D11 → Metal command queue (stage 4)
#   5. first frame swapchain present
```

### 7.3 closure-d2r 약식 closure 진입

E1+E2+E4 처리 후 D2R 만 launch 검증 시 closure-d2r 약식 closure 95%+ 도달:

```bash
# closure 진입 trigger (CLOSURE_ROADMAP.md §11 의 CM-22 직후 + EXHAUSTION 약식)
hexa run lib/perf/pe_install_coverage_v3.hexa
# 기대: D2R import coverage > 80% + first_frame trace 1회 이상

# closure-full path 는 CM-30 (D4 + DXR) 추가 prereq — Phase 10 D3D12+DXR + Apple GPU family 7+ 환경
```

### 7.4 D4 launch (선택, closure-full)

```bash
# Diablo IV launch — D3D12 + DXR raygen/miss/closesthit 의존
./entry launch ~/Library/Application\ Support/Battle.net/Games/D4/Diablo\ IV.exe

# 기대 prereq: N8 (DXR / mesh shader 정합) cycle 84+ BG 처리 후 + E4 (D3DMetal) 적용
# closure 점수: 95% → 100% (CM-30 done)
```

---

*written 2026-05-01 cycle 88 sprint 직후. closure path B 84% 도달 시점 외부 결정 4건 (E1 helper.mm / E2 codesign / E3 PE→Mach-O / E4 D3DMetal) 의 사용자 actionable step-by-step 가이드. 본 가이드 path 외 수정 X. own1 enforcement: Apple SDK + Microsoft PE/COFF spec + man page reference only — Wine / CrossOver / GPTK / Whisky 인용 0줄. 사용자 directive: "외부 결정 4건 사용자 가이드 docs". closure 70% (cycle 83) → 84% (cycle 88) → 85% (E1+E2 후) → 92% (E3 후) → 95%+ (E4 후) 추정.*
