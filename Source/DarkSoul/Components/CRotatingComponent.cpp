// Copyright Epic Games, Inc. All Rights Reserved.
#include "CRotatingComponent.h"
#include "DarkSoul/_Utility/CLog.h"
#include "DarkSoul/Interfaces/I_CHasRotatingComponent.h"

/// Unreal Engine
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

UCRotatingComponent::UCRotatingComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UCRotatingComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UCRotatingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bDebugDraw)
	{
		Debug();
	}

	if (bShouldRotate)
	{
		TimeElapsed += DeltaTime;
		if (TimeElapsed <= RotateTime)
		{
			FRotator DesiredRotator = Cast<II_CHasRotatingComponent>(GetOwner())->GetDesiredRotation();
			FRotator InterpRotator = UKismetMathLibrary::RInterpTo_Constant(GetOwner()->GetActorRotation(), DesiredRotator, DeltaTime, MaxDegreePerSecond);
			GetOwner()->SetActorRelativeRotation(InterpRotator.Quaternion());
		}
		else
		{
			StopRotating();
		}
	}
}

void UCRotatingComponent::StartRotatingWithTime(const float Time)
{
	TimeElapsed = 0.0f;
	bShouldRotate = true;
	RotateTime = Time;
	OnRotatingStart.Broadcast();
}

void UCRotatingComponent::SetRotationMode(ERotationMode NewMode)
{
	ACharacter* Owner = Cast<ACharacter>(GetOwner());
	if (Owner)
	{
		UCharacterMovementComponent* CharacterMovement = Owner->GetCharacterMovement();
		switch (NewMode)
		{
			case ERotationMode::FACE_CAMERA:
			{
				Owner->bUseControllerRotationYaw = false;
				CharacterMovement->bUseControllerDesiredRotation = true;
				CharacterMovement->bOrientRotationToMovement = false;
			}
				break;
			case ERotationMode::ORIENT_TO_MOVEMENT:
			{
				Owner->bUseControllerRotationYaw = false;
				CharacterMovement->bUseControllerDesiredRotation = false;
				CharacterMovement->bOrientRotationToMovement = true;
			}
				break;
		}
	}
}

void UCRotatingComponent::StartRotatingWithLimit()
{
	RotateTime = MaxAllowedDegrees / MaxDegreePerSecond;
	TimeElapsed = 0.0f;
	bShouldRotate = true;
	OnRotatingStart.Broadcast();
}

void UCRotatingComponent::StopRotating()
{
	bShouldRotate = false;
	OnRotatingEnd.Broadcast();
}

void UCRotatingComponent::Debug()
{
	AActor* Owner = GetOwner();
	CLOG_ERROR_CHECK_RETURN(Owner);
	FRotator DesiredRotator = Cast<II_CHasRotatingComponent>(Owner)->GetDesiredRotation();
	FVector Start = Owner->GetActorLocation();
	FVector End = (UKismetMathLibrary::Conv_RotatorToVector(DesiredRotator) * 250.0f) + Owner->GetActorLocation();
	
	// Draw the arrow
	UKismetSystemLibrary::DrawDebugArrow(this, Start, End, DebugArrowLength, FColor::Red, 0.0f, 5.0f);
}