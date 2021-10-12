// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DarkSoul/Enumerations/CEApplyEffectMethod.h"
#include "DarkSoul/Structs/CFEffect.h"
#include "CEffectsComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnEffectApplied, EEffectType);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnEffectRemoved, EEffectType);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DARKSOUL_API UCEffectsComponent : public UActorComponent
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category = "Tick", meta = (AllowPrivateAccess = "true"))
		/** 업데이트 간격(Interval) */
		float UpdateInterval = 0.1f;

	UPROPERTY(EditDefaultsOnly, Category = "Tick", meta = (AllowPrivateAccess = "true"))
		float BurningDamage = 0.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Tick", meta = (AllowPrivateAccess = "true"))
		float BackAttackDownDamage = 0.0f;

//////////////////////////////////////////////////////////////////////////////////////////////////
// General Functions
//////////////////////////////////////////////////////////////////////////////////////////////////
public:	
	UCEffectsComponent();

protected:
	virtual void BeginPlay() override;

private:
	void UpdateEffectsDuration();
	void UpdateEffect(EEffectType Type, EApplyEffectMethod Method, AActor* Applier, float Duration);
	void RemoveEffect(EEffectType Type);

public:
	const bool ApplyEffect(EEffectType Type, EApplyEffectMethod Method, AActor* Applier, float Duration = 1.0f);
	const bool ApplyBackAttackDown(float Damage, EApplyEffectMethod Method, AActor* Applier, float Duration = 1.0f);
	const bool ApplyBurningEffect(float Damage, EApplyEffectMethod Method, AActor* Applier, float Duration = 1.0f);
	void AdjustEffectTime(EEffectType Type, float NewDuration);

//////////////////////////////////////////////////////////////////////////////////////////////////
// Condition Check Functions
//////////////////////////////////////////////////////////////////////////////////////////////////
public:
	/** If parried, impact, stun(hit), back attack down is applied do not reset state */
	const bool CanBeResetState() const;
	const bool IsEffectApplied(EEffectType Type) const;

public:
	FOnEffectApplied OnEffectApplied;
	FOnEffectRemoved OnEffectRemoved;

private:
	FTimerHandle UpdateEffectsTimerHandle;
	TMap<EEffectType, FEffect> AppliedEffects;
};