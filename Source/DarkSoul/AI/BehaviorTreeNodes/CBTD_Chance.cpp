// Copyright Epic Games, Inc. All Rights Reserved.
#include "CBTD_Chance.h"

/// Unreal Engine
#include "Kismet/KismetMathLibrary.h"

bool UCBTD_Chance::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	if (UKismetMathLibrary::RandomIntegerInRange(1, 100) <= ChancePercent)
	{
		// OK
		return true;
	}

	// Failed
	return false;
}