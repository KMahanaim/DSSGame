// Copyright Epic Games, Inc. All Rights Reserved.
#include "CBTT_MoveToPatrolPoint.h"
#include "DarkSoul/_Utility/CLog.h"
#include "DarkSoul/AI/CPatrolPath.h"
#include "DarkSoul/Characters/CBaseAI.h"
#include "DarkSoul/Components/CPatrolComponent.h"
#include "DarkSoul/Controllers/CBaseAIController.h"

/// Unreal Engine
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UCBTT_MoveToPatrolPoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = EBTNodeResult::Failed;
	FBTMoveToPatrolPointMemory* Memory = CastInstanceNodeMemory<FBTMoveToPatrolPointMemory>(NodeMemory);
	ACBaseAIController* OwnerController = Cast<ACBaseAIController>(OwnerComp.GetAIOwner());
	if (OwnerController == nullptr)
	{
		CLOG_ERROR_CHECK(OwnerController);
		Result = EBTNodeResult::Failed;
	}
	else
	{
		ACBaseAI* Owner = Cast<ACBaseAI>(OwnerController->GetPawn());
		if (Owner == nullptr)
		{
			CLOG_ERROR_CHECK(Owner);
			Result = EBTNodeResult::Failed;
		}
		else
		{
			UCPatrolComponent* PatrolComponent = Owner->GetPatrolComponent();
			if (PatrolComponent == nullptr)
			{
				CLOG_ERROR_CHECK(PatrolComponent);
				Result = EBTNodeResult::Failed;
			}
			else
			{
				Memory->PatrolComponent = PatrolComponent;
				ACPatrolPath* PatrolPath = PatrolComponent->GetPatrolPath();
				if (PatrolPath == nullptr)
				{
					CLOG_ERROR_CHECK(PatrolPath);
					Result = EBTNodeResult::Failed;
				}
				else
				{
					OwnerController->MoveToLocation
					(
						PatrolComponent->GetSplinePointLocation(PatrolComponent->GetPointIndex()),
						AcceptanceRadius,
						true	/** Stop On Overlap*/
					);

					if (Memory->bIsToDoBindFunc == false)
					{
						Memory->bIsToDoBindFunc = true;
						OwnerController->OnSuccessPatrolMove.AddUObject(this, &UCBTT_MoveToPatrolPoint::OnMoveComplete);
					}
					Result = EBTNodeResult::InProgress;
				}
			}
		}
	}

	return Result;
}

EBTNodeResult::Type UCBTT_MoveToPatrolPoint::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ACBaseAIController* OwnerController = Cast<ACBaseAIController>(OwnerComp.GetAIOwner());
	if (OwnerController == nullptr)
	{
		CLOG_ERROR_CHECK(OwnerController);
	}
	else
	{
		OwnerController->StopMovement();
	}
	return EBTNodeResult::Aborted;
}

void UCBTT_MoveToPatrolPoint::OnMoveComplete(const UBlackboardComponent& Blackboard)
{
	EBTNodeResult::Type Result = EBTNodeResult::Failed;
	UBehaviorTreeComponent* BehaviorComponent = Cast<UBehaviorTreeComponent>(Blackboard.GetBrainComponent());
	if (BehaviorComponent == nullptr)
	{
		CLOG_ERROR_CHECK(BehaviorComponent);
	}
	else
	{
		uint8* RawMemory = BehaviorComponent->GetNodeMemory(this, BehaviorComponent->FindInstanceContainingNode(this));
		FBTMoveToPatrolPointMemory* Memory = CastInstanceNodeMemory<FBTMoveToPatrolPointMemory>(RawMemory);
		if (Memory->PatrolComponent == nullptr)
		{
			CLOG_ERROR_CHECK(Memory->PatrolComponent);
		}
		else
		{
			Memory->PatrolComponent->UpdatePatrolIndex();
			Result = EBTNodeResult::Succeeded;
		}
	}
	FinishLatentTask(*BehaviorComponent, Result);
}

uint16 UCBTT_MoveToPatrolPoint::GetInstanceMemorySize() const
{
	return sizeof(FBTMoveToPatrolPointMemory);
}