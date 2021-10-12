// Copyright Epic Games, Inc. All Rights Reserved.
#include "CUW_ItemSlot.h"
#include "DarkSoul/_Utility/CLog.h"

/// Unreal Engine
#include "Components/Image.h"

void UCUW_ItemSlot::NativeConstruct()
{
	Super::NativeConstruct();

	ItemImage = Cast<UImage>(GetWidgetFromName(FName("Item")));
	if (ItemImage == nullptr)
	{
		CLOG_FUNC_TEXT(L"ItemImage is nullptr");
	}
}

void UCUW_ItemSlot::SetItemImage(UTexture2D* NewImage)
{
	if (ItemImage != nullptr)
	{
		ItemImage->SetBrushResourceObject(NewImage);
	}
	else
	{
		CLOG_FUNC_TEXT(L"ItemImage is nullptr");
	}
}