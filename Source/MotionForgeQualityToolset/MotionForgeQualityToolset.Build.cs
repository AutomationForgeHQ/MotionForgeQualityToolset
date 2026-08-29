using UnrealBuildTool;

public class MotionForgeQualityToolset : ModuleRules
{
	public MotionForgeQualityToolset(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"CoreUObject",
				"Engine",
				"ToolsetRegistry",      // UToolsetDefinition and UAgentSkill are public base classes
				"MotionForgeQuality",   // the capability, and the types in these signatures
			}
			);

		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"UnrealEd",   // GEditor, to reach the subsystem
			}
			);
	}
}
