// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "Characters/CCombatCharacter.h"
#include "DarkSoul/Interfaces/I_CInteraction.h"
#include "CPlayerCharacter.generated.h"

DECLARE_DELEGATE(FPlayInteraction)
DECLARE_DELEGATE(FOffInteractionMassage)
DECLARE_DELEGATE_OneParam(FOnInteractionMassage, FInteractionMassage)
DECLARE_MULTICAST_DELEGATE(FToggleHUD)
DECLARE_MULTICAST_DELEGATE(FOnLoading)

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
class DARKSOUL_API ACPlayerCharacter : public ACCombatCharacter, public II_CInteraction
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
	virtual void OnInteraction(FInteractionMassage InteractionMassage) final;
	virtual void OffInteraction() final;

	/** Action */
	virtual void RollAction() final;
	virtual float AttackAction(EAttackType NewAttackType) final;
	virtual void WeaponSwitchAction(EWeaponSwitchType SwitchType) final;

	void OnCinematic();
	void EndCinematic();
	void OnBeginLoading();

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
// Condition Check Functions
//////////////////////////////////////////////////////////////////////////////////////////////////
public:
	const bool IsInLoading() const { return bIsInLoading; }

//////////////////////////////////////////////////////////////////////////////////////////////////
// Player Key Input Bind Functions
//////////////////////////////////////////////////////////////////////////////////////////////////
private:
	/** Character Moving */
	void MoveForwardOrBackward(float AxisValue);
	void MoveRightOrLeft(float AxisValue);

	/** Camera Control */
	void HorizontalLook(float AxisValue);
	void VerticalLook(float AxisValue);

	/** Interaction */
	virtual void Interaction() final;

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

	/** HUD Hide */
	void HUDHide();

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
	bool bIsKeyBlock = false;
	bool bIsInLoading = false;
	/** Block(Defense) Play Duration, 1.0f = Defense On */
	float BlockAlpha = 0.0f;
	/** Blocking(Defense) Time Line */
	FTimeline BlockingTimeline;

	/** Timer Handles */
	FTimerHandle WeaponSwitchMotionTimerHandle;

public:
	/** Delegate */
	FOnLoading OnLoading;
	FToggleHUD ToggleHUD;
	FPlayInteraction PlayInteraction;
	FOnInteractionMassage OnInteractionMassage;
	FOffInteractionMassage OffInteractionMassage;
};