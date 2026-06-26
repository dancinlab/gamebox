# Changelog

All notable changes to `gamebox` are documented in this file.

## [Unreleased]

### Added

- feat(F-NSWINDOW-E5 r8): **CRT→user-entry 핸드오프 도달(합성) + BT/BTS/BTR/BTC
  디코더 갭 종결 + 마지막 CRT import 2개 바인딩** — r7 의 벽(미등록 IAT 호출
  @`0x539318`, insns=44)을 **넘어 CRT-init 의 종착점인 `call WinMain`(CRT→user-entry
  핸드오프)에 도달**한다. **이것이 정직한 E4 완료 마일스톤이며, 렌더된 게임 프레임이
  아니다 — `validated_manjeom` 은 여전히 0.** (1) `native/i386_cpu.{c,h}` 에 두 개의
  kernel32 셰임: `GetCommandLineW()`(0-arg → **합성 in-image 커맨드라인 포인터**
  `base+0x1FE0`), `SetUnhandledExceptionFilter(fn)`(1-arg → 이전 필터 NULL=0). IAT
  슬롯 `0x53802C`/`0x538030` 으로 13개(count=13). (2) `native/i386_decode.{c,h}` +
  byte-equal `.hexa` 미러: enum `I386_OP_BT_RM_R`(61,`0F A3`)/`_BTS`(62,`0F AB`)/
  `_BTR`(63,`0F B3`)/`_BTC`(64,`0F BB`) + 0F 2-byte 분기(0F AF IMUL 과 동일한 `/r`
  ModR/M 형태), op_name="bt"/"bts"/"btr"/"btc". 인터프리터: **CF ← bit(r/m, idx)**
  (idx=ModR/M.reg 레지스터, mod 32 마스크), BTS/BTR/BTC 는 수정 비트 write-back —
  평범한 Intel SDM Vol.2 비트 연산(보호 아님). RUNEQ corpus D(0F 전수)에서 C↔hexa
  byte-equal 확인. (3) **핸드오프 검출**: `i386_cpu_t.user_entry_va` 필드 + halt
  `I386_HALT_USER_ENTRY` 추가 — `call` 의 타깃이 `user_entry_va` 와 같으면 리턴 주소를
  push 한 뒤(faithful "about to enter") 정직하게 정지. (4) hermetic 테스트
  (`native/i386_cpu_test.c`) Run A 는 r7 체인(44) 뒤에 `GetCommandLineW` → `pop ecx`
  (잔여 push 정리) → `mov esi,eax` → `SetUnhandledExceptionFilter` → `bt/bts/btr/btc`
  → WinMain 4-인자 stdcall 프레임 push(nShowCmd/lpCmdLine/hPrevInstance/hInstance) →
  `call 0x539400`(합성 user entry) 를 이어 붙여 **insns 44→60, halt
  `0x539318`(unbound)→`0x539400`(user_entry)**, bound=13, last=SetUnhandledException-
  Filter 를 증명. WinMain 인자 프레임([esp+4..0x10] = hInstance·hPrevInstance·
  lpCmdLine·nShowCmd) + entry CALL 리턴주소(`0x5388AB`) 무결성까지 검증. B/D
  sentinel 은 `0F A3`(이제 BT 로 실행됨) → `0F B1`(CMPXCHG, 다음 진짜 갭, r9) 으로
  교체. 측정 라인 `__SHIM__ PARTIAL phase=e4_reached_user_entry insns=60 bound=13
  last=SetUnhandledExceptionFilter entry_va=0x539400 halt=user_entry halt_op=call
  (SYNTHETIC user-entry; validated_manjeom=0)` + `__SHIM__ INFO
  real_pe_path=structurally_ready needs:real_i386_PE+IAT_autobind_by_import_name+
  wider_opcode_coverage+D3DMetal+display`, `__SHIM_TEST__ PASS`(CI 게이트, Run A 25
  checks 전부 green, 로컬 clang `-Wall -Wextra -Wpedantic -std=c11` 청정). (5)
  **실-바이너리 경로 점검**: `i386_cpu_load_pe`(argv[1]) 는 섹션 매핑·엔트리 설정·
  이미지 할당까지 **구조적으로 배선됨** — 실제 i386 PE 를 주면 엔트리부터 실행 가능.
  단, K32_IAT 슬롯이 hermetic 합성 VA(`0x538000..`)라 실제 PE 의 IAT 와는 매칭되지
  않으므로, 실행이 첫 import 를 넘으려면 **PE import 테이블 파싱 + 이름 기반 자동
  바인딩**이 필요(real-PE emit 가 INFO 로 명시). **own1**: 자기 import 를 네이티브
  구현에 묶는 **로딩**(우회 아님), 합성 TEB/커맨드라인/user-entry VA 는 **정직 라벨**,
  BT 류는 평범한 CPU 비트 연산 — Wine/보호 없음. **`validated_manjeom>0`(실 프레임)
  까지 남은 거리**: real-asset 게이트(실 i386 게임 PE + D3DMetal SDK + 실 디스플레이 —
  사용자만 제공 가능) + 남은 엔지니어링(실 PE IAT 이름 자동바인딩, 더 넓은 opcode
  커버리지, 게임 자신의 WinMain → 메시지 루프 → CreateWindowEx → D3D→Metal 프레임).
  현재는 **합성 user-entry 도달**까지이며 실 프레임은 자율 범위 밖(real-asset-gated).

- feat(F-NSWINDOW-E5 r7): **CRT import 3개 추가 바인딩 + IMUL/CPUID/RDTSC 디코더
  갭 종결 + `__security_init_cookie` 산술 실행** — r6 의 벽(미등록 IAT 호출
  @`0x5392D4`, insns=30)을 **넘는다**. (1) `native/i386_cpu.{c,h}` 에 세 개의
  kernel32 셰임 추가: `GetStartupInfoW(LPSTARTUPINFOW)`(1-arg ptr, push 된 포인터
  `[esp]` 의 STARTUPINFOW 0x44B 를 0 으로 + `cb=0x44` 스탬프 — 버퍼-쓰기),
  `GetSystemInfo(LPSYSTEM_INFO)`(1-arg ptr, **bounded-synthetic** SYSTEM_INFO
  0x24B — dwPageSize=0x1000·nCPU=4·arch=INTEL, 나머지 0), `GetProcAddress(HMODULE,
  LPCSTR)`(2-arg → **합성 in-image 스텁 주소** `base+0x1FF0`). IAT 등록 슬롯
  `0x538000..0x538028` 로 11개(count=11). (2) `native/i386_decode.{c,h}` +
  byte-equal `.hexa` 미러: enum `I386_OP_IMUL_R_RM`(57, `0F AF`)/`_IMM`(58, `69 id`/
  `6B ib`)/`I386_OP_CPUID`(59, `0F A2`)/`I386_OP_RDTSC`(60, `0F 31`) + 디코드
  분기 추가, op_name="imul"/"cpuid"/"rdtsc". 인터프리터: **IMUL** 은 32-bit signed
  곱의 하위 32-bit 를 dst(ModR/M.reg)에, int32 오버플로면 CF=OF=1(SDM Vol.2);
  **CPUID** 는 leaf(eax)별 **합성** 피처셋(leaf0 vendor "GenuineIntel"·max-leaf,
  leaf1 family/feature) 을 eax/ebx/ecx/edx 에; **RDTSC** 는 `cpu->tsc += step` 합성
  단조 카운터를 edx:eax 에 기록. CPUID/RDTSC 는 평범한 CPU 명령(보호 아님). (3)
  **보안쿠키 산술**: hermetic 프롤로그가 FILETIME 엔트로피 로드 → `xor` →
  `imul eax,eax,0x01000193`(신규 0x69 IMUL) → 글로벌 슬롯에 **비기본 쿠키** 저장 →
  `cpuid`(GenuineIntel) → `rdtsc`. (4) hermetic 테스트(`native/i386_cpu_test.c`)
  Run A 는 r6 체인 뒤에 `call GetStartupInfoW`(cb=0x44 검증) / `call GetSystemInfo`
  (page=0x1000·nCPU=4 검증) / `push×2; call GetProcAddress` / 쿠키 `mov/xor/imul/mov`
  (쿠키 == `(FT_lo^FT_hi)*0x01000193` 재유도 검증, ≠0) / `cpuid`(ebx="Genu"·
  ecx="ntel" 검증) / `rdtsc`(edx:eax 합성 검증) / 미등록 `call` 을 이어 붙여 경계
  통과를 증명: **insns 30→44, halt `0x5392D4`→`0x539318`**(reason=UNBOUND_IMPORT,
  slot `0x53802C`), bound=11, last=GetProcAddress. B/D sentinel 은 `0F A2`(이제 실행됨)
  → `0F A3`(BT, 다음 진짜 디코더 갭) 으로 교체. 측정 라인 `__SHIM__ PARTIAL
  phase=e4_crt_security_cookie insns=44 bound=11 last=GetProcAddress
  halt_va=0x539318 halt=unbound_import halt_op=call unbound_slot=0x53802C`,
  `__SHIM_TEST__ PASS`(CI 게이트, Run A 18 checks 전부 green, 로컬 clang
  `-Wall -Wextra -Wpedantic -std=c11` 청정). **own1**: 로더가 자기 프로세스의 **자기
  import**(kernel32 OS API)를 네이티브 구현에 묶는 **로딩** — 우회 아님; CPUID/RDTSC
  는 평범한 CPU 명령, 쿠키는 **자기 버퍼 위의 자기 산술**, 합성 SYSTEM_INFO/스텁은
  정직 라벨. **`main`/`WinMain` 미도달**: 아직 CRT-init(import 바인딩) 단계이며
  hermetic 프롤로그는 실제 바이너리가 아닌 문서화된 SHAPE 라 진짜 `call main` 은
  없음 — 남은 CRT 단계(추가 import + `__scrt_common_main_seh` → user entry). `validated_manjeom`
  은 여전히 **0**(로더 진척이지 렌더된 프레임 아님). 다음 r8: 미등록 slot `0x53802C`
  바인딩 + `0F A3` BT 디코더 갭 + 실제 바이너리에서 `mainCRTStartup→main` 거리 측정.

- feat(F-NSWINDOW-E5 r6): **CRT-security import 3개 추가 바인딩 + 0F B6/B7/BE/BF
  MOVZX/MOVSX 디코더 갭 종결 + 합성 TEB→PEB→ImageBase 체인** — r5 의 벽(미등록 IAT
  호출 @`0x5392A1`, insns=18)을 **넘는다**. (1) `native/i386_cpu.{c,h}` 에 세 개의
  kernel32 셰임 추가: `IsProcessorFeaturePresent(DWORD)`(1-arg → BOOL 1, 기능
  present 결정적 주장), `InitializeSListHead(PSLIST_HEADER)`(1-arg ptr, push 된
  포인터 `[esp]` 의 8-byte SLIST 헤더를 0 으로 — 버퍼-쓰기), `GetModuleHandleW(
  LPCWSTR)`(1-arg, HMODULE=로더가 아는 **자기 이미지 베이스** 반환). IAT 등록
  슬롯 `0x538000..0x53801C` 로 8개(count=8). (2) `native/i386_decode.{c,h}` +
  byte-equal `.hexa` 미러: 2-byte(0F) 맵에 enum `I386_OP_MOVZX_RM8`(53)/`_RM16`(54)/
  `I386_OP_MOVSX_RM8`(55)/`_RM16`(56) + 디코드 분기(B6/B7/BE/BF, ModR/M reg=dst
  r32, r/m8|r/m16 src) 추가, op_name="movzx"/"movsx"; 0F 의 `avail<6` 가드를
  `avail<2`(+Jcc 만 `<6`)로 분리해 짧은 movzx 도 디코드. 인터프리터는 r/m8(레지스터
  바이트맵 al..bh / mem `mem_read8`)·r/m16 을 zero/sign-extend → r32, **EFLAGS 불변**.
  (3) **BOUNDED-SYNTHETIC TEB**: `i386_cpu` 에 `teb_base` 필드 추가 — 메모리 오퍼랜드가
  FS 세그먼트-override 프리픽스(0x64, 이미 디코드됨)를 달면 `i386_ea` 가 유효주소를
  `teb_base+disp` 로 리디렉트. 합성 TEB(self@+0x18) → PEB(@TEB+0x30) → ImageBase
  (@PEB+8) 의 몇 개 필드만 이미지 메모리에 깔아 보안쿠키/CRT-startup 프롤로그의
  `mov ecx, fs:[0x18]` 체인이 **돈다**(OS TEB 아님·보호구조 아님, 정직 라벨). hermetic
  테스트(`native/i386_cpu_test.c`) Run A 는 r5 합성 프롤로그 뒤에 `push 0xA; call
  IsProcessorFeaturePresent` / `movzx ecx,al; movsx edx,cl`(신규 두-바이트 실증) /
  `push &slist; call InitializeSListHead`(8B 제로화 검증, 사전 더티 `0xFFFFFFFF`) /
  `push 0; call GetModuleHandleW`(→ 이미지 베이스) / `mov ecx,fs:[0x18]; mov edx,
  [ecx+0x30]; mov eax,[edx+8]`(TEB→PEB→ImageBase) / 미등록 `call` 을 이어 붙여
  경계 통과를 증명: **insns 18→30, halt `0x5392A1`→`0x5392D4`**(reason=
  UNBOUND_IMPORT, slot `0x538020`), bound=8, last=GetModuleHandleW,
  eax=ImageBase(`0x538000`)·ecx=TEB(`0x539C00`)·edx=PEB(`0x539C80`) 실증,
  esp=esp0-8(엔트리 반환주소+미pop push). B/D sentinel 은 `0F B6`(이제 실행됨)→
  `0F A2`(CPUID, 다음 진짜 디코더 갭) 으로 교체. 측정 라인 `__SHIM__ PARTIAL
  phase=e4_crt_security_teb insns=30 bound=8 last=GetModuleHandleW
  halt_va=0x5392D4 halt=unbound_import halt_op=call unbound_slot=0x538020`,
  `__SHIM_TEST__ PASS`(CI 게이트, Run A 17 checks 전부 green, 로컬 clang
  `-Wall -Wextra -Wpedantic -std=c11` 청정). **own1**: 로더가 자기 프로세스의 **자기
  import**(kernel32 OS API)를 네이티브 구현에 묶는 **로딩** — 우회 아님; TEB 는 **자기
  합성 블록**(필드 몇 개)이지 OS TEB 도, 보호장치도 아님. `validated_manjeom` 은
  여전히 **0**(로더 진척이지 렌더된 프레임 아님). 다음 r7: 미등록 slot `0x538020`
  바인딩(GetStartupInfoW/GetProcAddress 등) + `0F A2` CPUID 디코더 갭 + 풀 보안쿠키
  연산(time/pid/tid XOR·mul) → `main`/`WinMain` 도달, user32 `CreateWindowEx` 거리.
- feat(F-NSWINDOW-E5 r5): **CRT-init import 5개 연속 바인딩 + 첫 버퍼-쓰기 셰임 +
  0xC1 shift-group 디코더 갭 종결** — r4 의 벽(미등록 IAT 호출 @`0x53927C`,
  insns=11)을 **넘는다**. `native/i386_cpu.{c,h}` 에 네 개의 kernel32 셰임 추가:
  `GetCurrentProcessId`(0-arg → 결정적 PID `0xD04`), `GetTickCount`(0-arg → `0x1D4C0`),
  그리고 **첫 버퍼-쓰기형 셰임** `GetSystemTimeAsFileTime(LPFILETIME)` —
  스택에서 caller 가 push 한 포인터 인자(`[esp]`, 호출 가로채기 시점이라 반환주소가
  아직 안 쌓임)를 읽어 이미지 메모리에 8-byte FILETIME(`mem_write32`×2)을 기록하고
  void 반환(stdcall 1-arg, callee-pop 4) — r4 의 0-arg 셰임 대비 **stub→이미지 메모리
  쓰기 + 실제 push 된 포인터 인자**라는 새 능력을 행사. `QueryPerformanceCounter(
  LARGE_INTEGER*)` 도 동일 패턴(8-byte 카운터 기록, BOOL=1 반환). 실행 루프는 이제
  bound 호출을 연달아 디스패치해 프롤로그가 다섯 import(GetCurrentThreadId →
  GetCurrentProcessId → GetSystemTimeAsFileTime → QueryPerformanceCounter →
  GetTickCount)를 통과하고, **여섯 번째·미등록 IAT 호출 @`0x5392A1`** 에서 정직하게
  HALT(`UNBOUND_IMPORT`, 다음 슬롯 `0x538014` 보고).
- feat(F-NSWINDOW-E5 r5): **0xC1/0xD1/0xD3 group-2 shift/rotate 디코더+실행** —
  r3 sentinel 이 지목한 `0xC1` 갭을 종결. `native/i386_decode.{c,h}` +
  byte-equal `.hexa` 미러에 enum `I386_OP_SHIFT_RM_IMM8`(50)/`_RM_1`(51)/`_RM_CL`(52)
  + 디코드 분기(count 출처: imm8 / 1 / CL) 추가, op_name="shift". 인터프리터
  (`i386_cpu.c`)는 ModR/M.reg 로 SHL/SAL(4/6)·SHR(5)·SAR(7)·ROL(0)·ROR(1) 을
  분기하고 SDM 대로 count 를 5-bit 마스킹 + CF/OF/ZF/SF/PF 갱신(count==0 → 무변화·
  무플래그); RCL/RCR(2/3) carry-chain 은 미모델 → 정직한 UNSUPPORTED. RUNEQ 는
  C/hexa 양쪽이 동일하게 바뀌므로 그린 유지(corpus A 의 0xC1/D1/D3 행만 동시 변화).
  hermetic 테스트(`native/i386_cpu_test.c`) Run A 는 합성 프롤로그를 0x53927C 까지
  유지하고 뒤에 `call GetCurrentProcessId` / `push &filetime; call
  GetSystemTimeAsFileTime` / `push &perfcount; call QueryPerformanceCounter` /
  `shl edx,4`(group-2 shift) / `call GetTickCount` / 미등록 `call` 을 이어 붙여
  경계 통과를 증명: **insns 11→18, halt `0x53927C`→`0x5392A1`**(reason=
  UNBOUND_IMPORT, slot `0x538014`), bound=5, last=GetTickCount, eax=GetTickCount
  stub, edx=TID<<4=`0x1A2B0`(shift 실증), ecx=0xEF(프롤로그 보존), esp=esp0-4,
  그리고 **버퍼 두 곳 검증**: `[0x539F00]`=FILETIME `01D7A1B2:C3D4E5F6`,
  `[0x539F10]`=counter `00000000:12345678`. B/D sentinel 은 `0xC1`(이제 실행됨)→
  `0F B6`(MOVZX, 다음 진짜 디코더 갭) 으로 교체. 측정 라인 `__SHIM__ PARTIAL
  phase=e4_kernel32_iat_bind insns=18 bound=5 last=GetTickCount halt_va=0x5392A1
  halt=unbound_import halt_op=call unbound_slot=0x538014`, `__SHIM_TEST__ PASS`
  (CI 게이트, Run A 16 checks 전부 green, 로컬 clang `-Wall -Wextra -std=c11` 청정).
  **own1**: 로더가 자기 프로세스의 **자기 import**(kernel32 OS API 표면)를 네이티브
  구현에 묶는 **로딩** — 우회 아님. FILETIME/perf-counter/tick/pid 는 CRT init 의
  결정적 STUB 이지 보호장치(DRM/Warden/anti-cheat)와 무관하고, 버퍼 쓰기는 프로그램
  **자기 버퍼**에 한정. `validated_manjeom` 은 여전히 **0**(로더 진척이지 렌더된
  프레임 아님). 다음 r6: 미등록 slot `0x538014` 바인딩 + `0F B6` MOVZX 디코더 갭 +
  더 깊은 CRT init(보안쿠키 XOR/__security_init_cookie, TEB/PEB 접근) → `main`/
  `WinMain` 도달, 그리고 user32 `CreateWindowEx` 까지의 거리 측정.

- feat(F-NSWINDOW-E5 r4): **첫 E4 kernel32 IAT 바인딩** — i386 인터프리터가 r3 의
  벽(`FF 15 [disp32]` 간접 IAT 호출 @`0x539274`, insns=9 에서 HALT)을 **넘는다**.
  `native/i386_cpu.{c,h}` 에 import-stub 레지스트리(`i386_iat_t`/`i386_import_t` +
  `i386_iat_lookup`) 와 첫 네이티브 kernel32 셰임(`i386_shim_GetCurrentThreadId`,
  stdcall 0-arg → 결정적 plausible TID `0x1A2B`)을 추가. 실행 루프의 `CALL_RM`/`JMP_RM`
  케이스는 이제 정규 IAT thunk 형태(ModR/M mod=00,rm=101 = `[disp32]`)를 인식해
  slot VA(=`insn.disp`)를 레지스트리에서 조회한다 — **바인딩되어 있으면** 네이티브
  셰임을 디스패치하고 EAX 에 반환값을 싣고(Win32 ABI) stdcall 인자를 pop 한 뒤
  실행을 **계속**한다. `__scrt_common_main` security-cookie init 의 첫 호출
  GetCurrentThreadId 가 바인딩되어 0x539274 의 호출이 **실행**되고(insn 10), 다음
  `mov edx,eax`(insn 11)까지 진행한 뒤 **두 번째·미등록 IAT 호출 @`0x53927C`** 에서
  정직하게 HALT(`UNBOUND_IMPORT` — 다음에 바인딩할 import 슬롯 `0x538004` 를 보고).
  hermetic 테스트(`native/i386_cpu_test.c`) Run A 는 합성 프롤로그를 0x539274 까지
  r3 와 byte-동일하게 유지하고 그 뒤에 bound-call/mov/unbound-call 을 이어 붙여
  경계 통과를 증명: **insns 9→11, halt 0x539274→0x53927C**, eax=TID, edx=TID,
  ecx=0xEF(프롤로그 보존), esp=esp0-4(0-arg stdcall 균형). 새 측정 라인
  `__SHIM__ PARTIAL phase=e4_kernel32_iat_bind insns=11 bound=GetCurrentThreadId
  halt_va=0x53927C halt=unbound_import unbound_slot=0x538004` 발신, `__SHIM_TEST__
  PASS`(CI 게이트, 24 checks). decoder(`i386_decode.c`/`.hexa`)는 미변경 → RUNEQ
  영향 없음. **own1**: 이것은 로더가 자기 프로세스의 **자기 import** 를 네이티브
  구현에 묶는 것 — 모든 PE 로더가 하는 **로딩**이지 우회가 아니다. kernel32 는 OS
  API 표면이지 보호장치(DRM/Warden/anti-cheat)가 아니며, 본 프롤로그 호출은 CRT
  init 일 뿐 보호와 무관. `validated_manjeom` 은 여전히 **0**(이것은 로더 진척이지
  렌더된 프레임이 아니다). 다음 r5: 두 번째 import(미등록 slot 0x538004) 바인딩 +
  버퍼-쓰기형 셰임(GetSystemTimeAsFileTime/QueryPerformanceCounter) + 0xC1 shift
  group 디코더 갭.

### Fixed

- test(purple cond.3): 2FA TOTP state machine 의 **window-drift / RFC 6238 §5.2 ±1
  resync leg** gap 종결 — `c_purple_2fa_totp_proxy.hexa` 는 `window_idx`(=unix_time/30)
  를 device 별로 기록하지만, 합성 시나리오는 거의 동일한 window(58000000/58000001)만
  써서 **동일 device 가 clock drift 로 인접 window(N, N+1)에 걸쳐 코드를 생성** 하는
  documented-but-unmodeled 축(RFC 6238 §5.2 — 서버 ±1 resync window 수용)을 한 번도
  행사하지 않았다. 합성 행 `record_totp(5004, 860, 58000010, 6)` +
  `record_totp(5004, 870, 58000011, 6)`(인접 window) 추가로 drift transition 을
  행사하고, 헬퍼 `count_by_window(idx)`(r1 의 `count_by_stage` 거울) +
  `device_window_span(device_id)`(max−min window = ±1 resync 정량) 추가, 의존
  assertion 전부 내부정합 갱신(totp_count 3→5, total_hmac_us 2550→4280, avg_hmac_us
  850→856, valid_code_len 2→4, stats[0..3] 동일 갱신, emit ≥4→≥7) + 새 emit 마커
  `totp_window_drift`(span=1, resync=accepted_pm1) 발신. CI 8파일 중
  `test_purple_lineage_offline_shim.hexa` 에 `check_purple_cond3_totp_drift()`
  추가(grep 마커 + live `out.contains("drift_span=1")`) + `track_b_purple_cond3_totp_drift`
  emit 배선(test emit ≥6→≥7) → 변경이 dead 가 아니라 CI 검증됨. **cond.3 status 는
  partial 유지**(purple_launcher.blk.1 — plaync online endpoint network-gated). own1:
  합성 데이터만, 실제 plaync account/network/TOTP secret 0건, **GameGuard
  blocked-not-bypassed**.
- fix(ci): CI 가 **아무것도 검증하지 않던** 공허 통과를 root-cause 교정 —
  8 테스트 파일은 전부 `fn self_test()` + `main()` dispatch(own2 mandate)이고
  `@test` fn 은 0개인데, CI 가 `hexa test <file>`(=@test 러너)을 써서 0개 실행 →
  무조건 green 이었다. `hexa run <file> self-test`(main→self_test dispatch)로
  교정 → 8 self_test 가 비로소 실제 실행·검증된다. (로컬 실측: `hexa test` 는
  무출력/행, `hexa run self-test` 는 own2 PASS 라인 출력.)
- test(battlenet cond.3): `test_d2r_archive_round_trip` 에
  `check_battlenet_cond_3_oauth()` 추가 — OAuth2 6 op_kind(특히 device_code_grant)
  emit 마커를 skeleton-validate(live + grep fallback). cond.3 deepening 이 CI 8파일
  루프에서 실제 검증되도록 배선(wire-to-prod). cond.3 status 는 partial 유지.
- test(purple cond.3): plaync OAuth state machine 의 **token_refresh-FAIL leg** gap 종결 —
  `c_purple_plaync_oauth_proxy.hexa` self_test 는 stage=3(token_refresh) 을 `success=1`
  (refresh OK) 으로만 행사했으나, header line 6(token_refresh stage) + line 29(success=0
  사유 "token 만료")가 DOCUMENT 한 **refresh_token 만료 → 재로그인(stage=3 success=0)**
  leg 는 MISSING 이었다. 합성 행 `record_oauth(4004, 3, 90000, 0)` 추가로 행사하고
  의존 assertion 전부 내부정합 갱신(oauth_count 4→5, fail_count 1→2, total_us
  740000→830000, count_by_stage(3) 1→2, ppop_stats[2] 740000→830000, emit ≥5→≥7) +
  새 emit 마커 `plaync_oauth_refresh_fail` 발신. CI 8파일 중
  `test_purple_lineage_offline_shim.hexa` 에 `check_purple_cond3_oauth_refresh_fail()`
  추가(grep 마커 + live `out.contains("refresh_fail=1")`) + `track_b_purple_cond3` emit
  배선 → 변경이 dead 가 아니라 CI 검증됨. **cond.3 status 는 partial 유지**
  (purple_launcher.blk.1 — plaync online endpoint network-gated). own1: 합성 데이터만,
  실제 plaync account/network/token 0건, **GameGuard blocked-not-bypassed**.
- test(battlenet cond.3): `test_d2r_archive_round_trip` 에
  `check_battlenet_cond_3_state_machines()` 추가 — cond.3 4개 모듈 중 그동안
  `pe_battle_net_oauth_token` 하나만 CI-pin 되어 있었고, 나머지 3개 perf 모듈의
  synthetic 상태기계 마커(`__BNET_LOGIN__ PARTIAL …` / `__BNET_TOKEN__ PARTIAL …` /
  `__BNET_CHAT__ PARTIAL …`)는 **각 모듈 자체 self_test 안에서만** 실행될 뿐 8 CI
  `[test].files` 어디서도 grep/검증되지 않던 "CI-dead" 갭을 닫음. 세 모듈의 종단
  상태(`TOKEN_EXPIRY` / `REFRESH_FAIL_RETRY` / `OFFLINE`)를 live self_test + grep
  fallback 으로 skeleton-validate → login_state / token_refresh / chat_presence
  상태기계가 CI 에서 실제 검증된다. 각 모듈 self_test 의 문서화된 상태(login 5,
  token 5, chat 4)는 이미 전부 exercise 되어 있어(self_test 축 고갈) 모듈 자체는
  무변경 — 갭은 CI-검증 부재였다. own1: 실 OAuth endpoint 0 / 실 XMPP 0 / socket 0 /
  network 0. cond.3 status 는 partial 유지(network-gated = blk.1, 정직).

### Added

- feat(F-NSWINDOW-E5): i386 **인터프리터** 최초 도입 — 디코드만 하던 E2 를 넘어
  실제 Win32 게임 바이너리 바이트를 **실행**(상태 변이)한다. `native/i386_cpu.{c,h}`
  = 레지스터 파일(`gpr[8]`/eip/eflags, esp=gpr[4]) + 플랫 VA→host 메모리 모델
  (`mem_read32`/`mem_write32`, 경계검사) + fetch→decode→**execute**→advance 루프.
  실행 op: PUSH_R / POP_R / PUSH_IMM / PUSH_RM([reg+disp]) / MOV_R_RM(8B) /
  MOV_RM_R(89) / CALL_REL(E8) / JMP_REL(E9·EB) / RET(C3·C2). 미커버 opcode 는
  `I386_OP_UNKNOWN` 으로 **정직하게 halt**(wall VA 기록). 분기 타깃은 디스어셈블러와
  동일한 `eip+len+imm` 공식 재사용. own1: 프로그램 자기 바이트 + Intel SDM 의미론만,
  Wine/CrossOver/Box86/QEMU/ReactOS 소스 0.
- test(F-NSWINDOW-E5): `native/i386_cpu_test.c` — Battle.net-Setup.exe 의 실제
  9-insn 엔트리 블록(entry_va 0x5388A6) 을 hermetic 플랫 이미지에 박아 CI 에서
  바이너리 없이 실행(파일 인자 주면 실 PE 도 로드·실행). 실측(로컬 clang 빌드,
  19/19 [ok], `__SHIM_TEST__ PASS`): Run A(엔트리에서 실행) → CALL_REL 1개 실행,
  리턴주소 0x5388AB push, eip=실타깃 0x539259 로 분기 후 디코더 커버리지 wall 에서
  halt → `__SHIM__ PARTIAL phase=entry_block_executed insns=1 halted=unknown halt_va=0x539259`.
  Run B(push/mov/push-rm), Run C(pop/pop/ret), Run D(jmp) 가 PUSH_R·MOV_R_RM·
  PUSH_RM·POP_R·RET·JMP_REL 의미론을 실 바이트로 검증. `native/build.sh` 에
  `i386_cpu_test` 타깃 등록(clang arm64 + ad-hoc codesign, 기존 빌드 재사용).
  → **게임 바이너리 바이트 최초 실 EXECUTION 마일스톤**. 다음 wall(r3): VA 0x539259
  (`__scrt_common_main` 프롤로그)의 첫 미커버 opcode — group-1 imm arith(0x83/0x81/
  0x80)·mov r/m imm(0xC7/0xC6)·byte mov(0x88/0x8A)·test(0x84)·and/or(0x09/0x0B/
  0x21/0x23) 디코더 커버리지 확장 대상.
- feat(F-NSWINDOW-E5 r3): r2 의 **1-instruction wall(0x539259) 돌파** — 디코더+
  인터프리터를 `__scrt_common_main` 프롤로그 opcode 집합으로 확장. `native/i386_decode.{c,h}`
  에 group-1 r/m,imm(0x80/0x81/0x83 — add/or/adc/sbb/and/sub/xor/cmp by ModR/M.reg)·
  mov r/m,imm(0xC7 /0 id, 0xC6 /0 ib)·byte mov+test(0x88/0x8A/0x84)·and/or(0x09/0x0B/
  0x21/0x23) 디코드(op enum 32..49 말미 추가 — 기존 0..31 ordinal 불변). `native/i386_cpu.{c,h}`
  에 **EFLAGS 모델**(CF/PF/AF/ZF/SF/OF — SDM ADD/SUB/CMP/logic 정확) + `alu_add`/
  `alu_sub`/`alu_logic` + r/m32 read/write 헬퍼 추가, 실행 op 확장: MOV_R_IMM(B8+)·
  MOV_RM_IMM(C7)·LEA(8D)·INC/DEC r(40+/48+, CF 보존)·ALU r/m,r+r,r/m(ADD/SUB/CMP/
  XOR/OR/AND/TEST)·group-1 imm(81/83). byte-width 형(80/C6/88/8A/84)은 디코드되되
  폭 미모델 → **정직 UNSUPPORTED**. 간접 IAT CALL(FF /2 [disp32])은 디코드(CALL_RM)
  되되 **의도적으로 미실행** = E4 kernel32 경계(own1: IAT 해석/DRM 상호작용 0).
- test(F-NSWINDOW-E5 r3): `native/i386_cpu_test.c` Run A 가 이제 엔트리 CALL 후
  0x539259 의 `__scrt_common_main` 프롤로그(own1: named-family 로 짠 문서화된 SHAPE —
  실 바이너리 부재 호스트, SDM-정확 자기 바이트)를 **8개 추가 실행**하고 첫 간접
  IAT CALL 에서 멈춘다. **실측(로컬 clang, 20/20 [ok], `__SHIM_TEST__ PASS`)**:
  `__SHIM__ PARTIAL phase=scrt_common_main_prologue insns=9 halted=unsupported halt_va=0x539274 halt_op=call`
  — r2 `insns=1 halt@0x539259` → r3 `insns=9 halt@0x539274`(reason=UNSUPPORTED=CALL_RM,
  E4 경계). hermetic sentinel(0x538BAB·0x53872A)은 아직-미커버 0xC1(shift group-2,
  다음 r4 디코더 갭)으로 교체 → Run B/D 는 계속 UNKNOWN wall 에서 정직 halt. .hexa
  트윈(`native/i386_decode.hexa`) 동일 미러 → **RUNEQ M4 1042/1042 + M3 byte-equal
  (로컬 실측 green)**, hexa self_test PASS. own1: 자기 바이트 + Intel SDM 의미론만,
  Wine/QEMU/Box86/ReactOS 0, IAT/DRM/Warden 상호작용 0. `validated_manjeom` 은 여전히
  **0**(실 게임 프레임 아님 — E3 인터프리터 전진이지 프레임 증거 아님). 다음 wall(r4):
  shift group-2(0xC1/0xD1/0xD3)·group-3(0xF7 test/neg/mul/div) 디코더 확장, 또는
  첫 조건분기(Jcc) EFLAGS 소비, 또는 E4 = 간접 IAT CALL 실행(kernel32 stub 바인딩) 경계.
- feat(battlenet cond.3): `pe_battle_net_oauth_token` self_test 가 op_kind 6
  `device_code_grant`(`/oauth/device` RFC8628) 을 비로소 exercise — 헤더에
  문서화됐으나 self_test 미커버였던 갭을 닫음(6 op_kind 전부 round-trip). 합성
  데이터·token 실값 0·length-only·실 네트워크 0(own1 준수). 어서션 정합:
  op_count 6→7, total_token_lifetime_sec 345600→432000, total_token_size_bytes
  5120→6144, count_by_op_kind(6) 0→1, stats `[7,432000,6144,2]`, emit ≥8.
  cond.3 status 는 partial 유지(실 OAuth 엔드포인트 network smoke 게이트 = blk.1).

- docs(CLAUDE.md): CI(Blacksmith) 규율 박제 — `## CI — builds run on Blacksmith,
  NOT locally` 섹션 추가(워크플로/러너/툴체인/실행 내용 + "로컬 빌드 말고 push
  하라" 규칙) + Structure 트리에 `.github/workflows/` 항목. 같은 결함(로컬 빌드로
  Mac 죽음) 재발 방지를 위해 거버넌스 SSOT 에 명시.


- ci(blacksmith): `.github/workflows/ci.yml` — gamebox 빌드+테스트를
  Blacksmith 클라우드 Apple-Silicon 러너(`blacksmith-6vcpu-macos-15`)에서 실행.
  gamebox 는 Apple-native(Win32 PE + D3DMetal)라 컴파일+apple-only 테스트에
  macOS 호스트가 필요한데, 그걸 로컬 dev Mac 에서 돌리면 242+ perf 모듈 +
  8 테스트가 메모리를 스파이크해 머신이 죽었다. 이제 push 마다 클라우드 Mac 이
  빌드/테스트를 맡고 로컬 박스는 `git push` 만 한다. 러너는 macos-15 핀
  (hexa-lang `release.yml` 와 동일 근거 — SDK min-OS 베이스라인 안정). hexa
  툴체인은 릴리스 바이너리를 canonical 설치 한 줄(`install.sh`)로 깐다. D3DMetal
  SDK 부재는 warn-only(skeleton-tier 로더는 importable). same-org 라
  Blacksmith 활성화 별도 작업 불필요(hexa-lang 이 이미 사용).

### Changed

- docs(architecture): restructure `ARCHITECTURE.json` into a deeper `children`
  tree per harness governance rule c4 — a LOSSLESS re-shape (restructure only,
  no content change). Decomposed 10 over-long dump cells (root `summary` 527c +
  root `note` 731c + 8 node `summary` fields 263–531c) that piled multiple
  distinct facts / ` · `-joined lists into one field. Each piled fact is now its
  own child node (short role line stays on the parent `summary`; each remaining
  sentence/list becomes a `{name, summary}` `ℹ️` info child). Node count
  26 → 47. The two C-PORT milestones >250c are single coherent done-records
  (`milestones[]` is already the per-item list) and stay intact — not dumps.
  Losslessness verified by non-whitespace char-multiset before vs after =
  identical (0 lost / 0 added; only scaffolding node-name labels added).
  `ARCHITECTURE.html` viewer + `python3 serve.py` stay renderable (generic over
  the `children` tree; new nodes use only `name`+`summary`).

- chore(architecture): retire `DOMAINS.tape` + `domains/` (22 domain `.md`
  snapshots + 22 `.log.md` logs) into a single `ARCHITECTURE.json` tree SSOT
  (hexa-codex/anima pattern), with the `ARCHITECTURE.html` viewer +
  `python3 serve.py` static server (copied + adapted from `dancinlab/anima`).
  Each domain's `@goal` → node `summary`, its `- [ ]/[x]` milestones →
  `milestones[]`, and the DOMAINS.tape `@link` edges → per-node `edges[]`
  (`--has-->` tree + `--uses-->`/`--targets-->` cross-edges preserved). Tree =
  22 domains (1 meta-root GAMEBOX + 6 메인구조 LOADER/WIN32/GFX/NET/PERF/NATIVE +
  C-PORT nested under NATIVE + 2 런처 BNET/PURPLE + 1 sprint-meta LAUNCHPAD+ +
  11 게임), grouped under 4 section headers → 26 JSON nodes. `ARCHITECTURE.md`
  stays as the component-level prose SSOT and now points to the JSON tree.
  `native/i386_decode.hexa` C-PORT cross-link repointed from `domains/C-PORT.md`
  → the C-PORT node in `ARCHITECTURE.json` (provenance string left intact).
  Full per-domain `.log.md` history is recoverable via git log; the substantive
  entries folded below.

  Domain-log digest (folded from `domains/*.log.md`):
  - All 22 domains scaffolded 2026-05-31 (domain init, seeded from the gamebox
    전수조사 — @goal + milestones from `.roadmap.*` + repo survey of
    lib/loader · lib/perf · native).
  - **C-PORT** (2026-06-03): M1 inventory+classify (4 native/*.c, 869 LOC;
    Tier-A 437 · Tier-B 113 · Tier-C 319) → M2 decode_modrm_disp port
    (RUNEQ 768/768, sha256 b4388479) → M3 decode_prefixes + i386_op_name +
    rd_s32/rd_s16 (RUNEQ 1069/1069, sha256 8e6acbb8) → M4 stateful
    i386_decode_one (RUNEQ 1042/1042, sha256 b2853d0a). Both M2 and M4 caught a
    real write-before-fail out-param divergence, fixed → byte-identical.
  - **GAMEBOX-D2R**: prerequisite @uses edges wired (LOADER/WIN32/GFX/NET +
    BNET); reconciled 3/4 — D3D11/MPQ/D2S code present + offline/online
    checkpoints landed; gameplay-tracks (26 c_d2r_*) left open.
  - **GAMEBOX-BNET**: reconciled 5/6 — 10 pe_battle(_)net_* + orchestrator
    present, D2R offline+online checkpoints prove the path; D2R non-blocking;
    D4/CS2 launch extension open.
  - **GAMEBOX-PURPLE**: reconciled 4/4 — 7 c_purple_* + Lineage M/W purple play
    checkpoints; scope = M/W; Classic depends on PURPLE for download only.
  - **GAMEBOX-LINEAGE-CLASSIC**: wired LOADER/WIN32/GFX @uses; reconciled 3/4
    (r28 loader + native first-frame + play checkpoint + drift-resolution track
    bv); two corrections recorded — PURPLE is the download channel (runtime
    native, "no Purple shell"), and Classic is an ONLINE MMO (not offline);
    playable gate = GameGuard (INCA) kernel AC honest-freeze.
  - **GAMEBOX-LAUNCHPAD+**: meta-domain init (`+` composition) targeting D2R +
    LINEAGE-CLASSIC; @has + @targets edges wired; shared "playable" smoke bar
    (boot→title→char→in-world) defined with online/anti-cheat honest-freeze.

### Removed

- `DOMAINS.tape` (domain roster + tree graph) — superseded by
  `ARCHITECTURE.json` (`meta` + per-node `edges[]`).
- `domains/` (22 `*.md` domain snapshots + 22 `*.log.md` logs) — `*.md` folded
  into `ARCHITECTURE.json` nodes; `*.log.md` digested into the entry above
  (full text recoverable via git log).

- chore(harness): perfect harness setup — engine submodule bumped to latest on
  `harness-hardcore`; authored `ARCHITECTURE.md` SSOT (overview + component map +
  data flow + governance/verify, English) over the stub; authored harness-standard
  `CLAUDE.md` (H1 + blurb + `## Structure` tree + governance + `## Harness` +
  quick reference) over the SPECKIT stub; added a `docs` discipline block to
  `harness.config.json` (`architecture`/`log`/`scratchDir` + `scopeDirs: [""]`
  root-only + `allow` list covering README/CHANGELOG/ARCHITECTURE/CLAUDE/AGENTS
  plus `RELEASE_NOTES_v1.0.*` and `TAPE-AUDIT.md`). `harness docs check` now
  reports `docs: ok` with 0 CLAUDE-MD violations. `.claude/settings.json` already
  carried the full guarded harness hook set (pre bash / pre write / post edit /
  prompt / prefs · easy · recommend inject / SessionStart) — preserved as-is.

## [1.0.4] — 2026-05-05

### Added

- Track CK — `docs/VAC_HONEST_FREEZE.md` mk1 narrative (Valve Anti-Cheat user-mode AC + Trusted Mode; resolves cs2.cond.3 deferred hook).
- Track CL — `docs/EAC_KERNEL_HONEST_FREEZE.md` mk1 narrative (EAC kernel + anti-tamper class; 3-cross-class delta_force + elden_ring base+DLC + nightreign).
- Track CM — Nightreign lib module fan-out at skeleton-tier:
  - `lib/loader/pe_elden_ring_nightreign_specific_track.hexa` (216 LOC)
  - `lib/perf/c_nightreign_co_op_session_init.hexa` (133 LOC)
  - `lib/perf/c_nightreign_matchmaking_probe.hexa` (129 LOC)
  - `lib/perf/c_nightreign_dx12_first_frame_co_op_variant.hexa` (131 LOC)
  - Total 609 LOC; retired_intentional posture inherited from blk.1 + blk.2.
- Track CN — `gamebox/doc/gamebox_hexa_interp_closure_perpetual_deferral_ack_track_cn_landed_2026_05_05.ai.md` (formally closes CJ §11 hook 4 as perpetually deferred; multi-year horizon).
- 4 NEW `gamebox_*` markers (CK + CL + CM + CN).
- 4 NEW `gamebox/doc/` handoff docs (CK + CL + CM + CN).

### Changed

- `hexa.toml` version: `1.0.3` → `1.0.4`.
- `README.md` Version badge: `version-1.0.3` → `version-1.0.4`.
- `.roadmap.elden_ring_nightreign`: `loader_count: 0 → 1`, `perf_count: 0 → 3`, NEW `loader_pattern` + `perf_pattern` + `ai_native_handoff_track_cm` fields, NEW comment header line 6 documenting Track CM skeleton-tier fan-out (Track BY documentation-only narrative line 5 preserved verbatim).

### Cumulative deltas (v1.0.3 → v1.0.4)

- gamebox-native markers: 15 → 19
- Honest freeze docs: 3 → 5 (Track-E predecessor + CK + CL)
- lib/loader files: 455 → 456 (+1 nightreign loader)
- lib/perf files: 242 → 245 (+3 nightreign perf)
- Forensic verdict: UNCHANGED (`SKELETON_TIER_100PCT_WIN32_VALIDATED_MANJEOM_0`)
- validated_manjeom: UNCHANGED (0 — skeleton-tier per Track BL/BM forensic baseline)

### Posture

- own1: Wine 0 / hexa-only / no DRM bypass / no AC bypass (5 classes covered: VAC + GameGuard + EAC kernel + EAC anti-tamper + Warden) / no FromSoft matchmaking bypass / no anima clone / `destructive_ops: 0` (maintained).
- own2: 4 CM modules emit RETIRED_INTENTIONAL posture annotation; CN explicit no-fake-PASS / no-fake-real-run-completion declaration; skeleton-tier vs real-tier distinction explicit; spec freeze ONLY scope explicit per CK + CL.

## [1.0.3] — 2026-05-05

### Added

- Track CE — `gamebox/doc/gamebox_closure_orchestrator_selftest_paper_mode_track_ce_landed_2026_05_05.ai.md` (S1-S11 paper-mode trace; BZ §11 hook partial resolution).
- Track CF — `gamebox/doc/gamebox_hexa_toml_modules_audit_track_cf_landed_2026_05_05.ai.md` (40 entries audit; 1 drift item flagged for CG).
- Track CG — `gamebox/doc/gamebox_c_d2r_archive_round_trip_drift_fix_track_cg_landed_2026_05_05.ai.md` (orphan reference option-b removal).
- Track CH — `gamebox/doc/gamebox_session_aggregate_v3_track_ch_landed_2026_05_05.ai.md` (first standalone-repo session aggregate covering BT-CG).
- hexa.toml `gamebox_perf_top_families` explanatory comment block (CG).
- 4 NEW `gamebox_*` markers (CE / CF / CG / CH).
- `.gitignore` pattern: `resource_op_[0-9]*.marker` (CE post-mortem).

### Changed

- `hexa.toml` version: `1.0.2` → `1.0.3`.
- `hexa.toml` `gamebox_perf_top_families`: orphan `c_d2r_archive_round_trip` entry removed (CG).
- `README.md` Version badge: `version-1.0.2` → `version-1.0.3`.
- `README.md` perf families table: `c_d2r_archive_round_trip` row removed (CG).

### Cumulative deltas (v1.0.2 → v1.0.3)

- gamebox-native markers: 9 → 15 (BT-CH)
- RFC 016 audit cumulative null floor: 28 → 34 cycles
- hexa.toml audit drift items: 1 → 0
- Forensic verdict: UNCHANGED (`SKELETON_TIER_100PCT_WIN32_VALIDATED_MANJEOM_0`)
- validated_manjeom: UNCHANGED (0)

### Posture

- own1: Wine 0 / hexa-only / no DRM / no AC / no anima clone / `destructive_ops: 0` (CG was reference-removal not file-deletion).
- own2: paper-mode vs real-run distinction explicit; drift fix option chosen documented; v3 vs v1/v2 distinction explicit.

## [1.0.2] — 2026-05-05

### Added

- **Layout migration** (`1d28b4c refactor(layout 2026-05-05)`): canonical `<feature>/doc/` layout — `airgenome/doc/` (75 predecessor handoffs) + `gamebox/doc/` (9 native handoffs incl. CB-CC).
- Track BX — per_game README §9 sections for 6 offline-relevant files (lineage_classic_r28 + d2r + battlenet + lineage_w + lineage_m + purple_launcher).
- Track BY — README Game Coverage refresh: wow row removed (historical preserve), elden_ring DLC scope clarified (Shadow of the Erdtree 2024-06-21), elden_ring_nightreign added (2025-05-30 standalone multiplayer co-op spinoff, documentation-only fan-out).
- NEW `.roadmap.elden_ring_nightreign` (documentation-only, 0 lib modules at land-time).
- NEW `docs/per_game/elden_ring_nightreign_README.ai.md` T1 baseline.
- `dlc_scope` + `spinoff_cross_link` + `ai_native_handoff_track_by` fields on `.roadmap.elden_ring`.
- Track BZ — `gamebox/doc/gamebox_closure_orchestrator_paper_invocation_track_bz_landed_2026_05_05.ai.md` (real-invocation attempt + paper-mode derivation; hexa interp blocker).
- Track CA — `gamebox/doc/gamebox_hexa_upstream_rfc_016_reassessment_track_ca_landed_2026_05_05.ai.md` + `docs/HEXA_UPSTREAM_RFC_016_REASSESSMENT_TRACK_CA_2026_05_05.md` (28-cycle null floor reaffirmed).
- Track CB — README "Native markers" + "Resolver telemetry markers (gitignored)" subsections.
- Track CC — per_game README §9 sections for 4 retired_intentional files (cs2 + delta_force + diablo4 + elden_ring); elden_ring §9 surfaces DLC scope narrative.
- `.gitignore` patterns for hexa-resolver runtime telemetry markers.
- 8 NEW `gamebox_*` markers post-v1.0.0 (BT through CC).

### Changed

- README.md TL;DR header: `.roadmap` 14 → 15 (14 active + 1 historical), `.checkpoint` met 1 → 3.
- README.md architecture caption: terse 7-figure → detailed inventory (455 .hexa files, 9 tests, 88 markers across 3 classes).
- README.md Loaders heading: `(35 modules)` → `(59 named groups across 455 .hexa files)`.
- `.roadmap.elden_ring` goal: scope = base game + Shadow of the Erdtree DLC.
- `docs/airgenome_*.ai.md` (75) → `airgenome/doc/` per layout migration.
- `docs/gamebox_*.ai.md` (7) → `gamebox/doc/` per layout migration.
- `state/markers/*.marker` cross-references updated (193 files / 621 lines via sed sweep in `1d28b4c`).
- `hexa.toml` version: `1.0.1` → `1.0.2`.
- README version badge: `version-1.0.1` → `version-1.0.2`.

### Posture

- Forensic verdict UNCHANGED: `SKELETON_TIER_100PCT_WIN32_VALIDATED_MANJEOM_0`.
- `validated_manjeom` UNCHANGED: 0 (no real game frames).
- own1: Wine 0 / hexa-only / no DRM / no AC / no GameGuard / no EAC / no FromSoft matchmaking bypass / no anima clone / `destructive_ops: 0`.
- own2: honest emit; per_game §9 sections explicit about retired_intentional stable + cross-cutting impact + validated_manjeom carry.

## [1.0.1] — 2026-05-05

### Added

- Track BU — README "Game Coverage" section (+40 LOC; 11 .roadmap targets table + per-game index + 7 cross-roadmap checkpoint table + honest scope footer).
- Track BU — `met_iso` / `met_via` / `evidence[]` fields on `.checkpoint.win32_50pct_coverage` + `.checkpoint.win32_full_coverage`.
- Track BV — `met_iso` / `met_via` / `evidence[]` fields on `.checkpoint.lineage_classic_r28_play`.
- Track BV — 9 NEW `status_flipped_*` fields across `.roadmap.lineage_classic_r28` cond.1/cond.2/cond.3.
- 4 NEW handoff docs + markers (BU + BV).

### Changed

- `.checkpoint.win32_50pct_coverage` status: `unmet 30.34/50` → `met 100/50` (BU).
- `.checkpoint.win32_full_coverage` status: `unmet 30.34/100` → `met 100/100` (BU).
- `.checkpoint.lineage_classic_r28_play` status: `partial 0/3` → `met 3/3` (BV, sideways-PASS).
- `.roadmap.lineage_classic_r28` cond.1/cond.2/cond.3 actual status: `partial` → `retired_intentional` (BV cascading flip).
- `hexa.toml` version: `1.0.0` → `1.0.1`.
- 3 checkpoint `blockers` arrays renamed `blockers_resolved` (verbatim list preserved).

### Drift resolution

- Track BO snapshot v2 drift_count: 3 → 0 (all 3 items resolved across BU + BV).

### Posture

- Forensic verdict UNCHANGED: `SKELETON_TIER_100PCT_WIN32_VALIDATED_MANJEOM_0`.
- `validated_manjeom` UNCHANGED: 0 (no real game frames evidenced).
- own1: Wine 0 / hexa-only / DRM bypass 0 / Warden bypass 0 / anti-cheat bypass 0 / GameGuard bypass 0 / anima clone 0.
- own2: PASS/FAIL/PARTIAL/RETIRED_INTENTIONAL emit honest; status flip rationale per checkpoint cited.

## [1.0.0] — 2026-05-05

### Initial extraction

- **Forensic verdict**: `SKELETON_TIER_100PCT_WIN32_VALIDATED_MANJEOM_0`
- **Source**: `dancinlife/airgenome-gamebox` 67-cycle session (2026-05-03 to 2026-05-05; deleted 2026-05-05 per closure-cycle directive, history not transferred per D3 fresh-start policy)
- **Reference layout**: `dancinlab/sim-universe` v1.0.0 (sibling repo)

### Inventory

- 35 loader modules under `lib/loader/` (32 PE *_core + 5 runtime + 5 DX + 9 Battle.net + 6 calling-conv + 2 launchers; some overlap)
  - 435/435 Win32 fn skeleton coverage (Track BP, 2026-05-05)
- 242 perf modules under `lib/perf/` across 104 unique families
  - 18+ families at 100% manjeom (Track BL/BM forensic)
- 14 `.roadmap.*` files (mk2 SSOT)
- 7 `.checkpoint.*` files (mk2 checkpoint mechanism); 1 met: `win32_50pct_coverage` (Track BR derivation guard)
- 2 tools under `tool/` (`roadmap_op.hexa` 14-subcmd anima parity broken intentional + `closure_orchestrator.hexa` weighted_threshold awareness)
- 8 tests under `tests/`
- 75 predecessor markers under `state/markers/airgenome_gamebox_*.marker` (preserved verbatim per own2 traceability)
- 4 hexa-lang upstream RFCs merged (rfc_012/_013/_014 PR #47 + rfc_015 PR #48)

### Posture

- **own1**: Wine 0 / hexa-only / DRM bypass 0 / Warden bypass 0 / anti-cheat bypass 0 / anima clone 0
- **own2**: PASS/FAIL emit honest, PARTIAL/RETIRED_INTENTIONAL emit explicit
- **L1-L5 online expansion lock-in carry** (EULA / Warden / multi-year / D2R-single / parallel) per `.roadmap.battlenet`
- **License**: Apache-2.0
