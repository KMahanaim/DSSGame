// Copyright Epic Games, Inc. All Rights Reserved.
#include "CBTT_SetMovementState.h"
#include "DarkSoul/_Utility/CLog.h"
#include "DarkSoul/Characters/CBaseAI.h"
#include "DarkSoul/Controllers/CBaseAIController.h"
#include "DarkSoul/Components/CMovementSpeedComponent.h"

EBTNodeResult::Type UCBTT_SetMovementState::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = EBTNodeResult::Failed;
	ACBaseAIController* ownerController = Cast<ACBaseAIController>(OwnerComp.GetAIOwner());
	if (ownerController == nullptr)
	{
		CLOG_FUNC_TEXT(L"Controller is nullptr");
		Result = EBTNodeResult::Failed;
	}
	else
	{
		ACBaseAI* owner = Cast<ACBaseAI>(ownerController->GetPawn());
		if (owner == nullptr)
		{
			CLOG_FUNC_TEXT(L"Owner is nullptr");
			Result = EBTNodeResult::Failed;
		}
		else
		{
			UCMovementSpeedComponent* movementSpeed = owner->GetMovementSpeed();
			if (movementSpeed == nullptr)
			{
				CLOG_FUNC_TEXT(L"UCMovementSpeedComponent is nullptr");
				Result = EBTNodeResult::Failed;
			}
			else
			{
				movementSpeed->SetMovementState(MovementState);
				Result = EBTNodeResult::Succeeded;
			}
		}
	}
	return Result;
}