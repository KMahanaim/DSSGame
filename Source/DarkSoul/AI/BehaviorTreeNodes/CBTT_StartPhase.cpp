// Copyright Epic Games, Inc. All Rights Reserved.
#include "CBTT_StartPhase.h"
#include "DarkSoul/_Utility/CLog.h"
#include "DarkSoul/Characters/CBossAI.h"
#include "DarkSoul/Components/CBossPhaseComponent.h"

EBTNodeResult::Type UCBTT_StartPhase::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	ACBossAI* Boss = Cast<ACBossAI>(Owner);
	if (Boss != nullptr)
	{
		if (Boss->GetPhaseComponent()->GetCurrentPhase() >= 1)
		{
			return EBTNodeResult::Succeeded;
		}
	}

	CLOG_ERROR_FUNC_TEXT(L"Boss is nullptr");
	return EBTNodeResult::InProgress;
}