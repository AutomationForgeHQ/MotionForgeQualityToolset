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
