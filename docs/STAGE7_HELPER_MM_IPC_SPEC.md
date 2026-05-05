# stage7 helper.mm IPC frame — full spec

> **status**: 작성 2026-05-01 / 사용자 directive: "남은 영역 all bg go"
> SSOT: cycle 80 pe_native_*_real 4 모듈 헤더 docstring 통합
> 형식 참고: `docs/STAGE7_NATIVE_BRIDGE_MANJEOM_REVIEW.md`
> own 1 enforcement: Apple SDK + man page only — Wine / CrossOver / GPTK / Whisky 인용 0

---

## 0. 본 문서 범위

본 문서는 cycle 80 R1 phase 1 에서 skeleton 화 된 4 모듈 (`pe_native_dlopen_real`, `pe_native_mmap_real`, `pe_native_mprotect_real`, `pe_native_pthread_real`) 의 헤더 docstring 에 분산되어 있던 stage7 helper.mm IPC frame 정의를 단일 SSOT 로 통합한다.

포함:
- helper.mm IPC frame 프로토콜 정의 (transport, layout, kind enumeration)
- 8 IPC kind 별 request / response binary layout
- helper.mm 측 reference sample (Objective-C — real 구현은 사용자 외부 결정 영역)
- entitlement plist (com.apple.security.cs.allow-jit, allow-unsigned-executable-memory)
- security boundary 명세

제외 (사용자 외부 결정 영역):
- helper.mm 실 구현 + Xcode/clang build
- codesign / notarization
- airgenome-gamebox 의 helper subprocess spawn 통합 path

---

## 1. IPC transport

| 후보 | 설명 | 장점 | 단점 |
|---|---|---|---|
| **stdin/stdout binary** | helper 가 spawn 된 subprocess, 부모가 fd 보유 | dependency 0, simplest, exec 1 회 | 다중 client X, async 시 framing 정확도 의존 |
| AF_UNIX socket | `/tmp/airgenome_helper.sock` 영구 socket file | 다중 client, reconnect 가능 | mkstemp + chmod 0600 필요, fd leak 위험 +1 |
| XPC | launchd-managed Mach service | Apple-native, 권한 분리, sandbox 친화 | plist + xpc_connection 비용, entitlement 복잡, debug 난이도 |

→ **권장: stdin/stdout binary frame** — own1 dependency 0 + helper 1 instance / parent 1 instance 1:1 모델 → 멀티플렉싱 불요. helper crash 시 부모가 spawn 재시작.

전제:
- 부모 (airgenome / hexa runner) 가 `posix_spawn(2)` 로 helper 를 child 로 띄우고 stdin/stdout 을 pipe 로 잡음
- stderr 는 log 전용 (frame 미사용)
- helper 는 stdin EOF 수신 시 graceful exit

---

## 2. Frame layout

little-endian (Apple Silicon arm64 native order). 모든 정수 unsigned 별도 표기 시 외 signed.

| 필드 | offset | 크기 | type | 의미 |
|---|---|---|---|---|
| magic | 0 | 4 byte | char[4] | `"AHI1"` (airgenome helper IPC v1) |
| kind | 4 | 4 byte | uint32 | IPC kind (1–8, 9~ reserved) |
| seq | 8 | 8 byte | uint64 | request sequence number — response 가 echo |
| payload_len | 16 | 4 byte | uint32 | payload byte length (≤ 65536) |
| reserved | 20 | 4 byte | uint32 | 0 fill (future flags) |
| payload | 24 | <var> | binary | kind-specific blob |

총 header = 24 byte. payload_len 상한 64 KiB — 단일 frame 으로 처리 가능한 데이터 한계 (path string + struct payload 합).

response frame 도 동일 layout — kind 는 request kind 와 일치, seq 는 request seq 그대로 echo, payload 는 kind 별 response 정의에 따른다.

error frame: kind 가 request 와 일치하되 payload 첫 4 byte 에 negative errno 가 들어오면 (`int32 < 0`) error frame 으로 간주.

---

## 3. 8 IPC kind 정의 (request + response payload)

### 3.1 kind=1 dlopen

SSOT: `lib/loader/pe_native_dlopen_real.hexa` 헤더 docstring.

**request payload**:
```
struct dlopen_req {
    int32   flags;          // RTLD_LAZY=1 | RTLD_NOW=2 | RTLD_LOCAL=4 | RTLD_GLOBAL=8
    uint32  path_len;       // utf-8 byte length (path nul 제외)
    char    path[path_len]; // utf-8, nul terminator 미포함
}
```

**response payload**:
```
struct dlopen_resp {
    int32   handle_kind;    // 1=valid_handle / 2=null_main_image / 3=error / 4=already_loaded
    uint64  native_handle;  // void* (helper 내 dlopen 반환값) — error 시 0
    int32   err;            // dlerror 매핑 errno — success 시 0
    uint32  err_msg_len;    // dlerror() 문자열 길이 (error 일 때만 > 0)
    char    err_msg[err_msg_len];
}
```

→ `pe_native_dlopen_real.record_invoke(invoke_id, dlopen_flags, handle_kind, elapsed_us)` 와 1:1 정합. dlopen_flags 는 RTLD_LAZY|RTLD_NOW + RTLD_LOCAL|RTLD_GLOBAL 조합 비트마스크 (1–15).

man page reference: `man 3 dlopen` (Apple).

---

### 3.2 kind=2 mmap

SSOT: `lib/loader/pe_native_mmap_real.hexa` 헤더 docstring.

**request payload**:
```
struct mmap_req {
    uint64  length;     // byte (must be multiple of 16384 — Apple Silicon 16 KiB page)
    int32   prot;       // PROT_READ=1 | PROT_WRITE=2 | PROT_EXEC=4
    int32   flags;      // MAP_PRIVATE=2 | MAP_FIXED=16 | MAP_JIT=2048 | MAP_ANON=4096
    uint64  addr_hint;  // MAP_FIXED 시 의무 주소 / 0=helper 자유 선택
}
```

**response payload**:
```
struct mmap_resp {
    uint64  addr;       // 매핑 base address — error 시 0
    int32   err;        // errno — success 시 0
}
```

Apple Silicon 제약 (helper 가 enforce):
- `length % 16384 == 0` 위반 시 helper 가 EINVAL 반환
- `(prot & PROT_WRITE) && (prot & PROT_EXEC)` 동시 요청 시 W^X 위반 → helper 가 EACCES 반환 (대신 MAP_JIT 사용 안내)
- `flags & MAP_JIT` 시 entitlement `com.apple.security.cs.allow-jit` 필수

man page reference: `man 2 mmap` (Apple).

---

### 3.3 kind=3 mprotect

SSOT: `lib/loader/pe_native_mprotect_real.hexa` 헤더 docstring.

**request payload**:
```
struct mprotect_req {
    uint64  addr;
    uint64  length;
    int32   prot;       // PROT_NONE=0 | PROT_READ=1 | PROT_WRITE=2 | PROT_EXEC=4
    int32   jit_toggle; // 0=일반 mprotect / 1=pthread_jit_write_protect_np(0) / 2=pthread_jit_write_protect_np(1)
}
```

**response payload**:
```
struct mprotect_resp {
    int32   success;    // 1=ok / 0=fail
    int32   err;        // errno — success 시 0
}
```

JIT region 의 RW↔RX 토글은 `mprotect()` 가 아닌 `pthread_jit_write_protect_np()` 가 정 path — `jit_toggle` 필드로 path 분리. PROT_WRITE|PROT_EXEC 동시 요청 시 helper 거부.

man page reference: `man 2 mprotect`, `man 3 pthread_jit_write_protect_np` (Apple).

---

### 3.4 kind=4 pthread_create

SSOT: `lib/loader/pe_native_pthread_real.hexa` 헤더 docstring.

**request payload**:
```
struct pthread_create_req {
    uint64  stack_size;     // byte (0 = default)
    int32   detach_state;   // 1=PTHREAD_CREATE_JOINABLE / 2=PTHREAD_CREATE_DETACHED
    int32   qos;            // 0=default / 1=QOS_CLASS_USER_INTERACTIVE / 2=USER_INITIATED / 3=UTILITY / 4=BACKGROUND
    uint64  start_routine;  // helper 측 callback (jump table id — 실 함수 ptr 직접 전달 불가)
    uint64  arg;            // void* — opaque
}
```

**response payload**:
```
struct pthread_create_resp {
    uint64  pthread_id;     // helper 가 부여한 opaque thread id — error 시 0
    int32   err;            // errno — success 시 0
}
```

Win32 → POSIX 매핑 메모:
- CreateThread `dwStackSize` → `pthread_attr_setstacksize`
- CREATE_SUSPENDED → helper 가 mutex 로 suspend, 별도 resume kind 로 unblock (현 spec v1 미포함 — v2 reserved)
- THREAD_PRIORITY_TIME_CRITICAL → `QOS_CLASS_USER_INTERACTIVE`

man page reference: `man 3 pthread_create`, `man 3 pthread_attr_setqos_np` (Apple).

---

### 3.5 kind=5 dlsym

`pe_native_dlsym_wrapper.hexa` 의 stage7 대응. real 모듈은 cycle 80 미생성 — 본 spec 이 선행 정의.

**request payload**:
```
struct dlsym_req {
    uint64  handle;     // dlopen response 의 native_handle
    uint32  name_len;
    char    name[name_len];     // utf-8, nul 미포함
}
```

**response payload**:
```
struct dlsym_resp {
    uint64  addr;       // 심볼 주소 — error 시 0
    int32   sym_kind;   // 1=normal / 2=forwarder / 3=ordinal_only / 4=not_found
    int32   err;        // errno — success 시 0
}
```

man page reference: `man 3 dlsym` (Apple).

---

### 3.6 kind=6 nsobject_lifetime

`pe_native_nsobject_lifetime.hexa` 의 stage7 대응. Objective-C `objc_msgSend` ABI 를 helper 가 dispatch.

**request payload**:
```
struct nsobj_req {
    int32   obj_kind;   // 1=NSString / 2=NSData / 3=NSDictionary / 4=NSArray / 5=NSError / 6=other
    int32   op;         // 1=alloc / 2=retain / 3=release / 4=copy / 5=autorelease
    uint64  target;     // op != alloc 일 때 대상 id, alloc 시 class id (1=NSString 등)
    uint32  init_arg_len;       // alloc + initWith* 시 추가 arg byte 수
    char    init_arg[init_arg_len];
}
```

**response payload**:
```
struct nsobj_resp {
    uint64  result_id;  // 결과 id (alloc / copy 시 새 id, retain/release 시 input 그대로)
    int32   ref_count;  // -1 = unknown (release 후 dealloc 가능성)
    int32   err;        // errno — success 시 0
}
```

reference: Apple `Foundation` framework header (`<Foundation/Foundation.h>`), Objective-C runtime `<objc/runtime.h>`.

---

### 3.7 kind=7 metal_device

`pe_native_metal_device_wrapper.hexa` 의 stage7 대응.

**request payload**:
```
struct metal_req {
    int32   op;         // 1=MTLCreateSystemDefaultDevice / 2=newCommandQueue / 3=release
    uint64  target;     // op=2,3 시 device handle / op=1 시 0
}
```

**response payload**:
```
struct metal_resp {
    uint64  device_handle;      // op=1: device id / op=2: command queue id
    int32   err;                // errno — success 시 0
}
```

reference: Apple `Metal.framework` (`<Metal/Metal.h>`) — `MTLCreateSystemDefaultDevice()`, `-[id<MTLDevice> newCommandQueue]`.

---

### 3.8 kind=8 signal_register

x86_64 SEH → arm64 unwind / signal trampoline. R4 (binary translation) 가 직접 의존하나 R1 prereq.

**request payload**:
```
struct signal_req {
    int32   signum;             // SIGSEGV=11, SIGBUS=10, SIGFPE=8, SIGILL=4
    uint64  handler_addr;       // helper 측 trampoline jump table id
    int32   sa_flags;           // SA_SIGINFO=64 | SA_ONSTACK=1 등
}
```

**response payload**:
```
struct signal_resp {
    uint64  prev_handler;       // 기존 핸들러 id (chain 위해)
    int32   err;                // errno — success 시 0
}
```

man page reference: `man 2 sigaction` (Apple).

---

## 4. helper.mm reference sample

> ⚠ 본 sample 은 **사용자 외부 결정 영역** 의 reference. 실 컴파일 / codesign / 배포는 사용자 책임.
> build: `clang++ -arch arm64 -fobjc-arc -framework Foundation -framework Metal helper.mm -o airgenome_helper`
> codesign: `codesign -s - --entitlements helper.entitlements --force airgenome_helper`

```objc
// helper.mm — airgenome stage7 IPC helper (reference sample)
// own 1: Apple SDK only (Foundation, Metal, dlfcn, sys/mman, pthread, signal)
//        Wine / CrossOver / GPTK / Whisky 인용 0 줄.

#import <Foundation/Foundation.h>
#import <Metal/Metal.h>
#include <dlfcn.h>
#include <sys/mman.h>
#include <pthread.h>
#include <signal.h>
#include <unistd.h>
#include <errno.h>
#include <stdint.h>
#include <string.h>

// ─── frame layout ───
#define AHI1_MAGIC   "AHI1"
#define AHI1_HDR_LEN 24

typedef struct {
    char     magic[4];
    uint32_t kind;
    uint64_t seq;
    uint32_t payload_len;
    uint32_t reserved;
} __attribute__((packed)) ahi1_hdr_t;

// ─── kind enumeration (spec § 3) ───
enum {
    AHI1_DLOPEN          = 1,
    AHI1_MMAP            = 2,
    AHI1_MPROTECT        = 3,
    AHI1_PTHREAD_CREATE  = 4,
    AHI1_DLSYM           = 5,
    AHI1_NSOBJECT        = 6,
    AHI1_METAL_DEVICE    = 7,
    AHI1_SIGNAL_REGISTER = 8,
};

// ─── frame read/write ───
static int read_full(int fd, void *buf, size_t n) {
    size_t got = 0;
    while (got < n) {
        ssize_t r = read(fd, (char*)buf + got, n - got);
        if (r <= 0) return -1;
        got += (size_t)r;
    }
    return 0;
}

static int write_full(int fd, const void *buf, size_t n) {
    size_t put = 0;
    while (put < n) {
        ssize_t w = write(fd, (const char*)buf + put, n - put);
        if (w <= 0) return -1;
        put += (size_t)w;
    }
    return 0;
}

static int read_frame(ahi1_hdr_t *hdr, void *payload_buf, size_t payload_cap) {
    if (read_full(STDIN_FILENO, hdr, AHI1_HDR_LEN) != 0) return -1;
    if (memcmp(hdr->magic, AHI1_MAGIC, 4) != 0) return -2;
    if (hdr->payload_len > payload_cap) return -3;
    if (hdr->payload_len > 0) {
        if (read_full(STDIN_FILENO, payload_buf, hdr->payload_len) != 0) return -1;
    }
    return 0;
}

static int write_frame(uint32_t kind, uint64_t seq, const void *payload, uint32_t plen) {
    ahi1_hdr_t hdr;
    memcpy(hdr.magic, AHI1_MAGIC, 4);
    hdr.kind = kind;
    hdr.seq = seq;
    hdr.payload_len = plen;
    hdr.reserved = 0;
    if (write_full(STDOUT_FILENO, &hdr, AHI1_HDR_LEN) != 0) return -1;
    if (plen > 0 && write_full(STDOUT_FILENO, payload, plen) != 0) return -1;
    return 0;
}

// ─── kind=1 dlopen handler ───
static void handle_dlopen(uint64_t seq, const uint8_t *p, uint32_t plen) {
    if (plen < 8) { /* error path elided */ return; }
    int32_t  flags;    memcpy(&flags, p, 4);
    uint32_t path_len; memcpy(&path_len, p + 4, 4);
    if (8 + path_len > plen) return;

    char path[4096];
    if (path_len >= sizeof(path)) return;
    memcpy(path, p + 8, path_len);
    path[path_len] = '\0';

    int rtld_flags = 0;
    if (flags & 1) rtld_flags |= RTLD_LAZY;
    if (flags & 2) rtld_flags |= RTLD_NOW;
    if (flags & 4) rtld_flags |= RTLD_LOCAL;
    if (flags & 8) rtld_flags |= RTLD_GLOBAL;

    void *h = dlopen(path[0] == '\0' ? NULL : path, rtld_flags);
    int32_t handle_kind;
    if (h != NULL) {
        handle_kind = (path[0] == '\0') ? 2 : 1;
    } else {
        handle_kind = 3;
    }

    uint8_t resp[256];
    uint32_t off = 0;
    memcpy(resp + off, &handle_kind, 4); off += 4;
    uint64_t nh = (uint64_t)(uintptr_t)h;
    memcpy(resp + off, &nh, 8); off += 8;
    int32_t err = (h == NULL) ? errno : 0;
    memcpy(resp + off, &err, 4); off += 4;
    const char *emsg = (h == NULL) ? dlerror() : "";
    uint32_t emsg_len = (uint32_t)strlen(emsg);
    if (off + 4 + emsg_len > sizeof(resp)) emsg_len = (uint32_t)(sizeof(resp) - off - 4);
    memcpy(resp + off, &emsg_len, 4); off += 4;
    memcpy(resp + off, emsg, emsg_len); off += emsg_len;

    write_frame(AHI1_DLOPEN, seq, resp, off);
}

// ─── kind=2 mmap handler ───
static void handle_mmap(uint64_t seq, const uint8_t *p, uint32_t plen) {
    if (plen < 24) return;
    uint64_t length;    memcpy(&length, p, 8);
    int32_t  prot;      memcpy(&prot, p + 8, 4);
    int32_t  flags;     memcpy(&flags, p + 12, 4);
    uint64_t addr_hint; memcpy(&addr_hint, p + 16, 8);

    // Apple Silicon W^X enforce
    int rprot = 0, rflags = 0;
    if (prot & 1) rprot |= PROT_READ;
    if (prot & 2) rprot |= PROT_WRITE;
    if (prot & 4) rprot |= PROT_EXEC;
    if ((rprot & PROT_WRITE) && (rprot & PROT_EXEC)) {
        // W^X 위반 — MAP_JIT 사용 안내, 일단 EACCES
        uint8_t resp[12]; uint64_t z = 0; int32_t e = EACCES;
        memcpy(resp, &z, 8); memcpy(resp + 8, &e, 4);
        write_frame(AHI1_MMAP, seq, resp, 12);
        return;
    }
    if (flags & 2)    rflags |= MAP_PRIVATE;
    if (flags & 16)   rflags |= MAP_FIXED;
    if (flags & 2048) rflags |= MAP_JIT;
    if (flags & 4096) rflags |= MAP_ANON;

    void *addr = mmap((void*)(uintptr_t)addr_hint, (size_t)length, rprot, rflags, -1, 0);
    uint8_t resp[12];
    uint64_t a = (addr == MAP_FAILED) ? 0 : (uint64_t)(uintptr_t)addr;
    int32_t  e = (addr == MAP_FAILED) ? errno : 0;
    memcpy(resp, &a, 8); memcpy(resp + 8, &e, 4);
    write_frame(AHI1_MMAP, seq, resp, 12);
}

// ─── kind=3 mprotect handler (jit_toggle path 분리) ───
static void handle_mprotect(uint64_t seq, const uint8_t *p, uint32_t plen) {
    if (plen < 24) return;
    uint64_t addr;       memcpy(&addr, p, 8);
    uint64_t length;     memcpy(&length, p + 8, 8);
    int32_t  prot;       memcpy(&prot, p + 16, 4);
    int32_t  jit_toggle; memcpy(&jit_toggle, p + 20, 4);

    int rc = 0;
    if (jit_toggle == 1) {
        pthread_jit_write_protect_np(0);  // RW
    } else if (jit_toggle == 2) {
        pthread_jit_write_protect_np(1);  // RX
    } else {
        int rprot = 0;
        if (prot & 1) rprot |= PROT_READ;
        if (prot & 2) rprot |= PROT_WRITE;
        if (prot & 4) rprot |= PROT_EXEC;
        rc = mprotect((void*)(uintptr_t)addr, (size_t)length, rprot);
    }
    uint8_t resp[8];
    int32_t success = (rc == 0) ? 1 : 0;
    int32_t e = (rc == 0) ? 0 : errno;
    memcpy(resp, &success, 4); memcpy(resp + 4, &e, 4);
    write_frame(AHI1_MPROTECT, seq, resp, 8);
}

// ─── kind=4..8 handler skeleton — 사용자 외부 영역 구현 ───
static void handle_pthread_create(uint64_t seq, const uint8_t *p, uint32_t plen) { /* TODO */ }
static void handle_dlsym         (uint64_t seq, const uint8_t *p, uint32_t plen) { /* TODO */ }
static void handle_nsobject      (uint64_t seq, const uint8_t *p, uint32_t plen) { /* TODO */ }
static void handle_metal_device  (uint64_t seq, const uint8_t *p, uint32_t plen) { /* TODO */ }
static void handle_signal_register(uint64_t seq, const uint8_t *p, uint32_t plen) { /* TODO */ }

// ─── main loop ───
int main(int argc, char *argv[]) {
    @autoreleasepool {
        ahi1_hdr_t hdr;
        uint8_t payload[65536];
        for (;;) {
            int rc = read_frame(&hdr, payload, sizeof(payload));
            if (rc != 0) break;  // EOF / bad magic / overflow → exit
            switch (hdr.kind) {
                case AHI1_DLOPEN:          handle_dlopen(hdr.seq, payload, hdr.payload_len); break;
                case AHI1_MMAP:            handle_mmap(hdr.seq, payload, hdr.payload_len); break;
                case AHI1_MPROTECT:        handle_mprotect(hdr.seq, payload, hdr.payload_len); break;
                case AHI1_PTHREAD_CREATE:  handle_pthread_create(hdr.seq, payload, hdr.payload_len); break;
                case AHI1_DLSYM:           handle_dlsym(hdr.seq, payload, hdr.payload_len); break;
                case AHI1_NSOBJECT:        handle_nsobject(hdr.seq, payload, hdr.payload_len); break;
                case AHI1_METAL_DEVICE:    handle_metal_device(hdr.seq, payload, hdr.payload_len); break;
                case AHI1_SIGNAL_REGISTER: handle_signal_register(hdr.seq, payload, hdr.payload_len); break;
                default: /* unknown kind — drop */ break;
            }
        }
    }
    return 0;
}
```

LOC: ≈ 230 라인 reference. real 구현 시 4..8 handler 충실 + error frame 처리 + signal forwarding 추가.

---

## 5. entitlement plist

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
    <!-- dyld interposing 허용 (DYLD_INSERT_LIBRARIES path 의존 시) -->
    <key>com.apple.security.cs.allow-dyld-environment-variables</key>
    <true/>
    <!-- library validation 우회 — DLL→dylib path 는 ad-hoc sign -->
    <key>com.apple.security.cs.disable-library-validation</key>
    <true/>
</dict>
</plist>
```

적용:
- ad-hoc dev: `codesign -s - --entitlements helper.entitlements --force airgenome_helper`
- distribution: Developer ID + notarization 별도 path

---

## 6. security boundary

- **helper 권한**: parent process 와 동일 sandbox. helper 가 별도 권한 상승 X.
- **JIT 허용 범위**: `MAP_JIT` mmap + `pthread_jit_write_protect_np` 토글 path 만. 일반 `mprotect(RWX)` 는 helper 가 거부 (W^X enforcement).
- **signal handler**: `SIGSEGV` / `SIGBUS` / `SIGFPE` / `SIGILL` 등 helper trampoline 등록 가능 — 그러나 parent 의 signal mask 분리 (`pthread_sigmask`) 권장.
- **fd inheritance**: helper spawn 시 stdin/stdout 외 모든 fd `O_CLOEXEC` 설정. parent 가 stdin/stdout 을 dup2 로 명시 전달.
- **payload 검증**: helper 는 `payload_len <= 65536` 강제 + frame magic 불일치 시 즉시 exit. malformed frame 은 RCE vector 가능성 있어 strict.
- **code signing**: ad-hoc (`-s -`) 가능 — distribution 시 Developer ID + notarization. user-installed binary 라면 quarantine bit (`xattr -d com.apple.quarantine`) 처리 필요.
- **own1 enforcement**: helper.mm 자체 구현. Wine `wineserver` / CrossOver `WINELOADER` / GPTK `D3DMetal` / Whisky launcher 코드 0 줄.

---

## 7. 후속 작업 (사용자 외부 결정 영역)

본 spec 이 일원화한 정의를 기반으로 사용자가 직접 수행:

1. **helper.mm 실 구현** — § 4 reference sample 의 TODO 4..8 handler 충실 + error frame path
2. **Xcode/clang build** — `clang++ -arch arm64 -fobjc-arc -framework Foundation -framework Metal helper.mm -o airgenome_helper`
3. **entitlement plist 적용** — § 5 plist 저장 후 codesign
4. **codesign** — ad-hoc 또는 Developer ID
5. **airgenome-gamebox 의 helper subprocess spawn path** — `lib/loader/pe_native_helper_invoke.hexa` 와 결합 (현재 dispatch gate 만 존재, 실제 spawn 미구현)
6. **frame round-trip 통합 테스트** — hexa side `pe_native_dlopen_real.record_invoke` 와 helper.mm response 정합 확인

---

## 8. 모듈 정합 체크리스트

| 모듈 | header docstring kind 정의 | § 3 spec 정합 |
|---|---|---|
| `pe_native_dlopen_real.hexa` | kind="dlopen", flags+path → handle_kind+native_handle+errno | ✓ § 3.1 |
| `pe_native_mmap_real.hexa` | kind="mmap", length+prot+flags+addr_hint → addr+errno | ✓ § 3.2 |
| `pe_native_mprotect_real.hexa` | kind="mprotect", addr+length+prot → success+errno | ✓ § 3.3 (+ jit_toggle 확장) |
| `pe_native_pthread_real.hexa` | kind="pthread_create", stack_size+detached+qos → pthread_id+errno | ✓ § 3.4 (+ start_routine, arg) |

선행 정의 (real 모듈 cycle 80 미생성):
- § 3.5 dlsym — `pe_native_dlsym_wrapper.hexa` 의 stage7 대응
- § 3.6 nsobject — `pe_native_nsobject_lifetime.hexa` 의 stage7 대응
- § 3.7 metal_device — `pe_native_metal_device_wrapper.hexa` 의 stage7 대응
- § 3.8 signal_register — R4 (binary translation) 직접 의존
