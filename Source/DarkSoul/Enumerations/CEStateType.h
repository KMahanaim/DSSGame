// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "Engine/UserDefinedEnum.h"
#include "CEStateType.generated.h"

/**
 * ĳ���� ���� ������ ���� �ߴ�.
 * ��� ĳ���ʹ� �Ʒ� ���¸� �������� �ൿ�Ѵ�.
 */
UENUM(BlueprintType)
enum class EStateType : uint8
{
	IDLE,
	DISABLED,
	ATTACKING,
	ROLLING,
	FALLING,
	INTERACTING,
	BACK_STABBING,
	PARRYING,
	DEAD
};

UCLASS()
class DARKSOUL_API UCEStateType : public UUserDefinedEnum
{
	GENERATED_BODY()

};