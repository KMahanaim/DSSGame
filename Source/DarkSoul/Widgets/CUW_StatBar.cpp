// Copyright Epic Games, Inc. All Rights Reserved.
#include "CUW_StatBar.h"
#include "DarkSoul/_Utility/CLog.h"
#include "DarkSoul/Components/CExtendedStatComponent.h"

/// Unreal Engine
#include "Components/ProgressBar.h"

void UCUW_StatBar::NativeConstruct()
{
	Super::NativeConstruct();

	StatBar = Cast<UProgressBar>(GetWidgetFromName(FName("StatBar")));
}

void UCUW_StatBar::Init(UCExtendedStatComponent* ExtendedStatComponent)
{
	if (ExtendedStatComponent != nullptr)
	{
		OnValueChanged(ExtendedStatComponent->GetCurrentValue(), ExtendedStatComponent->GetMaxValue());
		ExtendedStatComponent->OnValueChanged.AddUObject(this, &UCUW_StatBar::OnValueChanged);
	}
}

void UCUW_StatBar::SetStatBarFillImage(UObject* Texture)
{
	if (Texture != nullptr)
	{
		FSlateBrush newSlateBrush = StatBar->WidgetStyle.FillImage;
		newSlateBrush.SetResourceObject(Texture);
		StatBar->WidgetStyle.SetFillImage(newSlateBrush);
	}
	else
	{
		CLOG_FUNC_TEXT(L"Texture is nullptr");
	}
}

void UCUW_StatBar::SetStatBarFillImage(UTexture2D* Texture)
{
	if (Texture != nullptr)
	{
		FSlateBrush newSlateBrush = StatBar->WidgetStyle.FillImage;
		newSlateBrush.SetResourceObject(Cast<UObject>(Texture));
		StatBar->WidgetStyle.SetFillImage(newSlateBrush);
	}
	else
	{
		CLOG_FUNC_TEXT(L"Texture is nullptr");
	}
}

void UCUW_StatBar::OnValueChanged(float NewValue, float MaxValue)
{
	if (StatBar != nullptr)
	{
		//CLog::Print(NewValue, 1.0f, FColor::Red);
		//CLog::Print(MaxValue, 1.0f, FColor::Green);
		StatBar->SetPercent(FMath::Clamp(NewValue / MaxValue, 0.0f, 1.0f));
	}

	// 능력치 최대값에 따른 스탯바 길이 조절
	// 미구현
}