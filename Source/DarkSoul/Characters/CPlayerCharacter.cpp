// Copyright Epic Games, Inc. All Rights Reserved.
#include "CPlayerCharacter.h"
#include "DarkSoul/_Utility/CLog.h"
#include "DarkSoul/Components/CEffectsComponent.h"
#include "DarkSoul/Components/CRotatingComponent.h"
#include "DarkSoul/Components/CTargetingComponent.h"
#include "DarkSoul/Components/CEquipmentComponent.h"
#include "DarkSoul/Components/CInputBufferComponent.h"
#include "DarkSoul/Components/CExtendedStatComponent.h"
#include "DarkSoul/Components/CStatsManagerComponent.h"
#include "DarkSoul/Components/CStateManagerComponent.h"
#include "DarkSoul/Components/CMovementSpeedComponent.h"
#include "DarkSoul/Components/CMontageManagerComponent.h"
#include "DarkSoul/Components/CExtendedCameraComponent.h"

/// Unreal Engine
#include "TimerManager.h"
#include "Engine/World.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

ACPlayerCharacter::ACPlayerCharacter()
{
	// Find Assets
	{
		FString Path = L"CurveFloat'/Game/DarkSoul/Animations/CF_BlockingCurveFloat.CF_BlockingCurveFloat'";
		static ConstructorHelpers::FObjectFinder<UCurveFloat> BlockCurve(*Path);
		if (BlockCurve.Succeeded())
		{
			BlockingCurveFloat = BlockCurve.Object;
		}
	}

	// Create Component
	{
		FollowingCamera = CreateDefaultSubobject<UCameraComponent>(FName("FollowingCamera"));
		CameraSpringArm = CreateDefaultSubobject<USpringArmComponent>(FName("CameraSpringArm"));
		ExtendedCamera = CreateDefaultSubobject<UCExtendedCameraComponent>(FName("ExtendedCamera"));
		InputBuffer = CreateDefaultSubobject<UCInputBufferComponent>(FName("InputBuffer"));
		Targeting = CreateDefaultSubobject<UCTargetingComponent>(FName("Targeting"));

		BlockingCurveFloat = CreateDefaultSubobject<UCurveFloat>(FName("BlockingCurveFloat"));
	}

	// Set Component Attach
	{
		/// 카메라 스프링 암에 유저가 보게 될 카메라를 붙였다.
		/// (상위) 캡슐 -> 스프링 암 -> 카메라 (하위)
		CameraSpringArm->AttachToComponent(GetCapsule(), FAttachmentTransformRules::KeepRelativeTransform);
		FollowingCamera->AttachToComponent(CameraSpringArm, FAttachmentTransformRules::KeepRelativeTransform);
	}

	// Set Component Defaults
	{
		/// Set Camera
		CameraSpringArm->TargetArmLength = 400.0f;
		CameraSpringArm->SetRelativeLocation(FVector(0.0f, 0.0f, 90.0f));
		CameraSpringArm->SetRelativeRotation(FRotator::ZeroRotator);
		CameraSpringArm->bUsePawnControlRotation = true;
		CameraSpringArm->bDoCollisionTest = true;
		CameraSpringArm->bEnableCameraLag = true;
		CameraSpringArm->bEnableCameraRotationLag = true;
	}
}

void ACPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Block Motion Timeline
	{
		FOnTimelineFloat BlockingCurveFloatCallback;
		FOnTimelineEventStatic BlockingTimelineFinishedCallback;

		BlockingCurveFloatCallback.BindUFunction(this, FName("OnTimelineFloat"));
		BlockingTimelineFinishedCallback.BindUFunction(this, FName("OnBlockingTimelineFinished"));

		BlockingTimeline.AddInterpFloat(BlockingCurveFloat, BlockingCurveFloatCallback);

		BlockingTimeline.SetTimelineFinishedFunc(BlockingTimelineFinishedCallback);
		BlockingTimeline.SetTimelineLength(BlockingTimelineLength);
		BlockingTimeline.SetPlayRate(BlockPlayRate);
	}

	// Delegate Bind and Duplicate clear
	{
		Rotating->OnRotatingEnd.AddUObject(this, &ACPlayerCharacter::OnRotatingEnd);

		InputBuffer->OnInputBufferClose.AddUObject(this, &ACPlayerCharacter::OnInputBufferClose);
		InputBuffer->OnInputBufferConsumed.AddUObject(this, &ACPlayerCharacter::OnInputBufferConsumed);

		ExtendedStamina->OnValueChanged.AddUObject(this, &ACPlayerCharacter::OnStaminaValueChanged);

		StateManager->OnStateChanged.AddUObject(this, &ACPlayerCharacter::OnStateChanged);
		StateManager->OnActivityChanged.AddUObject(this, &ACPlayerCharacter::OnActivityChanged);

		/** Already Bind ACCombatCharacter BeginPlay */
		Effects->OnEffectApplied.Clear();
		Effects->OnEffectApplied.AddUObject(this, &ACPlayerCharacter::OnEffectApplied);
		Effects->OnEffectRemoved.Clear();
		Effects->OnEffectRemoved.AddUObject(this, &ACPlayerCharacter::OnEffectRemoved);
	}
}

void ACPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	BlockingTimeline.TickTimeline(DeltaTime);
}

void ACPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Character Moving
	PlayerInputComponent->BindAxis(L"MoveForward", this, &ACPlayerCharacter::MoveForwardOrBackward);
	PlayerInputComponent->BindAxis(L"MoveRight", this, &ACPlayerCharacter::MoveRightOrLeft);
	PlayerInputComponent->BindAction(L"Run/Rolling", IE_Repeat, this, &ACPlayerCharacter::Run);
	PlayerInputComponent->BindAction(L"Run/Rolling", IE_Released, this, &ACPlayerCharacter::Roll);

	// Camera Control
	PlayerInputComponent->BindAxis(L"HorizontalLook", this, &ACPlayerCharacter::HorizontalLook);
	PlayerInputComponent->BindAxis(L"VerticalLook", this, &ACPlayerCharacter::VerticalLook);

	// Interaction
	PlayerInputComponent->BindAction(L"Interaction", IE_Pressed, this, &ACPlayerCharacter::Interaction);

	// HUD Hide
	PlayerInputComponent->BindAction(L"HUDHide", IE_Pressed, this, &ACPlayerCharacter::HUDHide);

	// Attack
	PlayerInputComponent->BindAction(L"LightAttack", IE_Pressed, this, &ACPlayerCharacter::LightAttack);
	PlayerInputComponent->BindAction(L"HeavyAttack", IE_Pressed, this, &ACPlayerCharacter::HeavyAttack);

	// Defense
	PlayerInputComponent->BindAction(L"Block", IE_Pressed, this, &ACPlayerCharacter::StartBlocking);
	PlayerInputComponent->BindAction(L"Block", IE_Released, this, &ACPlayerCharacter::StopBlocking);

	// Weapon Switch
	PlayerInputComponent->BindAction(L"ToggleDrawWeapon", IE_Pressed, this, &ACPlayerCharacter::ToggleDrawWeapon);
	PlayerInputComponent->BindAction(L"WeaponSwitchLeft", IE_Pressed, this, &ACPlayerCharacter::WeaponSwitchLeft);
	PlayerInputComponent->BindAction(L"WeaponSwitchRight", IE_Pressed, this, &ACPlayerCharacter::WeaponSwitchRight);

	// Targeting
	PlayerInputComponent->BindAction(L"ToggleLockOn", IE_Pressed, this, &ACPlayerCharacter::ToggleLockOn);
	PlayerInputComponent->BindAxis(L"ChangeToUpAndDownTarget", this, &ACPlayerCharacter::ChangeToUpAndDownTarget);
	PlayerInputComponent->BindAxis(L"ChangeToRightAndLeftTarget", this, &ACPlayerCharacter::ChangeToRightAndLeftTarget);
}

bool ACPlayerCharacter::TakeDamage(const FHitData& HitData, EAttackResult& OutResultType)
{
	return Super::TakeDamage(HitData, OutResultType);
}

void ACPlayerCharacter::OnInteraction(FInteractionMassage InteractionMassage)
{
	// Visible Interaction HUD
	if (OnInteractionMassage.IsBound())
	{
		OnInteractionMassage.Execute(InteractionMassage);
	}
	else
	{
		CLOG_FUNC_TEXT(L"On Intertaction Massage is not Bind");
	}
}

void ACPlayerCharacter::OffInteraction()
{
	// InVisible Interaction HUD
	if (OffInteractionMassage.IsBound())
	{
		OffInteractionMassage.Execute();
	}
	else
	{
		CLOG_FUNC_TEXT(L"Off Intertaction Massage is not Bind");
	}
}

void ACPlayerCharacter::RollAction()
{
	if (CanRoll())
	{
		StateManager->SetState(EStateType::ROLLING);
		UAnimMontage* RollMontage = GetRollMontage();
		if (RollMontage == nullptr)
		{
			StateManager->ResetState(0.0f);
		}
		else
		{
			// 마지막 방향 입력값에 맞춰 캐릭터 회전
			SetActorRelativeRotation(GetDesiredRotation());
			PlayAnimMontage(RollMontage);
			// Remove Stamina
			ExtendedStamina->ModifyStat(RollStaminaCost * -1.0f, true);
		}
	}
}

float ACPlayerCharacter::AttackAction(EAttackType NewAttackType)
{
	float MontagePlayTime = 0.0f;

	if (CanMeleeAttack())
	{
		if (GetCharacterMovement()->IsFalling())
		{
			AttackType = EAttackType::FALLING;
			ResetAttackCount();
		}
		else
		{
			AttackType = NewAttackType;
		}

		StateManager->SetState(EStateType::ATTACKING);
		GetWorldTimerManager().ClearTimer(ResetAttackCounterTimerHandle);
		UAnimMontage* AttackMontage = GetMeleeAttackMontage(AttackType);
		if (AttackMontage == nullptr)
		{
			StateManager->ResetState(0.0f);
			ResetAttackCount();
		}

		// Reset Attack Counter
		MontagePlayTime = PlayAnimMontage(AttackMontage, StatsManager->GetStatValue(EStatsType::ATTACK_SPEED, true));
		GetWorldTimerManager().SetTimer(ResetAttackCounterTimerHandle, this, &ACPlayerCharacter::ResetAttackCount, MontagePlayTime * 0.8f);

		// Remove Stamina
		float StaminaCost = StatsManager->GetStatValue(EStatsType::MELEE_ATTACK_STAMINA_COST, true);
		StaminaCost = ScaleAttackStaminaCostByType(StaminaCost, AttackType);
		ExtendedStamina->ModifyStat(StaminaCost * -1.0f, true);
	}

	return MontagePlayTime;
}

void ACPlayerCharacter::WeaponSwitchAction(EWeaponSwitchType SwitchType)
{
	if (Equipment->IsInCombat())
	{
		float AnimMontagePlayTime = 0.0f;
		StateManager->SetState(EStateType::INTERACTING);

		UAnimMontage* Montage = MontageManager->GetMontageforAction(EMontageAction::UNDRAW_WEAPON);
		if (Montage != nullptr)
		{
			// Weapon Undraw
			GetWorldTimerManager().ClearTimer(WeaponSwitchMotionTimerHandle);
			AnimMontagePlayTime = PlayAnimMontage(Montage);
			GetWorldTimerManager().SetTimer(WeaponSwitchMotionTimerHandle, this, &ACPlayerCharacter::ToggleCombat, AnimMontagePlayTime * 0.8);
			Equipment->SwitchWeapon(SwitchType);
		}
		else
		{
			// is not have weapon undraw motion, Play Weapon Draw
			// Switch weapon draw and prev weapon is set invisible
			Equipment->SwitchWeapon(SwitchType);
			Montage = MontageManager->GetMontageforAction(EMontageAction::DRAW_WEAPON);
			if (Montage != nullptr)
			{
				PlayAnimMontage(Montage);
			}
		}
	}
	else
	{
		// Switch Weapon, Change the item slot image and selected weapon
		Equipment->SwitchWeapon(SwitchType);
	}
}

void ACPlayerCharacter::OnCinematic()
{
	bIsKeyBlock = true;
	ToggleHUD.Broadcast();
}

void ACPlayerCharacter::EndCinematic()
{
	bIsKeyBlock = false;
	ToggleHUD.Broadcast();
}

void ACPlayerCharacter::OnBeginLoading()
{
	bIsInLoading = true;
	OnLoading.Broadcast();
}

void ACPlayerCharacter::UpdateBlocking()
{
	CLOG_CHECK_RETURN_TEXT(BlockingCurveFloat, L"Blocking Curve Float is nullptr, Check the Blueprint");

	if (IsActivityPure(EActivity::IS_BLOCKING_PRESSED) &&
		IsIdleandNotFall() &&
		Equipment->CanBlock() &&
		IsEnoughStamina(5.0f))
	{
		BlockingTimeline.SetPlayRate(BlockPlayRate);
		BlockingTimeline.PlayFromStart();
	}
	else
	{
		BlockingTimeline.Reverse();
	}
}

FRotator ACPlayerCharacter::GetDesiredRotation()
{
	FRotator result = GetActorRotation();
	// Magic Casting 미구현

	if (Targeting->IsLockOnActivity())
	{
		// 백스탭 회피를 하면 타겟팅된 상대방을 바라보며 뒤로 움직이게 한다.
		if (IsStateSame(EStateType::BACK_STABBING))
		{
			if (Targeting->GetTarget())
			{
				// 타겟 방향으로 회전 시키기 위해
				// 현재 캐릭터의 Rotation 값에 Yaw 회전값만 대입한다.
				result.Yaw = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), Targeting->GetTarget()->GetActorLocation()).Yaw;
			}
		}
		else
		{
			// 앞구르기 회피를 할 때 방향을 마지막 입력값으로 정해준다.
			if (HasMovementInput())
			{
				if (IsStateSame(EStateType::ROLLING))
				{
					if (MontageManager->GetLastRequestedAction() == EMontageAction::ROLL_FORWARD)
					{
						result = UKismetMathLibrary::Conv_VectorToRotator(GetLastMovementInputVector());
					}
					else
					{
						result.Yaw = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), Targeting->GetTarget()->GetActorLocation()).Yaw;
					}
				}
				else
				{
					result.Yaw = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), Targeting->GetTarget()->GetActorLocation()).Yaw;
				}
			}
			else
			{
				if (IsStateSame(EStateType::ROLLING))
				{
					if ((MontageManager->GetLastRequestedAction() == EMontageAction::ROLL_FORWARD) == false)
					{
						result.Yaw = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), Targeting->GetTarget()->GetActorLocation()).Yaw;
					}
				}
				else
				{
					result.Yaw = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), Targeting->GetTarget()->GetActorLocation()).Yaw;
				}
			}
		}
	}
	else
	{
		if (HasMovementInput())
		{
			result = UKismetMathLibrary::Conv_VectorToRotator(GetLastMovementInputVector());
		}
	}
	return result;
}

void ACPlayerCharacter::MoveForwardOrBackward(float AxisValue)
{
	if (bIsKeyBlock)
	{
		return;
	}

	if (IsAlive() == false) return;

	FRotator rotation = GetControlRotation();
	rotation = FRotator(0.0f, rotation.Yaw, 0.0f);

	AddMovementInput(FQuat(rotation).GetForwardVector(), AxisValue);
}

void ACPlayerCharacter::MoveRightOrLeft(float AxisValue)
{
	if (bIsKeyBlock)
	{
		return;
	}

	if (IsAlive() == false) return;

	FRotator rotation = GetControlRotation();
	rotation = FRotator(0.0f, rotation.Yaw, 0.0f);

	AddMovementInput(FQuat(rotation).GetRightVector(), AxisValue);
}

void ACPlayerCharacter::HorizontalLook(float AxisValue)
{
	if (bIsKeyBlock)
	{
		return;
	}

	float deltaTime = UGameplayStatics::GetWorldDeltaSeconds(this);
	AxisValue = HorizontalLookRate * deltaTime * AxisValue;
	
	AddControllerYawInput(AxisValue);
	//CLog::Print(AxisValue, 1.0f, FColor::Cyan, 3);

	// 임시 함수
	if (FMath::IsNearlyZero(AxisValue, KINDA_SMALL_NUMBER))
	{
		//CLog::Print(L"False", 1.0f, FColor::Cyan, 1);
		ExtendedCamera->CameraKeyInput(false);
	}
	else
	{
		//CLog::Print(L"True", 1.0f, FColor::Cyan, 1);
		ExtendedCamera->CameraKeyInput(true);
	}
}

void ACPlayerCharacter::VerticalLook(float AxisValue)
{
	if (bIsKeyBlock)
	{
		return;
	}

	float deltaTime = UGameplayStatics::GetWorldDeltaSeconds(this);
	AxisValue = VerticalLookRate * deltaTime * AxisValue;

	AddControllerPitchInput(AxisValue);
	//CLog::Print(AxisValue, 1.0f, FColor::Red, 4);

	// 임시 함수
	if (FMath::IsNearlyZero(AxisValue, KINDA_SMALL_NUMBER))
	{
		//CLog::Print(L"False", 1.0f, FColor::Red, 2);
		ExtendedCamera->CameraKeyInput(false);
	}
	else
	{
		//CLog::Print(L"True", 1.0f, FColor::Red, 2);
		ExtendedCamera->CameraKeyInput(true);
	}
}

void ACPlayerCharacter::Interaction()
{
	if (bIsKeyBlock)
	{
		return;
	}

	if (PlayInteraction.IsBound())
	{
		InputBuffer->UpdateKey(EInputBufferKey::INTERACTION);
	}
}

void ACPlayerCharacter::Run()
{
	if (bIsKeyBlock)
	{
		return;
	}

	MovementSpeed->SetMovementState(EMovementState::RUN);
}

void ACPlayerCharacter::Roll()
{
	if (bIsKeyBlock)
	{
		return;
	}

	if (MovementSpeed->GetMovementState() == EMovementState::RUN)
	{
		MovementSpeed->SetMovementState(EMovementState::JOG);
	}
	else
	{
		InputBuffer->UpdateKey(EInputBufferKey::ROLL);
	}
}

void ACPlayerCharacter::LightAttack()
{
	if (bIsKeyBlock)
	{
		return;
	}

	const bool bisInCombat = Equipment->IsInCombat();
	if (bisInCombat)
	{
		const bool bisCombatTypeMelee = Equipment->IsCombatTypeEqual(ECombatType::MELEE);
		if (bisCombatTypeMelee)
		{
			InputBuffer->UpdateKey(EInputBufferKey::LIGHT_ATTACK);
		}
	}
	else
	{
		InputBuffer->UpdateKey(EInputBufferKey::TOGGLE_COMBAT);
	}
}

void ACPlayerCharacter::HeavyAttack()
{
	if (bIsKeyBlock)
	{
		return;
	}

	const bool bisInCombat = Equipment->IsInCombat();
	if (bisInCombat)
	{
		const bool bisCombatTypeMelee = Equipment->IsCombatTypeEqual(ECombatType::MELEE);
		if (bisCombatTypeMelee)
		{
			InputBuffer->UpdateKey(EInputBufferKey::HEAVY_ATTACK);
		}
	}
	else
	{
		InputBuffer->UpdateKey(EInputBufferKey::TOGGLE_COMBAT);
	}
}

void ACPlayerCharacter::StartBlocking()
{
	if (bIsKeyBlock)
	{
		return;
	}

	StateManager->SetActivity(EActivity::IS_BLOCKING_PRESSED, true);
}

void ACPlayerCharacter::StopBlocking()
{
	if (bIsKeyBlock)
	{
		return;
	}

	StateManager->SetActivity(EActivity::IS_BLOCKING_PRESSED, false);
}

void ACPlayerCharacter::ToggleDrawWeapon()
{
	if (bIsKeyBlock)
	{
		return;
	}

	InputBuffer->UpdateKey(EInputBufferKey::TOGGLE_COMBAT);
}

void ACPlayerCharacter::WeaponSwitchLeft()
{
	if (bIsKeyBlock)
	{
		return;
	}

	if (StateManager->IsStateEqual(EStateType::IDLE))
	{
		InputBuffer->UpdateKey(EInputBufferKey::SWITCH_WEAPON_LEFT);
	}
}

void ACPlayerCharacter::WeaponSwitchRight()
{
	if (bIsKeyBlock)
	{
		return;
	}

	if (StateManager->IsStateEqual(EStateType::IDLE))
	{
		InputBuffer->UpdateKey(EInputBufferKey::SWITCH_WEAPON_RIGHT);
	}
}

void ACPlayerCharacter::ToggleLockOn()
{
	if (bIsKeyBlock)
	{
		return;
	}

	Targeting->ToggleLockOn();

	if (Targeting->IsLockOnActivity())
	{
		Rotating->SetRotationMode(ERotationMode::FACE_CAMERA);
	}
	else
	{
		Rotating->SetRotationMode(ERotationMode::ORIENT_TO_MOVEMENT);
	}
}

void ACPlayerCharacter::ChangeToUpAndDownTarget(float AxisValue)
{
	if (bIsKeyBlock)
	{
		return;
	}

	Targeting->ChangeToUpAndDownTarget(AxisValue);
}

void ACPlayerCharacter::ChangeToRightAndLeftTarget(float AxisValue)
{
	if (bIsKeyBlock)
	{
		return;
	}

	Targeting->ChangeToRightAndLeftTarget(AxisValue);
}

void ACPlayerCharacter::HUDHide()
{
	if (bIsKeyBlock)
	{
		return;
	}

	ToggleHUD.Broadcast();
}

void ACPlayerCharacter::OnInputBufferClose()
{
	const bool bisStateDisable = IsStateSame(EStateType::DISABLED);
	if (bisStateDisable == false)
	{
		StateManager->ResetState(0.0f);
	}
}

void ACPlayerCharacter::OnInputBufferConsumed(EInputBufferKey Key)
{
	if (IsAlive())
	{
		switch (Key)
		{
			case EInputBufferKey::LIGHT_ATTACK:
			{
				AttackAction(EAttackType::LIGHT);
			}
			break;
			case EInputBufferKey::HEAVY_ATTACK:
			{
				AttackAction(EAttackType::HEAVY);
			}
			break;
			case EInputBufferKey::THRUST_ATTACK:
			{
				AttackAction(EAttackType::THRUST);
			}
			break;
			case EInputBufferKey::SPECIAL_ATTACK:
			{
				AttackAction(EAttackType::SPECIAL);
			}
			break;
			case EInputBufferKey::FALLING_ATTACK:
			{
				AttackAction(EAttackType::FALLING);
			}
			break;
			case EInputBufferKey::ROLL:
			{
				RollAction();
			}
			break;
			case EInputBufferKey::JUMP:
				break;
			case EInputBufferKey::PARRY:
				break;
			case EInputBufferKey::TOGGLE_COMBAT:
			{
				ToggleCombat();
			}
			break;
			case EInputBufferKey::SWITCH_WEAPON_LEFT:
			{
				WeaponSwitchAction(EWeaponSwitchType::LEFT);
			}
			break;
			case EInputBufferKey::SWITCH_WEAPON_RIGHT:
			{
				WeaponSwitchAction(EWeaponSwitchType::RIGHT);
			}
			break;
			case EInputBufferKey::ABILITY_ATTACK:
				break;
			case EInputBufferKey::SET_SPELL_ACTIVE:
				break;
			case EInputBufferKey::INTERACTION:
			{
				if (PlayInteraction.IsBound())
				{
					PlayInteraction.Execute();
				}
				else
				{
					CLOG_FUNC_TEXT(L"Interaction is not Bind");
				}
			}
				break;
			default:
				break;
		}
	}
}

void ACPlayerCharacter::OnRotatingEnd()
{
	if (IsStateSame(EStateType::ATTACKING) &&
		Equipment->IsCombatTypeEqual(ECombatType::MELEE) &&
		Targeting->IsLockOnActivity() &&
		Equipment->IsInCombat())
	{
		Rotating->StartRotatingWithTime(0.5f);
	}
}

void ACPlayerCharacter::OnStaminaValueChanged(float NewValue, float MaxValue)
{
	if ((NewValue / MaxValue) <= 0.1f)
	{
		UpdateBlocking();
	}
}

void ACPlayerCharacter::OnStateChanged(EStateType PrevState, EStateType NewState)
{
	UpdateBlocking();
	if (PrevState == EStateType::ATTACKING)
	{
		AttackType = EAttackType::NONE;
	}
}

void ACPlayerCharacter::OnActivityChanged(EActivity Activity, bool bValue)
{
	switch (Activity)
	{
		case EActivity::NONE:
			break;
		case EActivity::IS_BLOCKING_PRESSED:
		{
			UpdateBlocking();
		}
		break;
		case EActivity::IS_AIMING_PRESSED:
			break;
		case EActivity::IS_IMMORTAL:
			break;
		case EActivity::CANT_BE_INTERRUPTED:
			break;
		case EActivity::IS_LOOKING_FORWARD:
			break;
		case EActivity::CAN_PARRY_HIT:
			break;
		case EActivity::IS_ZOOMING:
			break;
		default:
			break;
	}
}

void ACPlayerCharacter::OnEffectApplied(EEffectType Type)
{
	Super::OnEffectApplied(Type);

	InputBuffer->UpdateKey(EInputBufferKey::NONE);
	ResetAttackCount();
	InputBuffer->OpenInputBuffer();
}

void ACPlayerCharacter::OnEffectRemoved(EEffectType Type)
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
		InputBuffer->CloseInputBuffer();
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// UFunction
//////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * 매 프레임마다 호출된다. 실시간으로 변하는 Float Value를 받아 멤버 변수에 저장한다.
 * Tick 마다 호출이 안된다면, Tick 호출이 True인지 확인하고, 바인딩이 잘되었는지 확인해보자.
 */

void ACPlayerCharacter::OnTimelineFloat()
{
	BlockAlpha = BlockingCurveFloat->GetFloatValue(BlockingTimeline.GetPlaybackPosition());
}

void ACPlayerCharacter::OnBlockingTimelineFinished()
{
	if (BlockingTimeline.IsReversing())
	{
		// 스테미나 자동 회복량 기본값 복구
		ExtendedStamina->ChangeRegenPercent(100);
	}
	else
	{
		// 스테미나 자동 회복량 감소
		ExtendedStamina->ChangeRegenPercent(25);
	}
}