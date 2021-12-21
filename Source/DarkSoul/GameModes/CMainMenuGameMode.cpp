// Copyright Epic Games, Inc. All Rights Reserved.
#include "CMainMenuGameMode.h"
#include "DarkSoul/_Utility/CLog.h"
#include "DarkSoul/HUD/CHUDInMainMenu.h"
#include "DarkSoul/Characters/CUIKeyInputPawn.h"
#include "DarkSoul/Controllers/CPlayerUIController.h"

ACMainMenuGameMode::ACMainMenuGameMode()
{
	CLOG_FUNC_TEXT(L"Set Defaults class");

	// Set Defaults class
	{
		DefaultPawnClass = ACUIKeyInputPawn::StaticClass();
		HUDClass = ACHUDInMainMenu::StaticClass();
		PlayerControllerClass = ACPlayerUIController::StaticClass();
	}
}