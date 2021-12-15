// Copyright Epic Games, Inc. All Rights Reserved.
#include "CBTS_UpdateBaseAI.h"
#include "DarkSoul/_Utility/CLog.h"
#include "DarkSoul/Characters/CBaseAI.h"
#include "DarkSoul/Components/CStateManagerComponent.h"
#include "DarkSoul/Components/CExtendedStatComponent.h"

/// Unreal Engine
#include "Engine/World.h"
#include "TimerManager.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

void UCBTS_UpdateBaseAI::TickNode(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	// 1. Get Instance Memory
	InstanceMemory = CastInstanceNodeMemory<FBTUpdateAIMemory>(NodeMemory);
	CLOG_ERROR_CHECK_RETURN(InstanceMemory);

	// 2. Get AI Controller
	if (InstanceMemory->OwnerController == nullptr)
	{
		InstanceMemory->OwnerController = Cast<AAIController>(OwnerComp.GetAIOwner());
		CLOG_ERROR_CHECK_RETURN(InstanceMemory->OwnerController);
	}

	// 3. Get AI Character
	if (InstanceMemory->OwnerCharacter == nullptr)
	{
		InstanceMemory->OwnerCharacter = Cast<ACBaseAI>(InstanceMemory->OwnerController->GetPawn());
		CLOG_ERROR_CHECK_RETURN_TEXT(InstanceMemory->OwnerCharacter, InstanceMemory->OwnerController->GetName() + L", NULL");
	}

	// 4. Delegate Bind, Only read this code once.
	if (InstanceMemory->bIsBindDelegate == false)
	{
		InstanceMemory->bIsBindDelegate = true;
		OnBehaviorChanged.AddUObject(InstanceMemory->OwnerCharacter, &ACBaseAI::OnBehaviorChanged);
		InstanceMemory->OwnerCharacter->GetStateManagerComponent()->OnStateChanged.AddUObject(this, &UCBTS_UpdateBaseAI::OnStateChanged);
		InstanceMemory->OwnerCharacter->GetExtendedStatComponent(EStatsType::STAMINA)->OnValueChanged.AddUObject(this, &UCBTS_UpdateBaseAI::OnStaminaValueChanged);
	}
}

uint16 UCBTS_UpdateBaseAI::GetInstanceMemorySize() const
{
	return sizeof(FBTUpdateAIMemory);
}

void UCBTS_UpdateBaseAI::SetBehavior(EAIBehavior Behavior)
{
	CLOG_ERROR_CHECK_RETURN(InstanceMemory->OwnerController);
	InstanceMemory->OwnerController->GetBlackboardComponent()->SetValueAsEnum(BehaviorKey.SelectedKeyName, static_cast<uint8>(Behavior));

	if (Behavior != EAIBehavior::STRAFE_AROUND)
	{
		TicksStrafe = 0.0f;
	}
}

void UCBTS_UpdateBaseAI::Update()
{
	CLOG_ERROR_CHECK_RETURN(InstanceMemory);
	CLOG_ERROR_CHECK_RETURN(InstanceMemory->OwnerCharacter);
	CLOG_ERROR_CHECK_RETURN(InstanceMemory->OwnerController);

	UpdateBehavior(InstanceMemory->OwnerCharacter, InstanceMemory->OwnerController);
	UpdateActivities(InstanceMemory->OwnerCharacter, InstanceMemory->OwnerController);
}

void UCBTS_UpdateBaseAI::UpdateBehavior(ACBaseAI* const OwnerCharacter, AAIController* const OwnerController)
{
	CLOG_FUNC_TEXT(L"The function was not inherited");
}

void UCBTS_UpdateBaseAI::UpdateActivities(ACBaseAI* const OwnerCharacter, AAIController* const OwnerController)
{
	CLOG_FUNC_TEXT(L"The function was not inherited");
}

void UCBTS_UpdateBaseAI::OnStateChanged(EStateType PrevState, EStateType NewState)
{
	if ((PrevState == EStateType::DISABLED) || (NewState == EStateType::DISABLED))
	{
		Update();
	}
}

void UCBTS_UpdateBaseAI::OnStaminaValueChanged(float NewValue, float MaxValue)
{
	// If not enough stamina
	if (NewValue <= 0.0f)
	{
		GetWorld()->GetTimerManager().ClearTimer(OutOfStaminaResetHandler);
		bIsOutOfStamina = true;
		GetWorld()->GetTimerManager().SetTimer(OutOfStaminaResetHandler, this, &UCBTS_UpdateBaseAI::ResetIsOutOfStamina, 2.0f, false);
	}
}

void UCBTS_UpdateBaseAI::DebugBehavior(const EAIBehavior Behavior) const
{
	// Print Current Behavior
	FString BehaviorText;
	switch (Behavior)
	{
		case EAIBehavior::IDLE:
		{
			BehaviorText = L"IDLE";
		}
		break;
		case EAIBehavior::PATROL:
		{
			BehaviorText = L"PATROL";
		}
		break;
		case EAIBehavior::MELEE_ATTACK:
		{
			BehaviorText = L"MELEE ATTACK";
		}
		break;
		case EAIBehavior::RANGE_ATTACK:
		{
			BehaviorText = L"RANGE ATTACK";
		}
		break;
		case EAIBehavior::APPROACH:
		{
			BehaviorText = L"APPROACH";
		}
		break;
		case EAIBehavior::FLEE:
		{
			BehaviorText = L"FLEE";
		}
		break;
		case EAIBehavior::STRAFE_AROUND:
		{
			BehaviorText = L"STRAFE AROUND";
		}
		break;
		case EAIBehavior::HIT:
		{
			BehaviorText = L"HIT";
		}
		break;
	}

	OnBehaviorChanged.Broadcast(BehaviorText);
}