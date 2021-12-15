// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once
#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "DarkSoul/Enumerations/CEStateType.h"
#include "DarkSoul/Enumerations/CEAIBehavior.h"
#include "CBTS_UpdateBaseAI.generated.h"

class ACBaseAI;
class AAIController;

// Debug
DECLARE_MULTICAST_DELEGATE_OneParam(FOnBehaviorChanged, FString);

struct FBTUpdateAIMemory
{
	/**
	 * Avoid duplicate connections of Delegate
	 * true : connected , false : Not connected
	 */
	bool bIsBindDelegate = false;

	ACBaseAI* OwnerCharacter = nullptr;
	AAIController* OwnerController = nullptr;
};

/**
 * This is the base of all AI UBTService
 * This is functions as search, tracking, patrol, and attack.
 */
UCLASS()
class DARKSOUL_API UCBTS_UpdateBaseAI : public UBTService
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, Category = "Behavior", meta = (ClampMax = 100.0f, ClampMin = 0.0f, UIMax = 100.0f, UIMin = 0.0f), meta = (AllowPrivateAccess = "true"))
		float EnoughNearTargetTime = 3.0f;

	UPROPERTY(EditAnywhere, Category = "Behavior", meta = (ClampMax = 1000.0f, ClampMin = 0.0f, UIMax = 1000.0f, UIMin = 0.0f), meta = (AllowPrivateAccess = "true"))
		float AttackBehaviorRange = 500.0f;

	UPROPERTY(EditAnywhere, Category = "Behavior", meta = (AllowPrivateAccess = "true"))
		FBlackboardKeySelector BehaviorKey;

	UPROPERTY(EditAnywhere, Category = "Behavior", meta = (AllowPrivateAccess = "true"))
		FBlackboardKeySelector TargetKey;

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	virtual uint16 GetInstanceMemorySize() const final;
	void SetBehavior(EAIBehavior Behavior);

	void ResetIsOutOfStamina() { bIsOutOfStamina = false; }

	virtual void Update();
	virtual void UpdateBehavior(ACBaseAI* const OwnerCharacter, AAIController* const OwnerController);
	virtual void UpdateActivities(ACBaseAI* const OwnerCharacter, AAIController* const OwnerController);

	/** Delegate Bind Function */
	void OnStateChanged(EStateType PrevState, EStateType NewState);
	void OnStaminaValueChanged(float NewValue, float MaxValue);

protected:
	bool bIsOutOfStamina = false;
	/* 타겟 뒤로 이동했던 시간 */
	float TicksStrafe;
	/* 타겟과 근접했던 시간 */
	float TicksNearTarget;
	FTimerHandle OutOfStaminaResetHandler;
	FBTUpdateAIMemory* InstanceMemory;

//////////////////////////////////////////////////////////////////////////////////////////////////
// Debug
//////////////////////////////////////////////////////////////////////////////////////////////////
protected:
	void DebugBehavior(const EAIBehavior Behavior) const;

public:
	FOnBehaviorChanged OnBehaviorChanged;
};