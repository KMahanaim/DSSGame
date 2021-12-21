// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once
#include "CoreMinimal.h"
#include "Characters/CBaseAI.h"
#include "DarkSoul/Interfaces/I_CBoss.h"
#include "CBossAI.generated.h"

class UCBossPhaseComponent;

/**
 * Boss Base Class
 */
UCLASS()
class DARKSOUL_API ACBossAI : public ACBaseAI, public II_CBoss
{
	GENERATED_BODY()

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

public:
	virtual void BossStart() override;

//////////////////////////////////////////////////////////////////////////////////////////////////
// Get, Set Functions
//////////////////////////////////////////////////////////////////////////////////////////////////
public:
	UCBossPhaseComponent* GetPhaseComponent() const { return Phase; };
};