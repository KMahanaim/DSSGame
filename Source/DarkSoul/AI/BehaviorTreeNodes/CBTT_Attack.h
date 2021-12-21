// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once
#include "CoreMinimal.h"
#include "AI/BehaviorTreeNodes/CBTT_NodeBase.h"
#include "DarkSoul/Enumerations/CEAttackType.h"
#include "CBTT_Attack.generated.h"

struct FBTAttacknMemory
{
	uint8* NodeMemory;
	UBehaviorTreeComponent* BTComponent;
	FTimerHandle AttackHandler;
	FTimerDelegate AttackFinishedDelegate;
};

UCLASS()
class DARKSOUL_API UCBTT_Attack : public UCBTT_NodeBase
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
		EAttackType AttackType = EAttackType::NONE;
	
protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);

private:
	void OnAttackFinished(UBehaviorTreeComponent* OwnerComp, uint8* NodeMemory);
	virtual uint16 GetInstanceMemorySize() const override;
};
