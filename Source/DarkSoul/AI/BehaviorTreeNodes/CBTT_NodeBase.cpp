// Copyright Epic Games, Inc. All Rights Reserved.
#include "CBTT_NodeBase.h"
#include "DarkSoul/_Utility/CLog.h"
#include "DarkSoul/Characters/CBaseAI.h"
#include "DarkSoul/Controllers/CBaseAIController.h"

EBTNodeResult::Type UCBTT_NodeBase::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	EBTNodeResult::Type Result = EBTNodeResult::InProgress;
	OwnerController = Cast<ACBaseAIController>(OwnerComp.GetAIOwner());
	if (OwnerController == nullptr)
	{
		CLOG_FUNC_TEXT(L"Controller is nullptr");
		Result = EBTNodeResult::Failed;
	}

	Owner = Cast<ACBaseAI>(OwnerController->GetPawn());
	if (Owner == nullptr)
	{
		CLOG_FUNC_TEXT(L"Owner is nullptr");
		Result = EBTNodeResult::Failed;
	}

	return Result;
}