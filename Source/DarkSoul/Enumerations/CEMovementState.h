// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once
#include "CoreMinimal.h"
#include "Engine/UserDefinedEnum.h"
#include "CEMovementState.generated.h"

/**
 * 이동 상태에 따라서 유형이 바뀐다.
 */
UENUM(BlueprintType)
enum class EMovementState : uint8
{
	IDLE,
	WALK,
	JOG,
	RUN
};

UCLASS()
class DARKSOUL_API UCEMovementState : public UUserDefinedEnum
{
	GENERATED_BODY()
	
};
