// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CUW_ItemSlot.generated.h"

class UImage;

/**
 * 
 */
UCLASS()
class DARKSOUL_API UCUW_ItemSlot : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetItemImage(UTexture2D* NewImage);

protected:
	virtual void NativeConstruct() override;

private:
	UImage* ItemImage = nullptr;
};