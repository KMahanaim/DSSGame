// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once
#include "CoreMinimal.h"
#include "Engine/UserDefinedStruct.h"
#include "CFInteractionMassage.generated.h"

class UTexture2D;

/**
 * 
 */
USTRUCT(BlueprintType)
struct FInteractionMassage
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		FText Massage = FText::FromString(L"Interaction Massage");

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		UTexture2D* KeyIcon;
};

UCLASS()
class DARKSOUL_API UCFInteractionMassage : public UUserDefinedStruct
{
	GENERATED_BODY()
};
