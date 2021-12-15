// Copyright Epic Games, Inc. All Rights Reserved.
#include "CHUDInGame.h"
#include "DarkSoul/_Utility/CLog.h"
#include "DarkSoul/Widgets/CUW_StatBars.h"
#include "DarkSoul/Widgets/CUW_ItemSlots.h"
#include "DarkSoul/Widgets/CUW_Interaction.h"
#include "DarkSoul/Characters/CPlayerCharacter.h"
#include "DarkSoul/Components/CEquipmentComponent.h"
#include "DarkSoul/Components/CSpriteComponent.h"

/// Unreal Engine
#include "PaperSprite.h"
#include "PaperFlipbook.h"
#include "Engine/World.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"
#include "Materials/MaterialInterface.h"

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

	// Create Components
	{
		SpriteManager = CreateDefaultSubobject<UCSpriteComponent>(FName("SpriteManager"));
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
	}

	// Setting Sprite Manager
	{
		if (LoadingIcon == nullptr)
		{
			CLOG_FUNC_TEXT(L"LoadingIcon is nullptr");
		}
		else
		{
			SpriteManager->SetPaperFlipbook(LoadingIcon);
		}
	}
}

void ACHUDInGame::DrawHUD()
{
	Super::DrawHUD();

	// Draw In Loading Screen
	CLOG_ERROR_CHECK_RETURN(Player);
	if (Player->IsInLoading())
	{
		CLOG_CHECK_RETURN(LoadingBackground);
		CLOG_CHECK_RETURN(PlayerOwner);

		int32 X, Y;
		PlayerOwner->GetViewportSize(X, Y);
		DrawMaterialSimple(LoadingBackground, 0, 0, X, Y);

		// Loading Icon Sprite Play
		UPaperSprite* Sprite = SpriteManager->GetCurrentSprite();
		FVector2D UVTopLeft = Sprite->GetSourceUV();
		UVTopLeft.X = (1.0f / 8.0f) * (UVTopLeft.X / 128.0f);
		UVTopLeft.Y = (1.0f / 4.0f) * (UVTopLeft.Y / 128.0f);

		FVector2D UVBottomRight;
		UVBottomRight.X = 1.0f / 8.0f;
		UVBottomRight.Y = 1.0f / 4.0f;

		DrawTexture
		(
			Sprite->GetSourceTexture(), 
			X - 200, Y - 200,					/** Screen Location X, Y */
			128, 128,							/** Draw Size */
			UVTopLeft.X, UVTopLeft.Y,			/** Texture UV Left(X), Top(Y)*/
			UVBottomRight.X, UVBottomRight.Y	/** Texture UV Right(X), Bottom(Y) */
		);
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
	if (PlayerEquipment.IsValid())
	{
		CLOG_ERROR_CHECK_RETURN(ItemSlots);
		ItemSlots->ItemWeaponSlotUpdate(PlayerEquipment);
	}
}