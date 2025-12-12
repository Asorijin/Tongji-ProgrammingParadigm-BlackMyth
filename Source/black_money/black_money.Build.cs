// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class black_money : ModuleRules
{
	public black_money(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput" ,"Json"});
	}
}
