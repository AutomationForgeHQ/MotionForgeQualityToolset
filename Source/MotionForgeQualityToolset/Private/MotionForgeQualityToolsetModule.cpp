#include "MotionForgeQualityToolsetModule.h"

#include "MotionForgeQualityToolset.h"
#include "ToolsetRegistry/UToolsetRegistry.h"

DEFINE_LOG_CATEGORY_STATIC(LogMotionForgeQualityToolset, Log, All);

void FMotionForgeQualityToolsetModule::StartupModule()
{
	if (!UToolsetRegistry::IsAvailable())
	{
		// Expected outside the editor and whenever the experimental plugins are off. Not an error.
		UE_LOG(LogMotionForgeQualityToolset, Log,
			TEXT("Toolset registry unavailable - motion quality tools not registered."));
		return;
	}

	if (!UToolsetRegistry::IsToolsetClassRegistered(UMotionForgeQualityToolset::StaticClass()))
	{
		UToolsetRegistry::RegisterToolsetClass(UMotionForgeQualityToolset::StaticClass());
	}

	UE_LOG(LogMotionForgeQualityToolset, Log, TEXT("Motion quality toolset %s."),
		UToolsetRegistry::IsToolsetClassRegistered(UMotionForgeQualityToolset::StaticClass())
			? TEXT("registered") : TEXT("failed to register"));
}

void FMotionForgeQualityToolsetModule::ShutdownModule()
{
	if (UToolsetRegistry::IsAvailable())
	{
		UToolsetRegistry::UnregisterToolsetClass(UMotionForgeQualityToolset::StaticClass());
	}
}

IMPLEMENT_MODULE(FMotionForgeQualityToolsetModule, MotionForgeQualityToolset);
