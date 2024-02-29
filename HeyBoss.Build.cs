// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class HeyBoss : ModuleRules
{
	public HeyBoss(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", 
			"Engine", "InputCore", "HeadMountedDisplay", "EnhancedInput",
			"UMG", "Slate", "SlateCore", "AIModule", "GameplayTasks", 
			"NavigationSystem", "Niagara", "MediaAssets", "MotionWarping",
			"AnimationWarpingEditor"});
	}
}
