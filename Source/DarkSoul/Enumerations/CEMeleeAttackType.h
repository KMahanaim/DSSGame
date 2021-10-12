// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once
#include "CoreMinimal.h"
#include "Engine/UserDefinedEnum.h"
#include "CEMeleeAttackType.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EMeleeAttackType : uint8
{
	NONE	UMETA(DisplayName = "None"),
	LIGHT	UMETA(DisplayName = "Light"),
	HEAVY	UMETA(DisplayName = "Heavy"),
	SPECIAL	UMETA(DisplayName = "Special"),
	THRUST	UMETA(DisplayName = "Thrust"),
	FALLING	UMETA(DisplayName = "Falling")
};

UCLASS()
class DARKSOUL_API UCEMeleeAttackType : public UUserDefinedEnum
{
	GENERATED_BODY()
	
};
