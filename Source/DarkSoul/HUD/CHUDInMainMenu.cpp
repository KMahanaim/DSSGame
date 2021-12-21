// Copyright Epic Games, Inc. All Rights Reserved.
#include "CHUDInMainMenu.h"
#include "DarkSoul/_Utility/CLog.h"
#include "DarkSoul/Characters/CUIKeyInputPawn.h"
#include "DarkSoul/Components/CGameLoadingComponent.h"

/// Unreal Engine
#include "TimerManager.h"
#include "Engine/World.h"
#include "Blueprint/UserWidget.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/TextBlock.h"

ACHUDInMainMenu::ACHUDInMainMenu()
{
	FString Path = L"";

	// Create Widget
	{
		Path = L"WidgetBlueprint'/Game/DarkSoul/Widget/HUD/InMainMenu.InMainMenu_C'";
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

void ACHUDInMainMenu::BeginPlay()
{
	Super::BeginPlay();

	SelectMenu = 0;
	DrawBackground = MenuBackground;

	CLOG_ERROR_CHECK_RETURN(HUDWidgetClass);

	CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), HUDWidgetClass);
	CLOG_ERROR_CHECK_RETURN(CurrentWidget);

	// Widget Output
	CurrentWidget->AddToPlayerScreen();

	ACUIKeyInputPawn* Player = Cast<ACUIKeyInputPawn>(PlayerOwner->GetPawn());
	CLOG_ERROR_CHECK_RETURN(Player);

	// Get Text
	{
		// New Game
		UTextBlock* Text = nullptr;
		Text = Cast<UTextBlock>(CurrentWidget->GetWidgetFromName(FName("NewGame")));
		if (Text != nullptr)
		{
			MenuList.Emplace(Text);
		}
		else
		{
			CLOG_ERROR_FUNC_TEXT(L"NewGame is nullptr");
		}

		// Continue
		Text = Cast<UTextBlock>(CurrentWidget->GetWidgetFromName(FName("Continue")));
		if (Text != nullptr)
		{
			MenuList.Emplace(Text);
		}
		else
		{
			CLOG_ERROR_FUNC_TEXT(L"Continue is nullptr");
		}

		// Exit
		Text = Cast<UTextBlock>(CurrentWidget->GetWidgetFromName(FName("Exit")));
		if (Text != nullptr)
		{
			MenuList.Emplace(Text);
		}
		else
		{
			CLOG_ERROR_FUNC_TEXT(L"Exit is nullptr");
		}
	}

	// Delegate Bind
	{
		Player->OnArrowUp.BindUObject(this, &ACHUDInMainMenu::ArrowUp);
		Player->OnArrowDown.BindUObject(this, &ACHUDInMainMenu::ArrowDown);

		Player->OnEnter.BindUObject(this, &ACHUDInMainMenu::EnterMenu);
	}
}

void ACHUDInMainMenu::DrawHUD()
{
	Super::DrawHUD();

	if (CurrentMenu != SelectMenu)
	{
		// CurrentMenu : Old, SelectMenu : New
		if (MenuList.IsValidIndex(CurrentMenu))
		{
			MenuList[CurrentMenu]->SetColorAndOpacity(FSlateColor::FSlateColor(FLinearColor::Black));
		}
		else
		{
			CLOG_ERROR_FUNC_TEXT(FString::FromInt(CurrentMenu));
		}
		
		if (MenuList.IsValidIndex(SelectMenu))
		{
			MenuList[SelectMenu]->SetColorAndOpacity(SelectColor);
		}
		else
		{
			CLOG_ERROR_FUNC_TEXT(FString::FromInt(SelectMenu));
		}

		CurrentMenu = SelectMenu;
	}

	// Draw Background
	if (DrawBackground != nullptr)
	{
		if (PlayerOwner != nullptr)
		{
			int32 ViewportSizeX, ViewportSizeY;
			PlayerOwner->GetViewportSize(ViewportSizeX, ViewportSizeY);
			DrawMaterialSimple(DrawBackground, 0, 0, ViewportSizeX, ViewportSizeY);
		}
		else
		{
			CLOG_ERROR_FUNC_TEXT(L"PlayerOwner is nullptr");
		}
	}
	else
	{
		CLOG_ERROR_FUNC_TEXT(L"Background is nullptr");
	}

	// Draw Loading Icon
	if (DrawBackground == LoadingManager->GetLoadingBackground())
	{
		// Get Player Viewport Size
		int32 ViewportSizeX, ViewportSizeY;
		PlayerOwner->GetViewportSize(ViewportSizeX, ViewportSizeY);

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

void ACHUDInMainMenu::ArrowUp()
{
	SelectMenu--;
	if (0 > SelectMenu)
	{
		SelectMenu = MenuList.Num() - 1;
	}
}

void ACHUDInMainMenu::ArrowDown()
{
	SelectMenu++;
	if (MenuList.Num() <= SelectMenu)
	{
		SelectMenu = 0;
	}
}

void ACHUDInMainMenu::EnterMenu()
{
	if (MenuList.IsValidIndex(CurrentMenu))
	{
		if (CurrentMenu == 0)
		{
			// Widget InVisible Setting
			for (int8 Index = 0; Index < MenuList.Num(); Index++)
			{
				MenuList[Index]->SetVisibility(ESlateVisibility::Hidden);
			}

			DrawBackground = LoadingManager->GetLoadingBackground();
			LoadingManager->LoadingStart();

			GetWorld()->GetTimerManager().SetTimer(LevelOpenDelayTimerHandle, this, &ACHUDInMainMenu::OpenLevel, 3, false);
		}
		else if (CurrentMenu == 2)
		{
			UKismetSystemLibrary::QuitGame(this, PlayerOwner, EQuitPreference::Quit, true);
		}
	}
}

void ACHUDInMainMenu::OpenLevel()
{
	UGameplayStatics::OpenLevel(this, FName("Arena"));
}