// Copyright Epic Games, Inc. All Rights Reserved.
#include "CEquipmentComponent.h"
#include "DarkSoul/_Utility/CLog.h"
#include "DarkSoul/Structs/DataTableRowBase/CFWeaponSpawn.h"
#include "DarkSoul/Items/Weapons/CItemWeapon.h"

/// Unreal Engine
#include "Engine/World.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/SkeletalMeshComponent.h"

UCEquipmentComponent::UCEquipmentComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UCEquipmentComponent::SpawnWeapons(USkeletalMeshComponent* Mesh, const UDataTable* WeaponPathTable)
{
	CLOG_ERROR_CHECK_RETURN(GetOwner());

	FString Path = L"";
	FString OwnerName = L"Owner(" + GetOwner()->GetName() + L"), ";
	FString LogDescription = L"";
	UWorld* World = GetWorld();

	CLOG_ERROR_CHECK_RETURN(Mesh);
	CLOG_ERROR_CHECK_RETURN_TEXT(WeaponPathTable, OwnerName + L"Weapon Path Table is nullptr, Check the Blueprint Detail");
	CLOG_ERROR_CHECK_RETURN(World);

	TArray<FWeaponSpawn*> WeaponPathList;
	WeaponPathTable->GetAllRows(L"", WeaponPathList);
	if (WeaponPathList.Num() <= 0)
	{
		// Data Table Empty
		return;
	}

	for (int32 WeaponPathIndex = 0; WeaponPathIndex < WeaponPathList.Num(); WeaponPathIndex++)
	{
		// Get Weapon Ref Path
		Path = WeaponPathList[WeaponPathIndex]->WeaponPath.ToString();

		// Get Blueprint UClass
		UClass* GeneratedBp = StaticLoadClass(ACItemWeapon::StaticClass(), nullptr, *Path);
		if (GeneratedBp == nullptr)
		{
			CLOG_ERROR_FUNC_TEXT(OwnerName + L"StaticLoadClass Failed.");
			continue;
		}

		// Spawn Weapon
		ACItemWeapon* Weapon = World->SpawnActor<ACItemWeapon>(GeneratedBp);
		if (Weapon == nullptr)
		{
			CLOG_ERROR_FUNC_TEXT(OwnerName + L"SpawnActor Failed.");
			continue;
		}

		// Add Inventory
		WeaponInventory.Emplace(Weapon);

		// Attach To Owner Mesh
		Weapon->AttachToComponent(Mesh, FAttachmentTransformRules::KeepRelativeTransform, Weapon->GetWeaponSocket()->RightHand);

		// Is Twin Weapon?
		if (Weapon->GetIsTwinWeapon())
		{
			ACItemWeapon* TwinWeapon = World->SpawnActor<ACItemWeapon>(GeneratedBp);
			TwinWeapon->AttachToComponent(Mesh, FAttachmentTransformRules::KeepRelativeTransform, TwinWeapon->GetWeaponSocket()->LeftHand);
			Weapon->ToLinkWeapon(TwinWeapon);
		}

		Weapon->InVisibleWeapon();
	}

	if (WeaponInventory.Num() > 0)
	{
		SetWeapon(WeaponInventory[0]);
	}
}

void UCEquipmentComponent::SwitchWeapon(EWeaponSwitchType WeaponSwitchType)
{
	int8 SwitchDirection = 0;
	switch (WeaponSwitchType)
	{
		case EWeaponSwitchType::LEFT:
		{
			SwitchDirection = -1;
		}
		break;
		case EWeaponSwitchType::RIGHT:
		{
			SwitchDirection = +1;
		}
		break;
	}

	int32 SelectedWeaponIndex = INDEX_NONE;
	if (WeaponInventory.Find(SelectedWeapon, SelectedWeaponIndex))
	{
		SelectedWeaponIndex += SwitchDirection;
		if (SelectedWeaponIndex < 0)
		{
			SelectedWeaponIndex = WeaponInventory.Num() - 1;
		}
		else if (SelectedWeaponIndex >= WeaponInventory.Num())
		{
			SelectedWeaponIndex = 0;
		}

		// Switching Weapon
		SetWeapon(WeaponInventory[SelectedWeaponIndex]);
	}
}

const ECombatType UCEquipmentComponent::GetCombatType() const
{
	ECombatType Result = ECombatType::NONE;
	if (SelectedWeapon != nullptr)
	{
		Result = SelectedWeapon->GetCombatType();
	}
	else
	{
		CLOG_ERROR_FUNC_TEXT(L"Selected Weapon is nulptr");
	}

	return Result;
}

const UDataTable* UCEquipmentComponent::GetWeaponMontagesDataTable() const
{
	if (SelectedWeapon == nullptr)
	{
		CLOG_ERROR_FUNC_TEXT(L"Selected Weapon is nullptr");
		return nullptr;
	}

	UDataTable* montagesDataTable = SelectedWeapon->GetMontagesDataTable();
	if (montagesDataTable == nullptr)
	{
		// Check Weapon Bluepint
		CLOG_ERROR_FUNC_TEXT(L"Weapon DataTable is nullptr, Check the BluePrint!");
		return nullptr;
	}

	return montagesDataTable;
}

const FItem* UCEquipmentComponent::GetItemInSlot(EItemSlotType TargetItemSlotType) const
{
	ACItemWeapon* Weapon = nullptr;
	int32 SelectedWeaponIndex = INDEX_NONE;
	if (WeaponInventory.Find(SelectedWeapon, SelectedWeaponIndex) == false) return nullptr;

	int8 SwitchDirection = 0;
	switch (TargetItemSlotType)
	{
		case EItemSlotType::PREV_WEAPON_SLOT:
		{
			SwitchDirection = -1;
		}
		break;
		case EItemSlotType::NEXT_WEAPON_SLOT:
		{
			SwitchDirection = +1;
		}
		break;
		case EItemSlotType::ACTIVE_ITEM_SLOT:
		{
			// ¹Ì±¸Çö ½½·Ô
			return nullptr;
		}
		break;
	}

	SelectedWeaponIndex += SwitchDirection;
	if (SelectedWeaponIndex < 0)
	{
		SelectedWeaponIndex = WeaponInventory.Num() - 1;
	}
	else if (SelectedWeaponIndex >= WeaponInventory.Num())
	{
		SelectedWeaponIndex = 0;
	}

	Weapon = WeaponInventory[SelectedWeaponIndex];
	if (Weapon == nullptr) return nullptr;

	return Weapon->GetItem();
}

void UCEquipmentComponent::SetWeapon(ACItemWeapon* SelecteWeapon)
{
	CLOG_ERROR_CHECK_RETURN(SelecteWeapon);
	if (SelecteWeapon != SelectedWeapon)
	{
		OldWeapon = SelectedWeapon;
		SelectedWeapon = SelecteWeapon;

		OnWeaponChanged.Broadcast(SelecteWeapon);
	}
}

void UCEquipmentComponent::SetIsInCombat(const bool bNewCombat)
{
	if (bIsInCombat != bNewCombat)
	{
		bIsInCombat = bNewCombat;
		OnInCombatChanged.Broadcast(bIsInCombat);
	}
}

const bool UCEquipmentComponent::CanBlock()
{
	if (bIsInCombat)
	{
		if (GetCombatType() == ECombatType::MELEE)
		{
			return true;
		}
	}

	return false;
}

const bool UCEquipmentComponent::IsCombatTypeEqual(const ECombatType CombatTypeToCompare) const
{
	if (GetCombatType() == CombatTypeToCompare)
	{
		return true;
	}

	return false;
}

void UCEquipmentComponent::ToggleCombat()
{
	SetIsInCombat(!bIsInCombat);
}

void UCEquipmentComponent::VisibleWeapon()
{
	CLOG_ERROR_CHECK_RETURN(SelectedWeapon);
	SelectedWeapon->VisibleWeapon();
}

void UCEquipmentComponent::InVisibleWeapon()
{
	CLOG_ERROR_CHECK_RETURN(SelectedWeapon);
	SelectedWeapon->InVisibleWeapon();
	CLOG_ERROR_CHECK_RETURN(OldWeapon);
	OldWeapon->InVisibleWeapon();
}