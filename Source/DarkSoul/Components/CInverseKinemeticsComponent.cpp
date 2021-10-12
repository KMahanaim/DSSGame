// Copyright Epic Games, Inc. All Rights Reserved.
#include "CInverseKinemeticsComponent.h"
#include "DarkSoul/_Utility/CLog.h"

#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

UCInverseKinemeticsComponent::UCInverseKinemeticsComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UCInverseKinemeticsComponent::BeginPlay()
{
	Super::BeginPlay();

	Character = Cast<ACharacter>(GetOwner());
	CLOG_CHECK_RETURN(Character);

	CapsuleHalfHeight = Character->GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight();
}

void UCInverseKinemeticsComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FVector leftNormal, rightNormal;
	float leftTraceDistance = Trace(LeftFootSocket, leftNormal);
	float rightTraceDistance = Trace(RightFootSocket, rightNormal);

	float offset = UKismetMathLibrary::Min(leftTraceDistance, rightTraceDistance);
	offset = offset > 0.0f ? 0.0f : offset;

	float half = CapsuleHalfHeight - FMath::Abs(offset) * 0.5f;
	UCapsuleComponent* capsule = Character->GetCapsuleComponent();
	CLOG_CHECK_RETURN(capsule);

	float scaleHalf = capsule->GetScaledCapsuleHalfHeight();
	float interpValue = UKismetMathLibrary::FInterpTo(scaleHalf, half, DeltaTime, HipsInterpSpeed);

	capsule->SetCapsuleHalfHeight(interpValue);

	IK.HipOffset = UKismetMathLibrary::FInterpTo(IK.HipOffset, offset, DeltaTime, HipsInterpSpeed);

	IK.Location_Left = UKismetMathLibrary::FInterpTo(IK.Location_Left, leftTraceDistance - offset, DeltaTime, FeetInterpSpeed);
	IK.Location_Right = UKismetMathLibrary::FInterpTo(IK.Location_Right, -(rightTraceDistance - offset), DeltaTime, FeetInterpSpeed);

	IK.Rotation_Left = UKismetMathLibrary::RInterpTo(IK.Rotation_Left, NormalToRotator(leftNormal), DeltaTime, FeetInterpSpeed);
	IK.Rotation_Right = UKismetMathLibrary::RInterpTo(IK.Rotation_Right, NormalToRotator(rightNormal), DeltaTime, FeetInterpSpeed);
}

float UCInverseKinemeticsComponent::Trace(FName Socket, FVector& OutImpactNormal)
{
	FVector location = Character->GetMesh()->GetSocketLocation(Socket);
	FVector start = FVector(location.X, location.Y, Character->GetActorLocation().Z);

	float z = start.Z - CapsuleHalfHeight - TraceDistance;
	FVector end = FVector(location.X, location.Y, z);

	TArray<AActor *> ignore;
	ignore.Add(GetOwner());

	FHitResult hit;
	EDrawDebugTrace::Type debug = bDebugDraw ? EDrawDebugTrace::ForOneFrame : EDrawDebugTrace::None;

	bool bHit = UKismetSystemLibrary::LineTraceSingle
	(
		GetWorld(), start, end, UEngineTypes::ConvertToTraceType(ECC_Visibility),
		true, ignore, debug, hit, true
	);

	OutImpactNormal = hit.ImpactNormal;
	if (hit.IsValidBlockingHit())
	{
		float length = (hit.ImpactPoint - hit.TraceEnd).Size();

		return AdjustOffset + length - TraceDistance;
	}

	return 0.0f;
}

FRotator UCInverseKinemeticsComponent::NormalToRotator(FVector Normal)
{
	float t = UKismetMathLibrary::DegAtan2(Normal.Y, Normal.Z);
	float t2 = -UKismetMathLibrary::DegAtan2(Normal.X, Normal.Z);

	return FRotator(t2, 0.0f, t);
}