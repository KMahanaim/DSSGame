// Copyright Epic Games, Inc. All Rights Reserved.
#include "CBTS_UpdateMeleeAI.h"
#include "DarkSoul/_Utility/CLog.h"
#include "DarkSoul/Characters/CBaseAI.h"
#include "DarkSoul/Components/CPatrolComponent.h"
#include "DarkSoul/Components/CRotatingComponent.h"
#include "DarkSoul/Components/CEquipmentComponent.h"
#include "DarkSoul/Components/CStateManagerComponent.h"
#include "DarkSoul/Components/CStatsManagerComponent.h"
#include "DarkSoul/Components/CExtendedStatComponent.h"

/// Unreal Engine
#include "AIController.h"
#include "Kismet/KismetMathLibrary.h"
#include "BehaviorTree/BlackboardComponent.h"

void UCBTS_UpdateMeleeAI::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	Update();
}

void UCBTS_UpdateMeleeAI::UpdateBehavior(ACBaseAI* const OwnerCharacter, AAIController* const OwnerController)
{
	// 1. Owner is disable?
	if (OwnerCharacter->GetStateManagerComponent()->GetState() == EStateType::DISABLED)
	{
		SetBehavior(EAIBehavior::HIT);
		return;
	}
	 
	// 2. Check the target exist or alive
	ACCombatCharacter* Target = Cast<ACCombatCharacter>(OwnerController->GetBlackboardComponent()->GetValueAsObject(TargetKey.SelectedKeyName));
	if ((Target == nullptr))
	{
		// Reset focus
		OwnerController->SetFocus(nullptr);
		OwnerCharacter->GetRotatingComponent()->SetRotationMode(ERotationMode::ORIENT_TO_MOVEMENT);

		if (OwnerCharacter->GetPatrolComponent()->IsPatrolPathValid())
		{
			SetBehavior(EAIBehavior::PATROL);
			return;
		}
		else
		{
			SetBehavior(EAIBehavior::IDLE);
			return;
		}
	}
	else if (Target->IsAlive() == false)
	{
		// Reset focus
		OwnerController->SetFocus(nullptr);
		OwnerCharacter->GetRotatingComponent()->SetRotationMode(ERotationMode::ORIENT_TO_MOVEMENT);

		if (OwnerCharacter->GetPatrolComponent()->IsPatrolPathValid())
		{
			SetBehavior(EAIBehavior::PATROL);
			return;
		}
		else
		{
			SetBehavior(EAIBehavior::IDLE);
			return;
		}
	}

	// Distance, Dot To Target
	float DistanceToTarget = Target->GetDistanceTo(OwnerCharacter);
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

	// 4. Is behind the target?
	if (DotProductToTarget <= -0.25f)
	{
		// Yes! Back Attack!
		if (bIsInAttackRange)
		{
			SetBehavior(EAIBehavior::MELEE_ATTACK);
			return;
		}
		else
		{
			SetBehavior(EAIBehavior::APPROACH);
			return;
		}
	}
	else
	{
		// No! I can't do!
		if (bIsInAttackRange)
		{
			// 4-1. How long neary the target
			if (TicksNearTarget >= EnoughNearTargetTime)
			{
				// Enough!
				SetBehavior(EAIBehavior::MELEE_ATTACK);
				if (UKismetMathLibrary::RandomBoolWithWeight(0.4f))
				{
					TicksNearTarget = 0.0f;
				}
				return;
			}
			else
			{
				// Not enough!
				TicksNearTarget += 1.0f;

				if (bIsOutOfStamina || StaminaPercent <= 40.0f || UKismetMathLibrary::RandomBoolWithWeight(0.1f))
				{
					SetBehavior(EAIBehavior::MELEE_ATTACK);
					return;
				}
				else
				{
					// 8-3. 타겟의 공격 유형이 마법 또는 원거리 입니까?
					if (TargetEquipment != nullptr)
					{
						ECombatType TargetCombatType = TargetEquipment->GetCombatType();
						if (TargetCombatType == ECombatType::MAGIC || TargetCombatType == ECombatType::RANGE)
						{
							SetBehavior(EAIBehavior::MELEE_ATTACK);
							return;
						}
						else
						{
							SetBehavior(EAIBehavior::STRAFE_AROUND);
							return;
						}
					}
					else
					{
						SetBehavior(EAIBehavior::STRAFE_AROUND);
						return;
					}
				}
			}
		}
		// Target is too far
		else
		{
			SetBehavior(EAIBehavior::APPROACH);
			return;
		}
	}
		
	// 9-1. 타겟과의 거리가 먼가요?
	TicksNearTarget = 0.0f;
	if (DistanceToTarget >= 2000.0f)
	{
		SetBehavior(EAIBehavior::APPROACH);
		return;
	}
	else
	{
		if (TargetEquipment == nullptr)
		{
			SetBehavior(EAIBehavior::APPROACH);
			return;
		}

		// 9-2. 타겟이 전투 중인가요?
		if (TargetEquipment->IsInCombat() == false)
		{
			SetBehavior(EAIBehavior::APPROACH);
			return;
		}

		// 9-3. 타겟의 공격 유형이 마법 또는 원거리 입니까?
		if (TargetEquipment != nullptr)
		{
			ECombatType TargetCombatType = TargetEquipment->GetCombatType();
			if (TargetCombatType == ECombatType::MAGIC || TargetCombatType == ECombatType::RANGE)
			{
				SetBehavior(EAIBehavior::MELEE_ATTACK);
				return;
			}
			else
			{
				SetBehavior(EAIBehavior::STRAFE_AROUND);
				return;
			}
		}
		else
		{
			SetBehavior(EAIBehavior::STRAFE_AROUND);
			return;
		}
	}
}

void UCBTS_UpdateMeleeAI::UpdateActivities(ACBaseAI* const OwnerCharacter, AAIController* const OwnerController)
{
	if (OwnerCharacter == nullptr) return;
	if (OwnerController == nullptr) return;

	switch (static_cast<EAIBehavior>(OwnerController->GetBlackboardComponent()->GetValueAsEnum(BehaviorKey.SelectedKeyName)))
	{
		case EAIBehavior::IDLE:
		{
			OwnerCharacter->GetStateManagerComponent()->SetActivity(EActivity::IS_BLOCKING_PRESSED, false);
		}
			break;
		case EAIBehavior::PATROL:
		{
			OwnerCharacter->GetStateManagerComponent()->SetActivity(EActivity::IS_BLOCKING_PRESSED, false);
		}
			break;
		case EAIBehavior::MELEE_ATTACK:
		{
			OwnerCharacter->GetStateManagerComponent()->SetActivity(EActivity::IS_BLOCKING_PRESSED, false);
		}
			break;
		case EAIBehavior::RANGE_ATTACK:
			break;
		case EAIBehavior::APPROACH:
		{
			OwnerCharacter->GetStateManagerComponent()->SetActivity
			(
				EActivity::IS_BLOCKING_PRESSED, 
				OwnerCharacter->GetEquipmentComponent()->IsInCombat() && !bIsOutOfStamina
			);
		}
			break;
		case EAIBehavior::FLEE:
		{
			OwnerCharacter->GetStateManagerComponent()->SetActivity(EActivity::IS_BLOCKING_PRESSED, false);
		}
			break;
		case EAIBehavior::STRAFE_AROUND:
		{
			OwnerCharacter->GetStateManagerComponent()->SetActivity
			(
				EActivity::IS_BLOCKING_PRESSED,
				OwnerCharacter->GetEquipmentComponent()->IsInCombat() && !bIsOutOfStamina
			);
		}
			break;
		case EAIBehavior::HIT:
		{
			OwnerCharacter->GetStateManagerComponent()->SetActivity(EActivity::IS_BLOCKING_PRESSED, false);
		}
			break;
	}
}