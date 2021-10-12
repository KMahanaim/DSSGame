// Copyright Epic Games, Inc. All Rights Reserved.
#include "CHUDInGame.h"
#include "DarkSoul/_Utility/CLog.h"
#include "DarkSoul/Widgets/CUW_StatBars.h"
#include "DarkSoul/Widgets/CUW_ItemSlots.h"
#include "DarkSoul/Characters/CPlayerCharacter.h"
#include "DarkSoul/Components/CEquipmentComponent.h"

/// Unreal Engine
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"

ACHUDInGame::ACHUDInGame()
{
	FString Path = L"";

	// Create Widget
	{
		Path = L"WidgetBlueprint'/Game/DarkSoul/Widget/HUD/InGame.InGame_C'";
		static ConstructorHelpers::FClassFinder<UUserWidget> HUDWidget(*Path);
		if (HUDWidget.Succeeded())
		{
			HUDWidgetClass = HUDWidget.Class;
		}
	}
}

void ACHUDInGame::BeginPlay()
{
	Super::BeginPlay();

	CLOG_ERROR_CHECK_RETURN(HUDWidgetClass);

	CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), HUDWidgetClass);
	CLOG_ERROR_CHECK_RETURN(CurrentWidget);

	// Widget Output
	CurrentWidget->AddToPlayerScreen();

	Player = Cast<ACPlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	CLOG_ERROR_CHECK_RETURN(Player);
	PlayerEquipmentManager = Player->GetEquipmentComponent();
	CLOG_ERROR_CHECK_RETURN(PlayerEquipmentManager);

	// Delegate Bind
	{
		Player->ToggleHUD.AddUObject(this, &ACHUDInGame::ToggleHUD);
		PlayerEquipmentManager->OnWeaponChanged.AddUObject(this, &ACHUDInGame::OnWeaponChanged);
	}

	// Get Widget
	{
		ItemSlots = Cast<UCUW_ItemSlots>(CurrentWidget->GetWidgetFromName(FName("ItemSlots")));
		if (ItemSlots == nullptr)
		{
			CLOG_FUNC_TEXT(L"ItemSlots is nullptr");
		}

		StatBars = Cast<UCUW_StatBars>(CurrentWidget->GetWidgetFromName(FName("StatBars")));
		if (StatBars == nullptr)
		{
			CLOG_FUNC_TEXT(L"StatBars is nullptr");
		}
		else
		{
			StatBars->SetStatBar(EStatsType::HEALTH, Player->GetExtendedStatComponent(EStatsType::HEALTH));
			StatBars->SetStatBar(EStatsType::MANA, Player->GetExtendedStatComponent(EStatsType::MANA));
			StatBars->SetStatBar(EStatsType::STAMINA, Player->GetExtendedStatComponent(EStatsType::STAMINA));
		}
	}
}

void ACHUDInGame::ToggleHUD()
{
	if (CurrentWidget->IsInViewport())
	{
		CurrentWidget->RemoveFromViewport();
	}
	else
	{
		CurrentWidget->AddToPlayerScreen();
	}
}

void ACHUDInGame::OnWeaponChanged(ACItemWeapon* NewWeapon)
{
	if (PlayerEquipmentManager.IsValid())
	{
		CLOG_ERROR_CHECK_RETURN(ItemSlots);
		ItemSlots->ItemWeaponSlotUpdate(PlayerEquipmentManager);
	}
}