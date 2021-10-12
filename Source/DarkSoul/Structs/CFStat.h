// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once
#include "CoreMinimal.h"
#include "Engine/UserDefinedStruct.h"
#include "DarkSoul/Enumerations/CEStatsType.h"
#include "CFStat.generated.h"

/**
 * 능력치를 구조체로 정의했다.
 * 각 능력치 타입마다 하나의 구조체를 생성해 가지고 있는다.
 */
USTRUCT(BlueprintType)
struct FStat
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		/** 능력치 타입 */
		EStatsType Type = EStatsType::NONE;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		/** 능력치 수치 */
		float BaseValue = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		/** 능력치 수정자(변경자) 수치 */
		float ModifiersValue = 0.0f;
};

UCLASS()
class DARKSOUL_API UCFStat : public UUserDefinedStruct
{
	GENERATED_BODY()
	
};
