// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once
#include "CoreMinimal.h"
#include "Engine/UserDefinedEnum.h"
#include "CEAIBehavior.generated.h"

/**
 * AI 행동 상태
 */
UENUM(BlueprintType)
enum class EAIBehavior : uint8
{
	IDLE = 0,
	PATROL,
	MELEE_ATTACK,
	RANGE_ATTACK,
	APPROACH,
	FLEE,
	STRAFE_AROUND,
	HIT
};

UCLASS()
class DARKSOUL_API UCEAIbehavior : public UUserDefinedEnum
{
	GENERATED_BODY()
	
};
