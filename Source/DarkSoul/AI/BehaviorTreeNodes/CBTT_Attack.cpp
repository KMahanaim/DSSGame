// Copyright Epic Games, Inc. All Rights Reserved.
#include "CBTT_Attack.h"
#include "DarkSoul/_Utility/CLog.h"
#include "DarkSoul/Characters/CBaseAI.h"

/// Unreal Engine
#include "Engine/World.h"
#include "TimerManager.h"
#include "Kismet/KismetSystemLibrary.h"

EBTNodeResult::Type UCBTT_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	FBTAttacknMemory* Memory = CastInstanceNodeMemory<FBTAttacknMemory>(NodeMemory);
	if (Memory == nullptr) return EBTNodeResult::Failed;
	if (GetWorld() == nullptr) return EBTNodeResult::Failed;

	GetWorld()->GetTimerManager().ClearTimer(Memory->AttackHandler);
	Memory->BTComponent = &OwnerComp;
	Memory->NodeMemory = NodeMemory;

	float MontagePlayTime = 0.0f;
	MontagePlayTime = Owner->AttackAction(AttackType);
	if (MontagePlayTime > 0.0f)
	{
		Memory->AttackFinishedDelegate = FTimerDelegate::CreateUObject(this, &UCBTT_Attack::OnAttackFinished, Memory->BTComponent, Memory->NodeMemory);
		GetWorld()->GetTimerManager().SetTimer(Memory->AttackHandler, Memory->AttackFinishedDelegate, MontagePlayTime, false);
	}
	else
	{
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::InProgress;
}

void UCBTT_Attack::OnAttackFinished(UBehaviorTreeComponent* OwnerComp, uint8* NodeMemory)
{
	FinishLatentTask(*OwnerComp, EBTNodeResult::Succeeded);
}

uint16 UCBTT_Attack::GetInstanceMemorySize() const
{
	return sizeof(FBTAttacknMemory);
}