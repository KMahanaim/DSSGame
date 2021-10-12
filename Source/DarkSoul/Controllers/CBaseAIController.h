// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once
#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "CBaseAIController.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnSuccessPatrolMove, const UBlackboardComponent&)

class UBehaviorTree;
class UBlackboardData;
class UPatrolComponent;

/**
 * ��� AI�� �������� ������ ��Ʈ�ѷ���.
 * ���� �Ļ��� ������ �ִ��ص� �� ��Ʈ�ѷ��� �θ���ϴ� �ڽ� Ŭ������ ���������.
 */
UCLASS()
class DARKSOUL_API ACBaseAIController : public AAIController
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleAnywhere)
		class UAIPerceptionComponent* Perception;

	UPROPERTY(VisibleAnywhere)
		class UAISenseConfig_Sight* Sight;

private:
	UFUNCTION()
		void OnSensingTarget(AActor* Actor, FAIStimulus Stimulus);

public:
	ACBaseAIController();

protected:
	virtual void BeginPlay() override;
	/** ���� ���� */
	virtual void OnPossess(APawn* InPawn) override;
	/** ���� ���� */
	virtual void OnUnPossess() override;
	/** �̵� ��û �Ϸ� ���� */
	virtual void OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result) override;

private:
	void OnInCombatChanged(bool bIsInCombat);
	void OnLostTarget();

public:
	FOnSuccessPatrolMove OnSuccessPatrolMove;

private:
	UBehaviorTree* BehaviorTree;
	UBlackboardData* BlackBoard;
	UPatrolComponent* PatrolComponent;

	FTimerHandle TargetMemoryRetentionTimerHandle;
};