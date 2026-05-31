# GAMEBOX-PERF — current state
@title: 📊 GAMEBOX-PERF — "성능 계기판"

@goal: Performance + telemetry + presentation: FPS overlay (Metal-composed), frame pacing, display modes (fullscreen/PIP/dpi), Metal optimization tracks, and per-session aggregation.

(edit me — describe current state in completed-form; no history, no changelog inside this file)
- [ ] FPS overlay: metal compose + history graph + hotkey + metric aggregator
- [ ] Frame pacing adaptive + frametime histogram + first-frame breakdown
- [ ] Display modes: fullscreen-exclusive Metal, PIP, dpi scale, focus routing, macOS menubar
- [ ] Metal opt: pipeline archive, private-heap prewarm, residency coalesce, simdgroup reduce
- [ ] Session aggregator + roofline/thermal/vram pressure forecast
