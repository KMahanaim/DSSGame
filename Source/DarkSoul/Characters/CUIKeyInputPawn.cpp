// Copyright Epic Games, Inc. All Rights Reserved.
#include "CUIKeyInputPawn.h"
#include "DarkSoul/_Utility/CLog.h"

ACUIKeyInputPawn::ACUIKeyInputPawn()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ACUIKeyInputPawn::BeginPlay()
{
	Super::BeginPlay();
}

void ACUIKeyInputPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(L"StickUpDown", this, &ACUIKeyInputPawn::StickUpDown);
	PlayerInputComponent->BindAxis(L"StickRightLeft", this, &ACUIKeyInputPawn::StickRightLeft);

	PlayerInputComponent->BindAction(L"ArrowUp", IE_Pressed, this, &ACUIKeyInputPawn::ArrowUp);
	PlayerInputComponent->BindAction(L"ArrowDown", IE_Pressed, this, &ACUIKeyInputPawn::ArrowDown);
	PlayerInputComponent->BindAction(L"ArrowRight", IE_Pressed, this, &ACUIKeyInputPawn::ArrowRight);
	PlayerInputComponent->BindAction(L"ArrowLeft", IE_Pressed, this, &ACUIKeyInputPawn::ArrowLeft);

	PlayerInputComponent->BindAction(L"Enter", IE_Pressed, this, &ACUIKeyInputPawn::Enter);
}

void ACUIKeyInputPawn::ArrowUp()
{
	if (OnArrowUp.IsBound())
	{
		OnArrowUp.Execute();
	}
}

void ACUIKeyInputPawn::ArrowDown()
{
	if (OnArrowDown.IsBound())
	{
		OnArrowDown.Execute();
	}
}

void ACUIKeyInputPawn::ArrowRight()
{
	if (OnArrowRight.IsBound())
	{
		OnArrowRight.Execute();
	}
}

void ACUIKeyInputPawn::ArrowLeft()
{
	if (OnArrowLeft.IsBound())
	{
		OnArrowLeft.Execute();
	}
}

void ACUIKeyInputPawn::StickUpDown(float AxisValue)
{
	if (FMath::IsNearlyZero(AxisValue, KINDA_SMALL_NUMBER) == false)
	{
		if (AxisValue > 0)
		{
			if (OnArrowUp.IsBound())
			{
				bIsAlreadyStickMove = true;
				OnArrowUp.Execute();
			}
		}
		else
		{
			if (OnArrowDown.IsBound())
			{
				bIsAlreadyStickMove = true;
				OnArrowDown.Execute();
			}
		}
	}
	else
	{
		bIsAlreadyStickMove = false;
	}
}

void ACUIKeyInputPawn::StickRightLeft(float AxisValue)
{
	if (FMath::IsNearlyZero(AxisValue, KINDA_SMALL_NUMBER) == false)
	{
		if (AxisValue > 0)
		{
			if (OnArrowRight.IsBound())
			{
				bIsAlreadyStickMove = true;
				OnArrowRight.Execute();
			}
		}
		else
		{
			if (OnArrowLeft.IsBound())
			{
				bIsAlreadyStickMove = true;
				OnArrowLeft.Execute();
			}
		}
	}
	else
	{
		bIsAlreadyStickMove = false;
	}
}

void ACUIKeyInputPawn::Enter()
{
	if (OnEnter.IsBound())
	{
		OnEnter.Execute();
	}
}