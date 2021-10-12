// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once
#include "CoreMinimal.h"
#include "Engine/UserDefinedEnum.h"
#include "CEAttackResult.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EAttackResult : uint8
{
	SUCCESS,
	FAILED,
	BLOCKED,
	PARRIED
};

UCLASS()
class DARKSOUL_API UCEAttackResult : public UUserDefinedEnum
{
	GENERATED_BODY()
};
