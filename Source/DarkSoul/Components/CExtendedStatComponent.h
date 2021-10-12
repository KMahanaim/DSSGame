// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DarkSoul/Enumerations/CEStatsType.h"
#include "CExtendedStatComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnValueChanged, float, float);

/**
 * ������ ��� �߰��� ���� ������Ʈ
 * ü��, ���׹̳�, ���� �ڵ� ȸ�� ��, ���� ������ �Ǵ� ��ɵ�
*/
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DARKSOUL_API UCExtendedStatComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly, Category = "Stat", meta = (AllowPrivateAccess = "true"))
		/** ���� Ÿ�� */
		EStatsType Type = EStatsType::NONE;

	UPROPERTY(EditDefaultsOnly, Category = "Stat", meta = (AllowPrivateAccess = "true"))
		/** ���� ȸ�� ��ų�� ���� */
		bool bDoseRegenerates = false;

	UPROPERTY(EditDefaultsOnly, Category = "Stat", meta = (AllowPrivateAccess = "true"))
		/** �ڵ� ȸ�� ��, InitialRegenValue = RegenValue */
		float RegenValue = 1.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Stat", meta = (AllowPrivateAccess = "true"))
		/**	�ڵ� ȸ�� ��Ȱ��ȭ �ð� */
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
	/**	�ڵ� ȸ�� ƽ ����  */
	float RegenerationTickInterval = 0.05f;
	float CurrentValue = 100.0f;
	float TopValue = 100.0f;
	float ModifierValue = 0.0f;
	float InitialRegenValue = 1.0f;
	FTimerHandle RegenTimerHandle;
};