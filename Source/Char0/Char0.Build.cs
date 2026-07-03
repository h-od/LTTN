// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Char0 : ModuleRules
{
	public Char0(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange([
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"UMG",
			"EnhancedInput",
			"MotionWarping",
			"AnimGraphRuntime",
			"AnimationWarpingRuntime",
			"AnimGraphRuntime",
			"AnimationWarpingRuntime",
			"AIModule",
			"GameplayStateTreeModule",
			"StateTreeModule",
			"Slate",
			"GameplayTags",
			"Chooser",
			"BlendStack",
			"PoseSearch",
			"NavigationSystem"
		]);

		PrivateDependencyModuleNames.AddRange(["Niagara"]);

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}