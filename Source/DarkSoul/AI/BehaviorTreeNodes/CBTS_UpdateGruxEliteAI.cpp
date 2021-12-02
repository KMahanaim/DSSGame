// Copyright Epic Games, Inc. All Rights Reserved.
#include "CBTS_UpdateGruxEliteAI.h"
#include "DarkSoul/_Utility/CLog.h"
#include "DarkSoul/Characters/CBaseAI.h"

/// Unreal Engien
#include "AIController.h"

void UCBTS_UpdateGruxEliteAI::TickNode(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	OwnerController = Cast<AAIController>(OwnerComp.GetAIOwner());
	CLOG_ERROR_CHECK_RETURN(OwnerController);

	OwnerCharacter = Cast<ACBaseAI>(OwnerController->GetPawn());
	CLOG_ERROR_CHECK_RETURN_TEXT(OwnerCharacter, OwnerController->GetName() + L", NULL");
}

void UCBTS_UpdateGruxEliteAI::SetBehavior(EAIBehavior Behavior)
{
}

void UCBTS_UpdateGruxEliteAI::Update()
{
}

void UCBTS_UpdateGruxEliteAI::UpdateBehavior()
{
}

void UCBTS_UpdateGruxEliteAI::UpdateActivities()
{
}

void UCBTS_UpdateGruxEliteAI::OnStateChanged(EStateType PrevState, EStateType NewState)
{
}