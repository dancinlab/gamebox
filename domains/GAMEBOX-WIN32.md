# GAMEBOX-WIN32 — current state
@title: 🪟 GAMEBOX-WIN32 — "윈도우 API 통역"

@goal: hexa-native Win32 API shim across ~14 DLLs (kernel32/user32/gdi32/ntdll/advapi32/winsock/ole32/…) — real implementations over stubs, Wine=0.

(edit me — describe current state in completed-form; no history, no changelog inside this file)
- [ ] kernel32 stage1→5: CreateProcess/CreateThread/Heap* real (not stub)
- [ ] user32 + gdi32 tier1 real: window/message/DC/GDI object
- [ ] ntdll native API: NtCreateFile/Event/Mutant/Semaphore/Thread real
- [ ] advapi32/crypt32/ole32/oleaut32/shell32 tier1 real coverage
- [ ] winsock ws2_32 + IOCP/overlapped/select surface
