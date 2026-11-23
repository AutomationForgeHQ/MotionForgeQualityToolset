<!-- kovati:header -->
<p align="center"><a href="https://kovati.dev/tools/motionforge/"><img src="https://raw.githubusercontent.com/AutomationForgeHQ/.github/main/profile/assets/motionforge.jpg" width="100%" alt="MotionForge, part of Kovati" /></a></p>

<p align="center"><b>Agent toolset for MotionForge</b> · Part of <a href="https://kovati.dev"><b>Kovati</b></a>, production tools for Unreal games · <a href="https://kovati.dev/animation/">Animation</a><br />
<a href="https://kovati.dev/tools/motionforge/">Product page</a> · <a href="https://docs.kovati.dev">Documentation</a> · <a href="https://kovati.dev/download/">Install with the Kovati Hub</a> · <a href="https://kovati.dev/discord">Discord</a></p>
<!-- /kovati:header -->

# MotionForge Quality Toolset

[MotionForge Quality](https://kovati.dev/plugins/motionforge/) as Model Context Protocol tools.

A surface and nothing else: every function forwards to `UMotionQualitySubsystem` and adds no logic,
no state and no safety of its own. Delete this plugin and the checks behave exactly as they did.

```
ToolsetRegistry  →  MotionForgeQualityToolset.MotionForgeQualityToolset
```

## The tools

| Tool | What it does |
|---|---|
| `CreateMotionMeasurementProxy` | Builds a physics asset dense enough to measure against. Writes an asset; a one-off per character. |
| `MeasureMotionPenetration` | Reads and writes nothing. Safe over a whole library. |
| `CorrectMotionPenetration` | Writes a new clip. **Parked** — see the capability's README. |

## Why it ships separately from the MotionForge toolset

The capability it wraps is an add-on somebody may not have installed, and an adapter that fails to
load takes every other tool in its module down with it. Same rule as every other toolset here: the
adapter is its own plugin, depends on `ToolsetRegistry` rather than `ModelContextProtocol`, and is
disabled by default.

## Reading a result

`surfaceCoveredPercent` first, always. It says how much of the character hangs off a bone that has a
collision body at all — under about 90%, a clean result is not a result, and the tool says so in its
notes. A character's own ragdoll typically covers a third of it.

Errors are raised rather than returned in an envelope, so a measurement that could not run reads as a
failure instead of as zero penetration.
