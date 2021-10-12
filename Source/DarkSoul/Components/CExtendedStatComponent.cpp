// Copyright Epic Games, Inc. All Rights Reserved.
#include "CExtendedStatComponent.h"
#include "DarkSoul/_Utility/CLog.h"
#include "DarkSoul/Components/CStatsManagerComponent.h"

/// Unreal Engine
#include "TimerManager.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"

UCExtendedStatComponent::UCExtendedStatComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UCExtendedStatComponent::BeginPlay()
{
	Super::BeginPlay();

	InitStat();
	InitialRegenValue = RegenValue;
	SetCurrentValue(TopValue, false);
	RefreshRegenTimer();

	// Load Game Data
	// 아직 미구현
}

void UCExtendedStatComponent::SetCurrentValue(const float Value, const bool bInterruptRegener)
{
	CurrentValue = FMath::Clamp(Value, -100.0f, GetMaxValue());
	OnValueChanged.Broadcast(CurrentValue, GetMaxValue());
	if (bInterruptRegener)
	{
		ClearRegenTimer();
		RefreshRegenTimer();
	}
}

void UCExtendedStatComponent::AddModifier(const float Value)
{
	ModifierValue += Value;
	SetCurrentValue(FMath::Clamp(CurrentValue, 0.0f, GetMaxValue()), false);
	RefreshRegenTimer();
}

void UCExtendedStatComponent::RemoveModifier(const float Value)
{
	ModifierValue = ModifierValue - Value;
	SetCurrentValue(FMath::Clamp(CurrentValue, 0.0f, GetMaxValue()), false);
	RefreshRegenTimer();
}

void UCExtendedStatComponent::ModifyStat(const float Value, const bool bInterruptRegener)
{
	if (Value != 0.0f)
	{
		if (bInterruptRegener)
		{
			ClearRegenTimer();
		}
		SetCurrentValue(CurrentValue + Value, false);
		RefreshRegenTimer();
	}
}

void UCExtendedStatComponent::ChangeRegenPercent(const uint8 Percent)
{
	RegenValue = (Percent / 100.0f) * InitialRegenValue;
}

void UCExtendedStatComponent::RegenTick()
{
	if (CurrentValue < GetMaxValue())
	{
		ModifyStat(RegenValue, false);
	}
	else
	{
		ClearRegenTimer();
	}
}

void UCExtendedStatComponent::InitStat()
{
	AActor* Owner = GetOwner();
	if (Owner != nullptr)
	{
		UCStatsManagerComponent* StatsManager = Cast<UCStatsManagerComponent>(Owner->GetComponentByClass(UCStatsManagerComponent::StaticClass()));
		if (StatsManager != nullptr)
		{
			TopValue = StatsManager->GetStatValue(Type, false);
			StatsManager->OnModifierAdded.AddUObject(this, &UCExtendedStatComponent::OnModifierAdded);
			StatsManager->OnModifierRemoved.AddUObject(this, &UCExtendedStatComponent::OnModifierRemoved);
			StatsManager->OnBaseValueChanged.AddUObject(this, &UCExtendedStatComponent::OnBaseValueChanged);
		}
		else
		{
			CLOG_FUNC_TEXT(L"Dose Not Have UCStatsManagerComponent");
		}
	}
	else
	{
		CLOG_FUNC_TEXT(L"Onwer is nullptr");
	}
}

void UCExtendedStatComponent::ClearRegenTimer()
{
	if (GetWorld() != nullptr)
	{
		GetWorld()->GetTimerManager().ClearTimer(RegenTimerHandle);
	}
}

void UCExtendedStatComponent::RefreshRegenTimer()
{
	if (bDoseRegenerates)
	{
		if (!GetWorld()->GetTimerManager().IsTimerActive(RegenTimerHandle))
		{
			// 활성화 된 타이머가 없다.
			GetWorld()->GetTimerManager().SetTimer(RegenTimerHandle, this, &UCExtendedStatComponent::StartRegenerating, ReEnableRegenTime, false);
		}
	}
}

void UCExtendedStatComponent::StartRegenerating()
{
	GetWorld()->GetTimerManager().SetTimer(RegenTimerHandle, this, &UCExtendedStatComponent::RegenTick, RegenerationTickInterval, true);
}

void UCExtendedStatComponent::OnModifierAdded(EStatsType TargetType, float Value)
{
	if (Type == TargetType)
	{
		AddModifier(Value);
	}
}

void UCExtendedStatComponent::OnModifierRemoved(EStatsType TargetType, float Value)
{
	if (Type == TargetType)
	{
		RemoveModifier(Value);
	}
}

void UCExtendedStatComponent::OnBaseValueChanged(EStatsType TargetType, float Value)
{
	if (Type == TargetType)
	{
		TopValue = Value;
		OnValueChanged.Broadcast(CurrentValue, GetMaxValue());
	}
}