// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once
#include "CoreMinimal.h"
#include "Engine/UserDefinedEnum.h"
#include "CEWeaponSwitchType.generated.h"

/**
 * 무기 전환 방식을 정의했다.
 */
UENUM(BlueprintType)
enum class EWeaponSwitchType : uint8
{
	LEFT,
	RIGHT,
};

UCLASS()
class DARKSOUL_API UCEWeaponSwitchType : public UUserDefinedEnum
{
	GENERATED_BODY()
	
};
