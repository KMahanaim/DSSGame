// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once
#include "CoreMinimal.h"
#include "DarkSoul/AI/BehaviorTreeNodes/CBTS_UpdateBaseAI.h"
#include "CBTS_UpdateMeleeAI.generated.h"

UCLASS()
class DARKSOUL_API UCBTS_UpdateMeleeAI : public UCBTS_UpdateBaseAI
{
	GENERATED_BODY()
	
private:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) final;

	virtual void UpdateBehavior(ACBaseAI* const OwnerCharacter, AAIController* const OwnerController) final;
	virtual void UpdateActivities(ACBaseAI* const OwnerCharacter, AAIController* const OwnerController) final;
};