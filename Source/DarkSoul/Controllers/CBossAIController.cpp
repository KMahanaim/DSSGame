// Copyright Epic Games, Inc. All Rights Reserved.
#include "CBossAIController.h"
#include "DarkSoul/_Utility/CLog.h"
#include "DarkSoul/Characters/CBaseAI.h"
#include "DarkSoul/Components/CEquipmentComponent.h"

/// Unreal Engine
#include "TimerManager.h"
#include "Engine/World.h"
#include "UObject/ConstructorHelpers.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AIPerceptionComponent.h"

void ACBossAIController::OnSensingTarget(AActor * Actor, FAIStimulus Stimulus)
{
}

ACBossAIController::ACBossAIController()
{
	// 1. Team ID Settings
	{
		/// 1 : Player, 2 : Monster, 3 : Neutral(�߸�)
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

	// 3. Set Detection Criteria (���� ���� ����)
	{
		/// Create Detection Component
		Perception = CreateDefaultSubobject<UAIPerceptionComponent>(L"Perception");
		Sight = CreateDefaultSubobject<UAISenseConfig_Sight>("Sight");

		InitSenseSight();
	}
}

void ACBossAIController::BeginPlay()
{
	Super::BeginPlay();
}

void ACBossAIController::InitSenseSight()
{
	/// Set Detection Range
	Sight->SightRadius = 3000.0f;								/// �þ� �Ÿ�
	Sight->PeripheralVisionAngleDegrees = 180.0f;				/// �þ� ����
	Sight->LoseSightRadius = 3000.0f;							/// ���� �ִ� �Ÿ�
	Sight->SetMaxAge(20.0f);									/// ���� ���� ���� �ð�

	/// Set Detection Target
	Sight->DetectionByAffiliation.bDetectEnemies = true;		/// ��
	Sight->DetectionByAffiliation.bDetectFriendlies = false;	/// �Ʊ�
	Sight->DetectionByAffiliation.bDetectNeutrals = false;		/// �߸�

	Perception->ConfigureSense(*Sight);
	Perception->SetDominantSense(*Sight->GetSenseImplementation());
}

void ACBossAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	SetPawn(InPawn);
	if (UseBlackboard(BlackBoard, Blackboard))
	{
		RunBehaviorTree(BehaviorTree);
	}
}

void ACBossAIController::OnUnPossess()
{
	Super::OnUnPossess();
}