// Copyright Epic Games, Inc. All Rights Reserved.
#include "CStateManagerComponent.h"
#include "DarkSoul/_Utility/CLog.h"

/// Unreal Engine
#include "Engine/World.h"
#include "TimerManager.h"

UCStateManagerComponent::UCStateManagerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UCStateManagerComponent::SetState(const EStateType NewState)
{
	if (bIsOutLogSetState)
	{
		CLOG_FUNC_TEXT(GetEStateAsString(NewState));
	}

	const EStateType prevState = CurrentState;
	CurrentState = NewState;

	if (CurrentState != prevState)
	{
		OnStateChanged.Broadcast(prevState, CurrentState);
	}
}

void UCStateManagerComponent::SetActivity(const EActivity Activity, const bool bValue)
{
	const bool bactivityValue = GetActivityValue(Activity);
	if (bactivityValue != bValue)
	{
		Activities.Add(Activity, bValue);
		OnActivityChanged.Broadcast(Activity, bValue);
	}
}

const bool UCStateManagerComponent::GetActivityValue(const EActivity Activity)
{
	const bool* bvalue = Activities.Find(Activity);
	if (bvalue == nullptr)
	{
		return false;
	}
	else
	{
		return *bvalue;
	}
}

FString UCStateManagerComponent::GetEStateAsString(EStateType State)
{
	const UEnum* enumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EStateType"), true);
	if (enumPtr == nullptr)
	{
		return FString(L"Failed to Find enumPtr");
	}

	return enumPtr->GetNameStringByIndex(static_cast<int32>(State));
}

void UCStateManagerComponent::TimerResetIdleState()
{
	SetIdleState();
}

void UCStateManagerComponent::ResetState(float InTime/** = 0.0f */)
{
	if (GetState() != EStateType::DEAD)
	{
		if (InTime <= 0.0f)
		{
			SetIdleState();
		}
		else
		{
			GetWorld()->GetTimerManager().SetTimer(ResetStateTimerHandle, this, &UCStateManagerComponent::SetIdleState, InTime, false);
		}
	}
}

const bool UCStateManagerComponent::IsStateEqual(const EStateType StateToCompare)
{
	if (CurrentState == StateToCompare)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void UCStateManagerComponent::SetIdleState()
{
	SetState(EStateType::IDLE);
}