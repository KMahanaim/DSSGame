// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GenericTeamAgentInterface.h"
#include "DarkSoul/Interfaces/I_CCanBeAttacked.h"
#include "DarkSoul/Interfaces/I_CCanGetEffects.h"
#include "DarkSoul/Interfaces/I_CHasRotatingComponent.h"
#include "DarkSoul/Interfaces/I_CMontageManagerComponent.h"
#include "Components/TimelineComponent.h"
#include "DarkSoul/Enumerations/CEActivity.h"
#include "DarkSoul/Enumerations/CEStatsType.h"
#include "DarkSoul/Enumerations/CEStateType.h"
#include "DarkSoul/Enumerations/CEDirection.h"
#include "DarkSoul/Enumerations/CEInputBufferKey.h"
#include "DarkSoul/Enumerations/CEMeleeAttackType.h"
#include "DarkSoul/Enumerations/CEWeaponSwitchType.h"
#include "CCombatCharacter.generated.h"

class UUserWidget;
class ACItemWeapon;
class UCapsuleComponent;
class UCEffectsComponent;
class UCDissolveComponent;
class UCRotatingComponent;
class UCEquipmentComponent;
class UCStatsManagerComponent;
class UCStateManagerComponent;
class UCExtendedStatComponent;
class UCMovementSpeedComponent;
class UCMontageManagerComponent;
class UCCollisionHandlerComponent;
class UCInverseKinemeticsComponent;

class UNiagaraComponent;

/**
 * 기본형이 되는 캐릭터 클래스
 * 플레이어와 몬스터는 이 클래스를 사용한다.
 */
UCLASS()
class DARKSOUL_API ACCombatCharacter : public ACharacter, public IGenericTeamAgentInterface, 
	public II_CCanBeAttacked, public II_CCanGetEffects, public II_CMontageManagerComponent, public II_CHasRotatingComponent
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, meta = (ClampMax = 1.0f, ClampMin = 0.0f, UIMax = 1.0f, UIMin = 0.0f), Category = "Montage", meta = (AllowPrivateAccess = "true"))
		float ResetEffectDurationPercent = 1.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Data Table", meta = (AllowPrivateAccess = "true"))
		UDataTable* WeaponPathTable;

//////////////////////////////////////////////////////////////////////////////////////////////////
// Components UPROPERTY
//////////////////////////////////////////////////////////////////////////////////////////////////
protected:
	UPROPERTY(VisibleAnywhere, Category = "Component", meta = (AllowPrivateAccess = "true"))
		UNiagaraComponent* NiagaraSystem;

	UPROPERTY(VisibleAnywhere, Category = "Component", meta = (AllowPrivateAccess = "true"))
		UCMontageManagerComponent* MontageManager;

	UPROPERTY(VisibleAnywhere, Category = "Component", meta = (AllowPrivateAccess = "true"))
		UCStateManagerComponent* StateManager;

	UPROPERTY(VisibleAnywhere, Category = "Component", meta = (AllowPrivateAccess = "true"))
		UCStatsManagerComponent* StatsManager;

	UPROPERTY(VisibleAnywhere, Category = "Component", meta = (AllowPrivateAccess = "true"))
		UCEquipmentComponent* Equipment;

	UPROPERTY(VisibleAnywhere, Category = "Component", meta = (AllowPrivateAccess = "true"))
		UCRotatingComponent* Rotating;

	UPROPERTY(VisibleAnywhere, Category = "Component", meta = (AllowPrivateAccess = "true"))
		UCEffectsComponent* Effects;

	UPROPERTY(VisibleAnywhere, Category = "Component", meta = (AllowPrivateAccess = "true"))
		UCMovementSpeedComponent* MovementSpeed;

	UPROPERTY(VisibleAnywhere, Category = "Component", meta = (AllowPrivateAccess = "true"))
		UCExtendedStatComponent* ExtendedHealth;

	UPROPERTY(VisibleAnywhere, Category = "Component", meta = (AllowPrivateAccess = "true"))
		UCExtendedStatComponent* ExtendedStamina;

	UPROPERTY(VisibleAnywhere, Category = "Component", meta = (AllowPrivateAccess = "true"))
		UCExtendedStatComponent* ExtendedMana;

private:
	UPROPERTY(VisibleAnywhere, Category = "Component", meta = (AllowPrivateAccess = "true"))
		UCCollisionHandlerComponent* CollisionHandler;

	UPROPERTY(VisibleAnywhere, Category = "Component", meta = (AllowPrivateAccess = "true"))
		UCDissolveComponent* Dissolve;

	UPROPERTY(VisibleAnywhere, Category = "Component")
		UCInverseKinemeticsComponent* InverseKinemetics;

//////////////////////////////////////////////////////////////////////////////////////////////////
// General Functions
//////////////////////////////////////////////////////////////////////////////////////////////////
public:
	ACCombatCharacter();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual bool TakeDamage(const FHitData& HitData, EAttackResult& OutResultType) override;
	void ToggleCombat();
	void ToggleCombat(float& OutMontagePlayTime);
	virtual void RollAction();
	virtual float MeleeAttackAction(EMeleeAttackType AttackType);
	virtual void WeaponSwitchAction(EWeaponSwitchType SwitchType);

protected:
	EMontageAction ConvertMeleeAttackTypeToAction(EMeleeAttackType AttackType);
	void ConvertHitResultToHitData(const FHitResult& HitResult, FHitData& OutHitData);
	const float ScaleMeleeDamageByType(const float Damage) const;
	const float ScaleMeleeAttackStaminaCostByType(float Cost, EMeleeAttackType AttackType);

	void ResetMeleeAttackCount() { MeleeAttackCount = 0; }

	/** Character Death */
	virtual void Death();

	/* Effect Play */
	void Block();
	virtual void Stunned();
	virtual void BackAttackDown();
	virtual void Impact();
	virtual void Parried();

//////////////////////////////////////////////////////////////////////////////////////////////////
// Get, Set Functions
//////////////////////////////////////////////////////////////////////////////////////////////////
public:
	/** 1 : Player, 2 : Monster, 3 : Neutral(중립) */
	//virtual FGenericTeamId GetGenericTeamId() const { return FGenericTeamId(3); }
	virtual FRotator GetDesiredRotation() { return GetActorRotation(); }
	EDirection GetHitDirection(FVector HitFromDirection, AActor* AttackedActor);

	/** Get Montages */
	virtual const UDataTable* GetMontages(EMontageAction SelectMontage) const override;
	UAnimMontage* GetRollMontage();
	UAnimMontage* GetStunMontage(EDirection Direction);
	UAnimMontage* GetBlockMontage();
	UAnimMontage* GetMeleeAttackMontage(EMeleeAttackType AttackType);

	/** Get Component */
	UCExtendedStatComponent* GetExtendedStatComponent(EStatsType Type) const;
	UCEffectsComponent* GetEffectsComponent() const { return Effects; }
	UCRotatingComponent* GetRotatingComponent() const { return Rotating; }
	UCEquipmentComponent* GetEquipmentComponent() const { return Equipment; }
	UCMovementSpeedComponent* GetMovementSpeed() const { return MovementSpeed; }
	UCStateManagerComponent* GetStateManagerComponent() const { return StateManager; }
	UCStatsManagerComponent* GetStatsManagerComponent() const { return StatsManager; }

protected:
	UCapsuleComponent* GetCapsule() const { return GetCapsuleComponent(); };

protected:
	void SetReceiveHitDirection(FVector HitFromDirection);

//////////////////////////////////////////////////////////////////////////////////////////////////
// Condition Check Functions
//////////////////////////////////////////////////////////////////////////////////////////////////
public:
	virtual bool IsAlive() const override;
	const bool IsFalling() const;
	virtual const bool CanEffectBeApplied(EEffectType Type, AActor* Applier) override;
	const bool CanBeAttacked() const;
	const bool CanBeStunned() const;
	const bool CanMeleeAttack() const;
	const bool CanRoll() const;
	const bool CanBeInterrupted() const { return !IsActivityPure(EActivity::CANT_BE_INTERRUPTED); };

protected:
	const bool IsActivityPure(const EActivity Activity) const;
	const bool IsIdleandNotFall() const;
	const bool IsStateSame(const EStateType StateToCompare) const;
	const bool IsEnoughStamina(const float Value) const;
	const bool HasMovementInput() const;

//////////////////////////////////////////////////////////////////////////////////////////////////
// Delegate Bind Functions
//////////////////////////////////////////////////////////////////////////////////////////////////
private:
	/** Stat Events */
	void OnHealthValueChanged(float NewValue, float MaxValue);

	/** Weapon Collision Events */
	void OnHit(FHitResult HitResult);
	void OnCollisionActivated();

	/** Equipment Component Events */
	void OnWeaponChanged(ACItemWeapon* Weapon);

protected:
	/** Effect Component Events */
	virtual void OnEffectApplied(EEffectType Type);
	virtual void OnEffectRemoved(EEffectType Type);

//////////////////////////////////////////////////////////////////////////////////////////////////
// General Member
//////////////////////////////////////////////////////////////////////////////////////////////////
protected:
	EDirection ReceiveHitDirection = EDirection::FRONT;

	/** Type - LightAttack, HeavyAttack...  */
	EMeleeAttackType MeleeAttackType = EMeleeAttackType::NONE;

	/** Attack Count */
	uint8 MeleeAttackCount = 0;
	FTimerHandle ResetMeleeAttackCounterTimerHandle;

	/** Reset State Timer */
	FTimerHandle ResetStateTimerHandle;
};