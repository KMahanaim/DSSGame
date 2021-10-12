// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DarkSoul/Structs/CFItem.h"
#include "DarkSoul/Enumerations/CEItemSlotType.h"
#include "CUW_ItemSlots.generated.h"

class UCUW_ItemSlot;
class UCEquipmentComponent;

/**
 * 
 */
UCLASS()
class DARKSOUL_API UCUW_ItemSlots : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

public:
	void ItemActiveSlotUpdate(TWeakObjectPtr<UCEquipmentComponent> PlayerEquipment);
	void ItemWeaponSlotUpdate(TWeakObjectPtr<UCEquipmentComponent> PlayerEquipment);

private:
	void ItemImageUpdate(const EItemSlotType TargetSlot, const FItem* Item);

private:
	TMap<EItemSlotType, UCUW_ItemSlot*> ItemSlots;
};
