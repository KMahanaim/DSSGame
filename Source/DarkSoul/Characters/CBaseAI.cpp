// Copyright Epic Games, Inc. All Rights Reserved.
#include "CBaseAI.h"
#include "DarkSoul/_Utility/CLog.h"
#include "DarkSoul/Widgets/CUW_StatBar.h"
#include "DarkSoul/Controllers/CBaseAIController.h"
#include "DarkSoul/Components/CPatrolComponent.h"
#include "DarkSoul/Components/CEffectsComponent.h"
#include "DarkSoul/Components/CEquipmentComponent.h"
#include "DarkSoul/Components/CStateManagerComponent.h"
#include "DarkSoul/Components/CStatsManagerComponent.h"
#include "DarkSoul/Components/CExtendedStatComponent.h"

// Unreal Engine
#include "TimerManager.h"
#include "Engine/World.h"
#include "AIController.h"
#include "BrainComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Animation/AnimInstance.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/WidgetComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/TextRenderComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AISense_Damage.h"

ACBaseAI::ACBaseAI()
{
	// Create Components
	{
		StatBar = CreateDefaultSubobject<UWidgetComponent>(FName("StatBar"));
		TargetMark = CreateDefaultSubobject<UWidgetComponent>(FName("TargetMark"));
		PatrolPath = CreateDefaultSubobject<UCPatrolComponent>(FName("PatrolPath"));
		BehaviorText = CreateDefaultSubobject<UTextRenderComponent>(FName("BehaviorText"));
	}

	// Set Components Attach
	{
		StatBar->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform);
		TargetMark->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform);
		BehaviorText->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	}

	// Set Components Defaults
	{
		FString Path = L"";

		/// Set Wdiget
		Path = L"WidgetBlueprint'/Game/DarkSoul/Widget/Bp_EnemyStatBar.Bp_EnemyStatBar_C'";
		static ConstructorHelpers::FClassFinder<UUserWidget> StatWidget(*Path);
		if (StatWidget.Succeeded())
		{
			StatBar->SetWidgetClass(StatWidget.Class);
			StatBar->SetDrawSize(FVector2D(150, 50));
			StatBar->SetRelativeLocation(FVector(0, 0, 210));
			StatBar->SetHiddenInGame(true);
		}

		Path = L"WidgetBlueprint'/Game/DarkSoul/Widget/Bp_TargetMark.Bp_TargetMark_C'";
		static ConstructorHelpers::FClassFinder<UUserWidget> TargetWidget(*Path);
		if (TargetWidget.Succeeded())
		{
			TargetMark->SetWidgetClass(TargetWidget.Class);
			TargetMark->SetDrawSize(FVector2D(70, 70));
			TargetMark->SetRelativeLocation(FVector(0, 0, 130));
			TargetMark->SetHiddenInGame(true);
		}

		StatBar->SetWidgetSpace(EWidgetSpace::Screen);
		TargetMark->SetWidgetSpace(EWidgetSpace::Screen);

		BehaviorText->SetText(FText::FromString(L"IDLE"));
		BehaviorText->SetRelativeLocation(FVector(0, 0, 100));
		BehaviorText->SetHorizontalAlignment(EHorizTextAligment::EHTA_Center);
	}

	// Set Controller
	{
		AIControllerClass = ACBaseAIController::StaticClass();
	}
}

void ACBaseAI::BeginPlay()
{
	Super::BeginPlay();
	
	// Controller Spawn
	if (Controller == nullptr)
	{
		SpawnDefaultController();
	}

	// Widget Init
	{
		UCUW_StatBar* CastStatBar = Cast<UCUW_StatBar>(StatBar->GetUserWidgetObject());
		if (CastStatBar != nullptr)
		{
			CastStatBar->Init(ExtendedHealth);
		}
		else
		{
			CLOG_ERROR_FUNC_TEXT(L"What?");
		}

		BehaviorText->SetRelativeLocation(FVector(0, 0, 100));
	}

	// Delegate Bind
	{
		Equipment->OnInCombatChanged.AddUObject(this, &ACBaseAI::OnInCombatChanged);
	}
}

void ACBaseAI::Death()
{
	Super::Death();

	// Stop Montage
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance != nullptr)
	{
		AnimInstance->Montage_Stop(0.2f);
	}
	else
	{
		CLOG_ERROR_FUNC;
	}

	// Stop Movement
	AAIController* AIController = Cast<AAIController>(GetController());
	if (AIController != nullptr)
	{
		AIController->StopMovement();

		// Stop Behavior
		UBrainComponent* Brain = AIController->GetBrainComponent();
		if (Brain != nullptr)
		{
			FString StopReason = L"Death";
			Brain->StopLogic(StopReason);
		}
		else
		{
			CLOG_ERROR_FUNC;
		}
	}
	else
	{
		CLOG_ERROR_FUNC;
	}

	// Disable Collision
	GetCapsule()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// Hide Widget
	HideWidget();

	// Destory Actor
	SetLifeSpan(DestroyTime);
}

bool ACBaseAI::TakeDamage(const FHitData& HitData, EAttackResult& OutResultType)
{
	if (CanBeAttacked())
	{
		if (FMath::IsNearlyZero(HitData.Damage, KINDA_SMALL_NUMBER) == false)
		{
			FVector Location = GetActorLocation();
			UAISense_Damage::ReportDamageEvent(GetWorld(), this, HitData.DamageCauser, HitData.Damage, Location, Location);
		}
	}

	ACBaseAIController* BaseAIController = Cast<ACBaseAIController>(Controller);
	if (BaseAIController != nullptr)
	{
		ACCombatCharacter* DamageCauser = Cast<ACCombatCharacter>(HitData.DamageCauser);
		if (DamageCauser != nullptr)
		{
			if (DamageCauser->GetGenericTeamId() == 1)
			{
				BaseAIController->Attacked(HitData.DamageCauser);
			}
		}
	}
	else
	{
		CLOG_ERROR_FUNC_TEXT(L"BaseAIController is nullptr");
	}

	return Super::TakeDamage(HitData, OutResultType);
}

void ACBaseAI::AttackAction(EAttackType NewAttackType)
{
	float MontageDuration = 0.0f;

	AttackType = NewAttackType;
	StateManager->SetState(EStateType::ATTACKING);
	GetWorldTimerManager().ClearTimer(ResetAttackCounterTimerHandle);
	UAnimMontage* AttackMontage = GetMeleeAttackMontage(AttackType);
	if (AttackMontage == nullptr)
	{
		// no montage
		StateManager->ResetState(0.0f);
		return MontageDuration;
	}

	MontageDuration = PlayAnimMontage(AttackMontage, StatsManager->GetStatValue(EStatsType::ATTACK_SPEED, true));
	StateManager->ResetState(MontageDuration);

	// Reset Attack Counter
	GetWorldTimerManager().SetTimer(ResetAttackCounterTimerHandle, this, &ACBaseAI::ResetAttackCount, MontageDuration * 1.5f);

	// Remove Stamina
	float StaminaCost = StatsManager->GetStatValue(EStatsType::MELEE_ATTACK_STAMINA_COST, true);
	StaminaCost = ScaleAttackStaminaCostByType(StaminaCost, AttackType);
	ExtendedStamina->ModifyStat(StaminaCost * -1.0f, true);
}

void ACBaseAI::HideWidget()
{
	StatBar->SetHiddenInGame(true);
	TargetMark->SetHiddenInGame(true);
}

void ACBaseAI::Stunned()
{
	GetController()->StopMovement();
	StopAnimMontage(nullptr);

	Super::Stunned();
}

void ACBaseAI::BackAttackDown()
{
	GetController()->StopMovement();
	StopAnimMontage(nullptr);

	Super::BackAttackDown();
}

void ACBaseAI::Impact()
{
	GetController()->StopMovement();
	StopAnimMontage(nullptr);

	Super::Impact();
}

void ACBaseAI::Parried()
{
	GetController()->StopMovement();
	StopAnimMontage(nullptr);

	Super::Parried();
}

FRotator ACBaseAI::GetDesiredRotation()
{
	FRotator Result = GetActorRotation();
	AAIController* MyController = Cast<AAIController>(GetController());
	if (MyController)
	{
		AActor* Target = Cast<AActor>(MyController->GetBlackboardComponent()->GetValueAsObject("Target"));
		if (Target)
		{
			Result.Yaw = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), Target->GetActorLocation()).Yaw;
			return Result;
		}
	}
	else
	{
		CLOG_FUNC_TEXT(L"Controller is nullptr");
	}
	return Result;
}

void ACBaseAI::OnInCombatChanged(bool bIsInCombat)
{
	StatBar->SetHiddenInGame(!bIsInCombat);

	// Targeted : True -> Stat Bar : Visible
	if (!TargetMark->bHiddenInGame)
	{
		StatBar->SetHiddenInGame(false);
	}
}

void ACBaseAI::OnSelected()
{
	TargetMark->SetHiddenInGame(false);
	StatBar->SetHiddenInGame(false);
}

void ACBaseAI::OnDeSelected()
{
	TargetMark->SetHiddenInGame(true);

	// Combat : True -> Stat Bar : Visible
	StatBar->SetHiddenInGame(!Equipment->IsInCombat());
}

const bool ACBaseAI::IsTargetAble()
{
	return IsAlive();
}

void ACBaseAI::OnBehaviorChanged(FString NewBehavior)
{
	BehaviorText->SetText(FText::FromString(NewBehavior));
}