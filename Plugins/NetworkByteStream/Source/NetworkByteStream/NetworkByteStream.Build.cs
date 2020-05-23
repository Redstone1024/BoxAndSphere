// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class NetworkByteStream : ModuleRules
{
	public NetworkByteStream(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
            new string[] 
            {
                "Core",
            }
            );

        PrivateDependencyModuleNames.AddRange(
            new string[] 
            {
                "CoreUObject",
                "Engine",
                "InputCore",
                "HeadMountedDisplay",
                "Sockets",
                "Networking",
            }
            );
        
    }
}
