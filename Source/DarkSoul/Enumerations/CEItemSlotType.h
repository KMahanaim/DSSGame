// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "Engine/UserDefinedEnum.h"
#include "CEItemSlotType.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EItemSlotType : uint8
{
	PREV_WEAPON_SLOT,
	CURRENT_WEAPON_SLOT,
	NEXT_WEAPON_SLOT,
	ACTIVE_ITEM_SLOT
};


UCLASS()
class DARKSOUL_API UCEItemSlotType : public UUserDefinedEnum
{
	GENERATED_BODY()
	
};
