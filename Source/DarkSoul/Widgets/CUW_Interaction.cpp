// Copyright Epic Games, Inc. All Rights Reserved.
#include "CUW_Interaction.h"
#include "DarkSoul/_Utility/CLog.h"

/// Unreal Engine
#include "Components/Image.h"
#include "Components/TextBlock.h"

void UCUW_Interaction::NativeConstruct()
{
	Super::NativeConstruct();

	KeyIcon = Cast<UImage>(GetWidgetFromName(FName("KeyIcon")));
	if (KeyIcon == nullptr)
	{
		CLOG_FUNC_TEXT(L"KeyIcon is nullptr");
	}

	Massage = Cast<UTextBlock>(GetWidgetFromName(FName("Massage")));
	if (Massage == nullptr)
	{
		CLOG_FUNC_TEXT(L"Massage is nullptr");
	}

	// Default Hide Massage Box
	SetVisibility(ESlateVisibility::Hidden);
}

void UCUW_Interaction::Interaction()
{
}

void UCUW_Interaction::OnInteraction(FInteractionMassage InteractionMassage)
{
	// Init Massage
	if (Massage != nullptr)
	{
		Massage->SetText(InteractionMassage.Massage);
	}
	else
	{
		CLOG_FUNC_TEXT(L"Massage is nullptr");
	}

	// Visible Massage Box
	SetVisibility(ESlateVisibility::Visible);
}

void UCUW_Interaction::OffInteraction()
{
	// Hide Massage Box
	SetVisibility(ESlateVisibility::Hidden);
}