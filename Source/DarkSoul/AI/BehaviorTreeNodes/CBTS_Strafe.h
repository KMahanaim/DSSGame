// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once
#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "CBTS_Strafe.generated.h"

class ACBaseAI;
class AAIController;

enum class EStrafeDirection
{
	Right,
	Left
};

/** BTService_RunEQS의 구조체를 빌려왔다. */
struct FBTEQSServiceMemory
{
	/** Query request ID */
	int32 RequestID = INDEX_NONE;
	TWeakObjectPtr<ACBaseAI> OwnerCharacter;
	TWeakObjectPtr<AAIController> OwnerController;
};

/**
 * 해당 서비스 클래스는 언리얼에서 제공하는 BTService_RunEQS를 참고해서 제작하였다.
 */
UCLASS()
class DARKSOUL_API UCBTS_Strafe : public UBTService
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
		bool bIsDebugDraw = false;

	UPROPERTY(EditAnywhere, meta = (ClampMax = 1000.0f, ClampMin = 0.0f, UIMax = 1000.0f, UIMin = 0.0f), meta = (AllowPrivateAccess = "true"))
		float AcceptanceRadius = 20.0f;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
		FBlackboardKeySelector TargetKey;

	UPROPERTY(EditAnywhere, Category = "EQS", meta = (AllowPrivateAccess = "true"))
		/** 오른쪽 회피를 위한 EQS Asset */
		FEQSParametrizedQueryExecutionRequest RightStrafe;

	UPROPERTY(EditAnywhere, Category = "EQS", meta = (AllowPrivateAccess = "true"))
		/** 왼쪽 회피를 위한 EQS Asset */
		FEQSParametrizedQueryExecutionRequest LeftStrafe;
	
private:
	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	virtual void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void OnCeaseRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	void Strafe(uint8* NodeMemory);

	/**
	 * 타겟의 왼쪽에 있다면, 오른쪽으로 회피(이동)
	 * 타겟의 오른쪽에 있다면, 왼쪽으로 회피(이동)
	 * AI가 타겟의 뒤로 최단 거리로 이동하도록 합니다.
	*/
	EStrafeDirection GetStrafeQuery(uint8* NodeMemory) const;
	virtual uint16 GetInstanceMemorySize() const override;

	/** Delegate Bind Function */
	void OnQueryFinished(TSharedPtr<FEnvQueryResult> Result);

public:
	FQueryFinishedSignature QueryFinishedDelegate;
};