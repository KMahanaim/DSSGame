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
	/** true : 이미 실행된 상태, false : 한 번 실행해야 한다*/
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

	// 미구현
	// 현재, State가 변하면서 delegate로 호출이 될 때, 인스턴스화가 부분적으로 되어 있어서 동작에 문제가 생길 수 있다.
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
	/* 타겟 뒤로 이동했던 시간 */
	float TicksStrafe;
	/* 타겟과 근접했던 시간 */
	float TicksNearTarget;
	FTimerHandle OutOfStaminaResetHandler;
	ACBaseAI* OwnerCharacter;
	AAIController* OwnerController;
};