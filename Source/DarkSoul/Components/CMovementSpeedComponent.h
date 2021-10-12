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
		/** ���� �̵� */
		float WalkSpeed = 200.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement Speed", meta = (ClampMax = 1000, ClampMin = 0, UIMax = 1000, UIMin = 0), meta = (AllowPrivateAccess = "true"))
		/** �Ϲ� �ȱ� �ӵ� */
		float JogSpeed = 400.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement Speed", meta = (ClampMax = 1000, ClampMin = 0, UIMax = 1000, UIMin = 0), meta = (AllowPrivateAccess = "true"))
		/** �ٱ� �ӵ� */
		float RunSpeed = 600.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement Speed", meta = (ClampMax = 100, ClampMin = 0, UIMax = 100, UIMin = 0), meta = (AllowPrivateAccess = "true"))
		/** �̵� �ӵ� ��ȭ ���� ��  */
		float SpeedChangeInterpSpeed = 3.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement State", meta = (AllowPrivateAccess = "true"))
		/** ���� �̵� ���� */
		EMovementState MovementState = EMovementState::IDLE;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement State", meta = (AllowPrivateAccess = "true"))
		/** ���� �̵� ���� */
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
	/** ���� ��ǥ �ӵ� */
	float TargetSpeed = 0.0f;
	TArray<EMovementState> StatesToToggle;
	/** �ӵ��� Tick ���� ������Ʈ �ϳ�? */
	bool bIsUpdatingSpeed = false;
	UCharacterMovementComponent* OwnerMovementComopnent;
};