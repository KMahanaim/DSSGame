// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once
#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "DarkSoul/Enumerations/CEWeaponType.h"
#include "CHUDInGame.generated.h"

class UUserWidget;
class UCUW_StatBars;
class UCUW_ItemSlots;
class UCUW_Interaction;
class UCUW_BossStatBar;

class ACItemWeapon;
class ACPlayerCharacter;
class UCEquipmentComponent;
class UCGameLoadingComponent;

/**
 * 
 */
UCLASS()
class DARKSOUL_API ACHUDInGame : public AHUD
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "Health", meta = (AllowPrivateAccess = "true"))
		TSubclassOf<UUserWidget> HUDWidgetClass = nullptr;

	UPROPERTY(EditAnywhere, Category = "Health", meta = (AllowPrivateAccess = "true"))
		UUserWidget* CurrentWidget = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "Component", meta = (AllowPrivateAccess = "true"))
		UCGameLoadingComponent* LoadingManager;
	
public:
	ACHUDInGame();
	virtual void BeginPlay() override;
	virtual void DrawHUD() override;

private:
	void HideAllWidget();
	void OnLoading();
	void ToggleHUD();
	void OnWeaponChanged(ACItemWeapon* NewWeapon);

private:
	UCUW_StatBars* StatBars = nullptr;
	UCUW_BossStatBar* BossStatBar = nullptr;
	UCUW_ItemSlots* ItemSlots = nullptr;
	UCUW_Interaction* InteractionMassage = nullptr;
	TWeakObjectPtr<ACPlayerCharacter> Player = nullptr;
	TWeakObjectPtr<UCEquipmentComponent> PlayerEquipment = nullptr;
};