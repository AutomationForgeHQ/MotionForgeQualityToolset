// MotionForge Quality, as tools an agent can call.

#pragma once

#include "CoreMinimal.h"
#include "MotionDepenetrate.h"
#include "MotionMeasurementProxy.h"
#include "MotionPenetration.h"
#include "ToolsetRegistry/ToolsetDefinition.h"
#include "MotionForgeQualityToolset.generated.h"

/**
 * Quality measurements over animation clips, exposed as Model Context Protocol tools.
 *
 * Forwards to UMotionQualitySubsystem and adds nothing. Signatures are the schema: parameters and
 * results are USTRUCTs, and these doc comments become what the agent reads before it calls anything,
 * so they are written for that reader rather than for us.
 *
 * Separate from the MotionForge toolset on purpose. The capability it wraps is an add-on somebody may
 * not have installed, and an adapter that fails to load takes every other tool in its module with it.
 */
UCLASS(BlueprintType)
class MOTIONFORGEQUALITYTOOLSET_API UMotionForgeQualityToolset : public UToolsetDefinition
{
	GENERATED_BODY()

public:

	virtual FString GetToolsetVersion() const override { return TEXT("0.1"); }

	/**
	 * Measure where a clip puts a limb inside its own body.
	 *
	 * The commonest visible defect in a generated clip, and invisible in everything a clip already
	 * reports about itself. Reads and writes nothing, so it is safe to run over a whole library.
	 *
	 * **It measures the physics proxy, not the skin** - capsules that stand a few centimetres away
	 * from the mesh - so treat the depth as an estimate of what a viewer would see. And **overlap is
	 * not automatically wrong**: folded arms rest on the chest, a hand sits on a hip. The tolerance is
	 * the line between limbs touching and a limb being inside the body, not a precision setting.
	 *
	 * Read `deepestCm` first, then `framesAffected` against `framesSampled`: a deep hit on a few frames
	 * is a moment to fix, and a shallow one on every frame is usually a physics asset that is slightly
	 * too fat for its mesh.
	 *
	 * **Read `surfaceCoveredPercent` before anything else.** It says how much of the character hangs
	 * off a bone that has a collision body at all. A physics asset is usually a ragdoll, and a ragdoll
	 * needs no fingers, no head and no feet - so a clip with its fingers buried in a thigh measures
	 * perfectly clean against one. Under about 90%, treat a clean result as no result and point this
	 * at a denser asset.
	 *
	 * @param SequencePath     The animation to measure.
	 * @param TargetMeshPath   The character the clip is played on. Empty uses the skeleton's preview
	 *        mesh. Its skin is what decides how much of the character is measurable.
	 * @param PhysicsAssetPath Bodies to test against. Empty uses the target mesh's own asset. A project
	 *        may keep several for one character - a ragdoll, a hit proxy, one dense enough to measure
	 *        against - and they are not interchangeable.
	 * @param ToleranceCm      Overlaps shallower than this are treated as contact and ignored.
	 * @param bRespectDisableTable Leave out the pairs the physics asset disables. **Say false unless you
	 *        know the asset**: that table says what a ragdoll should simulate against, and a ragdoll
	 *        commonly disables self-collision wholesale - honour it on such an asset and the measurement
	 *        tests nothing while reporting a perfectly clean clip.
	 * @param MinJointSeparation How many joints apart two bodies must be before their overlap is
	 *        counted. Three is right for a humanoid: below that the bodies belong to the same limb and
	 *        touch by design, so a bent knee reports itself. Raise it to quieten a noisy rig, and know
	 *        that raising it far enough will quieten real findings too.
	 */
	UFUNCTION(meta = (AICallable), Category = "MotionForge|Discovery")
	static FMotionPenetrationReport MeasureMotionPenetration(
		const FString& SequencePath, const FString& TargetMeshPath, const FString& PhysicsAssetPath,
		float ToleranceCm, bool bRespectDisableTable, int32 MinJointSeparation);

	/**
	 * Get the limbs back out of the body, and write the result to a new clip.
	 *
	 * **This writes an asset.** The source is never modified - the corrected clip is a new one, so
	 * the original stays as the record of what the model actually produced.
	 *
	 * Measure first and show the user the numbers. This is worth running when a clip has deep
	 * overlaps on a handful of frames; a clip that is wrong on every frame usually wants regenerating,
	 * and one whose deepest overlap is near the tolerance wants leaving alone.
	 *
	 * Read the result rather than trusting it: `before` and `after` are both real measurements, taken
	 * of the source and of the asset that was written, and a correction that made nothing better says
	 * so. `largestTurnDegrees` is the honesty check - a few degrees is a nudge nobody will see, twenty
	 * means the fix has rewritten the performance.
	 *
	 * @param SequencePath      The clip to correct.
	 * @param TargetMeshPath    The character it is played on. Empty uses the skeleton's preview mesh.
	 * @param PhysicsAssetPath  Bodies to test against. Empty uses the target mesh's own asset.
	 * @param OutputPath        Where to write. Empty writes beside the source with a `_Fixed` suffix.
	 * @param ToleranceCm       Overlaps shallower than this are contact and are left alone.
	 * @param MarginCm          How far past merely touching to push.
	 * @param MaxTurnDegrees    How far any one bone may turn on any one frame. Twelve is sensible;
	 *        much more and the fix is rewriting the performance rather than correcting it.
	 */
	UFUNCTION(meta = (AICallable), Category = "MotionForge|Quality")
	static FMotionDepenetrateResult CorrectMotionPenetration(
		const FString& SequencePath, const FString& TargetMeshPath, const FString& PhysicsAssetPath,
		const FString& OutputPath, float ToleranceCm, float MarginCm, float MaxTurnDegrees);

	/**
	 * Build a physics asset dense enough to measure against, fitted to one character.
	 *
	 * **This writes an asset, and it is a one-off.** A ragdoll and a measurement proxy are different
	 * objects: a ragdoll has few bodies, no fingers, no head and no feet because none of those help it
	 * simulate, which is exactly why measuring against one reports a clean clip with its hands inside
	 * its thighs.
	 *
	 * The result is never assigned to the mesh - the character keeps its ragdoll - so name this asset
	 * when measuring. Read `surfaceCoveredPercent` against `previousCoveredPercent` to see what it
	 * bought.
	 *
	 * Rebuild it only when the mesh changes shape. Nothing does that automatically, because a proxy
	 * that regenerated itself would quietly change what every past measurement meant.
	 *
	 * @param MeshPath      The character to build for.
	 * @param OutputPath    Where to write it, e.g. `/Game/Colony/Core/Physics/PA_Quinn_Measure`.
	 * @param MinBoneSizeCm Bones shorter than this get no body. One centimetre reaches fingers; the
	 *        engine's own default of twenty does not reach a forearm twist.
	 * @param bOverwrite    Replace an existing asset at that path.
	 */
	UFUNCTION(meta = (AICallable), Category = "MotionForge|Quality")
	static FMotionProxyResult CreateMotionMeasurementProxy(
		const FString& MeshPath, const FString& OutputPath, float MinBoneSizeCm, bool bOverwrite);
};
