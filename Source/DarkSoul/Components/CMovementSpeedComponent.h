// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Enumerations/CEMovementState.h"
#include "CMovementSpeedComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnMovementStateStart, EMovementState);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnMovementStateEnd, EMovementState);

class UCharacterMovementComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DARKSOUL_API UCMovementSpeedComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement Speed", meta = (ClampMax = 1000, ClampMin = 0, UIMax = 1000, UIMin = 0), meta = (AllowPrivateAccess = "true"))
		/** 느린 이동 */
		float WalkSpeed = 200.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement Speed", meta = (ClampMax = 1000, ClampMin = 0, UIMax = 1000, UIMin = 0), meta = (AllowPrivateAccess = "true"))
		/** 일반 걷기 속도 */
		float JogSpeed = 400.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement Speed", meta = (ClampMax = 1000, ClampMin = 0, UIMax = 1000, UIMin = 0), meta = (AllowPrivateAccess = "true"))
		/** 뛰기 속도 */
		float RunSpeed = 600.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement Speed", meta = (ClampMax = 100, ClampMin = 0, UIMax = 100, UIMin = 0), meta = (AllowPrivateAccess = "true"))
		/** 이동 속도 변화 보간 값  */
		float SpeedChangeInterpSpeed = 3.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement State", meta = (AllowPrivateAccess = "true"))
		/** 현재 이동 상태 */
		EMovementState MovementState = EMovementState::IDLE;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement State", meta = (AllowPrivateAccess = "true"))
		/** 최초 이동 상태 */
		EMovementState StartMovementState = EMovementState::JOG;

public:
	UCMovementSpeedComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	const EMovementState GetMovementState() const { return MovementState; }
	const float GetMaxPossibleSpeed() const;

	void SetMovementState(EMovementState NewMovementState);

private:
	void UpdateMaxSpeed();

public:
	FOnMovementStateStart OnMovementStateStart;
	FOnMovementStateEnd OnMovementStateEnd;

private:
	/** 최종 목표 속도 */
	float TargetSpeed = 0.0f;
	TArray<EMovementState> StatesToToggle;
	/** 속도를 Tick 마다 업데이트 하나? */
	bool bIsUpdatingSpeed = false;
	UCharacterMovementComponent* OwnerMovementComopnent;
};