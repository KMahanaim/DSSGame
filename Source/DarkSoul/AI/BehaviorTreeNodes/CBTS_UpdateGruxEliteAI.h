// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once
#include "CoreMinimal.h"
#include "DarkSoul/AI/BehaviorTreeNodes/CBTS_UpdateBaseAI.h"
#include "CBTS_UpdateGruxEliteAI.generated.h"

class ACBaseAI;
class AAIController;

UCLASS()
class DARKSOUL_API UCBTS_UpdateGruxEliteAI : public UCBTS_UpdateBaseAI
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "Combat", meta = (ClampMax = 1.0f, ClampMin = 0.0f, UIMax = 1.0f, UIMin = 0.0f), meta = (AllowPrivateAccess = "true"))
		float RangeAttackWeight = 0.8f;

	UPROPERTY(EditAnywhere, Category = "Combat", meta = (ClampMax = 1.0f, ClampMin = 0.0f, UIMax = 1.0f, UIMin = 0.0f), meta = (AllowPrivateAccess = "true"))
		float ResetNearTargetTimeWeight = 0.4f;
	
private:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	virtual void UpdateBehavior(ACBaseAI* const OwnerCharacter, AAIController* const OwnerController) final;
	virtual void UpdateActivities(ACBaseAI* const OwnerCharacter, AAIController* const OwnerController) final;
};
