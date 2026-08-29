#pragma once

#include "Modules/ModuleManager.h"

/** Registers the quality toolset with ToolsetRegistry, and nothing else. */
class FMotionForgeQualityToolsetModule : public IModuleInterface
{
public:

	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
