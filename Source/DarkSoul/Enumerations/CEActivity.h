// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once
#include "CoreMinimal.h"
#include "Engine/UserDefinedEnum.h"
#include "CEActivity.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EActivity : uint8
{
	NONE,
	IS_BLOCKING_PRESSED,
	IS_AIMING_PRESSED,
	IS_IMMORTAL,
	CANT_BE_INTERRUPTED,
	IS_LOOKING_FORWARD,
	CAN_PARRY_HIT,
	IS_ZOOMING
};

UCLASS()
class DARKSOUL_API UCEActivity : public UUserDefinedEnum
{
	GENERATED_BODY()
};
