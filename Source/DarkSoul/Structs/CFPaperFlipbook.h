// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once
#include "CoreMinimal.h"
#include "Engine/UserDefinedStruct.h"
#include "CFPaperFlipbook.generated.h"

class UPaperFlipbook;

USTRUCT(BlueprintType)
struct FPaperFlipbookData
{
	GENERATED_USTRUCT_BODY();

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UPaperFlipbook* PaperFlipbook = nullptr;
};

/**
 * 
 */
UCLASS()
class DARKSOUL_API UCFPaperFlipbook : public UUserDefinedStruct
{
	GENERATED_BODY()
	
};
