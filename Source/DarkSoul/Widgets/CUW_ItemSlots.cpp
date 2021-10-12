// Copyright Epic Games, Inc. All Rights Reserved.
#include "CUW_ItemSlots.h"
#include "DarkSoul/_Utility/CLog.h"
#include "DarkSoul/Widgets/CUW_ItemSlot.h"
#include "DarkSoul/Components/CEquipmentComponent.h"

void UCUW_ItemSlots::NativeConstruct()
{
	Super::NativeConstruct();

	// Prev Weapon Slot
	UCUW_ItemSlot* ItemSlot = Cast<UCUW_ItemSlot>(GetWidgetFromName(FName("PrevWeapon")));
	if (ItemSlot == nullptr)
	{
		CLOG_FUNC_TEXT(L"Prev Weapon Slot is nullptr");
	}
	else
	{
		ItemSlots.Emplace(EItemSlotType::PREV_WEAPON_SLOT, ItemSlot);
	}

	// Current Weapon Slot
	ItemSlot = Cast<UCUW_ItemSlot>(GetWidgetFromName(FName("CurrentWeapon")));
	if (ItemSlot == nullptr)
	{
		CLOG_FUNC_TEXT(L"Current Weapon Slot is nullptr");
	}
	else
	{
		ItemSlots.Emplace(EItemSlotType::CURRENT_WEAPON_SLOT, ItemSlot);
	}

	// Next Weapon Slot
	ItemSlot = Cast<UCUW_ItemSlot>(GetWidgetFromName(FName("NextWeapon")));
	if (ItemSlot == nullptr)
	{
		CLOG_FUNC_TEXT(L"Next Weapon Slot is nullptr");
	}
	else
	{
		ItemSlots.Emplace(EItemSlotType::NEXT_WEAPON_SLOT, ItemSlot);
	}

	// Active Slot (Potion)
	ItemSlot = Cast<UCUW_ItemSlot>(GetWidgetFromName(FName("ActiveItem")));
	if (ItemSlot == nullptr)
	{
		CLOG_FUNC_TEXT(L"Active Item Slot is nullptr");
	}
	else
	{
		ItemSlots.Emplace(EItemSlotType::ACTIVE_ITEM_SLOT, ItemSlot);
	}
}

void UCUW_ItemSlots::ItemActiveSlotUpdate(TWeakObjectPtr<UCEquipmentComponent> PlayerEquipment)
{
	const FItem* Item = PlayerEquipment->GetItemInSlot(EItemSlotType::ACTIVE_ITEM_SLOT);
	ItemImageUpdate(EItemSlotType::ACTIVE_ITEM_SLOT, Item);
}

void UCUW_ItemSlots::ItemWeaponSlotUpdate(TWeakObjectPtr<UCEquipmentComponent> PlayerEquipment)
{
	const FItem* Item = nullptr;
	Item = PlayerEquipment->GetItemInSlot(EItemSlotType::PREV_WEAPON_SLOT);
	ItemImageUpdate(EItemSlotType::PREV_WEAPON_SLOT, Item);

	Item = PlayerEquipment->GetItemInSlot(EItemSlotType::CURRENT_WEAPON_SLOT);
	ItemImageUpdate(EItemSlotType::CURRENT_WEAPON_SLOT, Item);

	Item = PlayerEquipment->GetItemInSlot(EItemSlotType::NEXT_WEAPON_SLOT);
	ItemImageUpdate(EItemSlotType::NEXT_WEAPON_SLOT, Item);
}

void UCUW_ItemSlots::ItemImageUpdate(const EItemSlotType TargetSlot, const FItem* Item)
{
	CLOG_ERROR_CHECK_RETURN_TEXT(Item, L"Item is nullptr, EItemSlotType(" + FString::FormatAsNumber(static_cast<int8>(TargetSlot)) + L")");

	UCUW_ItemSlot* ItemSlotWidget = *(ItemSlots.Find(TargetSlot));
	if (ItemSlotWidget != nullptr)
	{
		ItemSlotWidget->SetItemImage(Item->Image);
		return;
	}
	else
	{
		CLOG_FUNC_TEXT(L"Not Contain Widget");
		return;
	}
}