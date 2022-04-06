#include "CMultiPlayerGameMode.h"
#include "DarkSoul/_Utility/CLog.h"

ACMultiPlayerGameMode::ACMultiPlayerGameMode()
{
	CLOG_FUNC;
}

void ACMultiPlayerGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	CLOG_FUNC;

	PlayerControllerList.Emplace(NewPlayer);
}

void ACMultiPlayerGameMode::BeginPlay()
{
	Super::BeginPlay();

	CLOG_FUNC;
}