// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DarkSoul/Enumerations/CERotationMode.h"
#include "CRotatingComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnRotatingStart);
DECLARE_MULTICAST_DELEGATE(FOnRotatingEnd);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DARKSOUL_API UCRotatingComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "Debug", meta = (AllowPrivateAccess = "true"))
		bool bDebugDraw = false;

	UPROPERTY(EditAnywhere, Category = "Debug", meta = (AllowPrivateAccess = "true"))
		float DebugArrowLength = 100.0f;

public:	
	UCRotatingComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void StartRotatingWithTime(const float Time);

	void SetRotationMode(ERotationMode NewMode);

	const bool IsRotating() const { return bShouldRotate; }

private:
	void StartRotatingWithLimit();
	void StopRotating();

	void Debug();

public:
	FOnRotatingStart OnRotatingStart;
	FOnRotatingEnd OnRotatingEnd;

private:
	bool bShouldRotate = false;
	/** 경과 시간 */
	float TimeElapsed = 0.0f;
	float RotateTime = 0.0f;
	const float MaxDegreePerSecond = 720.0f;
	const float MaxAllowedDegrees = 180.0f;
};