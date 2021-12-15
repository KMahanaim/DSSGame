// Copyright Epic Games, Inc. All Rights Reserved.
#include "CAN_ToggleCombat.h"
#include "DarkSoul/_Utility/CLog.h"
#include "DarkSoul/Components/CEquipmentComponent.h"

/// Unreal Engine
#include "Components/SkeletalMeshComponent.h"

FString UCAN_ToggleCombat::GetNotifyName_Implementation() const
{
	return FString(L"Toggle Combat");
}

void UCAN_ToggleCombat::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	CLOG_CHECK_RETURN(MeshComp->GetOwner());

	UCEquipmentComponent* Equipment = Cast<UCEquipmentComponent>(MeshComp->GetOwner()->GetComponentByClass(UCEquipmentComponent::StaticClass()));
	CLOG_ERROR_CHECK_RETURN(Equipment);

	Equipment->SetIsInCombat(bIsInCombat);
}