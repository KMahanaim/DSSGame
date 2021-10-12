// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once
#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "CBTD_Chance.generated.h"

UCLASS()
class DARKSOUL_API UCBTD_Chance : public UBTDecorator
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
		int32 ChancePercent = 100;

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const;
};
