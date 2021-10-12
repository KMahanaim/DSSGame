// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CExtendedCameraComponent.generated.h"

class AController;
class UCameraComponent;
class USpringArmComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DARKSOUL_API UCExtendedCameraComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "Interp", meta = (AllowPrivateAccess = "true"))
		float InterpSpeed = 100.0f;

	UPROPERTY(EditAnywhere, Category = "Interp", meta = (AllowPrivateAccess = "true"))
		float InterpRotationCountTime = 2.0f;

public:	
	UCExtendedCameraComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void CameraKeyInput(const bool bIsCamerakeyInput) { bIsKeyInput = bIsCamerakeyInput; }
	void IsKeyInputCamera(const bool bKeyInput) { bIsKeyInput = bKeyInput; }

private:
	void InterpRotation(float DeltaTime);

	void ActivateExtendedCamera();
	void DeActivateExtendedCamera();

private:
	bool bIsActivate = false;
	bool bIsKeyInput = false;

	AActor* Owner;
	AController* OwnerController;

	FTimerHandle OnActivateTimerHandle;
};
