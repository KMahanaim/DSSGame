// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once
#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "DarkSoul/Enumerations/CEStateType.h"
#include "DarkSoul/Enumerations/CEAIBehavior.h"
#include "CBTS_UpdateGruxEliteAI.generated.h"

class ACBaseAI;
class AAIController;

//struct FBTUpdateGruxEliteAIMemory
//{
//};

UCLASS()
class DARKSOUL_API UCBTS_UpdateGruxEliteAI : public UBTService
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "Behavior", meta = (AllowPrivateAccess = "true"))
		FBlackboardKeySelector BehaviorKey;

	UPROPERTY(EditAnywhere, Category = "Behavior", meta = (AllowPrivateAccess = "true"))
		FBlackboardKeySelector TargetKey;
	
private:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
	//virtual uint16 GetInstanceMemorySize() const override;
	void SetBehavior(EAIBehavior Behavior);

	void Update();
	void UpdateBehavior();
	void UpdateActivities();

	/** Delegate Bind Function */
	void OnStateChanged(EStateType PrevState, EStateType NewState);

private:
	ACBaseAI* OwnerCharacter;
	AAIController* OwnerController;
};
