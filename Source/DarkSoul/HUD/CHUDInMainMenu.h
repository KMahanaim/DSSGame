// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once
#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "CHUDInMainMenu.generated.h"

class UTextBlock;
class UUserWidget;
class UMaterialInterface;

class ACUIKeyInputPawn;
class UCGameLoadingComponent;

/**
 * 
 */
UCLASS()
class DARKSOUL_API ACHUDInMainMenu : public AHUD
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditAnywhere, Category = "Menu", meta = (AllowPrivateAccess = "true"))
		TSubclassOf<UUserWidget> HUDWidgetClass = nullptr;

	UPROPERTY(EditAnywhere, Category = "Menu", meta = (AllowPrivateAccess = "true"))
		UUserWidget* CurrentWidget = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Menu", meta = (AllowPrivateAccess = "true"))
		FSlateColor SelectColor;

	UPROPERTY(EditDefaultsOnly, Category = "Menu", meta = (AllowPrivateAccess = "true"))
		UMaterialInterface* MenuBackground;

	UPROPERTY(VisibleAnywhere, Category = "Component", meta = (AllowPrivateAccess = "true"))
		UCGameLoadingComponent* LoadingManager;

public:
	ACHUDInMainMenu();
	virtual void BeginPlay() override;
	virtual void DrawHUD() override;

private:
	void ArrowUp();
	void ArrowDown();

	void EnterMenu();
	void OpenLevel();

private:
	int32 SelectMenu;
	int32 CurrentMenu;
	TArray<UTextBlock*> MenuList;
	UMaterialInterface* DrawBackground;
	FTimerHandle LevelOpenDelayTimerHandle;
};