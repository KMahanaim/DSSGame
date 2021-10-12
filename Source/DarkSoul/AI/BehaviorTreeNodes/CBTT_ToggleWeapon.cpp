// Copyright Epic Games, Inc. All Rights Reserved.
#include "CBTT_ToggleWeapon.h"
#include "DarkSoul/_Utility/CLog.h"
#include "DarkSoul/Characters/CBaseAI.h"

/// Unreal Engine
#include "TimerManager.h"
#include "Engine/World.h"
EBTNodeResult::Type UCBTT_ToggleWeapon::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	FBTToggleWeaopnMemory* Memory = CastInstanceNodeMemory<FBTToggleWeaopnMemory>(NodeMemory);
	if (Memory == nullptr) return EBTNodeResult::Failed;
	if (GetWorld() == nullptr) return EBTNodeResult::Failed;

	GetWorld()->GetTimerManager().ClearTimer(Memory->MontagePlayTimeHandler);
	Memory->BTComponent = &OwnerComp;
	Memory->NodeMemory = NodeMemory;

	float MontagePlayTime = 0.0f;
	Owner->ToggleCombat(MontagePlayTime);
	if (MontagePlayTime > 0.0f)
	{
		Memory->MontagePlayFinishedDelegate = FTimerDelegate::CreateUObject(this, &UCBTT_ToggleWeapon::OnMontagePlayFinished, Memory->BTComponent, Memory->NodeMemory);
		GetWorld()->GetTimerManager().SetTimer(Memory->MontagePlayTimeHandler, Memory->MontagePlayFinishedDelegate, MontagePlayTime, false);
	}
	else
	{
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::InProgress;
}

void UCBTT_ToggleWeapon::OnMontagePlayFinished(UBehaviorTreeComponent* OwnerComp, uint8* NodeMemory)
{
	FinishLatentTask(*OwnerComp, EBTNodeResult::Succeeded);
}

uint16 UCBTT_ToggleWeapon::GetInstanceMemorySize() const
{
	return sizeof(FBTToggleWeaopnMemory);
}
