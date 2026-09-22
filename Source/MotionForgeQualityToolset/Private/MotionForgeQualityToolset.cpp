#include "MotionForgeQualityToolset.h"

#include "MotionQualitySubsystem.h"

#include "Kismet/KismetSystemLibrary.h"
#include "Interfaces/IPluginManager.h"

FMotionPenetrationReport UMotionForgeQualityToolset::MeasureMotionPenetration(
	const FString& SequencePath, const FString& TargetMeshPath, const FString& PhysicsAssetPath,
	float ToleranceCm, bool bRespectDisableTable, int32 MinJointSeparation)
{
	UMotionQualitySubsystem* Quality = UMotionQualitySubsystem::Get();

	if (Quality == nullptr)
	{
		UKismetSystemLibrary::RaiseScriptError(TEXT(
			"MotionForge Quality is not available. The plugin is disabled, or this is not an editor "
			"session."));

		return FMotionPenetrationReport();
	}

	FMotionPenetrationReport Report = Quality->MeasurePenetration(
		SequencePath, TargetMeshPath, PhysicsAssetPath, ToleranceCm, bRespectDisableTable,
		MinJointSeparation);

	// Raised rather than returned quietly: a report of no penetration and a report that nothing could
	// be measured look identical in every field that matters.
	if (!Report.bMeasured)
	{
		UKismetSystemLibrary::RaiseScriptError(Report.Error);
	}

	return Report;
}

FMotionDepenetrateResult UMotionForgeQualityToolset::CorrectMotionPenetration(
	const FString& SequencePath, const FString& TargetMeshPath, const FString& PhysicsAssetPath,
	const FString& OutputPath, float ToleranceCm, float MarginCm, float MaxTurnDegrees)
{
	UMotionQualitySubsystem* Quality = UMotionQualitySubsystem::Get();

	if (Quality == nullptr)
	{
		UKismetSystemLibrary::RaiseScriptError(TEXT("MotionForge Quality is not available."));
		return FMotionDepenetrateResult();
	}

	FMotionDepenetrateResult Result = Quality->CorrectPenetration(
		SequencePath, TargetMeshPath, PhysicsAssetPath, OutputPath, ToleranceCm, MarginCm,
		MaxTurnDegrees);

	if (!Result.bCorrected)
	{
		UKismetSystemLibrary::RaiseScriptError(Result.Error);
	}

	return Result;
}

FMotionProxyResult UMotionForgeQualityToolset::CreateMotionMeasurementProxy(
	const FString& MeshPath, const FString& OutputPath, float MinBoneSizeCm, bool bOverwrite)
{
	UMotionQualitySubsystem* Quality = UMotionQualitySubsystem::Get();

	if (Quality == nullptr)
	{
		UKismetSystemLibrary::RaiseScriptError(TEXT("MotionForge Quality is not available."));
		return FMotionProxyResult();
	}

	FMotionProxyResult Result =
		Quality->CreateMeasurementProxy(MeshPath, OutputPath, MinBoneSizeCm, bOverwrite);

	if (!Result.bCreated)
	{
		UKismetSystemLibrary::RaiseScriptError(Result.Error);
	}

	return Result;
}

FString UMotionForgeQualityToolset::GetToolsetVersion() const
{
	// The descriptor is the version. Reading it here rather than repeating it means there is no
	// second copy to keep true - and no window, between a bump and a fix, where an agent is told
	// a number the package does not carry.
	const TSharedPtr<IPlugin> Plugin = IPluginManager::Get().FindPlugin(TEXT(UE_PLUGIN_NAME));
	return Plugin.IsValid() ? Plugin->GetDescriptor().VersionName : FString();
}
