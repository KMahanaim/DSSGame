// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "Engine/UserDefinedEnum.h"
#include "CEStateType.generated.h"

/**
 * 캐릭터 상태 종류를 정의 했다.
 * 모든 캐릭터는 아래 상태를 기준으로 행동한다.
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