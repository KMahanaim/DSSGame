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
 * 모든 AI가 공통으로 가지는 컨트롤러다.
 * 따로 파생될 유형이 있다해도 이 컨트롤러를 부모로하는 자식 클래스로 만들어진다.
 */
UCLASS()
class DARKSOUL_API ACBaseAIController : public AAIController
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere)
		class UAIPerceptionComponent* Perception;

	UPROPERTY(VisibleAnywhere)
		class UAISenseConfig_Sight* Sight;

protected:
	UFUNCTION()
		virtual void OnSensingTarget(AActor* Actor, FAIStimulus Stimulus);

public:
	ACBaseAIController();

protected:
	virtual void BeginPlay() override;
	/** Reset Detection Criteria */
	void InitSenseSight();
	/** 빙의 시작 */
	virtual void OnPossess(APawn* InPawn) override;
	/** 빙의 해제 */
	virtual void OnUnPossess() override;
	/** 이동 요청 완료 보고 */
	virtual void OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result) override;

public:
	void Attacked(AActor* DamageCauser);

private:
	void OnInCombatChanged(bool bIsInCombat);
	void OnLostTarget();

public:
	FOnSuccessPatrolMove OnSuccessPatrolMove;

protected:
	UBehaviorTree* BehaviorTree;
	UBlackboardData* BlackBoard;
	UPatrolComponent* PatrolComponent;

private:
	FTimerHandle TargetMemoryRetentionTimerHandle;
};