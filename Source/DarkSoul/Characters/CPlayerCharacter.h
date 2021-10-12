// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "Characters/CCombatCharacter.h"
#include "CPlayerCharacter.generated.h"

DECLARE_MULTICAST_DELEGATE(FToggleHUD)

class UCurveFloat;
class UCameraComponent;
class USpringArmComponent;
class UCTargetingComponent;
class UCInputBufferComponent;
class UCExtendedCameraComponent;

/**
 * The Class that the Player has
 * 플레이어가 가지게 될 클래스
 */
UCLASS()
class DARKSOUL_API ACPlayerCharacter : public ACCombatCharacter
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly, Category = "Action Cost", meta = (ClampMax = 2.0f, ClampMin = 0.0f, UIMax = 2.0f, UIMin = 0.0f), meta = (AllowPrivateAccess = "true"))
		float RollStaminaCost = 25.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
		float HorizontalLookRate = 45.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
		float VerticalLookRate = 45.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Timeline", meta = (ClampMax = 10.0f, ClampMin = 0.0f, UIMax = 10.0f, UIMin = 0.0f), meta = (AllowPrivateAccess = "true"))
		float BlockPlayRate = 7.5f;

	UPROPERTY(EditDefaultsOnly, Category = "Timeline", meta = (ClampMax = 2.0f, ClampMin = 0.0f, UIMax = 2.0f, UIMin = 0.0f), meta = (AllowPrivateAccess = "true"))
		float BlockingTimelineLength = 1.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Timeline", meta = (AllowPrivateAccess = "true"))
		UCurveFloat* BlockingCurveFloat;

	UPROPERTY(VisibleAnywhere, Category = "Component", meta = (AllowPrivateAccess = "true"))
		USpringArmComponent* CameraSpringArm;

	UPROPERTY(VisibleAnywhere, Category = "Component", meta = (AllowPrivateAccess = "true"))
		UCameraComponent* FollowingCamera;

	UPROPERTY(VisibleAnywhere, Category = "Component", meta = (AllowPrivateAccess = "true"))
		UCExtendedCameraComponent* ExtendedCamera;

	UPROPERTY(VisibleAnywhere, Category = "Component", meta = (AllowPrivateAccess = "true"))
		UCInputBufferComponent* InputBuffer;

	UPROPERTY(VisibleAnywhere, Category = "Component", meta = (AllowPrivateAccess = "true"))
		UCTargetingComponent* Targeting;

//////////////////////////////////////////////////////////////////////////////////////////////////
// Delegate Bind UFUNCTION
//////////////////////////////////////////////////////////////////////////////////////////////////
private:
	/** Block(Defense) Function */
	UFUNCTION()
		void OnTimelineFloat();

	UFUNCTION()
		void OnBlockingTimelineFinished();

//////////////////////////////////////////////////////////////////////////////////////////////////
// General Functions
//////////////////////////////////////////////////////////////////////////////////////////////////
public:
	ACPlayerCharacter();

protected:
	virtual void BeginPlay() final;

public:
	virtual void Tick(float DeltaTime) final;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) final;
	virtual bool TakeDamage(const FHitData& HitData, EAttackResult& OutResultType) final;

	/** Action */
	virtual void RollAction() final;
	virtual float MeleeAttackAction(EMeleeAttackType AttackType) final;
	virtual void WeaponSwitchAction(EWeaponSwitchType SwitchType) final;

private:
	/** Defense */
	void UpdateBlocking();

//////////////////////////////////////////////////////////////////////////////////////////////////
// Get, Set Functions
//////////////////////////////////////////////////////////////////////////////////////////////////
public:
	/** 마지막 방향 입력값을 반환한다. */
	virtual FRotator GetDesiredRotation() final;
	const float GetBlockAlpha() const { return BlockAlpha; };
	virtual FGenericTeamId GetGenericTeamId() const { return FGenericTeamId(1); }

//////////////////////////////////////////////////////////////////////////////////////////////////
// Player Key Input Bind Functions
//////////////////////////////////////////////////////////////////////////////////////////////////
private:
	/* Character Moving */
	void MoveForwardOrBackward(float AxisValue);
	void MoveRightOrLeft(float AxisValue);

	/** Camera Control */
	void HorizontalLook(float AxisValue);
	void VerticalLook(float AxisValue);

	/** HUD Hide */
	void HUDHide();

	/** Key Input Action */
	void Run();
	void Roll();
	void LightAttack();
	void HeavyAttack();
	void StartBlocking();
	void StopBlocking();
	void ToggleDrawWeapon();
	void WeaponSwitchLeft();
	void WeaponSwitchRight();
	void ToggleLockOn();
	void ChangeToUpAndDownTarget(float AxisValue);
	void ChangeToRightAndLeftTarget(float AxisValue);

//////////////////////////////////////////////////////////////////////////////////////////////////
// Delegate Bind Functions
//////////////////////////////////////////////////////////////////////////////////////////////////
private:
	/** Input Buffer Events */
	void OnInputBufferClose();
	void OnInputBufferConsumed(EInputBufferKey Key);

	/** Rotating Events */
	void OnRotatingEnd();

	/** Stat Events */
	void OnStaminaValueChanged(float NewValue, float MaxValue);

	/** State Events */
	void OnStateChanged(EStateType PrevState, EStateType NewState);
	void OnActivityChanged(EActivity Activity, bool bValue);

	/** Effect Component Events */
	virtual void OnEffectApplied(EEffectType Type) final;
	virtual void OnEffectRemoved(EEffectType Type) final;

private:
	/** Block(Defense) Play Duration, 1.0f = Defense On */
	float BlockAlpha = 0.0f;
	/** Blocking(Defense) Time Line */
	FTimeline BlockingTimeline;

	/** Timer Handles */
	FTimerHandle WeaponSwitchMotionTimerHandle;

public:
	/** Delegate */
	FToggleHUD ToggleHUD;
};