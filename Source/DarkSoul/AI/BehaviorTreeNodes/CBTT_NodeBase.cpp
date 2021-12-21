// Copyright Epic Games, Inc. All Rights Reserved.
#include "CBTT_NodeBase.h"
#include "DarkSoul/_Utility/CLog.h"
#include "DarkSoul/Characters/CBaseAI.h"

/// Unreal Engine
#include "AIController.h"

EBTNodeResult::Type UCBTT_NodeBase::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	EBTNodeResult::Type Result = EBTNodeResult::InProgress;
	OwnerController = OwnerComp.GetAIOwner();
	if (OwnerController == nullptr)
	{
		CLOG_FUNC_TEXT(L"Controller is nullptr");
		Result = EBTNodeResult::Failed;
	}
	else
	{
		Owner = Cast<ACBaseAI>(OwnerController->GetPawn());
		if (Owner == nullptr)
		{
			CLOG_FUNC_TEXT(L"Owner is nullptr");
			Result = EBTNodeResult::Failed;
		}
	}

	return Result;
}