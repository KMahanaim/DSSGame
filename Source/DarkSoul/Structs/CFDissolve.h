// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once
#include "CoreMinimal.h"
#include "Engine/UserDefinedStruct.h"
#include "CFDissolve.generated.h"

class UMaterialInterface;
class UMaterialInstanceDynamic;

/**
 * 
 */
USTRUCT(BlueprintType)
struct FDissolve
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
		UPrimitiveComponent* Component;

	UPROPERTY(EditAnywhere)
		float Value;

	UPROPERTY(EditAnywhere)
		TArray<UMaterialInterface*> Materials;

	UPROPERTY(EditAnywhere)
		TArray<UMaterialInstanceDynamic*> DissolveMaterials;

	UPROPERTY(EditAnywhere)
		bool bReverse;

	UPROPERTY(EditAnywhere)
		bool bIsRunning;
};

UCLASS()
class DARKSOUL_API UCFDissolve : public UUserDefinedStruct
{
	GENERATED_BODY()
};