// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once
#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "DarkSoul/Enumerations/CEWeaponType.h"
#include "CHUDInGame.generated.h"

class UUserWidget;
class ACItemWeapon;
class UCUW_StatBars;
class UCUW_ItemSlots;
class ACPlayerCharacter;
class UCEquipmentComponent;

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
	
public:
	ACHUDInGame();
	virtual void BeginPlay() override;

private:
	void ToggleHUD();
	void OnWeaponChanged(ACItemWeapon* NewWeapon);

private:
	UCUW_StatBars* StatBars = nullptr;
	UCUW_ItemSlots* ItemSlots = nullptr;
	TWeakObjectPtr<ACPlayerCharacter> Player = nullptr;
	TWeakObjectPtr<UCEquipmentComponent> PlayerEquipmentManager = nullptr;
};