// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once
#include "CoreMinimal.h"
#include "Engine/UserDefinedEnum.h"
#include "CEInputBufferKey.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EInputBufferKey : uint8
{
	NONE,
	LIGHT_ATTACK,
	HEAVY_ATTACK,
	THRUST_ATTACK,
	SPECIAL_ATTACK,
	FALLING_ATTACK,
	ROLL,
	JUMP,
	PARRY,
	TOGGLE_COMBAT,
	SWITCH_WEAPON_LEFT,
	SWITCH_WEAPON_RIGHT,
	ABILITY_ATTACK,
	SET_SPELL_ACTIVE,
	INTERACTION,
};

UCLASS()
class DARKSOUL_API UCEInputBufferKey : public UUserDefinedEnum
{
	GENERATED_BODY()
	
};
