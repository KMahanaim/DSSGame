// Copyright Epic Games, Inc. All Rights Reserved.
#include "CBossGameModeBase.h"
#include "DarkSoul/HUD/CHUDInGame.h"
#include "DarkSoul/Characters/CCombatCharacter.h"
#include "DarkSoul/Controllers/CPlayerController.h"

/// Unreal Engine
#include "UObject/ConstructorHelpers.h"

ACBossGameModeBase::ACBossGameModeBase()
{
	FString Path = "";

	// 1. Create Player
	{
		DefaultPawnClass = ACCombatCharacter::StaticClass();
		Path = L"Blueprint'/Game/DarkSoul/Characters/Bp_CPlayerCharacter.Bp_CPlayerCharacter_C'";
		ConstructorHelpers::FClassFinder<ACCombatCharacter> player(*Path);
		if (player.Succeeded())
			DefaultPawnClass = player.Class;
	}

	HUDClass = ACHUDInGame::StaticClass();
	PlayerControllerClass = ACPlayerController::StaticClass();
}