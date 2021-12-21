// Copyright Epic Games, Inc. All Rights Reserved.
#include "CAN_CastComplete.h"
#include "DarkSoul/_Utility/CLog.h"
#include "DarkSoul/Characters/CCombatCharacter.h"

/// Unreal Engine
#include "Components/SkeletalMeshComponent.h"

FString UCAN_CastComplete::GetNotifyName_Implementation() const
{
	return FString(L"Cast Complete");
}

void UCAN_CastComplete::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	CLOG_CHECK_RETURN(MeshComp->GetOwner());

	ACCombatCharacter* Character = Cast<ACCombatCharacter>(MeshComp->GetOwner());
	CLOG_ERROR_CHECK_RETURN(Character);

	Character->CastComplete();
}