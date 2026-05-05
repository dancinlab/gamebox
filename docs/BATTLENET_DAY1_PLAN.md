# BATTLENET_DAY1_PLAN — first NSWindow → install → launcher day-by-day plan

**작성**: 2026-05-01 · base commit `0a24e22` (shim wave6 — 8360 inst stable)
**목표**: Battle.net Setup 의 **첫 NSWindow dialog 실 표시** 도달 + 그 이후 install
완료 → launcher 실행 → 게임 launch 까지의 day-by-day actionable steps.
**가정**: own1 §A enforcement 100% 유지 — Wine / CrossOver / GPTk / Whisky / Box86 /
QEMU / Bochs / Capstone / Zydis / iced / community 0줄. Apple SDK + 자체 코드 only.

---

## Day 0 — 현재 status (cycle 103, 2026-05-01)

| 항목 | 값 |
|---|---|
| inst 진척 | **8360** (wave6 8-bit MOV/arith expand + fake_heap + 0F escape modrm 통합 후 stable) |
| 차단 위치 | USER32 RegisterClassExA / CreateWindowExA **미도달** |
| 마지막 halt 분류 | wave6 통합 후 더 이상 ret_to_unmapped 자연 halt 부재, 단 진척이 8360 에서 plateau (run-by-run) |
| KERNEL32 stub hits | 7-9 hit (V6 시점) → wave5 후 추가 hit (CRT init) |
| GetProcAddress dispatch | 부분 통합 — fake addr 발급 OK, 단 stdcall arg unwind 만 처리, dispatch back 미통합 |
| USER32/GDI32 IPC bridge | shim wiring 완료 (kind=14/15), helper.mm NSWindow real alloc 완료, **호출 도달 0건** |
| helper.mm | NSEvent → Win32 MSG 변환 + DefWindowProcA + keymap 자체 표 완료 |
| own1 §A 정합 | 100% — Wine 등 lift 0줄 (검증 완료, V6 §8) |

**핵심 diagnose**: 8360 inst plateau — GetProcAddress 가 반환한 fake fnptr 이
indirect call 시점에 dispatch back 되지 않아 단순 `c3 ret` 로 처리되며 caller stack
이 corrupt → CRT init 깊은 영역 loop. wave6 의 진척은 opcode coverage 확장 결과로
의미 있는 진보 X.

---

## Day 1 — R3-12 첫 NSWindow 도달 trigger

**목표**: shim 측 GetProcAddress dispatch back + 0F XADD/CMOVcc/SETcc 추가 →
CRT init 통과 → WinMain 진입 → RegisterClassExA → CreateWindowExA → **실 NSWindow
표시**. 진척 8000 → 20000+ inst 예상.

### 1.1 shim 측 GetProcAddress dispatch back (4-byte stub method)

`native/pe_to_macho_shim.cpp`:

1. 신규 글로벌 `g_dispatch_addr_table[512]` — `{ addr, dll, fn, arg_count }`
2. 4-byte stub area mmap. 각 entry: `83 c4 NN c3` = `add esp, NN; ret` (NN = arg_count × 4)
3. `GetProcAddress(hmod, name)` stub 강화: dispatch table lookup → arg_count 결정,
   g_dispatch_addr_table 등록 + unique offset (0x40001000 + idx*4) 반환
4. phase 17 의 `0xFF /2 CALL r/m32` mod==11 처리: target addr 가 dispatch table 안이면
   해당 dll/fn stub_dispatch 호출, args read, fake EAX, ESP += arg_count × 4, EIP = ret_addr

### 1.2 0F escape opcode 추가 — XADD / CMOVcc / SETcc

phase 17 의 `0x0F` escape 분기:

| opcode | mnemonic | 처리 |
|---|---|---|
| 0x0F C0 / C1 | XADD r/m, r | atomic exchange-add, 실 mem swap + 합산 |
| 0x0F 40-4F | CMOVcc r32, r/m32 | flag 조건 만족 시 mov, 아니면 no-op |
| 0x0F 90-9F | SETcc r/m8 | flag 조건 만족 시 dest=1, 아니면 0 |

CRT runtime init 영역 자주 출현 — cookie 비교 → SETcc, atomic counter → XADD,
env ptr null check → CMOVcc.

### 1.3 진척 expectations

- inst[8360] plateau 해소 → CRT init `_initterm` 통과 → `__scrt_initialize_crt` →
  `_get_initial_narrow_environment` (fake env ptr 반환) → `_get_narrow_winmain_command_line`
- inst[15000+] WinMain 진입 (`Battle.net Setup.exe` lpCmdLine 받음)
- inst[18000+] RegisterClassExA → atom 반환 (fake 0x10001)
- inst[20000+] CreateWindowExA → helper IPC kind=14 fn_id=2 → **NSWindow alloc**
- inst[22000+] ShowWindow / UpdateWindow → makeKeyAndOrderFront → **macOS Dock 에 Battle.net Setup 아이콘 등장**
- inst[23000+] GetMessageA loop entry → block until first NSEvent

**Day 1 마일스톤**: macOS 화면에 NSWindow 1개 표시. content 는 빈 상태일 수 있음
(WM_PAINT 미처리 시).

---

## Day 2-3 — 사용자 클릭 도달 (message loop + WM_PAINT + WndProc)

**목표**: 사용자가 첫 dialog 의 "Continue" / "Install" 버튼을 클릭하고 그 클릭이
i386 PE32 코드 안의 wndproc 함수까지 dispatch 됨.

### 2.1 GetMessageA → NSEvent loop block

IPC kind=14 fn_id=5: worker → main queue dispatch (NSApplication.run 은 main only),
next NSEvent dequeue, MSG struct marshal `{ hwnd, message, wParam, lParam, time, pt }`,
shim 측 MSG buffer 에 write.

### 2.2 DispatchMessageA → wndproc 호출 (phase 17 재진입)

i386 측 GetMessageA loop:
```
while (GetMessageA(&msg, NULL, 0, 0)) {
    TranslateMessage(&msg);
    DispatchMessageA(&msg);  // ← 핵심
}
```

DispatchMessageA 처리:
1. shim 측 IAT thunk → helper IPC kind=14 fn_id=6
2. helper 측: window class map 에서 hwnd 의 wndproc 함수 ptr 조회
3. helper 가 shim 으로 reverse-IPC: "wndproc_invoke fn_ptr=0x401234, hwnd, msg, wparam, lparam"
4. shim 측 phase 17 interpreter 재진입 — `eip = wndproc_fn_ptr`, args push (4 push), fake ret addr push
5. wndproc 함수 실행 → ret 시 EAX 값을 helper 에 IPC reply 로 회신
6. helper 가 main thread 의 GetMessageA 측 caller 에게 응답 return

### 2.3 WM_LBUTTONDOWN 처리

NSLeftMouseDown 이벤트:
- helper.mm 의 keymap 자체 표 → WM_LBUTTONDOWN + (x,y) lParam
- DispatchMessageA → wndproc(hwnd, WM_LBUTTONDOWN, 0, MAKELPARAM(x,y))
- wndproc 안 button hit-test (x,y 가 "Continue" 버튼 영역에 있는지) → 다음 dialog 표시 또는 download 시작

**Day 2-3 마일스톤**: 사용자 클릭 → wndproc 도달 → next dialog 또는 TLS 시작.
inst 100K+ 예상.

---

## Day 4-5 — Battle.net 서버 접속 (TLS / HTTP)

**목표**: Battle.net Setup 이 update server 에서 file list / installer payload 을
download. 사용자 click → first byte 받음.

### 4.1 WININET / WINHTTP IAT thunk → helper IPC kind=16 (Network framework)

자주 호출되는 함수:
- `WINHTTP!WinHttpOpen / WinHttpConnect / WinHttpOpenRequest / WinHttpSendRequest / WinHttpReceiveResponse / WinHttpQueryDataAvailable / WinHttpReadData`
- `WININET!InternetOpenA / InternetConnectA / HttpOpenRequestA / HttpSendRequestA / InternetReadFile`

shim 측 `dispatch_iat_thunk`:
```cpp
} else if (strcmp(e->dll, "WINHTTP") == 0 || strcmp(e->dll, "WININET") == 0) {
    network_ipc_kind16_dispatch(e->func, esp_at_call);
}
```

helper.mm 의 IPC kind=16 (신규):
- Apple `Network.framework` (`nw_connection_t` / `nw_endpoint_t` / `nw_parameters_t`)
- TLS handshake → Apple BoringSSL 통합 (Network framework 자동)
- HTTP request 자체 작성 (Wine wininet/winhttp lift 0줄)
- response chunk 를 fake handle 별 buffer 에 누적

### 4.2 own1 정합

- Apple Network framework 사용 — 정합
- OpenSSL 직접 link X (Network framework 가 BoringSSL 내장 사용)
- HTTP parser 자체 작성 (HTTP/1.1 chunked + content-length)
- TLS cert pin 은 macOS keychain 신뢰

### 4.3 TLS handshake / HTTP GET 마일스톤

- inst[200,000+] 첫 WinHttpSendRequest → server connect 성공
- inst[250,000+] WinHttpReceiveResponse → status 200 + content-length 받음
- inst[300,000+] InternetReadFile loop → 첫 chunk file extract layer 로 전달

**Day 4-5 마일스톤**: macOS Activity Monitor 에서 entry 프로세스의 outbound TCP
connection 이 `*.battle.net:443` 으로 표시됨. install progress bar 진척.

---

## Day 6-7 — File extract + install

**목표**: download 한 압축 파일을 fake C: drive 에 unzip → install 완료.

### 6.1 CreateFileA → fake C: drive

shim KERNEL32 stub 강화:
- `CreateFileA(C:\Program Files\Battle.net\...)` → path translate:
  `~/.airgenome/gamebox/c_drive/Program Files/Battle.net/...`
- 실 macOS open(2) → fd, fake HANDLE = (fd | 0x80000000) wrap
- WriteFile / ReadFile / CloseHandle 도 동일 fake HANDLE 처리
- SetFilePointer / FlushFileBuffers / GetFileAttributesExA 추가

path translation 표:
| Windows path | macOS path |
|---|---|
| `C:\Program Files\Battle.net\` | `~/.airgenome/gamebox/c_drive/Program Files/Battle.net/` |
| `C:\Users\Default\AppData\Local\` | `~/.airgenome/gamebox/c_drive/Users/Default/AppData/Local/` |
| `C:\Windows\System32\` | `~/.airgenome/gamebox/c_drive/Windows/System32/` |

### 6.2 libcompression.dylib 으로 unzip

Battle.net Setup 의 압축 포맷 = MPQ archive 또는 자체 포맷. zlib / lzma / xz 등의
표준 알고리즘이라면:
- Apple `libcompression.dylib` (`compression_decode_buffer` / `COMPRESSION_ZLIB` /
  `COMPRESSION_LZMA`) 직접 사용 — own1 정합 (Apple SDK)
- MPQ 같은 Blizzard 특수 포맷이면 Microsoft 의 spec 공개 문서 reference 로 자체
  parser 작성

### 6.3 fake registry simulation

`RegOpenKeyExA / RegSetValueEx / RegQueryValueEx / RegCloseKey` IAT thunk →
in-memory hash + 백업 `~/.airgenome/gamebox/fake_registry.tsv`. key 형태:
`\HKLM\SOFTWARE\Blizzard\...\Value\TYPE\Data`.

### 6.4 install 완료 마일스톤

- inst[1M+] 첫 file extract 완료
- inst[10M+] 모든 install 파일 unzip + write 완료
- Battle.net Setup window 의 final dialog "Install Complete. Launch Battle.net?" 표시
- 사용자 "Launch" click → Battle.net Setup 자체는 종료 + Battle.net.exe spawn
  요청 (CreateProcessA)

**Day 6-7 마일스톤**: `~/.airgenome/gamebox/c_drive/Program Files/Battle.net/`
디렉토리에 launcher binary + asset 파일 모두 존재. install complete dialog 표시.

---

## Day 8+ — Launcher 실행 (Battle.net.exe + OAuth + 게임 launch)

**시점**: Battle.net Setup 완료 후. Battle.net.exe (실제 launcher) 가 install 된
바이너리. 더 큰 PE32 — multi-thread / D3D11 / websocket / OAuth.

### 8.1 CreateProcessA recursion

`CreateProcessA("Battle.net.exe")` → entry tool 재귀 spawn
(`./entry launch <path>`). 부모는 fake handle 반환 + WaitForSingleObject 시 child
종료까지 block.

### 8.2 launcher 본체

| layer | 처리 |
|---|---|
| multi-thread | CreateThread → pthread_create wrap (TEB per-thread alloc + i386 reg state per-thread) |
| D3D11 graphics | DXGI swap chain → Apple Metal CAMetalLayer + command buffer (helper.mm 의 d3dmetal probe 활용, GPTk 사용 X — own1 거부) |
| websocket | WININET WSA* → Apple Network framework `nw_connection_t` upgrade |
| agent IPC | Battle.net agent (named pipe) → macOS unix domain socket 또는 mach port |

### 8.3 OAuth login

- launcher 가 OAuth challenge URL 을 default browser 로 redirect 요청
  (`ShellExecuteA "open" url`) → helper.mm `[NSWorkspace openURL:url]`
- 사용자가 Safari 에서 login 후 redirect URL `airgenome://oauth/callback?code=...`
- 자체 URL scheme 등록 (Info.plist) → entry 가 callback 받음 → launcher 측 callback ptr 호출

### 8.4 게임 launch

D2R / D4 / Overwatch click → `CreateProcessA("D2R.exe")` 다시 entry recursion →
D3D11 → Metal 변환 + 실 게임 화면. own1: Apple D3DMetal binary 만 OK, Wine fork
(GPTk / Whisky / CrossOver) 거부.

---

## 7. own1 정합 enforcement — 매일 재확인

매 day 종료 시 자동 검증 (`./tool/own1_check.sh` 또는 동등):

| 검증 항목 | 명령 | 기대 |
|---|---|---|
| Wine / CrossOver lift | `grep -r -i "wine\|crossover" native/ lib/` | 0 hit |
| GPTK / Whisky lift | `grep -r -i "gptk\|whisky\|game.porting.toolkit" native/ lib/` | 0 hit |
| Box86 / QEMU / Bochs | `grep -r -i "box86\|qemu\|bochs" native/ lib/` | 0 hit |
| Capstone / Zydis / iced | `grep -r -i "capstone\|zydis\|^iced" native/ lib/` | 0 hit |
| MSVCRT source lift | `grep -r -i "ucrt-source\|msvcrt-source" native/ lib/` | 0 hit |
| Wine USER32 lift | `grep -r -i "winex11\|winecocoa\|wineqcoa" native/ lib/` | 0 hit |
| 외부 fpu emul | `grep -r -i "softfloat\|berkeley.softfloat" native/ lib/` | 0 hit |

검증 통과 X 시 즉시 해당 commit revert + 재작업.

또한 own #1 — 사용자 응답에서 trigger / game-changer / fix 로 외부 Wine fork 추천
표현 0건 (enforcement 맥락에서의 거부 언급만 OK).

---

## 8. 위험 요소 + 회피 전략

| 위험 | 영향 | 회피 |
|---|---|---|
| Battle.net Setup 의 cert pin / TLS extra check | TLS handshake 통과 후 application-layer 추가 검증 실패 | macOS keychain 의 root CA 신뢰 + Network framework 의 default behavior 활용 |
| MPQ / 자체 압축 포맷 | libcompression 으로 처리 불가 | Microsoft / public spec 기반 자체 parser 작성 (Wine MPQ 코드 lift X) |
| D3D11 → Metal 변환 의 shader cross-compile | shader code 가 HLSL → MSL 변환 필요 | 자체 SPIRV-Cross 류 파서 작성 또는 Apple D3DMetal binary 활용 (단 GPTk 거부) |
| WinSock TCP listen | inbound connection 이 firewall 막힘 | macOS Network framework `nw_listener_t` 사용, App Sandbox 권한 처리 |
| 32-bit address space 4GB cap | install 파일 mmap 시 부족 | streaming read + chunk-by-chunk 처리 (전체 file mmap X) |
| OAuth redirect URI scheme | Safari 가 callback URL 처리 안 함 | Info.plist 의 CFBundleURLSchemes 에 `airgenome` 등록 |
| multi-thread (CreateThread) | i386 register state per-thread + TEB 분리 필요 | TEB pool alloc + thread context switch shim phase 24 신설 |
| run-by-run 변동 | inst 수치 plateau 진단 어려움 | RDTSC 응답을 deterministic counter 로 변경 (mach_absolute_time 기반 단조) |
| Battle.net agent (외부 process) | 별도 PE32 spawn 필요 | CreateProcessA recursion + entry tool 의 자체 재진입 |

---

## 9. 마일스톤 요약 표

| Day | 마일스톤 | inst 예상 | 사용자 가시 결과 |
|---|---|---|---|
| 0 (현) | wave6 8360 inst plateau | 8360 | macOS 화면 변화 0건 |
| 1 | first NSWindow 표시 | 20000+ | Battle.net Setup window 1개 등장 |
| 2-3 | 사용자 click + wndproc 도달 | 100K+ | next dialog / install 시작 |
| 4-5 | TLS handshake + HTTP GET | 300K+ | progress bar 진척, network traffic |
| 6-7 | install complete | 10M+ | install 완료 dialog + 파일 디스크 존재 |
| 8 | launcher (Battle.net.exe) 표시 | 100M+ | Battle.net Launcher 화면 |
| 9-10 | OAuth login | — | Safari redirect + 로그인 완료 |
| 11+ | 게임 launch (D2R 등) | — | 실 게임 화면 표시 |

---

---

*written 2026-05-01 cycle 103. base `0a24e22` (8360 inst). Day 1 trigger =
GetProcAddress dispatch back + 0F XADD/CMOVcc/SETcc. own1 §A 100% — Apple SDK +
자체 코드 + Microsoft public docs reference only. 본 path 외 수정 X.*
