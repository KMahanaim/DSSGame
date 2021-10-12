// Copyright Epic Games, Inc. All Rights Reserved.
#include "CPlayerAnimInstance.h"
#include "DarkSoul/_Utility/CLog.h"
#include "DarkSoul/Characters/CPlayerCharacter.h"

void UCPlayerAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();
}

void UCPlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	CLOG_ERROR_CHECK_RETURN(Owner);
	BlockAlpha = Cast<ACPlayerCharacter>(Owner)->GetBlockAlpha();
}