// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CTargetingComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnTargetChanged, AActor*)
DECLARE_MULTICAST_DELEGATE_OneParam(FOnTargetingToggled, bool)

class USphereComponent;
class APlayerController;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DARKSOUL_API UCTargetingComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly, Category = "Line Trace", meta = (ClampMax = 10.0f, ClampMin = 1.0f, UIMax = 10.0f, UIMin = 1.0f), meta = (AllowPrivateAccess = "true"))
		bool bIsIgnoreBlock = false;

	UPROPERTY(EditDefaultsOnly, Category = "Line Trace", meta = (ClampMax = 10.0f, ClampMin = 1.0f, UIMax = 10.0f, UIMin = 1.0f), meta = (AllowPrivateAccess = "true"))
		float TargetBlockAllowedTime = 2.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Line Trace", meta = (ClampMax = 300.0f, ClampMin = 0.0f, UIMax = 300.0f, UIMin = 0.0f), meta = (AllowPrivateAccess = "true"))
		float LineTraceDepthOffset = 0.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Line Trace", meta = (ClampMax = 300.0f, ClampMin = 0.0f, UIMax = 300.0f, UIMin = 0.0f), meta = (AllowPrivateAccess = "true"))
		float LineTraceHeightOffset = 0.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Target", meta = (ClampMax = 2.0f, ClampMin = 0.1f, UIMax = 2.0f, UIMin = 0.1f), meta = (AllowPrivateAccess = "true"))
		float ChangeTargetSensitive = 0.5f;

	UPROPERTY(VisibleAnywhere, Category = "Target", meta = (AllowPrivateAccess = "true"))
		USphereComponent* DetecteSphere;

//////////////////////////////////////////////////////////////////////////////////////////////////
// Delegate Bind UFUNCTION
//////////////////////////////////////////////////////////////////////////////////////////////////
private:
	UFUNCTION()
		void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

//////////////////////////////////////////////////////////////////////////////////////////////////
// General Functions
//////////////////////////////////////////////////////////////////////////////////////////////////
public:	
	UCTargetingComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void ToggleLockOn();
	void ChangeToLeftTarget();
	void ChangeToRightTarget();
	void ChangeToUpAndDownTarget(float AxisValue);
	void ChangeToRightAndLeftTarget(float AxisValue);

private:
	void UpdateOwnerRotation();
	void UpdateIgnoreLookInput();

	void FindTarget();
	const bool ActorToScreenPosition(const AActor* Target, FVector2D& OutScreenPosition) const;

	void OnLockOn();
	void OffLockOn();
	void ResetTarget();
	void ResetBlockingTimer();

//////////////////////////////////////////////////////////////////////////////////////////////////
// Get, Set Functions
//////////////////////////////////////////////////////////////////////////////////////////////////
public:
	AActor* GetTarget() const { return CurrentTarget; };

private:
	const FVector GetLineTraceStartLocation() const;
	void SetTarget(AActor* NewTarget);

//////////////////////////////////////////////////////////////////////////////////////////////////
// Condition Check Functions
//////////////////////////////////////////////////////////////////////////////////////////////////
public:
	const bool IsLockOnActivity() const { return bIsLockOnActivity; };

private:
	const bool IsInViewport(const FVector2D ScreenPosition) const;
	/** @return TRUE if OutHits contains any blocking hit entries */
	const bool IsTargetBlockingTrace(const AActor* Target) const;

//////////////////////////////////////////////////////////////////////////////////////////////////
// General Member
//////////////////////////////////////////////////////////////////////////////////////////////////
private:
	bool bCanTargetChange = false;
	bool bIsLockOnActivity = false;
	bool bBlockingTimerActivate = false;
	AActor* CurrentTarget = nullptr;
	APlayerController* OwnerController;
	TArray<AActor*> TargetList;
	FTimerHandle BlockingTimerHandle;
};