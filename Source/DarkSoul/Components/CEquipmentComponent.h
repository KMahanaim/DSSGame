// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DarkSoul/Structs/CFItem.h"
#include "DarkSoul/Enumerations/CECombatType.h"
#include "DarkSoul/Enumerations/CEWeaponType.h"
#include "DarkSoul/Enumerations/CEItemSlotType.h"
#include "DarkSoul/Enumerations/CEWeaponSwitchType.h"
#include "CEquipmentComponent.generated.h"

class AActor;
class UDataTable;
class ACItemBase;
class ACItemWeapon;
class USkeletalMeshComponent;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnInCombatChanged, bool);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnWeaponChanged, ACItemWeapon*);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DARKSOUL_API UCEquipmentComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCEquipmentComponent();

	void SpawnWeapons(USkeletalMeshComponent* Mesh, const UDataTable* WeaponPathTable);
	void SwitchWeapon(EWeaponSwitchType WeaponSwitchType);

	ACItemWeapon* GetWeapon() const { return SelectedWeapon; }
	const ECombatType GetCombatType() const;
	const UDataTable* GetWeaponMontagesDataTable() const;
	const FItem* GetItemInSlot(EItemSlotType TargetItemSlotType) const;

private:
	void SetWeapon(ACItemWeapon* SelecteWeapon);

public:
	void SetIsInCombat(const bool bNewCombat);

	const bool IsInCombat() const { return bIsInCombat; }
	const bool CanBlock();
	const bool IsCombatTypeEqual(const ECombatType CombatTypeToCompare) const;

	void ToggleCombat();
	void VisibleWeapon();
	void InVisibleWeapon();

public:
	FOnInCombatChanged OnInCombatChanged;
	FOnWeaponChanged OnWeaponChanged;

private:
	bool bIsInCombat = false;
	/** Store the old weapon when switching weapons. It's necessary for weapon undraw */
	ACItemWeapon* OldWeapon = nullptr;
	ACItemWeapon* SelectedWeapon = nullptr;
	TArray<ACItemBase*> PotionInventory;
	TArray<ACItemWeapon*> WeaponInventory;
};