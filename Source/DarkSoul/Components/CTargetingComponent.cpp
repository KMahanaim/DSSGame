// Copyright Epic Games, Inc. All Rights Reserved.
#include "CTargetingComponent.h"
#include "DarkSoul/_Utility/CLog.h"
#include "DarkSoul/Interfaces/I_CTargetAble.h"

/// Unreal Engine
#include "Engine/World.h"
#include "TimerManager.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameFramework/PlayerController.h"
#include "Components/SphereComponent.h"

UCTargetingComponent::UCTargetingComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	// Create Component
	{
		DetecteSphere = CreateDefaultSubobject<USphereComponent>(FName("DetecteSphere"));
	}

	// Set Component Defaults
	{
		DetecteSphere->SetSphereRadius(2000.0f);
		DetecteSphere->bHiddenInGame = false;
	}
}

void UCTargetingComponent::BeginPlay()
{
	Super::BeginPlay();

	AActor* Owner = GetOwner();
	CLOG_ERROR_CHECK_RETURN(Owner);

	OwnerController = Cast<APlayerController>(Owner->GetInstigatorController());
	CLOG_ERROR_CHECK_RETURN(OwnerController);

	// Set Component Attach
	{
		DetecteSphere->AttachToComponent(GetOwner()->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
	}

	// Set Member Value
	{
		bCanTargetChange = false;
		bIsLockOnActivity = false;
		bBlockingTimerActivate = false;
	}

	// Delegate Bind
	{
		DetecteSphere->OnComponentBeginOverlap.AddDynamic(this, &UCTargetingComponent::OnBeginOverlap);
		DetecteSphere->OnComponentEndOverlap.AddDynamic(this, &UCTargetingComponent::OnEndOverlap);
	}

	// Already Overlapped Check
	{
		DetecteSphere->GetOverlappingActors(TargetList, AActor::StaticClass());
		for (int32 Index = 0; Index < TargetList.Num();)
		{
			AActor* Target = TargetList[Index];
			const bool bIsTargetAble = UKismetSystemLibrary::DoesImplementInterface(Target, UI_CTargetAble::StaticClass());
			if (bIsTargetAble == false)
			{
				TargetList.RemoveSwap(Target);
			}
			else
			{
				Index++;
			}
		}
	}
}

void UCTargetingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Is Activate?
	if (bIsLockOnActivity)
	{
		// Is Line Trace Block? If Block persists(Áö¼Ó) for more then TargetBlockAllowedTime, Switch Target
		const bool bIsTargetBlock = IsTargetBlockingTrace(CurrentTarget);
		if (bIsTargetBlock && (bBlockingTimerActivate == false))
		{
			bBlockingTimerActivate = true;
			GetWorld()->GetTimerManager().SetTimer(BlockingTimerHandle, this, &UCTargetingComponent::ToggleLockOn, TargetBlockAllowedTime, false);
		}
		else if (bIsTargetBlock == false)
		{
			ResetBlockingTimer();
		}

		UpdateOwnerRotation();
	}
}

void UCTargetingComponent::ToggleLockOn()
{
	if (bIsLockOnActivity)
	{
		OffLockOn();
	}
	else
	{
		OnLockOn();
	}
}

void UCTargetingComponent::ChangeToLeftTarget()
{

}

void UCTargetingComponent::ChangeToRightTarget()
{
	
}

void UCTargetingComponent::ChangeToUpAndDownTarget(float AxisValue)
{
	if (bIsLockOnActivity == false) return;
	if (TargetList.Num() <= 1) return;
	return;

	if (bCanTargetChange)
	{
		bCanTargetChange = false;

		int32 UpAndDownOffset;
		// Up : +1, Down : -1
		if (AxisValue >= 0)
		{
			UpAndDownOffset = -1;
		}
		else
		{
			UpAndDownOffset = +1;
		}

		float PotentialTargetDistance = 9999.9f;
		AActor* PotentialTarget = nullptr;
		FVector2D CurrentTargetScreenPosition, OtherTargetScreenPosition;
		for (int32 Index = 0; Index < TargetList.Num(); Index++)
		{
			if (CurrentTarget == TargetList[Index]) continue;

			ActorToScreenPosition(TargetList[Index], OtherTargetScreenPosition);
			if (IsInViewport(OtherTargetScreenPosition) == false) continue;

			ActorToScreenPosition(CurrentTarget, CurrentTargetScreenPosition);
			
			// Check Y Position
			if (OtherTargetScreenPosition.Y * UpAndDownOffset > CurrentTargetScreenPosition.Y * UpAndDownOffset)
			{
				 float Distance = FVector2D::Distance(OtherTargetScreenPosition, CurrentTargetScreenPosition);
				 if (PotentialTargetDistance > Distance)
				 {
					 PotentialTargetDistance = Distance;
					 PotentialTarget = TargetList[Index];
				 }
			}
		}

		if (PotentialTarget != nullptr)
		{
			SetTarget(PotentialTarget);
		}
	}
}

void UCTargetingComponent::ChangeToRightAndLeftTarget(float AxisValue)
{
	if (bIsLockOnActivity == false) return;
	if (TargetList.Num() <= 1) return;

	if (FMath::IsNearlyZero(AxisValue, KINDA_SMALL_NUMBER))
	{
		bCanTargetChange = true;
	}

	if (bCanTargetChange && (FMath::Abs(AxisValue) > ChangeTargetSensitive))
	{
		bCanTargetChange = false;

		int32 RightAndLeftOffset;
		// Right : +1, Left : -1
		if (AxisValue >= 0)
		{
			RightAndLeftOffset = 1;
		}
		else
		{
			RightAndLeftOffset = -1;
		}

		float PotentialTargetDistance = 9999.9f;
		AActor* PotentialTarget = nullptr;
		FVector2D CurrentTargetScreenPosition, OtherTargetScreenPosition;
		for (int32 Index = 0; Index < TargetList.Num(); Index++)
		{
			if (CurrentTarget == TargetList[Index]) continue;

			ActorToScreenPosition(TargetList[Index], OtherTargetScreenPosition);
			if (IsInViewport(OtherTargetScreenPosition) == false) continue;

			ActorToScreenPosition(CurrentTarget, CurrentTargetScreenPosition);

			// Check X Position
			if (OtherTargetScreenPosition.X * RightAndLeftOffset > CurrentTargetScreenPosition.X * RightAndLeftOffset)
			{
				float Distance = FVector2D::Distance(OtherTargetScreenPosition, CurrentTargetScreenPosition);
				if (PotentialTargetDistance > Distance)
				{
					PotentialTargetDistance = Distance;
					PotentialTarget = TargetList[Index];
				}
			}
		}

		if (PotentialTarget != nullptr)
		{
			SetTarget(PotentialTarget);
		}
	}
}

void UCTargetingComponent::UpdateOwnerRotation()
{
	CLOG_ERROR_CHECK_RETURN(OwnerController);
	if (bIsLockOnActivity)
	{
		FVector OwnerLocation = GetOwner()->GetActorLocation();
		FRotator Rotation = UKismetMathLibrary::FindLookAtRotation(OwnerLocation, CurrentTarget->GetActorLocation());
		OwnerController->SetControlRotation(Rotation);
	}
}

void UCTargetingComponent::UpdateIgnoreLookInput()
{
	CLOG_ERROR_CHECK_RETURN(OwnerController);
	if (IsLockOnActivity())
	{
		OwnerController->SetIgnoreLookInput(true);
	}
	else
	{
		OwnerController->ResetIgnoreLookInput();
	}
}

void UCTargetingComponent::FindTarget()
{
	// Reset Blocking Timer
	ResetBlockingTimer();

	AActor* PotentialTarget = nullptr;
	AActor* NextTarget = nullptr;
	FVector2D ScreenPosition;
	float DistanceFromCenterOfViewport;
	int32 ViewportSizeX, ViewportSizeY;
	OwnerController->GetViewportSize(ViewportSizeX, ViewportSizeY);

	if (ViewportSizeX > ViewportSizeY)
	{
		DistanceFromCenterOfViewport = ViewportSizeX;
	}
	else
	{
		DistanceFromCenterOfViewport = ViewportSizeY;
	}
	
	for (int32 Index = 0; Index < TargetList.Num(); Index++)
	{
		NextTarget = TargetList[Index];
		II_CTargetAble* TargetInterface = Cast<II_CTargetAble>(NextTarget);
		// Is Target Able?
		if (TargetInterface->IsTargetAble() == false) continue;

		ActorToScreenPosition(NextTarget, ScreenPosition);
		if (IsInViewport(ScreenPosition))
		{
			// Is Target Block?, if TRUE next index;
			if (IsTargetBlockingTrace(NextTarget)) continue;

			// What's the nearest Target
			ActorToScreenPosition(NextTarget, ScreenPosition);
			float NextTargetFromCenterOfViewport = FVector2D::Distance(FVector2D(ViewportSizeX / 2, ViewportSizeY / 2), ScreenPosition);
			if (NextTargetFromCenterOfViewport < DistanceFromCenterOfViewport)
			{
				DistanceFromCenterOfViewport = NextTargetFromCenterOfViewport;
				PotentialTarget = NextTarget;
			}
		}
	}

	if (PotentialTarget != nullptr)
	{
		SetTarget(PotentialTarget);
	}
	else
	{
		ResetTarget();
	}
}

const bool UCTargetingComponent::ActorToScreenPosition(const AActor* Target, FVector2D& OutScreenPosition) const
{
	if (UGameplayStatics::ProjectWorldToScreen(UGameplayStatics::GetPlayerController(GetWorld(), 0), Target->GetActorLocation(), OutScreenPosition))
	{
		return true;
	}

	return false;
}

void UCTargetingComponent::OnLockOn()
{
	// On
	bIsLockOnActivity = true;

	// Find the Target and Check Target Exsit
	FindTarget();

	// If Target is nullptr, Off
	if (CurrentTarget == nullptr)
	{
		bIsLockOnActivity = false;
	}

	UpdateIgnoreLookInput();
}

void UCTargetingComponent::OffLockOn()
{
	// Off
	bIsLockOnActivity = false;
	ResetTarget();
	UpdateIgnoreLookInput();
}

void UCTargetingComponent::ResetTarget()
{
	if (CurrentTarget != nullptr)
	{
		if (bIsLockOnActivity == false)
		{
			II_CTargetAble* TargetInterface = Cast<II_CTargetAble>(CurrentTarget);
			TargetInterface->OnDeSelected();
			CurrentTarget = nullptr;
		}
		else
		{
			CLOG_ERROR_FUNC_TEXT(L"bIsLockOnActivity must be false to reset the CurrentTarget");
		}
	}
}

void UCTargetingComponent::ResetBlockingTimer()
{
	bBlockingTimerActivate = false;
	GetWorld()->GetTimerManager().ClearTimer(BlockingTimerHandle);
}

const FVector UCTargetingComponent::GetLineTraceStartLocation() const
{
	AActor* Owner = GetOwner();
	FVector Result = Owner->GetActorLocation();

	// Add Line Trace Start Offset
	Result += Owner->GetActorForwardVector() * LineTraceDepthOffset;
	Result += Owner->GetActorUpVector() * LineTraceHeightOffset;

	return Result;
}

void UCTargetingComponent::SetTarget(AActor* NewTarget)
{
	CLOG_ERROR_CHECK_RETURN(NewTarget);

	if (bIsLockOnActivity)
	{
		II_CTargetAble* TargetInterface = nullptr;
		if (CurrentTarget != nullptr)
		{
			TargetInterface = Cast<II_CTargetAble>(CurrentTarget);
			TargetInterface->OnDeSelected();
		}

		TargetInterface = Cast<II_CTargetAble>(NewTarget);
		TargetInterface->OnSelected();

		CurrentTarget = NewTarget;
	}
}

const bool UCTargetingComponent::IsInViewport(const FVector2D ScreenPosition) const
{
	int32 SizeX, SizeY;
	OwnerController->GetViewportSize(SizeX, SizeY);
	// Check in the Viewport 
	if ((ScreenPosition.X > 0.0f) &&
		(ScreenPosition.Y > 0.0f) &&
		(ScreenPosition.X <= SizeX) &&
		(ScreenPosition.Y <= SizeY))
	{
		return true;
	}

	return false;
}

const bool UCTargetingComponent::IsTargetBlockingTrace(const AActor* Target) const
{
	// Check Ignore Block
	if (bIsIgnoreBlock) return false;

	TArray<FHitResult> HitResults;
	FCollisionQueryParams CollisionQueryParams;
	CollisionQueryParams.AddIgnoredActor(GetOwner());

	return GetWorld()->LineTraceMultiByChannel
	(
		HitResults,
		GetLineTraceStartLocation(),
		Target->GetActorLocation(),
		ECollisionChannel::ECC_GameTraceChannel2,
		CollisionQueryParams
	);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// UFunction
//////////////////////////////////////////////////////////////////////////////////////////////////
void UCTargetingComponent::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	const bool bIsTargetAble = UKismetSystemLibrary::DoesImplementInterface(OtherActor, UI_CTargetAble::StaticClass());
	if (bIsTargetAble == false) return;
	if (TargetList.Contains(OtherActor)) return;

	TargetList.Emplace(OtherActor);
}

void UCTargetingComponent::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	const bool bIsTargetAble = UKismetSystemLibrary::DoesImplementInterface(OtherActor, UI_CTargetAble::StaticClass());
	if (bIsTargetAble == false) return;
	
	int32 Index = INDEX_NONE;
	if (TargetList.Find(OtherActor, Index))
	{
		TargetList.RemoveSwap(OtherActor);
		if (OtherActor == CurrentTarget)
		{
			// Off
			ToggleLockOn();
		}
	}
}