// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once
#include "CoreMinimal.h"
#include "Characters/CBaseAI.h"
#include "CBossAI.generated.h"

class UCBossPhaseComponent;

/**
 * Boss Base Class
 */
UCLASS()
class DARKSOUL_API ACBossAI : public ACBaseAI
{
	GENERATED_BODY()

	// 보스 전용 UI가 표시되어야 한다.
	// 페이즈를 정해줄 클래스 혹은 구조체가 필요하다.

	UPROPERTY(EditDefaultsOnly, Category = "Boss", meta = (AllowPrivateAccess = "true"))
		FName BossName = "None";

	UPROPERTY(VisibleAnywhere, Category = "Component", meta = (AllowPrivateAccess = "true"))
		UCBossPhaseComponent* Phase;
	
//////////////////////////////////////////////////////////////////////////////////////////////////
// General Functions
//////////////////////////////////////////////////////////////////////////////////////////////////
public:
	ACBossAI();

protected:
	virtual void BeginPlay() override;
};