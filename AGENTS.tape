#!/usr/bin/env tape
# ══════════════════════════════════════════════════════════════════════
# .tape v1.2 — grammar primer (cold-read by any LLM / agent / human)
# ══════════════════════════════════════════════════════════════════════
# Form: each entry is `@<type> <id> := "<subject>" :: <kind> [<grades>]`
#       optionally followed by body lines (2-space indent) — key=value,
#       edges (<- -> => …), or quoted prose.
#
# ENTRY TYPES (17):
#   Runtime (10): @S session  @U user  @A assistant  @T tool-call  @R result
#                 @H hook  @D decision  @K cost  @P provider  @? anomaly
#   Foundation:   @I identity-claim
#   Declarative:  @X external-citation  @F forbidden-pattern  @N note
#                 @C config  @L layout  @V spec-version
#
# EDGES (12, on body lines, 2-space indent):
#   Causal:     <- caused-by   -> triggers   == continues
#               ~> supersedes  !! aborts     |> verified-by
#   Effect:     => produces (followed by quoted natural-language)
#   Structural: <: specializes  :> generalizes  ?> soft-depends
#               !> conflicts-with  @> projects-to (e.g. @> AGENTS.md)
#
# GRADE TAGS (in [...] on entry header):
#   Delivery:   ok · err · denied · cancelled · partial · superseded
#   Governance: required · recommended · optional · draft · active · deprecated
#   Scoped:     allow:<x> · deny:<x>
#   Time/index: T<n> (turn)  N<n> (wall-sec)  d=<YYYY-MM-DD>
#
# PAYLOAD SYNTAX (body lines):
#   key = "value"           single-line string
#   key = [a, b, c]         array literal
#   key <<~EOF              heredoc (multi-line; ends with EOF on own line)
#     multi-line text
#   EOF
#   "see [@x1] §3"          inline citation by entry id
#   `cmd or path`           backtick code-span (inline only)
#
# Full spec: ~/core/tape/spec/tape.md (or github.com/dancinlab/tape)
# ══════════════════════════════════════════════════════════════════════
#
# AGENTS.tape for `dancinlab/gamebox`

@V := "tape" :: spec [active]
  version = "1.2"

@X wilson_pool := "wilson pool — resource utilization" :: guide [active]
  tldr = "Heavy / deterministic work (build · drill · probe · compile · test-sweep · GPU fire) → do NOT serialize on localhost; route to the host pool: `wilson pool list` then `wilson pool on <host> <cmd>`, or a ToolUse `\"host\"` field. Empty roster = degrade-to-local, fine."
  detail = "~/core/atlas/POOL.tape"

@I id001 := "gamebox" :: identity-claim [d=2026-05-14 active]
  kind = "dancinlab project (gamebox)"
  brief = "dancinlab gamebox project"
  parent = "dancinlab"

@L l1 := "repo-layout" :: structure [active]
  .checkpoint.d2r_battlenet_offline_play -> "file"
  .checkpoint.d2r_battlenet_online_play -> "file"
  .checkpoint.lineage_classic_r28_play -> "file"
  .checkpoint.lineage_m_purple_play -> "file"
  .checkpoint.lineage_w_purple_play -> "file"
  .checkpoint.win32_50pct_coverage -> "file"
  .checkpoint.win32_full_coverage -> "file"
  .roadmap.battlenet -> "file"
  .roadmap.cs2 -> "file"
  .roadmap.delta_force -> "file"
  .roadmap.diablo2_resurrected -> "file"
  .roadmap.diablo4 -> "file"
  .roadmap.elden_ring -> "file"
  .roadmap.elden_ring_nightreign -> "file"
  .roadmap.lineage_classic_r28 -> "file"
  .roadmap.lineage_m -> "file"
  .roadmap.lineage_w -> "file"
  .roadmap.loader_dx -> "file"
  .roadmap.loader_pe -> "file"
  .roadmap.loader_win32 -> "file"
  .roadmap.purple_launcher -> "file"
  .roadmap.wow -> "file"
  AGENTS.tape -> "this file · governance + identity (.tape v1.2)"
  CHANGELOG.md -> "change log"
  CITATION.cff -> "file"
  CLAUDE.md -> "symlink → AGENTS.tape"
  hexa.toml -> "TOML config"
  install.hexa -> "hexa-lang source"
  LICENSE -> "license file"
  README.md -> "human intro · 18-block format (atlas/README-FORMAT.md)"
  RELEASE_NOTES_v1.0.0.md -> "documentation"
  RELEASE_NOTES_v1.0.1.md -> "documentation"
  RELEASE_NOTES_v1.0.2.md -> "documentation"
  RELEASE_NOTES_v1.0.3.md -> "documentation"
  RELEASE_NOTES_v1.0.4.md -> "documentation"
  TAPE-AUDIT.md -> ".tape v1.x adoption audit ledger"
  .claude/ -> "Claude Code state (gitignored typically)"
  airgenome/ -> "subdir (1 items)"
  cli/ -> "subdir (1 items)"
  docs/ -> "documentation (129 items)"
  gamebox/ -> "subdir (1 items)"
  lib/ -> "library code (4 items)"
  native/ -> "subdir (11 items)"
  state/ -> "runtime state (gitignored typically) (1 items)"
  tests/ -> "test suite (9 items)"
  tool/ -> "tool helpers (3 items)"

@D g_inherit := "inherits-lattice-policy" :: governance [required]
  rule = "All echoes/LATTICE_POLICY.md rules apply — real-limits-first · lattice-is-tool · verification-anchor-real-limit · no-artificial-ceilings · honesty-obligation-external."
  authority = "[@x_lattice_policy]"
  @> AGENTS.md

@D g1 := "real-limits-first" :: governance [required]
  rule = "The project's ceiling is set by REAL math/physics/engineering limits, NEVER by the n=6 invariant lattice."
  authority = "[@x_lattice_policy]"
  @> AGENTS.md

@D g2 := "lattice-is-tool" :: governance [required]
  rule = "n=6 lattice is a TOOL, not a CONSTRAINT. Use only where it naturally fits."
  <: g1
  @> AGENTS.md

@D g3 := "honesty-obligation-external" :: governance [required deny:write]
  rule = "Claims about external entities (companies · fabs · vendors) must NOT include lattice-fit assertions. Use their OWN invariants."
  @> AGENTS.md

@D g4 := "commit-conventions" :: governance [recommended]
  rule = "AI-assisted commits include \`Co-Authored-By: <model> <noreply@anthropic.com>\` trailer; Conventional Commits title."
  @> AGENTS.md

@D g_arch_vs_log_split := "v1.2 architecture-vs-history split" :: governance [required d=2026-05-14 active]
  rule = "UPPERCASE.tape files are EDITABLE architecture (latest-wins per id). Append-only event history lives in UPPERCASE.log.tape siblings."
  why = "declarative entries (governance · config · layout · identity) evolve naturally; locking them as append-only via ~> supersedes creates noisy histories. The split lets architecture stay current and history stay immutable."
  applies-to = "AGENTS.tape · all UPPERCASE.tape root files"
  authority = "[@x_tape_spec] §Domain-tape sibling §Architecture-vs-history split"
  @> AGENTS.md


@F f1 := "lattice-fit-on-external-entity" :: governance [required deny:write]
  pattern = "applying n=6 lattice as derivation rule to TSMC · ASML · IPCC · NASA · SpaceX · CERN · DeepMind · NIST · or any external vendor"
  exception = "values may coincidentally match — observation OK, derivation NOT OK"

@X x_lattice_policy := "echoes/LATTICE_POLICY.md" :: policy [d=2026-05-12 active]
  url = "https://github.com/dancinlab/echoes/blob/main/LATTICE_POLICY.md"

@X x_tape_spec := "tape v1.2 spec" :: spec [d=2026-05-14 active]
  url = "https://github.com/dancinlab/tape/blob/main/spec/tape.md"
  scope = "this file's grammar reference"

@H h1 := "agents-md-replaced" :: generator [active]
  note = "AGENTS.md replaced by this AGENTS.tape (tape v1.2)"

@H h2 := "claude-md-symlink" :: generator [active]
  rule = "CLAUDE.md → AGENTS.tape symlink"
  command = `ln -sf AGENTS.tape CLAUDE.md`
