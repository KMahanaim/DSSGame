// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once
#include "CoreMinimal.h"
#include "Engine/UserDefinedStruct.h"
#include "DarkSoul/Enumerations/CEStatsType.h"
#include "CFStat.generated.h"

/**
 * �ɷ�ġ�� ����ü�� �����ߴ�.
 * �� �ɷ�ġ Ÿ�Ը��� �ϳ��� ����ü�� ������ ������ �ִ´�.
 */
USTRUCT(BlueprintType)
struct FStat
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		/** �ɷ�ġ Ÿ�� */
		EStatsType Type = EStatsType::NONE;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		/** �ɷ�ġ ��ġ */
		float BaseValue = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		/** �ɷ�ġ ������(������) ��ġ */
		float ModifiersValue = 0.0f;
};

UCLASS()
class DARKSOUL_API UCFStat : public UUserDefinedStruct
{
	GENERATED_BODY()
	
};
