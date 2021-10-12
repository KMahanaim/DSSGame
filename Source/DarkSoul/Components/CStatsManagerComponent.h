// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DarkSoul/Structs/CFStat.h"
#include "CStatsManagerComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnModifierAdded, EStatsType, float)
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnModifierRemoved, EStatsType, float)
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnBaseValueChanged, EStatsType, float)

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DARKSOUL_API UCStatsManagerComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly, Category = "Timer", meta = (ClampMax = 10.0f, ClampMin = 0.0f, UIMax = 10.0f, UIMin = 0.0f), meta = (AllowPrivateAccess = "true"))
		float RecentlyReceivedDamageResetTimer = 4.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Stat", meta = (AllowPrivateAccess = "true"))
		TArray<FStat> Stats;


//////////////////////////////////////////////////////////////////////////////////////////////////
// General Functions
//////////////////////////////////////////////////////////////////////////////////////////////////
public:	
	UCStatsManagerComponent();

protected:
	virtual void BeginPlay() override;

public:
	void Initialize();
	void TakeDamage(const float Damage, const bool bIgnoreStamina);

//////////////////////////////////////////////////////////////////////////////////////////////////
// Get, Set Functions
//////////////////////////////////////////////////////////////////////////////////////////////////
public:	
	const float GetDamage() const;
	const float GetStatValue(const EStatsType Type, const bool bIncludeModifiers) const;
	const int32 GetStatIndex(const EStatsType Type) const;

	const float GetRecentlyReceivedDamage() const { return RecentlyReceivedDamage; }
	const float GetRecentlyReceivedDamageStact() const { return RecentlyReceivedDamageStact; }
	const int32 GetRecentlyReceivedHitsCount() const { return RecentlyReceivedHitsCount; }

	void SetStatBaseValue(const EStatsType Type, const float NewValue);

	void ResetRecentlyReceivedDamage();

public:
	FOnModifierAdded OnModifierAdded;
	FOnModifierRemoved OnModifierRemoved;
	FOnBaseValueChanged OnBaseValueChanged;

private:
	void MakeNewStat(const EStatsType NewType, const float NewBaseValue);

private:
	float InitialBlockValue;
	float RecentlyReceivedDamage;
	float RecentlyReceivedDamageStact;
	int32 RecentlyReceivedHitsCount;
	FTimerHandle ResetRecentlyReceivedDamageTimerHandle;
};