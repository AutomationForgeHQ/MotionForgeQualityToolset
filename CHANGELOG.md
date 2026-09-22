# MotionForgeQualityToolset

Every released version of MotionForgeQualityToolset, newest first. A release publishes **one** section of this
file — the one whose heading matches its tag — as its release notes; for an `open` plugin those
notes are posted to Discord `#releases` automatically. Write for someone who installs the plugin,
not for the commit log.

Headings are `## <x.y.z> — <date>`. Use `Added` / `Changed` / `Fixed` / `Compatibility` /
`Known issues`, only the ones that apply.

## 0.1.2 — 2026-09-22

### Changed
- The version this toolset reports to an agent is now read from the plugin's own
  descriptor rather than repeated in C++, so it can no longer answer a number the
  installed package does not carry.

### Compatibility
- Rebuilt against MotionForgeQuality 0.1.2, which it needs.

## 0.1.1 — 2026-09-08
- Packaging fix: the release now carries everything the register allows. `BuildPlugin`'s filter excludes `Config/` and every `public_extra` path, so earlier zips shipped without them.
- The package now carries its Apache-2.0 LICENSE, which earlier zips omitted.
- `GetToolsetVersion()` answers this plugin's real version; it had drifted from the descriptor.

## 0.1.0 — 2026-08-28
- Quality becomes its own add-on, beside the providers rather than inside core
- Getting the limb back out, and refusing when it cannot
- The proxy could see 38.8% of the character, and said nothing about it
- A proxy that can see the whole character, calibrated against itself
- Write down what was learned, and park the corrector
