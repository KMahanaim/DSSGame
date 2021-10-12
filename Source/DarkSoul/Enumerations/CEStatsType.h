// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once
#include "CoreMinimal.h"
#include "Engine/UserDefinedEnum.h"
#include "CEStatsType.generated.h"

/**
 * 능력치 타입을 구분한다.
 * 체력, 스테미나, 공격력, 치명타, 방어력 등
 */
UENUM(BlueprintType)
enum class EStatsType : uint8
{
	NONE,
	HEALTH,
	STAMINA,
	MANA,
	DAMAGE,
	ARMOR,
	CRITICAL_CHANCE,
	CRITICAL_MULTIPLIER,
	ATTACK_SPEED,
	BLOCK,
	MELEE_ATTACK_STAMINA_COST,
	MAGIC_DAMAGE,
	CASTING_SPEED,
};

UCLASS()
class DARKSOUL_API UCEStatsType : public UUserDefinedEnum
{
	GENERATED_BODY()
	
};
