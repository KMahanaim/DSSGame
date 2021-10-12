// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once
#include "CoreMinimal.h"
#include "Engine/UserDefinedEnum.h"
#include "CEDirection.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EDirection : uint8
{
	FRONT,
	BACK,
	RIGHT,
	LEFT
};

UCLASS()
class DARKSOUL_API UCEDirection : public UUserDefinedEnum
{
	GENERATED_BODY()
	
};