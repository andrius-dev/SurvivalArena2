// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class TopDown2 : ModuleRules {

	public TopDown2(ReadOnlyTargetRules Target) : base(Target) {
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new[] {
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"NavigationSystem",
			"AIModule",
			"Niagara",
			"EnhancedInput"
		});
	}

}