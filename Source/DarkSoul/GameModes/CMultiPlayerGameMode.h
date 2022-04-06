#pragma once
#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "CMultiPlayerGameMode.generated.h"

/**
 * 멀티플레이어 게임을 위한 모드
 */
UCLASS()
class DARKSOUL_API ACMultiPlayerGameMode : public AGameMode
{
	GENERATED_BODY()
	
public:
	ACMultiPlayerGameMode();

protected:
	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void BeginPlay() override;

private:
	TArray<class APlayerController*> PlayerControllerList;
};