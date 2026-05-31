# GAMEBOX-NET — current state
@title: 🌐 GAMEBOX-NET — "네트워크 프로토콜"

@goal: Networking/IPC protocol layer: winsock/IOCP, IPC (mach port/xpc/named pipe/shared mem), TLS (schannel/apple-network), OAuth, and TACT/NGDP/CASC content delivery primitives shared by launchers.

(edit me — describe current state in completed-form; no history, no changelog inside this file)
- [ ] winsock ws2_32 + IOCP/overlapped/select + iphlpapi
- [ ] IPC matrix: mach port, xpc, unix socket, named pipe, shared memory, tcp loopback
- [ ] TLS: schannel + apple-network HTTP/TLS sink + https certificate
- [ ] OAuth device/PKCE flow + redirect capture proxy
- [ ] TACT/NGDP/CASC parser + CDN chunk assemble/dedup/resume
