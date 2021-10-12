// Copyright Epic Games, Inc. All Rights Reserved.
#include "CBTT_MeleeAttack.h"
#include "DarkSoul/_Utility/CLog.h"
#include "DarkSoul/Characters/CBaseAI.h"

/// Unreal Engine
#include "Engine/World.h"
#include "TimerManager.h"
#include "Kismet/KismetSystemLibrary.h"

EBTNodeResult::Type UCBTT_MeleeAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	FBTMeleeAttacknMemory* Memory = CastInstanceNodeMemory<FBTMeleeAttacknMemory>(NodeMemory);
	if (Memory == nullptr) return EBTNodeResult::Failed;
	if (GetWorld() == nullptr) return EBTNodeResult::Failed;

	GetWorld()->GetTimerManager().ClearTimer(Memory->MeleeAttackHandler);
	Memory->BTComponent = &OwnerComp;
	Memory->NodeMemory = NodeMemory;

	float MontagePlayTime = 0.0f;
	MontagePlayTime = Owner->MeleeAttackAction(AttackType);
	if (MontagePlayTime > 0.0f)
	{
		Memory->MeleeAttackFinishedDelegate = FTimerDelegate::CreateUObject(this, &UCBTT_MeleeAttack::OnMeleeAttackFinished, Memory->BTComponent, Memory->NodeMemory);
		GetWorld()->GetTimerManager().SetTimer(Memory->MeleeAttackHandler, Memory->MeleeAttackFinishedDelegate, MontagePlayTime, false);
	}
	else
	{
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::InProgress;
}

void UCBTT_MeleeAttack::OnMeleeAttackFinished(UBehaviorTreeComponent* OwnerComp, uint8* NodeMemory)
{
	FinishLatentTask(*OwnerComp, EBTNodeResult::Succeeded);
}

uint16 UCBTT_MeleeAttack::GetInstanceMemorySize() const
{
	return sizeof(FBTMeleeAttacknMemory);
}
