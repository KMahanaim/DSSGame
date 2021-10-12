// Copyright Epic Games, Inc. All Rights Reserved.
#include "CEffectsComponent.h"
#include "DarkSoul/_Utility/CLog.h"
#include "DarkSoul/Interfaces/I_CCanGetEffects.h"

/// Unreal Engine
#include "TimerManager.h"
#include "Engine/World.h"
#include "Kismet/KismetSystemLibrary.h"

UCEffectsComponent::UCEffectsComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UCEffectsComponent::BeginPlay()
{
	Super::BeginPlay();

	UWorld* World = GetWorld();
	CLOG_ERROR_CHECK_RETURN(World);

	World->GetTimerManager().SetTimer(UpdateEffectsTimerHandle, this, &UCEffectsComponent::UpdateEffectsDuration, UpdateInterval, true);
}

void UCEffectsComponent::UpdateEffectsDuration()
{
	if (AppliedEffects.Num() > 0)
	{
		for (auto Effect = AppliedEffects.CreateIterator(); Effect; ++Effect)
		{
			Effect->Value.Duration -= UpdateInterval;
			if (Effect->Value.Duration <= UpdateInterval)
			{
				RemoveEffect(Effect->Key);
			}
		}
	}
}

void UCEffectsComponent::UpdateEffect(EEffectType Type, EApplyEffectMethod Method, AActor* Applier, float Duration)
{
	FEffect* Effect = AppliedEffects.Find(Type);
	if (Effect != nullptr)
	{
		// Was already applied effect
		Effect->Applier = Applier;

		switch (Method)
		{
			case EApplyEffectMethod::STACK :
			{
				Effect->Duration += Duration;
			}
			break;
			case EApplyEffectMethod::REPLACE:
			{
				Effect->Duration = Duration;
			}
			break;
		}
	}
	else
	{
		// Add the new effect
		FEffect NewEffect;
		NewEffect.Type = Type;
		NewEffect.Duration = Duration;
		NewEffect.Applier = Applier;

		AppliedEffects.Add(Type, NewEffect);
	}
}

void UCEffectsComponent::RemoveEffect(EEffectType Type)
{
	AppliedEffects.Remove(Type);
	OnEffectRemoved.Broadcast(Type);
}

const bool UCEffectsComponent::ApplyEffect(EEffectType Type, EApplyEffectMethod Method, AActor* Applier, float Duration/**= 1.0f*/)
{
	bool bApplied = false;

	AActor* Owner = GetOwner();
	if (Owner == nullptr)
	{
		CLOG_ERROR_FUNC;
		return bApplied;
	}

	if (UKismetSystemLibrary::DoesImplementInterface(Owner, UI_CCanGetEffects::StaticClass()))
	{
		II_CCanGetEffects* OwnerEffect = Cast<II_CCanGetEffects>(Owner);
		bApplied = OwnerEffect->CanEffectBeApplied(Type, Applier);
	}

	UpdateEffect(Type, Method, Applier, Duration);
	OnEffectApplied.Broadcast(Type);
	bApplied = true;

	return bApplied;
}

const bool UCEffectsComponent::ApplyBackAttackDown(float Damage, EApplyEffectMethod Method, AActor* Applier, float Duration/**= 1.0f*/)
{
	bool bApplied = false;
	const EEffectType Type = EEffectType::BACK_ATTACK_DOWN;

	AActor* Owner = GetOwner();
	if (Owner == nullptr)
	{
		CLOG_ERROR_FUNC;
		return bApplied;
	}

	if (UKismetSystemLibrary::DoesImplementInterface(Owner, UI_CCanGetEffects::StaticClass()))
	{
		II_CCanGetEffects* OwnerEffect = Cast<II_CCanGetEffects>(Owner);
		if (OwnerEffect->CanEffectBeApplied(Type, Applier))
		{
			BackAttackDownDamage = Damage;
			UpdateEffect(Type, Method, Applier, Duration);
			OnEffectApplied.Broadcast(Type);
			bApplied = true;
		}
	}

	return bApplied;
}

const bool UCEffectsComponent::ApplyBurningEffect(float Damage, EApplyEffectMethod Method, AActor* Applier, float Duration/**= 1.0f*/)
{
	bool bApplied = false;

	AActor* Owner = GetOwner();
	if (Owner == nullptr)
	{
		CLOG_ERROR_FUNC;
		return bApplied;
	}

	if (UKismetSystemLibrary::DoesImplementInterface(Owner, UI_CCanGetEffects::StaticClass()))
	{
		II_CCanGetEffects* OwnerEffect = Cast<II_CCanGetEffects>(Owner);
		bApplied = OwnerEffect->CanEffectBeApplied(EEffectType::BURNING, Applier);

		if (bApplied == false)
		{
			return bApplied;
		}
	}

	BurningDamage = Damage;
	UpdateEffect(EEffectType::BURNING, Method, Applier, Duration);

	OnEffectApplied.Broadcast(EEffectType::BURNING);

	return bApplied;
}

void UCEffectsComponent::AdjustEffectTime(EEffectType Type, float NewDuration)
{
	FEffect* Effect = AppliedEffects.Find(Type);
	if (Effect != nullptr)
	{
		Effect->Duration = NewDuration;
	}
}

const bool UCEffectsComponent::CanBeResetState() const
{
	TArray<EEffectType> Types;
	Types.Emplace(EEffectType::STUN);
	Types.Emplace(EEffectType::IMPACT);
	Types.Emplace(EEffectType::PARRIED);
	Types.Emplace(EEffectType::BACK_ATTACK_DOWN);

	for (int32 Index = 0; Index < Types.Num(); Index++)
	{
		if (AppliedEffects.Contains(Types[Index]))
		{
			return false;
		}
	}

	return true;
}

const bool UCEffectsComponent::IsEffectApplied(EEffectType Type) const
{
	return AppliedEffects.Contains(Type);
}