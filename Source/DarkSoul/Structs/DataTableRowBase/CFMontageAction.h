// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once
#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "DarkSoul/Enumerations/CEMontageAction.h"
#include "Engine/UserDefinedStruct.h"
#include "CFMontageAction.generated.h"

class UAnimMontage;

/**
 * 
 */
USTRUCT(BlueprintType)
struct FMontageAction : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EMontageAction			Action;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<UAnimMontage*>	Montages;
};

UCLASS()
class DARKSOUL_API UCFMontageAction : public UUserDefinedStruct
{
	GENERATED_BODY()
	
};
