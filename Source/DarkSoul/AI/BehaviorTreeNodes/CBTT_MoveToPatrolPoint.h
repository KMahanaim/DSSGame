// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once
#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "CBTT_MoveToPatrolPoint.generated.h"

class UCPatrolComponent;
class ACBaseAIController;

struct FBTMoveToPatrolPointMemory
{
	/** true : 이미 실행된 상태, false : 한 번 실행해야 한다*/
	bool bIsToDoBindFunc = false;
	TWeakObjectPtr<UCPatrolComponent> PatrolComponent;
};

/**
 * 
 */
UCLASS()
class DARKSOUL_API UCBTT_MoveToPatrolPoint : public UBTTaskNode
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
		float AcceptanceRadius = 100.0f;

private:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);
	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);
	void OnMoveComplete(const UBlackboardComponent& Blackboard);
	virtual uint16 GetInstanceMemorySize() const override;
};