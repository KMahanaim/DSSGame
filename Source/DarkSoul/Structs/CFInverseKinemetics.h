// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once
#include "CoreMinimal.h"
#include "Engine/UserDefinedStruct.h"
#include "CFInverseKinemetics.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FInverseKinemetics
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		float Location_Left;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		float Location_Right;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		float HipOffset;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		FRotator Rotation_Left;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		FRotator Rotation_Right;
};

UCLASS()
class DARKSOUL_API UCFInverseKinemetics : public UUserDefinedStruct
{
	GENERATED_BODY()
};
