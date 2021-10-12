// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once
#include "CoreMinimal.h"
#include "Engine/UserDefinedEnum.h"
#include "CEItemType.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EItemType : uint8
{
	NONE,
	SPELL,
	SHIELD,
	HEAD,
	TOP,
	LEGS,
	HANDS,
	FEET,
	ARROWS,
	TOOL,
	MATERIAL,
	RING,
	MELEE_WEAPON,
	RANGE_WEAPON,
	NECKLACE
};

UCLASS()
class DARKSOUL_API UCEItemType : public UUserDefinedEnum
{
	GENERATED_BODY()
	
};
