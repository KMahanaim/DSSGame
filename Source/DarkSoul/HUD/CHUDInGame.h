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
class UCUW_Interaction;
class UCUW_BossStatBar;
class UPaperFlipbook;
class UMaterialInterface;
class ACPlayerCharacter;
class UCEquipmentComponent;
class UCSpriteComponent;

/**
 * 
 */
UCLASS()
class DARKSOUL_API ACHUDInGame : public AHUD
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly, Category = "Loading", meta = (AllowPrivateAccess = "true"))
		UMaterialInterface* LoadingBackground;

	UPROPERTY(EditDefaultsOnly, Category = "Loading", meta = (AllowPrivateAccess = "true"))
		UPaperFlipbook* LoadingIcon;

	UPROPERTY(EditAnywhere, Category = "Health", meta = (AllowPrivateAccess = "true"))
		TSubclassOf<UUserWidget> HUDWidgetClass = nullptr;

	UPROPERTY(EditAnywhere, Category = "Health", meta = (AllowPrivateAccess = "true"))
		UUserWidget* CurrentWidget = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "Components", meta = (AllowPrivateAccess = "true"))
		UCSpriteComponent* SpriteManager;
	
public:
	ACHUDInGame();
	virtual void BeginPlay() override;
	virtual void DrawHUD() override;

private:
	void ToggleHUD();
	void OnWeaponChanged(ACItemWeapon* NewWeapon);

private:
	UCUW_StatBars* StatBars = nullptr;
	UCUW_ItemSlots* ItemSlots = nullptr;
	UCUW_Interaction* InteractionMassage = nullptr;
	TWeakObjectPtr<ACPlayerCharacter> Player = nullptr;
	TWeakObjectPtr<UCEquipmentComponent> PlayerEquipment = nullptr;
};