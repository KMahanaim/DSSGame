// Copyright Epic Games, Inc. All Rights Reserved.
#include "CCombatCharacter.h"
#include "DarkSoul/_Utility/CLog.h"
#include "DarkSoul/Items/Weapons/CItemWeapon.h"
#include "DarkSoul/Components/CEffectsComponent.h"
#include "DarkSoul/Components/CDissolveComponent.h"
#include "DarkSoul/Components/CRotatingComponent.h"
#include "DarkSoul/Components/CEquipmentComponent.h"
#include "DarkSoul/Components/CStatsManagerComponent.h"
#include "DarkSoul/Components/CStateManagerComponent.h"
#include "DarkSoul/Components/CExtendedStatComponent.h"
#include "DarkSoul/Components/CMovementSpeedComponent.h"
#include "DarkSoul/Components/CMontageManagerComponent.h"
#include "DarkSoul/Components/CCollisionHandlerComponent.h"
#include "DarkSoul/Components/CInverseKinemeticsComponent.h"

/// Unreal Engine
#include "TimerManager.h"
#include "Engine/World.h"
#include "NiagaraComponent.h"
#include "DrawDebugHelpers.h"
#include "Curves/CurveFloat.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/ArrowComponent.h"
#include "Components/InputComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

ACCombatCharacter::ACCombatCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	// Create Components
	{
		Effects = CreateDefaultSubobject<UCEffectsComponent>(FName("Effects"));
		Dissolve = CreateDefaultSubobject<UCDissolveComponent>(FName("Dissolve"));

		Equipment = CreateDefaultSubobject<UCEquipmentComponent>(FName("Equipment"));
		StateManager = CreateDefaultSubobject<UCStateManagerComponent>(FName("StateManager"));
		StatsManager = CreateDefaultSubobject<UCStatsManagerComponent>(FName("StatsManager"));
		MontageManager = CreateDefaultSubobject<UCMontageManagerComponent>(FName("MontageManager"));

		Rotating = CreateDefaultSubobject<UCRotatingComponent>(FName("Rotating"));
		MovementSpeed = CreateDefaultSubobject<UCMovementSpeedComponent>(FName("MovementSpeed"));
		CollisionHandler = CreateDefaultSubobject<UCCollisionHandlerComponent>(FName("CollisionHandler"));
		InverseKinemetics = CreateDefaultSubobject<UCInverseKinemeticsComponent>(FName("InverseKinemetics"));

		ExtendedMana = CreateDefaultSubobject<UCExtendedStatComponent>(FName("ExtendedMana"));
		ExtendedHealth = CreateDefaultSubobject<UCExtendedStatComponent>(FName("ExtendedHealth"));
		ExtendedStamina = CreateDefaultSubobject<UCExtendedStatComponent>(FName("ExtendedStamina"));

		NiagaraSystem = CreateDefaultSubobject<UNiagaraComponent>(FName("NiagaraSystem"));
	}

	// Set Components Attach
	{
		SetRootComponent(GetCapsuleComponent());
		GetMesh()->AttachToComponent(GetCapsuleComponent(), FAttachmentTransformRules::KeepRelativeTransform);
	}

	// Set Components Defaults
	{
		/// Set Camera Movement
		bUseControllerRotationYaw = false;
		GetCharacterMovement()->bOrientRotationToMovement = true;

		/// Set Capsule Component
		GetCapsuleComponent()->SetCapsuleHalfHeight(95.0f);
		GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, -98.0f));
		GetMesh()->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));

		/// Set Extended Stat Component Type
		ExtendedHealth->SetStatType(EStatsType::HEALTH);
		ExtendedMana->SetStatType(EStatsType::MANA);
		ExtendedStamina->SetStatType(EStatsType::STAMINA);
		ExtendedStamina->SetRegenerate(true);

		/// Set Collision
		GetCapsuleComponent()->SetCollisionProfileName(FName("CapsuleCollision"));
		GetMesh()->SetCollisionProfileName(FName("CombatCharacter"));
	}
}

void ACCombatCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Delegate Bind, Add
	{
		CollisionHandler->OnHit.AddUObject(this, &ACCombatCharacter::OnHit);
		CollisionHandler->OnCollisionActivated.AddUObject(this, &ACCombatCharacter::OnCollisionActivated);

		Equipment->OnWeaponChanged.AddUObject(this, &ACCombatCharacter::OnWeaponChanged);

		ExtendedHealth->OnValueChanged.AddUObject(this, &ACCombatCharacter::OnHealthValueChanged);

		Effects->OnEffectApplied.AddUObject(this, &ACCombatCharacter::OnEffectApplied);
		Effects->OnEffectRemoved.AddUObject(this, &ACCombatCharacter::OnEffectRemoved);
	}

	// Spawn Weapons
	{
		Equipment->SpawnWeapons(GetMesh(), WeaponPathTable);
	}
}

void ACCombatCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACCombatCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

bool ACCombatCharacter::TakeDamage(const FHitData& HitData, EAttackResult& OutResultType)
{
	if (CanBeAttacked())
	{
		// Is not zero damage?
		if (FMath::IsNearlyZero(HitData.Damage, KINDA_SMALL_NUMBER) == false)
		{
			// Check hit direction is front?
			const bool bHitFront = ReceiveHitDirection == EDirection::FRONT;

			// Is parried?
			if (bHitFront &&
				IsActivityPure(EActivity::CAN_PARRY_HIT) &&
				HitData.bCanBeParried)
			{
				CLOG_FUNC_TEXT(L"Parried");
				// Apply parry
				UCEffectsComponent* CauserEffects = Cast<ACCombatCharacter>(HitData.DamageCauser)->GetEffectsComponent();
				if (CauserEffects != nullptr)
				{
					const bool bApplied = CauserEffects->ApplyEffect(EEffectType::PARRIED, EApplyEffectMethod::REPLACE, this);

					// Sound play
					if (bApplied)
					{
						// 미구현
						// 사운드 재생
					}
				}

				OutResultType = EAttackResult::PARRIED;
				return true;
			}

			// Check Blocked
			bool bIsBlocked = false;
			if (bHitFront &&
				IsActivityPure(EActivity::IS_BLOCKING_PRESSED) &&
				HitData.bCanBeBlocked)
			{
				bIsBlocked = true;
			}

			// Take Damage
			StatsManager->TakeDamage(HitData.Damage, !bIsBlocked);
			if (IsAlive() && bIsBlocked)
			{
				CLOG_FUNC_TEXT(L"Blocked");
				// 미구현
				// 사운드 재생
				Block();

				if (ExtendedStamina->GetCurrentValue() > 0.0f)
				{
					if (HitData.bCanReceiveImpact)
					{
						UCEffectsComponent* CauserEffect = Cast<ACCombatCharacter>(HitData.DamageCauser)->GetEffectsComponent();
						if (CauserEffect != nullptr)
						{
							CauserEffect->ApplyEffect(EEffectType::IMPACT, EApplyEffectMethod::REPLACE, this);
						}
					}
				}

				OutResultType = EAttackResult::BLOCKED;
				return false;
			}

			OutResultType = EAttackResult::SUCCESS;
			return true;
		}
	}

	OutResultType = EAttackResult::FAILED;
	return false;
}

void ACCombatCharacter::ToggleCombat()
{
	const bool bIsStateIdle = StateManager->IsStateEqual(EStateType::IDLE);
	if (bIsStateIdle)
	{
		UAnimMontage* Montage = nullptr;
		if (Equipment->IsInCombat())
		{
			// Combat -> Idle, Undraw
			StateManager->SetState(EStateType::INTERACTING);
			Montage = MontageManager->GetMontageforAction(EMontageAction::UNDRAW_WEAPON);
		}
		else
		{
			// Idle -> Combat, Draw
			StateManager->SetState(EStateType::INTERACTING);
			Montage = MontageManager->GetMontageforAction(EMontageAction::DRAW_WEAPON);
		}

		if (Montage != nullptr)
		{
			GetWorldTimerManager().ClearTimer(ResetStateTimerHandle);
			float AnimMontagePlayTime = PlayAnimMontage(Montage);
			GetWorldTimerManager().SetTimer(ResetStateTimerHandle, StateManager, &UCStateManagerComponent::TimerResetIdleState, AnimMontagePlayTime * 0.8f);
		}
		else
		{
			// have no motion
			StateManager->ResetState(0.0f);
			const bool bIsCombatTypeNone = Equipment->IsCombatTypeEqual(ECombatType::NONE);
			if (bIsCombatTypeNone)
			{
				// have no weapon
				return;
			}
			else
			{
				Equipment->ToggleCombat();
			}
		}
	}
}

/** Used the AI Behavior Tree */
void ACCombatCharacter::ToggleCombat(float& OutMontagePlayTime)
{
	const bool bIsStateIdle = StateManager->IsStateEqual(EStateType::IDLE);
	if (bIsStateIdle)
	{
		UAnimMontage* Montage = nullptr;
		if (Equipment->IsInCombat())
		{
			// Combat -> Idle - Undraw
			StateManager->SetState(EStateType::INTERACTING);
			Montage = MontageManager->GetMontageforAction(EMontageAction::UNDRAW_WEAPON);
		}
		else
		{
			// Idle -> Combat - Draw
			StateManager->SetState(EStateType::INTERACTING);
			Montage = MontageManager->GetMontageforAction(EMontageAction::DRAW_WEAPON);
		}

		if (Montage != nullptr)
		{
			GetWorldTimerManager().ClearTimer(ResetStateTimerHandle);
			OutMontagePlayTime = PlayAnimMontage(Montage);
			GetWorldTimerManager().SetTimer(ResetStateTimerHandle, StateManager, &UCStateManagerComponent::TimerResetIdleState, OutMontagePlayTime * 0.8f);
		}
		else
		{
			// have no motion
			StateManager->ResetState(0.0f);
			const bool bIsCombatTypeNone = Equipment->IsCombatTypeEqual(ECombatType::NONE);
			if (bIsCombatTypeNone)
			{
				// have no weapon
				return;
			}
			else
			{
				Equipment->ToggleCombat();
			}
		}
	}
}

void ACCombatCharacter::RollAction()
{
	CLOG_ERROR_FUNC_TEXT(L"Should be override RollAction Function");
}

float ACCombatCharacter::MeleeAttackAction(EMeleeAttackType AttackType)
{
	CLOG_ERROR_FUNC_TEXT(L"Should be override MeleeAttackAction Function");
	return 0.0f;
}

void ACCombatCharacter::WeaponSwitchAction(EWeaponSwitchType SwitchType)
{
	CLOG_ERROR_FUNC_TEXT(L"Should be override WeaponSwitchAction Function");
}

EMontageAction ACCombatCharacter::ConvertMeleeAttackTypeToAction(EMeleeAttackType AttackType)
{
	switch (AttackType)
	{
		case EMeleeAttackType::NONE:
		{
			return EMontageAction::NONE;
		}
		break;
		case EMeleeAttackType::LIGHT:
		{
			return EMontageAction::LIGHT_ATTACK;
		}
		break;
		case EMeleeAttackType::HEAVY:
		{
			return EMontageAction::HEAVY_ATTACK;
		}
		break;
		case EMeleeAttackType::SPECIAL:
		{
			return EMontageAction::SPECIAL_ATTACK;
		}
		break;
		case EMeleeAttackType::THRUST:
		{
			return EMontageAction::THRUST_ATTACK;
		}
		break;
		case EMeleeAttackType::FALLING:
		{
			return EMontageAction::FALLING_ATTACK;
		}
		break;
		default:
		{
			return EMontageAction::NONE;
		}
		break;
	}
}

void ACCombatCharacter::ConvertHitResultToHitData(const FHitResult& HitResult, FHitData& OutHitData)
{
	float Damage = StatsManager->GetDamage();
	OutHitData.Damage = ScaleMeleeDamageByType(Damage);
	OutHitData.DamageCauser = Cast<AActor>(this);

	// 공격 방향 얻기
	FVector AttackActorLocation = GetActorLocation(); // 공격하는 액터 위치
	FVector HitActorLocation = HitResult.Actor->GetActorLocation(); // 공격 받는 액터 위치
	FVector Direction = HitActorLocation - AttackActorLocation;
	Direction.Normalize();
	OutHitData.HitFromDirection = Direction;
	if (MeleeAttackType == EMeleeAttackType::LIGHT)
	{
		OutHitData.bCanBeParried = true;
	}
}

const float ACCombatCharacter::ScaleMeleeDamageByType(const float Damage) const
{
	switch (MeleeAttackType)
	{
		case EMeleeAttackType::NONE:
		{
			return Damage * 1.0f;
		}
		break;
		case EMeleeAttackType::LIGHT:
		{
			return Damage * 1.0f;
		}
		break;
		case EMeleeAttackType::HEAVY:
		{
			return Damage * 1.5f;
		}
		break;
		case EMeleeAttackType::SPECIAL:
		{
			return Damage * 1.5f;
		}
		break;
		case EMeleeAttackType::THRUST:
		{
			return Damage * 1.0f;
		}
		break;
		case EMeleeAttackType::FALLING:
		{
			return Damage * 1.0f;
		}
		break;
		default:
		{
			return Damage;
		}
		break;
	}
}

const float ACCombatCharacter::ScaleMeleeAttackStaminaCostByType(float Cost, EMeleeAttackType AttackType)
{
	float Result = Cost;

	switch (AttackType)
	{
		case EMeleeAttackType::HEAVY:
		{
			Result *= 1.75;
		}
			break;
		case EMeleeAttackType::SPECIAL:
		{
			Result *= 1.75;
		}
			break;
		case EMeleeAttackType::THRUST:
		{
			Result *= 1.75;
		}
			break;
		case EMeleeAttackType::FALLING:
		{
			Result *= 0.75;
		}
			break;
	}

	return Result;
}

void ACCombatCharacter::Death()
{
	StateManager->SetState(EStateType::DEAD);

	HandleMeshOnDeath();

	// Disable Targeting (Camera Lock)
	// 미구현

	// DeActivate Collision Handler
	CollisionHandler->DeActivateCollision();

	// Remove Widget (HUD)
	// 미구현
}

void ACCombatCharacter::HandleMeshOnDeath()
{
	TArray<AActor*> AttachedActors;
	GetAttachedActors(AttachedActors, true);

	// Character mesh make ragdoll
	GetMesh()->SetCollisionProfileName("Ragdoll");
	GetMesh()->SetSimulatePhysics(true);

	// Simulate physics on weapon if character is in combat
	if (Equipment->IsInCombat())
	{
		ACItemWeapon* Weapon = Equipment->GetWeapon();
		CLOG_ERROR_CHECK_RETURN(Weapon);

		Weapon->SimulatePhysics();
		if (Weapon->GetIsTwinWeapon())
		{
			ACItemWeapon* TwinWeapon = Weapon->GetLinkedWeapon().Get();
			CLOG_ERROR_CHECK_RETURN(TwinWeapon);

			TwinWeapon->SimulatePhysics();
		}
	}

	// Dissolve all attached actors and character mesh
	CLOG_FUNC;
	for (int32 Index = 0; Index < AttachedActors.Num(); Index++)
	{
		AActor* AttachedActor = AttachedActors[Index];
		if (AttachedActor != nullptr)
		{

		}
	}

	Dissolve->StartDissolve(GetMesh());
	//GetWorldTimerManager().SetTimer(DissolveDelayHandle, FTimerDelegate::CreateLambda([&]()
	//{
	//	

	//}), DissolveDelayTime, false);
}

void ACCombatCharacter::Block()
{
	UAnimMontage* BlockMontage = GetBlockMontage();
	if (BlockMontage != nullptr)
	{
		PlayAnimMontage(BlockMontage);
		ResetMeleeAttackCount();
	}
}

void ACCombatCharacter::Stunned()
{
	/** Hit is stun, same motion*/
	UAnimMontage* Montage = GetStunMontage(ReceiveHitDirection);
	if (Montage != nullptr)
	{
		CLOG_FUNC;
		float MontagePlayDuration = PlayAnimMontage(Montage);
		Effects->AdjustEffectTime(EEffectType::STUN, MontagePlayDuration * ResetEffectDurationPercent);
	}
	else
	{
		StateManager->ResetState();
	}
}

void ACCombatCharacter::BackAttackDown()
{
	UAnimMontage* Montage = MontageManager->GetMontageforAction(EMontageAction::BACK_ATTACK_DOWN);
	if (Montage != nullptr)
	{
		CLOG_FUNC;
		float MontagePlayDuration = PlayAnimMontage(Montage);
		Effects->AdjustEffectTime(EEffectType::BACK_ATTACK_DOWN, MontagePlayDuration * ResetEffectDurationPercent);
	}
	else
	{
		StateManager->ResetState();
	}
}

void ACCombatCharacter::Impact()
{
	UAnimMontage* Montage = MontageManager->GetMontageforAction(EMontageAction::IMPACT);
	if (Montage != nullptr)
	{
		CLOG_FUNC;
		float MontagePlayDuration = PlayAnimMontage(Montage);
		Effects->AdjustEffectTime(EEffectType::IMPACT, MontagePlayDuration * ResetEffectDurationPercent);
	}
	else
	{
		StateManager->ResetState();
	}
}

void ACCombatCharacter::Parried()
{
	UAnimMontage* Montage = MontageManager->GetMontageforAction(EMontageAction::PARRIED);
	if (Montage != nullptr)
	{
		CLOG_FUNC;
		float MontagePlayDuration = PlayAnimMontage(Montage);
		Effects->AdjustEffectTime(EEffectType::PARRIED, MontagePlayDuration * ResetEffectDurationPercent);
	}
	else
	{
		StateManager->ResetState();
	}
}

EDirection ACCombatCharacter::GetHitDirection(FVector HitFromDirection, AActor* AttackedActor)
{
	if (AttackedActor != nullptr)
	{
		// 1. 전후 방향 확인
		FVector actorForwardVector = AttackedActor->GetActorForwardVector();
		/// Z축은(높이) 무시한다.
		actorForwardVector.Z = 0.0f;
		HitFromDirection.Z = 0.0f;
		actorForwardVector.Normalize();
		HitFromDirection.Normalize();
		float dotProduct = FVector::DotProduct(actorForwardVector, HitFromDirection);

		{
			// Front
			/// 0.25는 cos 그래프를 생각하면 쉽게 이해할 수 있다.
			/// 0.25는 22.5도가 되며, 각 방향을 45도 범위를 가지고 확인한다.
			/// 적의 위치 방향을 반환한다.
			if (dotProduct >= 0.25f)
			{
				return EDirection::BACK;
			}

			// Back
			if (dotProduct <= -0.25f)
			{
				return EDirection::FRONT;
			}
		}

		// 2. 좌우 방향 확인
		FVector actorRightDirection = AttackedActor->GetActorRightVector();
		/// Z축은(높이) 무시한다.
		actorForwardVector.Z = 0.0f;
		HitFromDirection.Z = 0.0f;
		actorForwardVector.Normalize();
		HitFromDirection.Normalize();
		dotProduct = FVector::DotProduct(actorForwardVector, HitFromDirection);

		{
			if (dotProduct >= 0.0f)
			{
				return EDirection::RIGHT;
			}
			else
			{
				return EDirection::LEFT;
			}
		}
	}

	return EDirection::FRONT;
};

const UDataTable* ACCombatCharacter::GetMontages(EMontageAction SelectMontage) const
{
	return Equipment->GetWeaponMontagesDataTable();
}

UAnimMontage* ACCombatCharacter::GetRollMontage()
{
	UAnimMontage* Result = nullptr;
	/**
	 * 캐릭터가 움직이지 않고 있다면, 회피 동작으로 뒤 구르기로 고정한다. 
	 * 하지만, 뒤 구르기 동작이 없다면, 앞 구르기로 대체한다.
	 */
	if (HasMovementInput() == false)
	{
		Result = MontageManager->GetMontageforAction(EMontageAction::ROLL_BACKWARD);
	}
	else
	{
		Result = MontageManager->GetMontageforAction(EMontageAction::ROLL_FORWARD);
	}

	CLOG_ERROR_CHECK(Result);
	return Result;
}

UAnimMontage* ACCombatCharacter::GetStunMontage(EDirection Direction)
{
	EMontageAction Action = EMontageAction::NONE;
	switch (Direction)
	{
		case EDirection::FRONT:
		{
			Action = EMontageAction::HIT_FRONT;
		}
		break;
		case EDirection::BACK:
		{
			Action = EMontageAction::HIT_BACK;
		}
		break;
		case EDirection::RIGHT:
		{
			Action = EMontageAction::HIT_RIGHT;
		}
		break;
		case EDirection::LEFT:
		{
			Action = EMontageAction::HIT_LEFT;
		}
		break;
	}

	UAnimMontage* HitMontage = MontageManager->GetMontageforAction(Action);
	if (HitMontage == nullptr && (Action != EMontageAction::HIT_FRONT))
	{
		HitMontage = MontageManager->GetMontageforAction(EMontageAction::HIT_FRONT);
	}

	return HitMontage;
}

UAnimMontage* ACCombatCharacter::GetBlockMontage()
{
	return MontageManager->GetMontageforAction(EMontageAction::BLOCK);
}

UAnimMontage* ACCombatCharacter::GetMeleeAttackMontage(EMeleeAttackType AttackType)
{
	UAnimMontage* Result = nullptr;
 	EMontageAction Action = ConvertMeleeAttackTypeToAction(AttackType);
	int8 LastIndex = MontageManager->GetMontageActionLastIndex(Action);
	if (MeleeAttackCount > LastIndex)
	{
		Result = MontageManager->GetMontageforAction(Action, LastIndex);
	}
	else
	{
		Result = MontageManager->GetMontageforAction(Action, MeleeAttackCount);
	}

	MeleeAttackCount++;
	if (MeleeAttackCount > LastIndex)
	{
		MeleeAttackCount = 0;
	}

	return Result;
}

UCExtendedStatComponent* ACCombatCharacter::GetExtendedStatComponent(EStatsType Type) const
{
	switch (Type)
	{
		case EStatsType::HEALTH:
		{
			return ExtendedHealth;
		}
			break;
		case EStatsType::STAMINA:
		{
			return ExtendedStamina;
		}
			break;
		case EStatsType::MANA:
		{
			return ExtendedMana;
		}
			break;
		default:
		{
			return nullptr;
		}
			break;
	}
}

void ACCombatCharacter::SetReceiveHitDirection(FVector HitFromDirection)
{
	ReceiveHitDirection = GetHitDirection(HitFromDirection, this);
}

bool ACCombatCharacter::IsAlive() const
{
	if (IsStateSame(EStateType::DEAD))
	{
		return false;
	}

	return true;
}

const bool ACCombatCharacter::IsFalling() const
{
	return GetCharacterMovement()->IsFalling();
}

const bool ACCombatCharacter::CanEffectBeApplied(EEffectType Type, AActor* Applier)
{
	if (CanBeAttacked())
	{
		switch (Type)
		{
			case EEffectType::STUN:
			{
				if (CanBeStunned() && CanBeInterrupted())
				{
					return true;
				}
			}
				break;
			case EEffectType::KNOCK_DOWN:
				break;
			case EEffectType::BURNING:
			{
				return true;
			}
				break;
			case EEffectType::BACK_ATTACK_DOWN:
			{
				if (CanBeStunned() && CanBeInterrupted())
				{
					return true;
				}
			}
				break;
			case EEffectType::IMPACT:
			{
				return CanBeInterrupted();
			}
				break;
			case EEffectType::PARRIED:
			{
				return CanBeInterrupted();
			}
				break;
		}
	}

	return false;
}

const bool ACCombatCharacter::CanBeAttacked() const
{
	if (IsAlive())
	{
		// 반환형에 Not 게이트(!)를 붙였다.
		return !IsActivityPure(EActivity::IS_IMMORTAL);
	}

	return false;
}

const bool ACCombatCharacter::CanBeStunned() const
{
	if (!IsFalling() && Effects->IsEffectApplied(EEffectType::BACK_ATTACK_DOWN))
	{
		return true;
	}

	return false;
}
const bool ACCombatCharacter::CanMeleeAttack() const
{
	if (IsStateSame(EStateType::IDLE) &&
		Equipment->IsInCombat() &&
		Equipment->IsCombatTypeEqual(ECombatType::MELEE) &&
		IsEnoughStamina(1.0f))
	{
		return true;
	}

	return false;
}

const bool ACCombatCharacter::CanRoll() const
{
	if (IsIdleandNotFall() && IsEnoughStamina(1.0f))
	{
		return true;
	}
	return false;
}

const bool ACCombatCharacter::IsActivityPure(const EActivity Activity) const
{
	return StateManager->GetActivityValue(Activity);
}

const bool ACCombatCharacter::IsIdleandNotFall() const
{
	if (IsStateSame(EStateType::IDLE))
	{
		if (GetCharacterMovement()->IsFalling() == false)
		{
			return true;
		}
	}
	return false;
}

const bool ACCombatCharacter::IsStateSame(const EStateType StateToCompare) const
{
	EStateType CurrentState = StateManager->GetState();

	if (CurrentState == StateToCompare)
	{
		return true;
	}
	return false;
}

const bool ACCombatCharacter::IsEnoughStamina(const float Value) const
{
	if (ExtendedStamina->GetCurrentValue() >= Value)
	{
		return true;
	}
	
	return false;
}

const bool ACCombatCharacter::HasMovementInput() const
{
	FVector LastInputVector = GetCharacterMovement()->GetLastInputVector();
	return UKismetMathLibrary::NotEqual_VectorVector(LastInputVector, FVector(0, 0, 0), 0.0001f);
}

void ACCombatCharacter::OnHealthValueChanged(float NewValue, float MaxValue)
{
	if (NewValue <= 0.0f)
	{
		Death();
	}
}

void ACCombatCharacter::OnHit(FHitResult HitResult)
{
	// FHitResult -> FHitData Convert
	FHitData HitData;
	ConvertHitResultToHitData(HitResult, HitData);

	// 피격된 액터에게 데미지 전달 처리
	EAttackResult AttackResult;
	ACCombatCharacter* HitActor = Cast<ACCombatCharacter>(HitResult.Actor);
	if (HitActor->TakeDamage(HitData, AttackResult))
	{
		// 미구현
		// 사운드 재생
		HitActor->GetEffectsComponent()->ApplyEffect(EEffectType::STUN, EApplyEffectMethod::REPLACE, this, 2.0f);
	}
}

void ACCombatCharacter::OnCollisionActivated()
{
	CollisionHandler->ResetMeshCollision();
	ACItemWeapon* Weapon = Equipment->GetWeapon();
	CLOG_ERROR_CHECK_RETURN(Weapon);

	const FWeaponSocket* WeaponSocket = Weapon->GetWeaponSocket();
	CLOG_ERROR_CHECK_RETURN(WeaponSocket);

	if (Weapon->GetWeaponType() == EWeaponType::UNARMED)
	{
		CollisionHandler->AddMeshCollision(GetMesh(), WeaponSocket);
	}
	else
	{
		CollisionHandler->AddMeshCollision(Weapon->GetMesh(), WeaponSocket);

		// Weapon has the SubMesh?
		if (Weapon->GetMesh(true) != nullptr)
		{
			CollisionHandler->AddMeshCollision(Weapon->GetMesh(true), WeaponSocket);
		}

		// Weapon Is TwinWeapon?
		if (Weapon->GetIsTwinWeapon())
		{
			Weapon = Weapon->GetLinkedWeapon().Get();
			CollisionHandler->AddMeshCollision(Weapon->GetMesh(), WeaponSocket);

			// Weapon has the SubMesh?
			if (Weapon->GetMesh(true) != nullptr)
			{
				CollisionHandler->AddMeshCollision(Weapon->GetMesh(true), WeaponSocket);
			}
		}
	}
}

void ACCombatCharacter::OnWeaponChanged(ACItemWeapon* Weapon)
{
	// Weapon Damage Reset
	FStat Damage = Weapon->GetDamageStat();
	StatsManager->SetStatBaseValue(Damage.Type, Damage.BaseValue);
}

void ACCombatCharacter::OnEffectApplied(EEffectType Type)
{
	switch (Type)
	{
		case EEffectType::STUN:
		{
			Stunned();
		}
			break;
		case EEffectType::BACK_ATTACK_DOWN:
		{
			BackAttackDown();
		}
			break;
		case EEffectType::IMPACT:
		{
			Impact();
		}
			break;
		case EEffectType::PARRIED:
		{
			Parried();
		}
			break;
		default:
		{
			return;
		}
			break;
	}

	StateManager->SetState(EStateType::DISABLED);
}

void ACCombatCharacter::OnEffectRemoved(EEffectType Type)
{
	bool bIsResetType = false;
	switch (Type)
	{
		case EEffectType::STUN:
		{
			bIsResetType = true;
		}
		break;
		case EEffectType::BACK_ATTACK_DOWN:
		{
			bIsResetType = true;
		}
		break;
		case EEffectType::IMPACT:
		{
			bIsResetType = true;
		}
		break;
		case EEffectType::PARRIED:
		{
			bIsResetType = true;
		}
		break;
	}

	if ((bIsResetType && Effects->CanBeResetState()) == true)
	{
		StateManager->ResetState();
	}
}