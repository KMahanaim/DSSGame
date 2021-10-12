// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once
#include "CoreMinimal.h"
#include "DarkSoul/AI/BehaviorTreeNodes/CBTT_NodeBase.h"
#include "CBTT_ToggleWeapon.generated.h"

struct FBTToggleWeaopnMemory
{
	uint8* NodeMemory;
	UBehaviorTreeComponent* BTComponent;
	FTimerHandle MontagePlayTimeHandler;
	FTimerDelegate MontagePlayFinishedDelegate;
};

UCLASS()
class DARKSOUL_API UCBTT_ToggleWeapon : public UCBTT_NodeBase
{
	GENERATED_BODY()
	
protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);

private:
	void OnMontagePlayFinished(UBehaviorTreeComponent* OwnerComp, uint8* NodeMemory);
	virtual uint16 GetInstanceMemorySize() const override;
};