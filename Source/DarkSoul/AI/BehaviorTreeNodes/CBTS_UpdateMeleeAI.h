// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once
#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "DarkSoul/Enumerations/CEStateType.h"
#include "DarkSoul//Enumerations/CEAIBehavior.h"
#include "CBTS_UpdateMeleeAI.generated.h"

class ACBaseAI;
class AAIController;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnBehaviorChanged, FString);

struct FBTUpdateMeleeAIMemory
{
	/** true : �̹� ����� ����, false : �� �� �����ؾ� �Ѵ�*/
	bool bIsToDoBindFunc = false;
};

UCLASS()
class DARKSOUL_API UCBTS_UpdateMeleeAI : public UBTService
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "Behavior", meta = (ClampMax = 100.0f, ClampMin = 0.0f, UIMax = 100.0f, UIMin = 0.0f), meta = (AllowPrivateAccess = "true"))
		float EnoughNearTargetTime = 3.0f;

	UPROPERTY(EditAnywhere, Category = "Behavior", meta = (ClampMax = 1000.0f, ClampMin = 0.0f, UIMax = 1000.0f, UIMin = 0.0f), meta = (AllowPrivateAccess = "true"))
		float AttackBehaviorRange = 500.0f;

	UPROPERTY(EditAnywhere, Category = "Behavior", meta = (AllowPrivateAccess = "true"))
		FBlackboardKeySelector BehaviorKey;

	UPROPERTY(EditAnywhere, Category = "Behavior", meta = (AllowPrivateAccess = "true"))
		FBlackboardKeySelector TargetKey;
	
private:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	virtual uint16 GetInstanceMemorySize() const override;
	void SetBehavior(EAIBehavior Behavior);

	void ResetIsOutOfStamina() { bIsOutOfStamina = false; }

	// �̱���
	// ����, State�� ���ϸ鼭 delegate�� ȣ���� �� ��, �ν��Ͻ�ȭ�� �κ������� �Ǿ� �־ ���ۿ� ������ ���� �� �ִ�.
	void Update();
	void UpdateBehavior();
	void UpdateActivities();

	/** Delegate Bind Function */
	void OnStateChanged(EStateType PrevState, EStateType NewState);
	void OnStaminaValueChanged(float NewValue, float MaxValue);

public:
	FOnBehaviorChanged OnBehaviorChanged;

private:
	bool bIsOutOfStamina = false;
	/* Ÿ�� �ڷ� �̵��ߴ� �ð� */
	float TicksStrafe;
	/* Ÿ�ٰ� �����ߴ� �ð� */
	float TicksNearTarget;
	FTimerHandle OutOfStaminaResetHandler;
	ACBaseAI* OwnerCharacter;
	AAIController* OwnerController;
};