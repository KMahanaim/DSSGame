// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class DarkSoul : ModuleRules
{
	public DarkSoul(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] 
        {
            /*기본 모듈*/
            "Core",
            "CoreUObject",
            "Engine",
            "InputCore", 

            /*추가한 모듈*/
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

        // 새 C++ Class를 생성할 때 만약 부모 클래스의 경로가 프로젝트 이름일 경우 찾지 못하고 에러가 발생한다.
        // 해당 에러를 방지하기 위해서 아래 코드를 작성했다.
        PublicIncludePaths.AddRange(new string[] { "DarkSoul" });

        // Uncomment if you are using Slate UI
        // PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

        // Uncomment if you are using online features
        // PrivateDependencyModuleNames.Add("OnlineSubsystem");

        // To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
    }
}
