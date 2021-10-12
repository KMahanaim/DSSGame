// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class DarkSoul : ModuleRules
{
	public DarkSoul(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] 
        {
            /*�⺻ ���*/
            "Core",
            "CoreUObject",
            "Engine",
            "InputCore", 

            /*�߰��� ���*/
            "NavigationSystem",
            "AIModule",
            "GameplayTasks",
            "HeadMountedDisplay",
            "UMG",
            "Slate",
            "SlateCore", 
            "Niagara", 
        });

        PrivateDependencyModuleNames.AddRange(new string[] {  });

        // �� C++ Class�� ������ �� ���� �θ� Ŭ������ ��ΰ� ������Ʈ �̸��� ��� ã�� ���ϰ� ������ �߻��Ѵ�.
        // �ش� ������ �����ϱ� ���ؼ� �Ʒ� �ڵ带 �ۼ��ߴ�.
        PublicIncludePaths.AddRange(new string[] { "DarkSoul" });

        // Uncomment if you are using Slate UI
        // PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

        // Uncomment if you are using online features
        // PrivateDependencyModuleNames.Add("OnlineSubsystem");

        // To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
    }
}
