// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once
#include "CoreMinimal.h"
#include "Engine/UserDefinedStruct.h"
#include "DarkSoul/Enumerations/CEItemType.h"
#include "CFItem.generated.h"

class UTexture2D;

/**
 * 
 */
USTRUCT(BlueprintType)
struct FItem
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		FName Name = L"None";

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		FText Description = FText::FromString(L"Item Decription");

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		EItemType Type = EItemType::NONE;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		bool bIsStackable = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		bool bIsDroppable = true;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		bool bIsConsumable = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		UTexture2D* Image;
};

UCLASS()
class DARKSOUL_API UCFItem : public UUserDefinedStruct
{
	GENERATED_BODY()
};
