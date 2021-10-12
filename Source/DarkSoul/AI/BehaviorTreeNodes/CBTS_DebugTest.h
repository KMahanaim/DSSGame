// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once
#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "CBTS_DebugTest.generated.h"

/**
 * 
 */
UCLASS()
class DARKSOUL_API UCBTS_DebugTest : public UBTService
{
	GENERATED_BODY()
	
private:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
