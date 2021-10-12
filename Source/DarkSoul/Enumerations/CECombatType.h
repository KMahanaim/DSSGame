// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once
#include "CoreMinimal.h"
#include "Engine/UserDefinedEnum.h"
#include "CECombatType.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class ECombatType : uint8
{
	NONE,
	MELEE,
	RANGE,
	MAGIC
};

UCLASS()
class DARKSOUL_API UCECombatType : public UUserDefinedEnum
{
	GENERATED_BODY()
	
};
