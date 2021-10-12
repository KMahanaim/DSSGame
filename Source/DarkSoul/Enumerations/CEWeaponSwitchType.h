// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once
#include "CoreMinimal.h"
#include "Engine/UserDefinedEnum.h"
#include "CEWeaponSwitchType.generated.h"

/**
 * ���� ��ȯ ����� �����ߴ�.
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
