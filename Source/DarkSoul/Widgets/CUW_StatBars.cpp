// Copyright Epic Games, Inc. All Rights Reserved.
#include "CUW_StatBars.h"
#include "DarkSoul/_Utility/CLog.h"
#include "DarkSoul/Widgets/CUW_StatBar.h"

/// Unreal Engine
#include "Engine/Texture2D.h"

void UCUW_StatBars::NativeConstruct()
{
	Super::NativeConstruct();

	StatBars.Emplace(EStatsType::HEALTH, Cast<UCUW_StatBar>(GetWidgetFromName(FName("HPBar"))));
	StatBars.Emplace(EStatsType::MANA, Cast<UCUW_StatBar>(GetWidgetFromName(FName("MPBar"))));
	StatBars.Emplace(EStatsType::STAMINA, Cast<UCUW_StatBar>(GetWidgetFromName(FName("SPBar"))));
	
	UCUW_StatBar* StatBar = *StatBars.Find(EStatsType::MANA);
	if (StatBar != nullptr)
	{
		StatBar->SetStatBarFillImage(MPTexture);
	}

	StatBar = *StatBars.Find(EStatsType::STAMINA);
	if (StatBar != nullptr)
	{
		StatBar->SetStatBarFillImage(SPTexture);
	}
}

void UCUW_StatBars::SetStatBar(const EStatsType TargetType, UCExtendedStatComponent* ExtendedStat)
{
	UCUW_StatBar* StatBar = *StatBars.Find(TargetType);
	if (StatBar != nullptr)
	{
		StatBar->Init(ExtendedStat);
	}
	else
	{
		CLOG_FUNC_TEXT(L"StatBar is nullptr");
	}
}