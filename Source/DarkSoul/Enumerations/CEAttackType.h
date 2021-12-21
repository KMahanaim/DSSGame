// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once
#include "CoreMinimal.h"
#include "Engine/UserDefinedEnum.h"
#include "CEAttackType.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EAttackType : uint8
{
	NONE	UMETA(DisplayName = "None"),
	LIGHT	UMETA(DisplayName = "Light"),
	HEAVY	UMETA(DisplayName = "Heavy"),
	SPECIAL	UMETA(DisplayName = "Special"),
	THRUST	UMETA(DisplayName = "Thrust"),
	FALLING	UMETA(DisplayName = "Falling"),
	MAGIC	UMETA(DisplayName = "Magic"),
};

UCLASS()
class DARKSOUL_API UCEAttackType : public UUserDefinedEnum
{
	GENERATED_BODY()
	
};
