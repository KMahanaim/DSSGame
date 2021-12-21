// Copyright Epic Games, Inc. All Rights Reserved.
#include "CGameLoadingComponent.h"
#include "DarkSoul/_Utility/CLog.h"

/// Unreal Engine
#include "PaperSprite.h"
#include "PaperFlipbook.h"
#include "TimerManager.h"
#include "Engine/World.h"

UCGameLoadingComponent::UCGameLoadingComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UCGameLoadingComponent::BeginPlay()
{
	Super::BeginPlay();

	Interval = 0.0f;
	CurrentKeyFrame = 0;

	CLOG_ERROR_CHECK_RETURN_TEXT(PaperFlipbook, L"PaperFlipbook is nullptr, check the bluprint");
	float FramesPerSecond = PaperFlipbook->GetFramesPerSecond();
	Interval = 1.0f / FramesPerSecond;
}

void UCGameLoadingComponent::LoadingStart()
{
	GetWorld()->GetTimerManager().SetTimer(PaperTurnTimerHandle, this, &UCGameLoadingComponent::PaperTurn, Interval, true);
}

void UCGameLoadingComponent::PaperTurn()
{
	CurrentKeyFrame++;
	if (PaperFlipbook->GetNumFrames() <= CurrentKeyFrame)
	{
		CurrentKeyFrame = 0;
	}
}

void UCGameLoadingComponent::GetTextureUV(FVector2D& OutTopLeft, FVector2D& OutBottomRight)
{
	OutTopLeft = PaperFlipbook->GetSpriteAtFrame(CurrentKeyFrame)->GetSourceUV();
	OutTopLeft.X = (1.0f / NumberOfWidth) * (OutTopLeft.X / DrawSize.X);
	OutTopLeft.Y = (1.0f / NumberOfHeight) * (OutTopLeft.Y / DrawSize.Y);

	OutBottomRight.X = 1.0f / NumberOfWidth;
	OutBottomRight.Y = 1.0f / NumberOfHeight;
}

UPaperSprite* UCGameLoadingComponent::GetSprite() const
{
	if (PaperFlipbook == nullptr)
	{
		CLOG_ERROR_FUNC_TEXT(L"PaperFlipbook is nullptr");
		return nullptr;
	}

	return PaperFlipbook->GetSpriteAtFrame(CurrentKeyFrame);
}

UTexture2D* UCGameLoadingComponent::GetResourceTexture2D() const
{
	if (PaperFlipbook == nullptr)
	{
		CLOG_ERROR_FUNC_TEXT(L"PaperFlipbook is nullptr");
		return nullptr;
	}

	return PaperFlipbook->GetSpriteAtFrame(CurrentKeyFrame)->GetSourceTexture();
}