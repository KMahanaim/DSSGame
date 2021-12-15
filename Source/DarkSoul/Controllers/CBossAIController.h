// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once
#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "CBossAIController.generated.h"

class UBehaviorTree;
class UBlackboardData;

/**
 * 
 */
UCLASS()
class DARKSOUL_API ACBossAIController : public AAIController
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
	ACBossAIController();

protected:
	virtual void BeginPlay() override;
	/** Reset Detection Criteria */
	void InitSenseSight();
	/** ∫˘¿« Ω√¿€ */
	virtual void OnPossess(APawn* InPawn) override;
	/** ∫˘¿« «ÿ¡¶ */
	virtual void OnUnPossess() override;

protected:
	UBehaviorTree* BehaviorTree;
	UBlackboardData* BlackBoard;
};