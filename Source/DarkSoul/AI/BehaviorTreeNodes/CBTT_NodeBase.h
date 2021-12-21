// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once
#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "CBTT_NodeBase.generated.h"

class ACBaseAI;
class AAIController;

/**
 * BaseAI 행동 트리에 들어가는 Task Node 기본
 */
UCLASS()
class DARKSOUL_API UCBTT_NodeBase : public UBTTaskNode
{
	GENERATED_BODY()

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);

protected:
	ACBaseAI* Owner;
	AAIController* OwnerController;
};