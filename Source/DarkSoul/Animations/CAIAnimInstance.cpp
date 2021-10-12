// Copyright Epic Games, Inc. All Rights Reserved.
#include "CAIAnimInstance.h"
#include "DarkSoul/_Utility/CLog.h"
#include "DarkSoul/Characters/CCombatCharacter.h"
#include "DarkSoul/Components/CStateManagerComponent.h"

/// Unreal Engine
#include "Kismet/KismetMathLibrary.h"

void UCAIAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	CLOG_ERROR_CHECK_RETURN(Owner);

	UCStateManagerComponent* StateManager = Owner->GetStateManagerComponent();
	CLOG_ERROR_CHECK_RETURN(StateManager);

	StateManager->OnActivityChanged.AddUObject(this, &UCAIAnimInstance::OnActivityChanged);
}

void UCAIAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (bIsBlocking)
	{
		BlockAlpha = UKismetMathLibrary::FInterpTo_Constant(BlockAlpha, 1.0f, DeltaSeconds, BlockInterpSpeed);
	}
	else
	{
		BlockAlpha = UKismetMathLibrary::FInterpTo_Constant(BlockAlpha, 0.0f, DeltaSeconds, BlockInterpSpeed);
	}
}

void UCAIAnimInstance::OnActivityChanged(EActivity Activity, bool Value)
{
	if (Activity == EActivity::IS_BLOCKING_PRESSED)
	{
		bIsBlocking = Value;
	}
}