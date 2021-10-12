// Copyright Epic Games, Inc. All Rights Reserved.
#include "CExtendedCameraComponent.h"
#include "_Utility/CLog.h"
#include "DarkSoul/Interfaces/I_CHasRotatingComponent.h"

/// Unreal Engine
#include "TimerManager.h"
#include "Engine/World.h"
#include "Camera/CameraComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

UCExtendedCameraComponent::UCExtendedCameraComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UCExtendedCameraComponent::BeginPlay()
{
	Super::BeginPlay();

	Owner = GetOwner();
	CLOG_ERROR_CHECK_RETURN(Owner);

	OwnerController = Owner->GetInstigatorController();
	CLOG_ERROR_CHECK_RETURN(OwnerController);
}

void UCExtendedCameraComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bIsActivate == false &&
		Owner->GetVelocity() != FVector::ZeroVector &&
		GetWorld()->GetTimerManager().IsTimerActive(OnActivateTimerHandle) == false)
	{
		GetWorld()->GetTimerManager().SetTimer(OnActivateTimerHandle, this, &UCExtendedCameraComponent::ActivateExtendedCamera, InterpRotationCountTime);
	}

	if (bIsActivate)
	{
		InterpRotation(DeltaTime);
	}
}

void UCExtendedCameraComponent::InterpRotation(float DeltaTime)
{
	CLOG_ERROR_CHECK_RETURN(Owner);
	CLOG_ERROR_CHECK_RETURN(OwnerController);

	//CLog::Print(Owner->GetVelocity(), 1.0f, FColor::Blue, 1);
	//CLog::Print(OwnerController->GetControlRotation(), 1.0f, FColor::Cyan, 2);

	// Owner must be moving
	if (Owner->GetVelocity() != FVector::ZeroVector && !bIsKeyInput)
	{
		FRotator DesiredRotator = Cast<II_CHasRotatingComponent>(Owner)->GetDesiredRotation();
		FRotator InterpRotator = FMath::RInterpConstantTo(OwnerController->GetControlRotation(), DesiredRotator, DeltaTime, InterpSpeed);
		InterpRotator.Pitch = -10;
		//CLog::Print(DesiredRotator, 1.0f, FColor::Red, 3);
		//CLog::Print(InterpRotator, 1.0f, FColor::Green, 4);
		CLog::Print(L"Enter Interp Rotation", 0.2f, FColor::Blue, 5);
		OwnerController->SetControlRotation(InterpRotator);
	}
	// Stop Rotation and clear timer
	else
	{
		DeActivateExtendedCamera();
	}
}

void UCExtendedCameraComponent::ActivateExtendedCamera()
{
	bIsActivate = true;
}

void UCExtendedCameraComponent::DeActivateExtendedCamera()
{
	bIsActivate = false;
	GetWorld()->GetTimerManager().ClearTimer(OnActivateTimerHandle);
}