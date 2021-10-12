// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once
#include "CoreMinimal.h"
#include "Engine/UserDefinedStruct.h"
#include "DarkSoul/Enumerations/CEEffectType.h"
#include "CFEffect.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FEffect
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EEffectType Type = EEffectType::NONE;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Duration = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		AActor* Applier = nullptr;
};

UCLASS()
class DARKSOUL_API UCFEffect : public UUserDefinedStruct
{
	GENERATED_BODY()
	
};