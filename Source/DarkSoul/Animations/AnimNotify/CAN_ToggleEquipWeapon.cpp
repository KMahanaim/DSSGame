// Copyright Epic Games, Inc. All Rights Reserved.
#include "CAN_ToggleEquipWeapon.h"
#include "DarkSoul/_Utility/CLog.h"
#include "DarkSoul/Components/CEquipmentComponent.h"

/// Unreal Engine
#include "Components/SkeletalMeshComponent.h"

FString UCAN_ToggleEquipWeapon::GetNotifyName_Implementation() const
{
	if (bDrawWeapon)
	{
		return FString(L"Draw Weapon");
	}
	else
	{
		return FString(L"UnDraw Weapon");
	}
}

void UCAN_ToggleEquipWeapon::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	CLOG_CHECK_RETURN(MeshComp->GetOwner());

	UCEquipmentComponent* Equipment = Cast<UCEquipmentComponent>(MeshComp->GetOwner()->GetComponentByClass(UCEquipmentComponent::StaticClass()));
	if (Equipment != nullptr)
	{
		if (bDrawWeapon)
		{
			Equipment->VisibleWeapon();
		}
		else
		{
			Equipment->InVisibleWeapon();
		}
	}
}