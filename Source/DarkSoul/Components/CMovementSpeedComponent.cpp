// Copyright Epic Games, Inc. All Rights Reserved.
#include "CMovementSpeedComponent.h"
#include "_Utility/CLog.h"

/// Unreal Engine
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "GameFramework/CharacterMovementComponent.h"

UCMovementSpeedComponent::UCMovementSpeedComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UCMovementSpeedComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerMovementComopnent = Cast<UCharacterMovementComponent>(GetOwner()->GetComponentByClass(UCharacterMovementComponent::StaticClass()));
	if (OwnerMovementComopnent == nullptr)
	{
		CLOG_FUNC_TEXT(GetOwner()->GetName() + L", Failed Casting the Character Movement Component");
		return;
	}

	StatesToToggle.Emplace(EMovementState::WALK);
	StatesToToggle.Emplace(EMovementState::JOG);

	MovementState = StartMovementState;
	SetMovementState(MovementState);
}

void UCMovementSpeedComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UpdateMaxSpeed();
}

const float UCMovementSpeedComponent::GetMaxPossibleSpeed() const
{
	if (OwnerMovementComopnent == nullptr)
	{
		CLOG_FUNC_TEXT(GetOwner()->GetName() + L", Fatal Error, Failed Casting the Character Movement Component");
		return 0.0f;
	}
	return OwnerMovementComopnent->MaxWalkSpeed;
}

void UCMovementSpeedComponent::SetMovementState(EMovementState NewMovementState)
{
	AActor* owner = GetOwner();
	if (owner == nullptr)
	{
		CLOG_FUNC_TEXT(L"Owner is nullptr");
		return;
	}

	if (owner->GetComponentByClass(UCharacterMovementComponent::StaticClass()) == nullptr)
	{
		CLOG_FUNC_TEXT(owner->GetName() + L", have not UCharacterMovementComponent");
		return;
	}

	OnMovementStateEnd.Broadcast(MovementState);
	MovementState = NewMovementState;

	// 목표 속도 선택
	switch (MovementState)
	{
		case EMovementState::IDLE:
		{
			TargetSpeed = 0.0f;
		}
			break;
		case EMovementState::WALK:
		{
			TargetSpeed = WalkSpeed;
		}
			break;
		case EMovementState::JOG:
		{
			TargetSpeed = JogSpeed;
		}
			break;
		case EMovementState::RUN:
		{
			TargetSpeed = RunSpeed;
		}
			break;
		default:
			break;
	}

	bIsUpdatingSpeed = true;
	OnMovementStateStart.Broadcast(MovementState);
}

void UCMovementSpeedComponent::UpdateMaxSpeed()
{
	if (bIsUpdatingSpeed)
	{
		if (OwnerMovementComopnent == nullptr)
		{
			OwnerMovementComopnent = Cast<UCharacterMovementComponent>(GetOwner()->GetComponentByClass(UCharacterMovementComponent::StaticClass()));
			if (OwnerMovementComopnent == nullptr)
			{
				CLOG_FUNC_TEXT(GetOwner()->GetName() + L", Fatal Error, Failed Casting the Character Movement Component");
				return;
			}
		}

		if (GetWorld() == nullptr)
		{
			CLOG_FUNC_TEXT(L"World is nullptr");
			return;
		}

		// 목표 속도로 보간
		OwnerMovementComopnent->MaxWalkSpeed = FMath::FInterpTo
		(
			OwnerMovementComopnent->MaxWalkSpeed,
			TargetSpeed,
			GetWorld()->GetDeltaSeconds(),
			SpeedChangeInterpSpeed
		);

		// 목표 속도 근접 확인
		if (FMath::IsNearlyEqual(OwnerMovementComopnent->MaxWalkSpeed, TargetSpeed, 1.0f))
		{
			OwnerMovementComopnent->MaxWalkSpeed = TargetSpeed;
			bIsUpdatingSpeed = false;
		}
	}
}