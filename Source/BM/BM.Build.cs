using UnrealBuildTool;

public class BM : ModuleRules
{
	public BM(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core", "CoreUObject", "Engine", "InputCore",
			"UMG", "Slate", "SlateCore", "EnhancedInput",
			"AnimGraphRuntime", "NetCore", "Chooser",
			"OnlineSubsystem",
            "OnlineSubsystemSteam",
            "OnlineSubsystemUtils",
			"GameplayAbilities",
			"GameplayTasks", "RetSteam", "GameplayTags",
			"AIModule"
		});

		PrivateDependencyModuleNames.AddRange(new string[] {
			"Eigen",
			"AnimationLocomotionLibraryRuntime",
			"OnlineSubsystemUtils",
			// 暂时不要启用 Advanced*，先把自带节点编过
			"GameplayAbilities",
			"GameplayTasks",
		});
	}
}