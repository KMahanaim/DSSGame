// Copyright Epic Games, Inc. All Rights Reserved.
#include "CUW_LoadingScreen.h"
#include "DarkSoul/_Utility/CLog.h"

/// Unreal Engine
#include "PaperFlipbook.h"
#include "Components/Image.h"

void UCUW_LoadingScreen::NativeConstruct()
{
	Super::NativeConstruct();

	LoadingIcon = Cast<UImage>(GetWidgetFromName(FName("LoadingIcon")));
	if (LoadingIcon == nullptr)
	{
		CLOG_FUNC_TEXT(L"LoadingIcon is nullptr");
	}

	Background = Cast<UImage>(GetWidgetFromName(FName("Background")));
	if (Background == nullptr)
	{
		CLOG_FUNC_TEXT(L"Background is nullptr");
	}

	if (PaperFlipbook == nullptr)
	{
		CLOG_FUNC_TEXT(L"PaperFlipbook is nullptr, Check the Widget Blueprint Asset");
	}

	// Init
	FrameCounter = 0;
	Delay = 12;
}

void UCUW_LoadingScreen::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UCUW_LoadingScreen::SetBackgroundSize(FVector2D Size)
{
	CLog::Print(Size);
}