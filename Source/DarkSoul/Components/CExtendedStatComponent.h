// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DarkSoul/Enumerations/CEStatsType.h"
#include "CExtendedStatComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnValueChanged, float, float);

/**
 * 스텟의 기능 추가를 위한 컴포넌트
 * 체력, 스테미나, 마나 자동 회복 등, 값이 변동이 되는 기능들
*/
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DARKSOUL_API UCExtendedStatComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly, Category = "Stat", meta = (AllowPrivateAccess = "true"))
		/** 스텟 타입 */
		EStatsType Type = EStatsType::NONE;

	UPROPERTY(EditDefaultsOnly, Category = "Stat", meta = (AllowPrivateAccess = "true"))
		/** 값을 회복 시킬지 여부 */
		bool bDoseRegenerates = false;

	UPROPERTY(EditDefaultsOnly, Category = "Stat", meta = (AllowPrivateAccess = "true"))
		/** 자동 회복 값, InitialRegenValue = RegenValue */
		float RegenValue = 1.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Stat", meta = (AllowPrivateAccess = "true"))
		/**	자동 회복 재활성화 시간 */
		float ReEnableRegenTime = 0.5f;
		
public:	
	UCExtendedStatComponent();

protected:
	virtual void BeginPlay() override;

public:
	void SetRegenerate(const bool bIsRegenerate) { bDoseRegenerates = bIsRegenerate; }
	void SetStatType(const EStatsType NewType) { Type = NewType; }
	void SetCurrentValue(const float Value, const bool InterruptRegener);

	const float GetCurrentValue() const { return CurrentValue; }
	const float GetMaxValue() const { return TopValue + ModifierValue; }
	const float GetPercent() const { return CurrentValue / TopValue * 100.0f; }

	void AddModifier(const float Value);
	void RemoveModifier(const float Value);
	void ModifyStat(const float Value, const bool bInterruptRegener);
	void ChangeRegenPercent(const uint8 Percent);

private:
	void RegenTick();
	void InitStat();
	void ClearRegenTimer();
	void RefreshRegenTimer();
	void StartRegenerating();

	void OnModifierAdded(EStatsType TargetType, float Value);
	void OnModifierRemoved(EStatsType TargetType, float Value);
	void OnBaseValueChanged(EStatsType TargetType, float Value);

public:
	FOnValueChanged OnValueChanged;

private:
	/**	자동 회복 틱 간격  */
	float RegenerationTickInterval = 0.05f;
	float CurrentValue = 100.0f;
	float TopValue = 100.0f;
	float ModifierValue = 0.0f;
	float InitialRegenValue = 1.0f;
	FTimerHandle RegenTimerHandle;
};