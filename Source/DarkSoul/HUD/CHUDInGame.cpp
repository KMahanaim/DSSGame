// Copyright Epic Games, Inc. All Rights Reserved.
#include "CHUDInGame.h"
#include "DarkSoul/_Utility/CLog.h"
#include "DarkSoul/Widgets/CUW_StatBars.h"
#include "DarkSoul/Widgets/CUW_ItemSlots.h"
#include "DarkSoul/Widgets/CUW_Interaction.h"
#include "DarkSoul/Widgets/CUW_BossStatBar.h"
#include "DarkSoul/Characters/CPlayerCharacter.h"
#include "DarkSoul/Components/CEquipmentComponent.h"
#include "DarkSoul/Components/CGameLoadingComponent.h"

/// Unreal Engine
#include "Engine/World.h"
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

	// Create Component
	{
		LoadingManager = CreateDefaultSubobject<UCGameLoadingComponent>(FName("LoadingManager"));
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
	PlayerEquipment = Player->GetEquipmentComponent();
	CLOG_ERROR_CHECK_RETURN(PlayerEquipment);

	// Delegate Bind
	{
		Player->OnLoading.AddUObject(this, &ACHUDInGame::OnLoading);
		Player->ToggleHUD.AddUObject(this, &ACHUDInGame::ToggleHUD);
		PlayerEquipment->OnWeaponChanged.AddUObject(this, &ACHUDInGame::OnWeaponChanged);
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

		InteractionMassage = Cast<UCUW_Interaction>(CurrentWidget->GetWidgetFromName(FName("InteractionMassage")));
		if (InteractionMassage == nullptr)
		{
			CLOG_FUNC_TEXT(L"Interaction Massage Widget is nullptr");
		}
		else
		{
			Player->PlayInteraction.BindUObject(InteractionMassage, &UCUW_Interaction::Interaction);
			Player->OnInteractionMassage.BindUObject(InteractionMassage, &UCUW_Interaction::OnInteraction);
			Player->OffInteractionMassage.BindUObject(InteractionMassage, &UCUW_Interaction::OffInteraction);
		}

		BossStatBar = Cast<UCUW_BossStatBar>(CurrentWidget->GetWidgetFromName(FName("BossStatBar")));
		if (BossStatBar != nullptr)
		{
			BossStatBar->SetVisibility(ESlateVisibility::Hidden);
		}
		else
		{
			CLOG_FUNC_TEXT(L"BossStatBar is nullptr");
		}
	}
}

void ACHUDInGame::DrawHUD()
{
	Super::DrawHUD();

	CLOG_ERROR_CHECK_RETURN(Player);
	if (Player->IsInLoading())
	{
		CLOG_CHECK_RETURN(PlayerOwner);
		CLOG_CHECK_RETURN(LoadingManager);

		// Get Player Viewport Size
		int32 ViewportSizeX, ViewportSizeY;
		PlayerOwner->GetViewportSize(ViewportSizeX, ViewportSizeY);
		DrawMaterialSimple(LoadingManager->GetLoadingBackground(), 0, 0, ViewportSizeX, ViewportSizeY);

		// Get Texture UV
		FVector2D UVTopLeft, UVBottomRight;
		LoadingManager->GetTextureUV(UVTopLeft, UVBottomRight);

		DrawTexture
		(
			LoadingManager->GetResourceTexture2D(),
			ViewportSizeX - LoadingManager->GetLoadingIconLocation().X,			/** Screen Location X */
			ViewportSizeY - LoadingManager->GetLoadingIconLocation().Y,			/** Screen Location Y */
			LoadingManager->GetDrawSize().X, LoadingManager->GetDrawSize().Y,	/** Draw Size */
			UVTopLeft.X, UVTopLeft.Y,					/** Texture UV Left(X), Top(Y)*/
			UVBottomRight.X, UVBottomRight.Y			/** Texture UV Right(X), Bottom(Y) */
		);
	}
}

void ACHUDInGame::HideAllWidget()
{
	// All Widget Hide
	StatBars->SetVisibility(ESlateVisibility::Hidden);
	BossStatBar->SetVisibility(ESlateVisibility::Hidden);
	ItemSlots->SetVisibility(ESlateVisibility::Hidden);
	InteractionMassage->SetVisibility(ESlateVisibility::Hidden);
}


void ACHUDInGame::OnLoading()
{
	HideAllWidget();

	// Loading Start
	LoadingManager->LoadingStart();
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
	if (PlayerEquipment.IsValid())
	{
		CLOG_ERROR_CHECK_RETURN(ItemSlots);
		ItemSlots->ItemWeaponSlotUpdate(PlayerEquipment);
	}
}