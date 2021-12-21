// Copyright Epic Games, Inc. All Rights Reserved.
#include "CBTT_SetMovementState.h"
#include "DarkSoul/_Utility/CLog.h"
#include "DarkSoul/Characters/CBaseAI.h"
#include "DarkSoul/Components/CMovementSpeedComponent.h"

/// Unreal Engine
#include "AIController.h"

EBTNodeResult::Type UCBTT_SetMovementState::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = EBTNodeResult::Failed;
	AAIController* OwnerController = OwnerComp.GetAIOwner();
	if (OwnerController == nullptr)
	{
		CLOG_FUNC_TEXT(L"Controller is nullptr");
		Result = EBTNodeResult::Failed;
	}
	else
	{
		ACBaseAI* Owner = Cast<ACBaseAI>(OwnerController->GetPawn());
		if (Owner == nullptr)
		{
			CLOG_FUNC_TEXT(L"Owner is nullptr");
			Result = EBTNodeResult::Failed;
		}
		else
		{
			UCMovementSpeedComponent* MovementSpeed = Owner->GetMovementSpeed();
			if (MovementSpeed == nullptr)
			{
				CLOG_FUNC_TEXT(L"UCMovementSpeedComponent is nullptr");
				Result = EBTNodeResult::Failed;
			}
			else
			{
				MovementSpeed->SetMovementState(MovementState);
				Result = EBTNodeResult::Succeeded;
			}
		}
	}
	return Result;
}