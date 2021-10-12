// Copyright Epic Games, Inc. All Rights Reserved.
#include "DarkSoulGameModeBase.h"
#include "DarkSoul/HUD/CHUDInGame.h"
#include "DarkSoul/Characters/CCombatCharacter.h"
#include "DarkSoul/Controllers/CPlayerController.h"

/// Unreal Engine
#include "UObject/ConstructorHelpers.h"

ADarkSoulGameModeBase::ADarkSoulGameModeBase()
{
	FString path = "";

	// 1. Create Player
	{
		DefaultPawnClass = ACCombatCharacter::StaticClass();
		path = L"Blueprint'/Game/DarkSoul/Characters/Bp_CPlayerCharacter.Bp_CPlayerCharacter_C'";
		ConstructorHelpers::FClassFinder<ACCombatCharacter> player(*path);
		if (player.Succeeded())
			DefaultPawnClass = player.Class;
	}

	HUDClass = ACHUDInGame::StaticClass();
	PlayerControllerClass = ACPlayerController::StaticClass();
}