// Copyright Epic Games, Inc. All Rights Reserved.
#include "CBTS_Strafe.h"
#include "DarkSoul/_Utility/CLog.h"
#include "DarkSoul/Characters/CBaseAI.h"
#include "DarkSoul/Components/CRotatingComponent.h"

/// Unreal Engine
#include "AIController.h"
#include "DrawDebugHelpers.h"
#include "Kismet/KismetMathLibrary.h"
#include "EnvironmentQuery/EnvQuery.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "BehaviorTree/BlackboardComponent.h"

void UCBTS_Strafe::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);

	bNotifyBecomeRelevant = true;
	bNotifyCeaseRelevant = true;

	RightStrafe.InitForOwnerAndBlackboard(*this, GetBlackboardAsset());
	LeftStrafe.InitForOwnerAndBlackboard(*this, GetBlackboardAsset());
}

void UCBTS_Strafe::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	FBTEQSServiceMemory* Memory = CastInstanceNodeMemory<FBTEQSServiceMemory>(NodeMemory);
	if (Memory->RequestID == INDEX_NONE)
	{
		Strafe(NodeMemory);
	}
}

void UCBTS_Strafe::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);

	FBTEQSServiceMemory* Memory = CastInstanceNodeMemory<FBTEQSServiceMemory>(NodeMemory);
	Memory->OwnerController = Cast<AAIController>(OwnerComp.GetAIOwner());
	CLOG_ERROR_CHECK_RETURN(Memory->OwnerController);

	Memory->OwnerCharacter = Cast<ACBaseAI>(Memory->OwnerController->GetPawn());
	CLOG_ERROR_CHECK_RETURN_TEXT(Memory->OwnerCharacter, L"Onwer is nullptr : " + Memory->OwnerController->GetName())

	AActor* Target = Cast<AActor>(Memory->OwnerController->GetBlackboardComponent()->GetValueAsObject(TargetKey.SelectedKeyName));
	CLOG_ERROR_CHECK_RETURN(Target);

	// Focus Target
	Memory->OwnerController->SetFocus(Target);
	Memory->OwnerCharacter->GetRotatingComponent()->SetRotationMode(ERotationMode::FACE_CAMERA);

	// Bind OnQueryFinished
	QueryFinishedDelegate = FQueryFinishedSignature::CreateUObject(this, &UCBTS_Strafe::OnQueryFinished);

	Memory->RequestID = INDEX_NONE;
	if (Memory->RequestID == INDEX_NONE)
	{
		Strafe(NodeMemory);
	}
}

void UCBTS_Strafe::OnCeaseRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnCeaseRelevant(OwnerComp, NodeMemory);

	FBTEQSServiceMemory* Memory = CastInstanceNodeMemory<FBTEQSServiceMemory>(NodeMemory);
	check(Memory);

	if (Memory->RequestID != INDEX_NONE)
	{
		Memory->OwnerController->ClearFocus(EAIFocusPriority::Default);
		Memory->OwnerController->StopMovement();
		Memory->OwnerCharacter->GetRotatingComponent()->SetRotationMode(ERotationMode::ORIENT_TO_MOVEMENT);

		UWorld* World = OwnerComp.GetWorld();
		if (World)
		{
			// make EQS abort that query
			UEnvQueryManager* EQSManager = UEnvQueryManager::GetCurrent(World);
			check(EQSManager);
			EQSManager->AbortQuery(Memory->RequestID);
		}
		Memory->RequestID = INDEX_NONE;
	}
}

void UCBTS_Strafe::Strafe(uint8* NodeMemory)
{
	// OnQueryFinished 바인딩과 함께 EQS 실행
	FBTEQSServiceMemory* Memory = CastInstanceNodeMemory<FBTEQSServiceMemory>(NodeMemory);
	switch (GetStrafeQuery(NodeMemory))
	{
		case EStrafeDirection::Right:
		{
			if (RightStrafe.IsValid())
			{
				Memory->RequestID = RightStrafe.Execute(*Memory->OwnerCharacter, Memory->OwnerController->GetBlackboardComponent(), QueryFinishedDelegate);
			}
			else
			{
				CLOG_FUNC_TEXT(L"Right Strafe is nullptr");
			}
		}
		break;
		case EStrafeDirection::Left:
		{
			if (LeftStrafe.IsValid())
			{
				Memory->RequestID = LeftStrafe.Execute(*Memory->OwnerCharacter, Memory->OwnerController->GetBlackboardComponent(), QueryFinishedDelegate);
			}
			else
			{
				CLOG_FUNC_TEXT(L"Left Strafe is nullptr");
			}
		}
		break;
	}
}

EStrafeDirection UCBTS_Strafe::GetStrafeQuery(uint8* NodeMemory) const
{
	FBTEQSServiceMemory* Memory = CastInstanceNodeMemory<FBTEQSServiceMemory>(NodeMemory);
	AActor* Target = Cast<AActor>(Memory->OwnerController->GetBlackboardComponent()->GetValueAsObject(TargetKey.SelectedKeyName));
	if (Target == nullptr)
	{
		CLOG_FUNC_TEXT(L"Target is nullptr");
	}
	else
	{
		FVector RightVector = Target->GetActorRightVector();
		RightVector.Z = 0.0f;
		RightVector.Normalize();

		/// Target -> AI
		FVector TargetDirection = UKismetMathLibrary::GetDirectionUnitVector(Target->GetActorLocation(), Memory->OwnerCharacter->GetActorLocation());
		TargetDirection.Z = 0.0f;
		TargetDirection.Normalize();

		if (UKismetMathLibrary::Dot_VectorVector(RightVector, TargetDirection) >= -0.0f)
		{
			if (LeftStrafe.IsValid() == false)
			{
				CLOG_FUNC_TEXT(L"Left Strafe is nullptr");
			}
			return EStrafeDirection::Left;
		}
		else
		{
			if (RightStrafe.IsValid() == false)
			{
				CLOG_FUNC_TEXT(L"Right Strafe is nullptr");
			}
			return EStrafeDirection::Right;
		}
	}
	return EStrafeDirection::Right;
}

uint16 UCBTS_Strafe::GetInstanceMemorySize() const
{
	return sizeof(FBTEQSServiceMemory);
}

void UCBTS_Strafe::OnQueryFinished(TSharedPtr<FEnvQueryResult> Result)
{
	if (Result->IsAborted())
	{
		return;
	}

	AActor* Owner = Cast<AActor>(Result->Owner.Get());
	APawn* PawnOwner = Cast<APawn>(Owner);
	if (PawnOwner)
	{
		Owner = PawnOwner->GetController();
	}

	UBehaviorTreeComponent* BehaviorTreeCompoennt = Owner ? Owner->FindComponentByClass<UBehaviorTreeComponent>() : NULL;
	if (!BehaviorTreeCompoennt)
	{
		UE_LOG(LogBehaviorTree, Warning, TEXT("Unable to find behavior tree to notify about finished query from %s!"), *GetNameSafe(Owner));
		return;
	}

	FBTEQSServiceMemory* Memory = CastInstanceNodeMemory<FBTEQSServiceMemory>(BehaviorTreeCompoennt->GetNodeMemory(this, BehaviorTreeCompoennt->FindInstanceContainingNode(this)));
	check(Owner);
	ensure(Memory->RequestID != INDEX_NONE);

	bool bSuccess = (Result->Items.Num() >= 1);
	if (bSuccess)
	{
		FVector Destination = Result->GetItemAsLocation(0);
		if (Destination == FVector::ZeroVector)
		{
			Cast<AAIController>(Owner)->StopMovement();
		}
		else
		{
			Cast<AAIController>(Owner)->MoveToLocation
			(
				Destination,
				AcceptanceRadius,
				false	/** Stop On Overlap*/
			);
			
			if (bIsDebugDraw)
			{
				DrawDebugLine(GetWorld(), PawnOwner->GetActorLocation(), Destination, FColor::Red, false, 2.0f, 0, 1.0f);
			}
		}
	}
	else
	{
		CLOG_FUNC_TEXT(L"Failed, Item Zero");
	}
	Memory->RequestID = INDEX_NONE;
}