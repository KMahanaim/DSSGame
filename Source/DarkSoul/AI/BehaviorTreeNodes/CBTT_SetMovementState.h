// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once
#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "DarkSoul/Enumerations/CEMovementState.h"
#include "CBTT_SetMovementState.generated.h"

/**
 * 
 */
UCLASS()
class DARKSOUL_API UCBTT_SetMovementState : public UBTTaskNode
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
		EMovementState MovementState;

private:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);
};