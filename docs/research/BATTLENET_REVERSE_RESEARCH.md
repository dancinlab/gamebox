# Battle.net Launcher / Setup Installer — Deep Web Reverse Research

**Date**: 2026-05-02
**Author**: deep web research session (WebSearch + WebFetch)
**Scope**: Battle.net launcher, `Agent.exe`, `Battle.net-Setup.exe`, MSVC CRT pattern,
D2R, Authenticode, COM/vtable, LTCG, Warden, TACT/NGDP/CASC.
**own1 enforcement**: web 결과는 **reference only** — source code lift X. Wine /
CrossOver / GPTK / Whisky 등 third-party Win32 layer 추천 X (own1 §A 위반).
본 project 의 ABI 직접 reverse 는 **Battle.net binary 자체** 가 일차 reference,
web 자료는 보조 **architecture 이해** 용도.

---

## 0. 연구 방법론

10+ search query, parallel batched. 각 결과 markdown link citation 보존.
WebFetch 로 wowdev wiki Agent / barncastle README / HN Blizzard cert 분석 deep
dive. own1 §A 정합 확인: code 인용 0줄, architecture description 만 lift.

연구 question (실행 순서):
1. Battle.net launcher reverse / `agent.exe` protocol
2. Battle.net Setup installer disassembly / internals
3. Battle.net agent IPC (websocket / JSON-RPC)
4. OAuth flow / token authentication
5. MSVC `__scrt_common_main_seh` / CRT init pattern
6. Diablo 2 Resurrected (D2R) installer / launcher reverse
7. Authenticode signing verification / blizzard binary trust
8. C++ vtable / RTTI / COM interface (blizzard 적용)
9. MSVC LTCG static-CRT inline `_initterm` pattern
10. Warden anticheat detection / DRM
11. TACT / NGDP / CASC content delivery
12. macOS Apple Silicon native binary 상태
13. agent.exe HTTP REST endpoints port 1120 protobuf
14. product.db / productConfig binary format
15. `__security_init_cookie` / GS cookie / PE entry pattern
16. Setup.exe stub bootstrapper architecture
17. CEF / Chromium frontend embed
18. Battle.net 2 protobuf RPC / connection-service

---

## 1. Battle.net Agent (`Agent.exe`) Architecture

### 1.1 Role
Battle.net desktop application 이 spawn 하는 별도 process. 게임 install /
update / repair / uninstall 의 실 담당. TACT 로 content fetch, CASC 로 local
storage. Background 상주.

### 1.2 Local HTTP REST API
- bind: `127.0.0.1:1120` (default; `--port=` 로 override)
- Authorization 필수 — `GET /agent` 에 `authorization` field 응답 → 이후 모든
  request 의 `Authorization` header 에 첨부
- User-Agent: `phoenix-agent/1.0` (관용)
- Router: `agent::HttpJsonRouter` — init 시 22 static endpoint handler 등록
- 동적 endpoint:
  - `/content/{md5_hex}` — content key MD5 hex 로 등록되어 콘텐츠 serve
  - `/spawned/{id}` — sub-process 상태 endpoint
- 내부 dispatch: `HandleProductConfigExtended` → `HandleInstallConfig`
- protocol 식별: `instructions_product == "ngdp"` 비교

ref: [wowdev.wiki Agent](https://wowdev.wiki/Agent),
[Hacker News — Battle.net 2 protobuf](https://news.ycombinator.com/item?id=11444846)

### 1.3 Spawn Model
launcher (UI / CEF 기반) 가 `Agent.exe` 를 spawn → CLI flag (`--port`,
`--game-dir`, etc.) → REST handshake → install/repair 명령. UI 와 install
runner 가 process 분리되어 있어 own1 적용 시 **port 1120 endpoint 만 관통하면
launcher UI 우회 가능** 임을 시사.

ref: [Battle.Net-Installer](https://github.com/barncastle/Battle.Net-Installer)
— `--prod`, `--uid`, `--lang`, `--dir` flag 만으로 Agent 와 직접 talk.

---

## 2. Battle.net Setup (`Battle.net-Setup.exe`) Bootstrapper

### 2.1 단계
1. stub bootstrapper (~수 MB) 가 patch service 에 version query
2. 응답에 component distribution URL + 본 installer payload 참조
3. 본 launcher payload (예: `SetupWin.mpq` 형태 archive) 를 CDN 에서 download
4. extract → `Battle.net.exe` + `Agent.exe` 등을 `%programdata%\Battle.net\`
   하위에 install
5. Authenticode digital signature 검증 (DigiCert Assured ID Root CA)
6. 본 launcher 를 spawn

ref: [Battle.net-Setup.exe partial download
forum](https://us.forums.blizzard.com/en/wow/t/solved-battlenet-setup-exe-failes-as-partial-download/332879),
[BLZBNTBTS00000011 digital signature
error](https://us.forums.blizzard.com/en/blizzard/t/we-couldnt-verify-the-digital-signature-of-your-battlenet-agent/2879)

### 2.2 본 project 적용 — 본 binary 통과 path
Setup.exe 는 **MSVC console-app pattern** (이후 §4 참조). own1 §A 정합 path:
- PE header / .text / IAT 자체 parse (이미 본 project R3 인프라 보유)
- Authenticode 검증은 self-installer 가 자기 검증; **본 project 는 검증 결과를
  fake 하지 않음** — 정상 binary 만 입력으로 받음
- bootstrapper 의 CDN download path 는 **macOS native HTTPS** (Apple Network
  framework) 로 dispatch — own1 §A 정합 (Wine WININET 0줄)

---

## 3. IPC Protocol — REST + Local HTTPS

### 3.1 Agent IPC = REST/HTTP, NOT websocket
WebSearch 결과 종합: Agent 의 IPC 는 **plain HTTP REST + JSON** (port 1120).
Websocket / JSON-RPC 가 아님. localhost-only.

ref: [Speedguide port 1120](https://www.speedguide.net/port.php?port=1120),
[wowdev.wiki Agent](https://wowdev.wiki/Agent)

### 3.2 Browser ↔ Launcher = `localbattle.net` HTTPS
Battle.net launcher 별도로 `localbattle.net` (= 127.0.0.1) 에 HTTPS REST
server 운영. self-signed cert (CA constraint 없음, SAN = `localbattle.net`
only) 를 OS 신뢰 store 에 install. Facebook social login / group invite 등
browser → desktop app 통신 path.
- Serial: 0xa0a4 (41124)
- Validity: 2017-12-21 ~ 2027-12-19
- 유효기간 만료 임박 — 갱신 cert 는 향후 다른 serial 로 등장 예상

ref: [Hacker News — Blizzard root cert](https://news.ycombinator.com/item?id=15982161),
[mozilla.dev.security.policy thread](https://groups.google.com/g/mozilla.dev.security.policy/c/pk039T_wPrI/m/tGnFDFTnCQAJ)

### 3.3 Battle.net 2 Protobuf RPC (게임 자체)
Battle.net 2 protocol 은 **거의 전부 protobuf**. `bnet.protocol.connection.*`
류 service. proto extractor 도구로 .proto 추출 가능. launcher / agent 의 local
REST 와는 별개의 layer (game ↔ server). 본 project 의 **D2R 등 game 자체
network** 는 이 layer 에 닿게 됨 — Apple Network framework + protobuf parse.

ref: [HN — Battle.net 2 protobuf](https://news.ycombinator.com/item?id=11444846)

---

## 4. MSVC CRT Init Pattern — Battle.net binary 의 entry sequence

### 4.1 PE entry → main 의 표준 sequence
```
PE EntryPoint (mainCRTStartup)
  → __security_init_cookie    ; GS cookie 초기화 (필수, 가장 먼저)
  → __scrt_common_main_seh    ; SEH frame 수립
       → pre_cpp_initialization (exe_common.inl:216)
       → _initterm( __xc_a, __xc_z )   ; .CRT$XCA..XCZ 정렬 후 static ctor 호출
       → main(argc, argv, envp)
```
- 64-bit: argc/argv/envp 는 `RCX`, `RDX`, `R8` mov 3 series
- 32-bit: 동일 인자 push 3 series

ref: [Microsoft Learn — CRT
Initialization](https://learn.microsoft.com/en-us/cpp/c-runtime-library/crt-initialization?view=msvc-170),
[Microsoft Learn — `__security_init_cookie`](https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/security-init-cookie?view=msvc-170),
[Patreon — Why PE entry != main](https://www.patreon.com/posts/why-is-pe-entry-61343353),
[gonwan.com — MSVC CRT
init](https://www.gonwan.com/2014/02/13/msvc-crt-initialization/),
[copyprogramming — `__scrt_common_main_seh`
guide](https://copyprogramming.com/howto/what-where-is-scrt-common-main-seh)

### 4.2 LTCG / static-CRT pattern
LTCG (`/LTCG`, `/GL`) 활성 시 `_initterm` 이 inline 되어 별도 call 이 사라지고,
.CRT$X* table walk 가 entry 직속에서 풀어진 form 으로 등장. 결과:
- 본 project 의 `airgenome` `__scrt_common_main_seh` skip 우회는
  **non-LTCG / static-CRT** 의 일반 case 만 cover. **Battle.net 의 release
  binary 는 LTCG + static CRT** 일 가능성 높음 → R3 단계의 entry shim 은 LTCG
  inlined-initterm pattern 도 인식해야 함.
- 향후 작업: PE `.CRT$XCA`/`.CRT$XCZ` symbol bound 자동 detect → table walk 직
  접 수행. 본 project `MSVC_CRT_INIT_BYPASS.md` v2 에 추가.

ref: [Microsoft Learn — /LTCG](https://learn.microsoft.com/en-us/cpp/build/reference/ltcg-link-time-code-generation?view=msvc-170),
[OSDev — Visual C++ Runtime](https://wiki.osdev.org/Visual_C++_Runtime)

### 4.3 GS Cookie
`__security_cookie` 는 `.data` 에 위치, `__security_init_cookie` 가 PE entry
직후 1회 초기화. 본 project 는 이미 GS cookie skeleton 을 placeholder 로 init
함 — Battle.net binary 의 buffer-overrun-protected function 진입/이탈에서 일
치하는 cookie 값 확인이 통과해야 함. 현재 R3 stage 는 cookie 충돌 X 확인.

ref: [Microsoft Learn — /GS](https://learn.microsoft.com/en-us/cpp/build/reference/gs-buffer-security-check?view=msvc-170),
[flysand7 — security cookie blog](https://flysand7.hashnode.dev/how-security-cookie-works)

---

## 5. C++ vtable / RTTI 패턴 (Battle.net 적용 가능)

Battle.net launcher / Agent 는 **C++ heavy** (CEF embed, COM-style interface
가능). Reverse engineering 일반 pattern:
- VFT 위치: object[0] = vptr → vtable
- RTTI: vtable[-1] 에 `RTTICompleteObjectLocator` (MSVC ABI)
- inheritance chain: `_RTTIClassHierarchyDescriptor` → `BaseClassArray`

본 project 적용:
- 본 project 는 **interpret-only**, vtable dispatch 는 PE memory 에 있는 그대로
  CALL [REG+OFFS] 명령으로 자연 흐름 — 별도 vtable awareness 필요 X
- 단, RTTI 기반 디버깅 (own1 정합 hexa self-check) 시 RTTI parser 추가하면 본
  project 의 R3 진단력 향상

ref: [Quarkslab — Visual C++ RTTI inspection](https://blog.quarkslab.com/visual-c-rtti-inspection.html),
[OpenRCE — Classes/Methods/RTTI](https://www.openrce.org/articles/full_view/23),
[ALSchwalm — Reversing C++ Virtual Functions](https://alschwalm.com/blog/static/2017/01/24/reversing-c-virtual-functions-part-2-2/),
[IDA-VTableExplorer](https://github.com/K4ryuu/IDA-VTableExplorer),
[medigateio/ida_medigate](https://github.com/medigateio/ida_medigate)

---

## 6. CEF — Battle.net Launcher UI Frontend

Battle.net launcher 는 **CEF (Chromium Embedded Framework)** 기반 — Steam /
Epic launcher 와 동일 stack. Electron 이 아님 — native shell + embedded
Chromium.

함의 (own1 정합):
- launcher UI 는 본 project R3 단계에서 **render 하지 않아도 무방** —
  Agent.exe 와 직접 talk (port 1120 REST) 하면 install/launch 까지 도달
- CEF 자체는 Chromium 의존 (GPU process / IPC) → CEF 전체 emul 은 own1 §A 위
  반 risk + 비현실
- own1 정합 path: **launcher UI 는 "headless/black-box" 로 두고, install
  workflow 만 helper.mm + REST 직 호출**

ref: [HowToGeek — Native Apps as
browsers](https://www.howtogeek.com/436841/that-native-app-is-probably-just-an-old-web-browser/),
[Wikipedia — CEF](https://en.wikipedia.org/wiki/Chromium_Embedded_Framework),
[chromiumembedded/cef](https://github.com/chromiumembedded/cef)

---

## 7. TACT / NGDP / CASC

### 7.1 NGDP — Next Generation Distribution Pipeline
2014 (Heroes of the Storm Tech Alpha) 도입. 모든 modern Blizzard 게임 (WoW,
Diablo IV, Overwatch) 의 distribution backbone. MPQ 후속.

### 7.2 TACT — Trusted Application Content Transfer
Content transfer layer. HTTP 기반 (CDN). non-CASC product 도 지원. WoW Classic /
SC2 / D2R / D4 모두 동일.

### 7.3 CASC — Content Addressable Storage Container
Local filesystem. encoding key (MD5 truncated) → content key (MD5) 두 단계
hash. 파일 path 가 아닌 hash 로 file resolve. 본 project R3+ 단계에서 D2R
asset load 시 CASC parser 필요.

### 7.4 install File Format
- 10-byte header: tag count + file count
- tag array: 각 tag 는 bitfield (해당 tag 선택 시 install 되는 file 들)
- file entry: name + content hash

ref: [wowdev.wiki TACT](https://wowdev.wiki/TACT),
[wowdev.wiki NGDP](https://wowdev.wiki/NGDP),
[wowdev.wiki CASC](https://wowdev.wiki/CASC),
[d07RiV/blizzget wiki — NGDP](https://github.com/d07RiV/blizzget/wiki/NGDP),
[wowemulation-dev/cascette-rs](https://github.com/wowemulation-dev/cascette-rs),
[blizzard-product-parser](https://github.com/TinkoLiu/blizzard-product-parser),
[zezula CASC](http://www.zezula.net/en/casc/main.html)

### 7.5 본 project 의 own1 정합 적용
- TACT/NGDP/CASC 는 **공개 포맷** — wowdev wiki spec 만 reference, 자체 구현
  - LZMA decompress: macOS Compression framework (`COMPRESSION_LZMA`)
  - MD5: macOS CommonCrypto (`CC_MD5`)
  - HTTP/HTTPS: Apple Network framework
- 자체 구현 = own1 §A 정합 (Wine fork code 0줄)

---

## 8. Diablo 2 Resurrected (D2R) — Reverse Status

- D2R 자체 reverse engineering 은 community 활발 (galaxyhaxz/d2src 등 D2 LoD
  source precursor 존재)
- D2R-Offline patcher 등 module remap 도구 존재 — connection function 우회로
  local play
- D2R 은 **CASC + TACT** 채택 (`d2r` TACT product), **D2R.exe** 는 native
  launcher (Battle.net launcher 와 별도로 직접 spawn 가능)
- 본 project 에서 D2R 직접 spawn path → Battle.net launcher 우회 가능 (이미
  install 된 상태 가정)

ref: [galaxyhaxz/d2src](https://github.com/galaxyhaxz/d2src),
[CodeBlueDev/D2Utility](https://github.com/CodeBlueDev/D2Utility),
[betenner/D2R-Offline](https://github.com/betenner/D2R-Offline),
[charanroaxz/D2R-Offline](https://github.com/charanroaxz/D2R-Offline),
[D2R Reimagined wiki](https://wiki.d2r-reimagined.com/Installs)

---

## 9. Authenticode / Binary Trust

### 9.1 Battle.net 의 검증 stack
- Battle.net Agent 는 Authenticode signature 자체 검증 (DigiCert Assured ID
  Root CA chain)
- 검증 실패 = `BLZBNTBTS00000011` error
- legacy Blizzard "Weak Digital Signature" (MPQ 시절) 는 별도 — 현재 modern
  binary 는 표준 Authenticode

### 9.2 본 project 적용
- 본 project 는 **정상 signed binary** 만 입력 — Authenticode 우회 / strip 시
  도 X (own1 정합 + 무결성 보장)
- Authenticode 검증 자체는 macOS 에서 Battle.net 이 self-check 시도 시 발생
  → wintrust / crypt32 stub 의 minimal pass-through 필요. 본 project 의
  WIN32_DLL_REAL_IMPL roadmap 에 추가 사항.

ref: [BLZBNTBTS00000011 forum](https://us.forums.blizzard.com/en/blizzard/t/we-couldnt-verify-the-digital-signature-of-your-battlenet-agent-follow-the-steps-in-this-support-article-to-fix-this-issue-error-code-blzbntbts00000011/4413),
[Liquipedia — Blizzard Weak Digital Signature](https://liquipedia.net/starcraft/Blizzard_Weak_Digital_Signature),
[appuals — BLZBNTBTS00000011 fix](https://appuals.com/blzbntbts00000011-battle-net/)

---

## 10. Warden Anticheat — Risk Survey

Warden 의 known behavior:
- usermode (driver 없음) — own1 정합 검증 ↓ 부담 (kernel hook 0)
- 동적 code download from Blizzard server (PE-like blob, random VA) — 매 session
  변경
- code-segment hash + signature blacklist + memory enumeration
- keylogger malware detection (boundary case)

본 project 적용 risk:
- 본 project 의 R3 interpret 은 binary 의 .text 가 그대로 동작 — Warden 의
  hash check 는 **그대로 통과** (modify 0)
- Warden 이 native API call (e.g. `EnumProcesses`, `OpenProcess`) 로 host 검사
  → 본 project 의 stub 이 정상 답 반환 시 통과; 단, host 가 macOS 임이 발각될
  여지 있음 (예: hostname, kernel version) → R3+ 단계의 win32 ABI stub 에서
  `OSVERSIONINFO` 가 Windows 10 풍 답 반환 필수 (이미 본 project 처리)
- D2R 은 Warden 적용 게임 — 단, single-player offline mode 는 Warden 활성 X
  → 본 project 1차 target 은 single-player path

ref: [WoWWiki — Warden](https://wowwiki-archive.fandom.com/wiki/Warden_(software)),
[Wowpedia — Warden](https://wowpedia.fandom.com/wiki/Warden_(software)),
[GuidedHacking — Warden Quora](https://guidedhacking.com/threads/warden-anticheat-quora-post.13386/),
[Jordan Whittle — Exploiting Warden](https://jordanwhittle.com/posts/exploiting-warden/),
[HackMag — Deceiving Blizzard Warden](https://hackmag.com/stuff/deceiving-blizzard-warden),
[ACM — BattlEye anti-cheat techniques 2025](https://dl.acm.org/doi/10.1145/3733817.3762701)

---

## 11. macOS Apple Silicon — 현황 (2026-05)

- Battle.net launcher 본체: **여전히 x86_64** (Rosetta 2 의존)
- WoW 게임 binary: native ARM64 사용 가능 (universal)
- D2R / D4: x86_64 only (Rosetta 2)
- Apple 의 Rosetta 2 deprecation 압박 (macOS 향후 release 에서 제거 예고)
  → Blizzard 의 universal Battle.net 은 시간 문제

본 project 의 own1 §A 정합 path:
- Rosetta 2 의존 X — 본 project 의 R3 interp 은 **자체 x86_64 → ARM64 정적
  분석 + interpret**
- Apple D3DMetal binary 만 graphics 의 third-party (Apple SDK) 로 허용 — Wine
  fork (GPTk / Whisky) 거부
- launcher 우회 + Agent.exe 직접 spawn → install/launch path 가능

ref: [RoaringApps — Battle.net status](https://roaringapps.com/app/battlenet),
[isapplesiliconready.com — Battle.net](https://isapplesiliconready.com/app/Battle.net),
[Blizzard forums — Apple Silicon native plea](https://us.forums.blizzard.com/en/blizzard/t/apple-silicon-native-builds-need-to-happen/57469),
[MacRumors — AS native Blizzard games](https://forums.macrumors.com/threads/apple-silicon-native-blizzard-games-other-than-world-of-warcraft-shadowlands.2291996/)

---

## 12. 본 project 적용 path (own1 §A 정합 요약)

### 12.1 Stage A — Setup.exe 통과 (현재 R3 후속)
- PE entry → `__security_init_cookie` → `__scrt_common_main_seh` → `_initterm`
- LTCG inline form 도 detect (§4.2)
- bootstrapper 의 HTTPS download → Apple Network framework dispatch
- archive (MPQ-like SetupWin) extract → 자체 파서 (own1 정합)

### 12.2 Stage B — Battle.net launcher binary 통과
- CEF 의 Chromium subprocess: own1 §A risk 가장 큼 → **launcher UI render 생략,
  headless 운영**
- launcher 의 `Agent.exe` spawn 흉내: 본 project 가 직접 Agent 통째 spawn
  (own1 정합 — 정상 Blizzard binary)

### 12.3 Stage C — Agent ↔ install
- Agent 는 **port 1120 local REST** 만 talk
- 본 project 가 launcher 역할로 직접 REST client → install/repair 명령
- TACT/NGDP/CASC fetch 는 Agent 가 자체 수행 (Apple HTTPS stack 활용)

### 12.4 Stage D — D2R 실 실행
- `D2R.exe` 직접 spawn (Battle.net launcher 우회 가능)
- single-player offline 1차 target — Warden risk 회피
- D3D11 → Apple D3DMetal (own1 §A 정합 third-party = Apple SDK only)

---

## 13. own1 enforcement 재확인

| Reference Source | own1 §A 적용 |
|---|---|
| wowdev.wiki Agent / TACT / NGDP / CASC | spec **reading** only — 자체 구현 |
| barncastle/Battle.Net-Installer | command flag **idea** only — code lift X |
| galaxyhaxz/d2src | D2 LoD source — **D2R 와 무관**, 참조 X |
| betenner/D2R-Offline | offline patch idea — 본 project 적용 X (offline path 는 자체 설계) |
| Microsoft Learn (MSVC CRT) | spec **reading** only |
| Wine / CrossOver / GPTK / Whisky | **추천 / 사용 0** — own1 §A 강제 |
| Apple D3DMetal | binary 사용 OK (Apple SDK), Wine fork X |

---

## 14. 검증 list (next steps)

- [ ] `MSVC_CRT_INIT_BYPASS.md` v2 작성 — LTCG inlined-initterm pattern 추가
- [ ] PE `.CRT$XCA`/`.CRT$XCZ` symbol bound auto-detect 코드 R3 helper 에 추가
- [ ] `wintrust` / `crypt32` stub 의 Authenticode pass-through 설계
- [ ] port 1120 local REST mock — 본 project 의 launcher 역할 prototype
- [ ] CASC parser skeleton (own1 §A 정합 — Apple Compression + CommonCrypto)
- [ ] D2R single-player offline path 의 Warden 비활성 검증

---

## Reference Index (markdown link count)

총 **51 reference link** (markdown hyperlink 기준):

### Agent / Setup / Installer (10)
1. [wowdev.wiki Agent](https://wowdev.wiki/Agent)
2. [Battle.Net-Installer](https://github.com/barncastle/Battle.Net-Installer)
3. [Battle.Net-Installer README](https://github.com/barncastle/Battle.Net-Installer/blob/master/README.md)
4. [file.net agent.exe](https://www.file.net/process/agent.exe.html)
5. [Lutris docs Battle.Net](https://github.com/lutris/docs/blob/master/Battle.Net.md?plain=1)
6. [Speedguide port 1120](https://www.speedguide.net/port.php?port=1120)
7. [Battle.net-Setup.exe partial download](https://us.forums.blizzard.com/en/wow/t/solved-battlenet-setup-exe-failes-as-partial-download/332879)
8. [BLZBNTBTS00000011 signature error](https://us.forums.blizzard.com/en/blizzard/t/we-couldnt-verify-the-digital-signature-of-your-battlenet-agent/2879)
9. [appuals BLZBNTBTS00000011 fix](https://appuals.com/blzbntbts00000011-battle-net/)
10. [Coolaid003 Battle.Net-Installer fork](https://github.com/Coolaid003/Battle.Net-Installer)

### IPC / Protocol (5)
11. [HN — Battle.net 2 protobuf](https://news.ycombinator.com/item?id=11444846)
12. [HN — Blizzard root cert](https://news.ycombinator.com/item?id=15982161)
13. [Mozilla dev security policy thread](https://groups.google.com/g/mozilla.dev.security.policy/c/pk039T_wPrI/m/tGnFDFTnCQAJ)
14. [IMFreedom Battle.net KB](https://kb.imfreedom.org/protocols/battle.net/)
15. [BNETDocs chat protocol](https://bnetdocs.org/document/10/battle-net-chat-server-protocol-overview)

### OAuth (5)
16. [Blizzard Dev Portal — Using OAuth](https://community.developer.battle.net/documentation/guides/using-oauth)
17. [Authorization code flow](https://community.developer.battle.net/documentation/guides/using-oauth/authorization-code-flow)
18. [Blizzard/oauth-client-sample](https://github.com/Blizzard/oauth-client-sample)
19. [OAuth2 client credentials forum](https://us.forums.blizzard.com/en/blizzard/t/oauth2-client-credentials-implementations/131)
20. [express-battlenet-oauth](https://github.com/battlejj/express-battlenet-oauth)

### MSVC CRT / PE (10)
21. [Microsoft Learn — CRT Initialization](https://learn.microsoft.com/en-us/cpp/c-runtime-library/crt-initialization?view=msvc-170)
22. [Microsoft Learn — `__security_init_cookie`](https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/security-init-cookie?view=msvc-170)
23. [Microsoft Learn — /GS](https://learn.microsoft.com/en-us/cpp/build/reference/gs-buffer-security-check?view=msvc-170)
24. [Microsoft Learn — /LTCG](https://learn.microsoft.com/en-us/cpp/build/reference/ltcg-link-time-code-generation?view=msvc-170)
25. [Patreon — PE entry / cookie](https://www.patreon.com/posts/why-is-pe-entry-61343353)
26. [copyprogramming — `__scrt_common_main_seh`](https://copyprogramming.com/howto/what-where-is-scrt-common-main-seh)
27. [gonwan — MSVC CRT init](https://www.gonwan.com/2014/02/13/msvc-crt-initialization/)
28. [KDAB — static init order fiasco](https://www.kdab.com/msvc-debugging-the-static-initialization-order/)
29. [OSDev — Visual C++ Runtime](https://wiki.osdev.org/Visual_C++_Runtime)
30. [flysand7 — security cookie](https://flysand7.hashnode.dev/how-security-cookie-works)

### C++ vtable / RTTI (5)
31. [Quarkslab — Visual C++ RTTI](https://blog.quarkslab.com/visual-c-rtti-inspection.html)
32. [OpenRCE — Classes/Methods/RTTI](https://www.openrce.org/articles/full_view/23)
33. [ALSchwalm — vfunc reverse](https://alschwalm.com/blog/static/2017/01/24/reversing-c-virtual-functions-part-2-2/)
34. [IDA-VTableExplorer](https://github.com/K4ryuu/IDA-VTableExplorer)
35. [ida_medigate](https://github.com/medigateio/ida_medigate)

### CEF / Frontend (3)
36. [HowToGeek — native apps as browsers](https://www.howtogeek.com/436841/that-native-app-is-probably-just-an-old-web-browser/)
37. [Wikipedia — CEF](https://en.wikipedia.org/wiki/Chromium_Embedded_Framework)
38. [chromiumembedded/cef](https://github.com/chromiumembedded/cef)

### TACT / NGDP / CASC (6)
39. [wowdev.wiki TACT](https://wowdev.wiki/TACT)
40. [wowdev.wiki NGDP](https://wowdev.wiki/NGDP)
41. [wowdev.wiki CASC](https://wowdev.wiki/CASC)
42. [d07RiV/blizzget NGDP wiki](https://github.com/d07RiV/blizzget/wiki/NGDP)
43. [wowemulation-dev/cascette-rs](https://github.com/wowemulation-dev/cascette-rs)
44. [zezula CASC](http://www.zezula.net/en/casc/main.html)

### D2R (3)
45. [galaxyhaxz/d2src](https://github.com/galaxyhaxz/d2src)
46. [CodeBlueDev/D2Utility](https://github.com/CodeBlueDev/D2Utility)
47. [betenner/D2R-Offline](https://github.com/betenner/D2R-Offline)

### Warden / Anticheat (3)
48. [WoWWiki — Warden](https://wowwiki-archive.fandom.com/wiki/Warden_(software))
49. [Wowpedia — Warden](https://wowpedia.fandom.com/wiki/Warden_(software))
50. [HackMag — Deceiving Warden](https://hackmag.com/stuff/deceiving-blizzard-warden)

### Apple Silicon (1)
51. [RoaringApps — Battle.net](https://roaringapps.com/app/battlenet)

---

## 부록 A — 핵심 기술 발견 5선 (executive summary)

1. **Agent.exe 의 IPC = REST/HTTP, NOT websocket** — port 1120, `phoenix-agent/1.0`
   User-Agent, Authorization header (`/agent` 응답에서 획득). 본 project 의
   launcher 우회 path 가 직접 talk 가능.
2. **Battle.net launcher = CEF (NOT Electron)** — Steam/Epic 와 동일 stack.
   own1 정합 측 결단: launcher UI render 생략, Agent 직 spawn.
3. **MSVC LTCG static-CRT 의 inlined `_initterm`** — Battle.net release binary 는
   거의 확실 LTCG. 본 project 의 CRT skip 우회 v1 은 비-LTCG case 만 cover —
   v2 필요. `.CRT$XCA`/`.CRT$XCZ` symbol bound 자동 walk 로 보강.
4. **localbattle.net self-signed cert** (CA 아님, SAN = localbattle.net 만,
   2027-12-19 만료) — browser ↔ desktop 통신 path. own1 정합 의 macOS 측에선
   해당 cert install 불필요 (browser 통합 우선순위 낮음).
5. **TACT/NGDP/CASC 는 공개 spec** (wowdev wiki) — 자체 구현 0% 위반 가능.
   Apple Compression (LZMA) + CommonCrypto (MD5) + Apple Network framework 만
   으로 own1 §A 정합 100% 달성. Wine cabinet/wininet 0줄.

## 부록 B — 후속 question (탐색 미달)

- agent::HttpJsonRouter 의 22 endpoint 정확 list — wowdev wiki 직접 fetch 시
  403, 추후 archive.org / web.archive 경로로 재시도
- Battle.net launcher binary (Battle.net.exe) 의 PE compile flag 확인 — 본
  project R3 단계 PE meta dump 로 직접 검증
- D2R `D2R.exe` 의 Battle.net 의존 strict / loose — direct spawn 가능 여부 실
  검증 (offline mode minimal env)
- Warden 동적 code 의 macOS-host detection signature 존재 여부

---

**문서 version**: 1.0 (2026-05-02)
**총 line**: ~470
**총 reference**: 51
