// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CUW_LoadingScreen.generated.h"

class UImage;
class UPaperFlipbook;

/**
 * 
 */
UCLASS()
class DARKSOUL_API UCUW_LoadingScreen : public UUserWidget
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly, Category = "Sprite", meta = (AllowPrivateAccess = "true"))
		UPaperFlipbook* PaperFlipbook;
	
protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	void SetBackgroundSize(FVector2D Size);

private:
	int8 Delay;
	int8 FrameCounter;
	UImage* LoadingIcon;
	UImage* Background;
};
