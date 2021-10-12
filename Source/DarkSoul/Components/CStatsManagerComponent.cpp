// Copyright Epic Games, Inc. All Rights Reserved.
#include "CStatsManagerComponent.h"
#include "DarkSoul/_Utility/CLog.h"
#include "DarkSoul/Characters/CCombatCharacter.h"
#include "DarkSoul/Components/CExtendedStatComponent.h"

/// Unreal Engine
#include "TimerManager.h"
#include "Engine/World.h"

UCStatsManagerComponent::UCStatsManagerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;


}

void UCStatsManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	// 플레이어 캐릭터에 필요한 기본적인 스탯 추가
	// 생성자에서 하면 Error 발생
	{
		MakeNewStat(EStatsType::HEALTH, 100.0f);
		MakeNewStat(EStatsType::STAMINA, 100.0f);
		MakeNewStat(EStatsType::MANA, 100.0f);
		MakeNewStat(EStatsType::DAMAGE, 10.0f);
		MakeNewStat(EStatsType::ARMOR, 0.0f);
		MakeNewStat(EStatsType::CRITICAL_CHANCE, 5.0f);
		MakeNewStat(EStatsType::CRITICAL_MULTIPLIER, 1.0f);
		MakeNewStat(EStatsType::ATTACK_SPEED, 1.0f);
		MakeNewStat(EStatsType::BLOCK, 50.0f);
		MakeNewStat(EStatsType::MELEE_ATTACK_STAMINA_COST, 15.0f);
		MakeNewStat(EStatsType::MAGIC_DAMAGE, 20.0f);
		MakeNewStat(EStatsType::CASTING_SPEED, 1.0f);
	}
}

void UCStatsManagerComponent::Initialize()
{
	InitialBlockValue = GetStatValue(EStatsType::BLOCK, false);
}

void UCStatsManagerComponent::TakeDamage(const float Damage, const bool bIgnoreStamina)
{
	CLOG_FUNC;

	ACCombatCharacter* Owner = Cast<ACCombatCharacter>(GetOwner());
	CLOG_ERROR_CHECK_RETURN(Owner);

	UCExtendedStatComponent* Health = Owner->GetExtendedStatComponent(EStatsType::HEALTH);
	CLOG_ERROR_CHECK_RETURN_TEXT(Health, Owner->GetName());

	GetWorld()->GetTimerManager().ClearTimer(ResetRecentlyReceivedDamageTimerHandle);
	GetWorld()->GetTimerManager().SetTimer(ResetRecentlyReceivedDamageTimerHandle, this, &UCStatsManagerComponent::ResetRecentlyReceivedDamage, RecentlyReceivedDamageResetTimer);

	// Calculate damage reduced by armor
	/// EX) Damage 10, Armor 50 -> Reduced Damage = 5 (10 - 10 * 0.5)
	const float Armor = GetStatValue(EStatsType::ARMOR, true);
	float ReducedDamage = Damage - ((Armor / 100) * Damage);
	ReducedDamage = FMath::Clamp(ReducedDamage, 0.0f, ReducedDamage);

	// If the function ignores the stamina, all it has to do is modify the stamina.
	if (bIgnoreStamina)
	{
		Health->ModifyStat(ReducedDamage * -1, true);

		RecentlyReceivedDamageStact += ReducedDamage;
		RecentlyReceivedHitsCount++;
		RecentlyReceivedDamage = ReducedDamage;
	}
	else
	{
		UCExtendedStatComponent* Stamina = Owner->GetExtendedStatComponent(EStatsType::STAMINA);
		CLOG_ERROR_CHECK_RETURN_TEXT(Stamina, Owner->GetName());

		// Calculate blocked damage
		/// EX) Reduced Damage 5, Block 80 -> Blocked Damage = 4 (5 * 0.8) -> if enough stamina take damage 1
		float Block = GetStatValue(EStatsType::BLOCK, true);
		float BlockedDamage = ReducedDamage * (Block / 100);

		// Check the enough stamina
		const bool bIsEnoughStamina = Stamina->GetCurrentValue() >= BlockedDamage;
		if (bIsEnoughStamina)
		{
			Stamina->ModifyStat(BlockedDamage * -1.25f, true);
		}
		else
		{
			Stamina->SetCurrentValue(0.0f, true);
			BlockedDamage = Stamina->GetCurrentValue();
		}

		ReducedDamage -= BlockedDamage;
		Health->ModifyStat(ReducedDamage * -1, true);
		RecentlyReceivedDamage = ReducedDamage;
	}
}

const float UCStatsManagerComponent::GetDamage() const
{
	float Damage = GetStatValue(EStatsType::DAMAGE, true);
	if (FMath::RandRange(1.0f, 100.0f) <= GetStatValue(EStatsType::CRITICAL_CHANCE, true))
	{
		return FMath::TruncToInt(Damage * GetStatValue(EStatsType::CRITICAL_MULTIPLIER, true));
	}
	else
	{
		return FMath::TruncToInt(Damage * 1.0f);
	}
}

const float UCStatsManagerComponent::GetStatValue(const EStatsType Type, const bool bIncludeModifiers) const
{
	int32 Index = GetStatIndex(Type);
	if (Index >= 0)
	{
		if (bIncludeModifiers)
		{
			return Stats[Index].BaseValue + Stats[Index].ModifiersValue;
		}
		else
		{
			return Stats[Index].BaseValue;
		}
	}
	return 0.0f;
}

const int32 UCStatsManagerComponent::GetStatIndex(const EStatsType Type) const
{
	if (Type == EStatsType::NONE)
	{
		CLOG_FUNC_TEXT(L"Type is NONE, Can not Find Type");
		return int32(INDEX_NONE);
	}

	for (int32 Index = 0; Index < Stats.Num(); Index++)
	{
		if (Stats[Index].Type == Type)
		{
			return Index;
		}
	}

	CLOG_ERROR_FUNC_TEXT(FString::FromInt(Stats.Num()));
	return int32(INDEX_NONE);
}

void UCStatsManagerComponent::SetStatBaseValue(const EStatsType Type, const float NewValue)
{
	int32 Index = GetStatIndex(Type);
	if (Index == INDEX_NONE)
	{
		MakeNewStat(Type, NewValue);
		CLOG_ERROR_RETURN_TEXT(L"Index NONE");
	}

	Stats[Index].BaseValue = NewValue;
	OnBaseValueChanged.Broadcast(Type, NewValue);
}

void UCStatsManagerComponent::ResetRecentlyReceivedDamage()
{
	RecentlyReceivedDamage = 0.0f;
	RecentlyReceivedDamageStact = 0.0f;
	RecentlyReceivedHitsCount = 0;
}

void UCStatsManagerComponent::MakeNewStat(const EStatsType NewType, const float NewBaseValue)
{
	FStat NewStat;
	NewStat.Type = NewType;
	NewStat.BaseValue = NewBaseValue;
	Stats.Emplace(NewStat);
}