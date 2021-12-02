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

	// ���� ���� UI�� ǥ�õǾ�� �Ѵ�.
	// ����� ������ Ŭ���� Ȥ�� ����ü�� �ʿ��ϴ�.

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