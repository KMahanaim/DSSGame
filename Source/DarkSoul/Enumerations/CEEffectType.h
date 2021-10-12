// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once
#include "CoreMinimal.h"
#include "Engine/UserDefinedEnum.h"
#include "CEEffectType.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EEffectType : uint8
{
	NONE,
	STUN,
	KNOCK_DOWN,
	BURNING,
	BACK_ATTACK_DOWN,
	IMPACT,
	PARRIED
};

UCLASS()
class DARKSOUL_API UCEEffectType : public UUserDefinedEnum
{
	GENERATED_BODY()
	
};