// Copyright Epic Games, Inc. All Rights Reserved.
#include "CUW_BossStatBar.h"
#include "DarkSoul/_Utility/CLog.h"

/// Unreal Engine
#include "Components/ProgressBar.h"

void UCUW_BossStatBar::NativeConstruct()
{
	Super::NativeConstruct();

	StatBar = Cast<UProgressBar>(GetWidgetFromName(FName("HPBar")));
}

void UCUW_BossStatBar::Init(UCExtendedStatComponent* ExtendedStatComponent)
{

}

void UCUW_BossStatBar::OnValueChanged(float NewValue, float MaxValue)
{

}