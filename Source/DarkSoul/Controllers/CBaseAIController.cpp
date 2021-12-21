// Copyright Epic Games, Inc. All Rights Reserved.
#include "CBaseAIController.h"
#include "DarkSoul/_Utility/CLog.h"
#include "DarkSoul/Characters/CBaseAI.h"
#include "DarkSoul/Components/CPatrolComponent.h"
#include "DarkSoul/Components/CEquipmentComponent.h"

/// Unreal Engine
#include "TimerManager.h"
#include "Engine/World.h"
#include "UObject/ConstructorHelpers.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AIPerceptionComponent.h"

ACBaseAIController::ACBaseAIController()
{
	// 1. Team ID Settings
	{
		/// 1 : Player, 2 : Monster, 3 : Neutral(중립)
		SetGenericTeamId(FGenericTeamId(2));
	}

	// 2. Create Behavior Tree and BlackBoard
	{
		FString Path = L"BehaviorTree'/Game/DarkSoul/AI/Melee/BT_MeleeAI.BT_MeleeAI'";
		static ConstructorHelpers::FObjectFinder<UBehaviorTree> BehaviorTreeObject(*Path);
		if (BehaviorTreeObject.Succeeded())
		{
			BehaviorTree = BehaviorTreeObject.Object;
		}

		Path = L"BlackboardData'/Game/DarkSoul/AI/BB_BaseAI.BB_BaseAI'";
		static ConstructorHelpers::FObjectFinder<UBlackboardData> BlackBoardDataObject(*Path);
		if (BlackBoardDataObject.Succeeded())
		{
			BlackBoard = BlackBoardDataObject.Object;
		}
	}

	// 3. Set Detection Criteria (감지 기준 설정)
	{
		/// Create Detection Component
		Perception = CreateDefaultSubobject<UAIPerceptionComponent>(L"Perception");
		Sight = CreateDefaultSubobject<UAISenseConfig_Sight>("Sight");

		InitSenseSight();
	}
}

void ACBaseAIController::BeginPlay()
{
	Super::BeginPlay();
}

void ACBaseAIController::InitSenseSight()
{
	/// Set Detection Range
	Sight->SightRadius = 400.0f;								/// 시야 거리
	Sight->PeripheralVisionAngleDegrees = 70.0f;				/// 시야 각도
	Sight->LoseSightRadius = 800.0f;							/// 추적 최대 거리
	Sight->SetMaxAge(5.0f);										/// 추적 상태 유지 시간

	/// Set Detection Target
	Sight->DetectionByAffiliation.bDetectEnemies = true;		/// 적
	Sight->DetectionByAffiliation.bDetectFriendlies = false;	/// 아군
	Sight->DetectionByAffiliation.bDetectNeutrals = false;		/// 중립

	Perception->ConfigureSense(*Sight);
	Perception->SetDominantSense(*Sight->GetSenseImplementation());
}

void ACBaseAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	SetPawn(InPawn);
	if (UseBlackboard(BlackBoard, Blackboard))
	{
		Blackboard->SetValueAsVector("PatrolLocation", InPawn->GetActorLocation());
		RunBehaviorTree(BehaviorTree);
	}

	// Delegate Bind, Add
	{
		Perception->OnTargetPerceptionUpdated.AddDynamic(this, &ACBaseAIController::OnSensingTarget);

		ACBaseAI* PawnCharacter = Cast<ACBaseAI>(GetPawn());
		CLOG_ERROR_CHECK_RETURN(PawnCharacter);

		UCEquipmentComponent* Equipment = PawnCharacter->GetEquipmentComponent();
		CLOG_ERROR_CHECK_RETURN(Equipment);
		Equipment->OnInCombatChanged.AddUObject(this, &ACBaseAIController::OnInCombatChanged);
	}
}

void ACBaseAIController::OnUnPossess()
{
	Super::OnUnPossess();
}

void ACBaseAIController::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	Super::OnMoveCompleted(RequestID, Result);

	if (Result.IsSuccess())
	{
		if (GetPawn() != nullptr)
		{
			OnSuccessPatrolMove.Broadcast(*Blackboard);
		}
		else
		{
			CLOG_FUNC_TEXT(L"GetPawn is nullptr or Cast Failed");
		}
	}
}

void ACBaseAIController::Attacked(AActor* DamageCauser)
{
	Blackboard->SetValueAsObject("Target", DamageCauser);
}

void ACBaseAIController::OnInCombatChanged(bool bIsInCombat)
{
	Blackboard->SetValueAsBool("IsInCombat", bIsInCombat);
}

void ACBaseAIController::OnLostTarget()
{
	//CLOG_FUNC_TEXT(L"On Lost Target(Timer)");
	Blackboard->SetValueAsObject("Target", nullptr);
	Sight->PeripheralVisionAngleDegrees = 70.0f;
	Perception->ConfigureSense(*Sight);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// UFunction
//////////////////////////////////////////////////////////////////////////////////////////////////

void ACBaseAIController::OnSensingTarget(AActor* Actor, FAIStimulus Stimulus)
{
	if (Stimulus.WasSuccessfullySensed())
	{
		// Target Detection
		//CLOG_FUNC_TEXT(L"Find Target, " + Actor->GetName());
		GetWorldTimerManager().ClearTimer(TargetMemoryRetentionTimerHandle);
		Blackboard->SetValueAsObject("Target", Actor);
		Sight->PeripheralVisionAngleDegrees = 180.0f;
		Perception->ConfigureSense(*Sight);
	}
	else
	{
		//CLOG_FUNC_TEXT(L"Set Lost Target Timer");
		GetWorldTimerManager().SetTimer(TargetMemoryRetentionTimerHandle, this, &ACBaseAIController::OnLostTarget, Sight->GetMaxAge());
	}
}