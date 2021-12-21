// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once
#include "CoreMinimal.h"
#include "Engine/UserDefinedEnum.h"
#include "CEWeaponType.generated.h"

/**
 * NONE, UNARMED�� ���� ���̴�.
 * END, ���������� ������ ������ ã�� ���ؼ� �߰��� ����̴�.
 */
UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	/** Player Weapons */
	NONE = 0		UMETA(DisplayName = "None"),
	UNARMED			UMETA(DisplayName = "Unarmed"),
	TWIN_SWORD		UMETA(DisplayName = "TwinSword"),
	TWIN_BLADES		UMETA(DisplayName = "TwinBlades"),

	/** Monster Weapons */
	SWORD			UMETA(DisplayName = "Sword"),
	BODY_WEAPON		UMETA(DisplayName = "BodyWeapon"), 
};

UCLASS()
class DARKSOUL_API UCEWeaponType : public UUserDefinedEnum
{
	GENERATED_BODY()
};
