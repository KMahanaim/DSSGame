// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once
#include "CoreMinimal.h"
#include "AI/BehaviorTreeNodes/CBTT_NodeBase.h"
#include "CBTT_StartPhase.generated.h"

/**
 * 
 */
UCLASS()
class DARKSOUL_API UCBTT_StartPhase : public UCBTT_NodeBase
{
	GENERATED_BODY()
	
protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);
};
