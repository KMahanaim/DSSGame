// Copyright Epic Games, Inc. All Rights Reserved.
#include "CBTS_UpdateGruxEliteAI.h"
#include "DarkSoul/_Utility/CLog.h"
#include "DarkSoul/Characters/CBaseAI.h"
#include "DarkSoul/Components/CRotatingComponent.h"
#include "DarkSoul/Components/CStatsManagerComponent.h"
#include "DarkSoul/Components/CExtendedStatComponent.h"

/// Unreal Engien
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

void UCBTS_UpdateGruxEliteAI::TickNode(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	Update();
}

void UCBTS_UpdateGruxEliteAI::UpdateBehavior(ACBaseAI* const OwnerCharacter, AAIController* const OwnerController)
{
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

	// Distance, Dot To Target
	float DistanceToTarget = Target->GetDistanceTo(OwnerCharacter);
	float DotProductToTarget = Target->GetDotProductTo(OwnerCharacter);

	// Hits count and stamina
	int32 ReceivedHitsCount = OwnerCharacter->GetStatsManagerComponent()->GetRecentlyReceivedHitsCount();
	float StaminaPercent = OwnerCharacter->GetExtendedStatComponent(EStatsType::STAMINA)->GetPercent();

	// Get the Target Equipment
	UCEquipmentComponent* TargetEquipment = Target->GetEquipmentComponent();

	if (DistanceToTarget)
	{

	}
}

void UCBTS_UpdateGruxEliteAI::UpdateActivities(ACBaseAI* const OwnerCharacter, AAIController* const OwnerController)
{

}