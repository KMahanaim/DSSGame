// Copyright Epic Games, Inc. All Rights Reserved.
#include "CBTS_UpdateGruxEliteAI.h"
#include "DarkSoul/_Utility/CLog.h"
#include "DarkSoul/Characters/CBossAI.h"
#include "DarkSoul/Components/CRotatingComponent.h"
#include "DarkSoul/Components/CEquipmentComponent.h"
#include "DarkSoul/Components/CBossPhaseComponent.h"
#include "DarkSoul/Components/CStatsManagerComponent.h"
#include "DarkSoul/Components/CExtendedStatComponent.h"
#include "DarkSoul/Components/CStateManagerComponent.h"

/// Unreal Engien
#include "AIController.h"
#include "Kismet/KismetMathLibrary.h"
#include "BehaviorTree/BlackboardComponent.h"

void UCBTS_UpdateGruxEliteAI::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	Update();
}

void UCBTS_UpdateGruxEliteAI::UpdateBehavior(ACBaseAI* const OwnerCharacter, AAIController* const OwnerController)
{
	ACBossAI* Boss = Cast<ACBossAI>(OwnerCharacter);

	// 보스방 페이즈가 시작되지 않았다.
	if (Boss->GetPhaseComponent()->GetCurrentPhase() < 1)
	{
		return;
	}

	// 1. Enter the Player?
	ACCombatCharacter* Target = Cast<ACCombatCharacter>(OwnerController->GetBlackboardComponent()->GetValueAsObject(TargetKey.SelectedKeyName));

	// 2. Check the target exist or alive
	if ((Target == nullptr))
	{
		// Reset focus
		OwnerController->SetFocus(nullptr);
		OwnerCharacter->GetRotatingComponent()->SetRotationMode(ERotationMode::ORIENT_TO_MOVEMENT);

		// Set Idle
		SetBehavior(EAIBehavior::IDLE);
		return;
	}
	else if ((Target->IsAlive() == false))
	{
		// Reset focus
		OwnerController->SetFocus(nullptr);
		OwnerCharacter->GetRotatingComponent()->SetRotationMode(ERotationMode::ORIENT_TO_MOVEMENT);

		// Set Idle
		SetBehavior(EAIBehavior::IDLE);
		return;
	}

	// 타겟과의 거리
	float DistanceToTarget = Target->GetDistanceTo(OwnerCharacter);
	// 타겟과의 내적 값
	float DotProductToTarget = Target->GetDotProductTo(OwnerCharacter);

	// Hits count and stamina
	int32 ReceivedHitsCount = OwnerCharacter->GetStatsManagerComponent()->GetRecentlyReceivedHitsCount();
	float StaminaPercent = OwnerCharacter->GetExtendedStatComponent(EStatsType::STAMINA)->GetPercent();

	// Get the Target Equipment
	UCEquipmentComponent* TargetEquipment = Target->GetEquipmentComponent();

	// 3. Check in attack range
	bool bIsInAttackRange = false;
	if (DistanceToTarget <= AttackBehaviorRange)
	{
		bIsInAttackRange = true;
	}

	if (bIsInAttackRange)
	{
		SetBehavior(EAIBehavior::MELEE_ATTACK);
	}
	else
	{
		if (UKismetMathLibrary::RandomBoolWithWeight(0.4f))
		{
			SetBehavior(EAIBehavior::RANGE_ATTACK);
		}
		else
		{
			SetBehavior(EAIBehavior::APPROACH);
		}
	}
}

void UCBTS_UpdateGruxEliteAI::UpdateActivities(ACBaseAI* const OwnerCharacter, AAIController* const OwnerController)
{
	if (OwnerCharacter == nullptr) return;
	if (OwnerController == nullptr) return;
}