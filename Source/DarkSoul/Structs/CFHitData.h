// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once
#include "CoreMinimal.h"
#include "Engine/UserDefinedStruct.h"
#include "CFHitData.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FHitData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Damage = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		/** 데미지를 준 상대 */
		AActor* DamageCauser = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector HitFromDirection = FVector(0.0f, 0.0f, 0.0f);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bCanBeParried = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bCanBeBlocked = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bCanReceiveImpact = true;
};

UCLASS()
class DARKSOUL_API UCFHitData : public UUserDefinedStruct
{
	GENERATED_BODY()
};
