// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once
#include "CoreMinimal.h"
#include "AI/BehaviorTreeNodes/CBTT_NodeBase.h"
#include "DarkSoul/Enumerations/CEMeleeAttackType.h"
#include "CBTT_MeleeAttack.generated.h"

struct FBTMeleeAttacknMemory
{
	uint8* NodeMemory;
	UBehaviorTreeComponent* BTComponent;
	FTimerHandle MeleeAttackHandler;
	FTimerDelegate MeleeAttackFinishedDelegate;
};

UCLASS()
class DARKSOUL_API UCBTT_MeleeAttack : public UCBTT_NodeBase
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
		EMeleeAttackType AttackType = EMeleeAttackType::NONE;
	
protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);

private:
	void OnMeleeAttackFinished(UBehaviorTreeComponent* OwnerComp, uint8* NodeMemory);
	virtual uint16 GetInstanceMemorySize() const override;
};
