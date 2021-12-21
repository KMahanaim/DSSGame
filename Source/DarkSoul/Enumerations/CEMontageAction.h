// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once
#include "CoreMinimal.h"
#include "Engine/UserDefinedEnum.h"
#include "CEMontageAction.generated.h"

/**
 * Stun is Hit Motion
 */
UENUM(BlueprintType)
enum class EMontageAction : uint8
{
	NONE				UMETA(DisplayName = "None"),
	LIGHT_ATTACK		UMETA(DisplayName = "LightAttack"),
	HEAVY_ATTACK		UMETA(DisplayName = "HeavyAttack"),
	THRUST_ATTACK		UMETA(DisplayName = "ThrustAttack"),
	SPECIAL_ATTACK		UMETA(DisplayName = "SpecialAttack"),
	FALLING_ATTACK		UMETA(DisplayName = "FallingAttack"),
	MAGIC_CASTING		UMETA(DisplayName = "MagicCasting"),
	ROLL_FORWARD		UMETA(DisplayName = "RollForward"),
	ROLL_BACKWARD		UMETA(DisplayName = "RollBackward"),
	DRAW_WEAPON			UMETA(DisplayName = "DrawWeapon"),
	UNDRAW_WEAPON		UMETA(DisplayName = "UnDrawWeapon"),
	IMPACT				UMETA(DisplayName = "Impact"),
	BLOCK				UMETA(DisplayName = "Block"),
	PARRY				UMETA(DisplayName = "Parry"),
	PARRIED				UMETA(DisplayName = "Parried"),
	HIT_FRONT			UMETA(DisplayName = "HitFront"),
	HIT_BACK			UMETA(DisplayName = "HitBack"),
	HIT_RIGHT			UMETA(DisplayName = "HitRight"),
	HIT_LEFT			UMETA(DisplayName = "HitLeft"),
	BACK_ATTACK_DOWN	UMETA(DisplayName = "BackAttackDown"),
	DEAD				UMETA(DisplayName = "Dead")
};

UCLASS()
class DARKSOUL_API UCEMontageAction : public UUserDefinedEnum
{
	GENERATED_BODY()
	
};
