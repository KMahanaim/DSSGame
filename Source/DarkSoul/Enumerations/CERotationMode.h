// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once
#include "CoreMinimal.h"
#include "Engine/UserDefinedEnum.h"
#include "CERotationMode.generated.h"

/**
 * ī�޶�(����) ���
 */
UENUM(BlueprintType)
enum class ERotationMode : uint8
{
	FACE_CAMERA,
	ORIENT_TO_MOVEMENT
};

UCLASS()
class DARKSOUL_API UCERotationMode : public UUserDefinedEnum
{
	GENERATED_BODY()
	
};
