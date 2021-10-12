// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once
#include "CoreMinimal.h"
#include "Engine/UserDefinedEnum.h"
#include "CEApplyEffectMethod.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EApplyEffectMethod : uint8
{
	STACK,
	REPLACE
};

UCLASS()
class DARKSOUL_API UCEApplyEffectMethod : public UUserDefinedEnum
{
	GENERATED_BODY()
	
};
