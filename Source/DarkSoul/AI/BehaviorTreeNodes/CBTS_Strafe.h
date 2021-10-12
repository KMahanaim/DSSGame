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

/** BTService_RunEQS�� ����ü�� �����Դ�. */
struct FBTEQSServiceMemory
{
	/** Query request ID */
	int32 RequestID = INDEX_NONE;
	TWeakObjectPtr<ACBaseAI> OwnerCharacter;
	TWeakObjectPtr<AAIController> OwnerController;
};

/**
 * �ش� ���� Ŭ������ �𸮾󿡼� �����ϴ� BTService_RunEQS�� �����ؼ� �����Ͽ���.
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
		/** ������ ȸ�Ǹ� ���� EQS Asset */
		FEQSParametrizedQueryExecutionRequest RightStrafe;

	UPROPERTY(EditAnywhere, Category = "EQS", meta = (AllowPrivateAccess = "true"))
		/** ���� ȸ�Ǹ� ���� EQS Asset */
		FEQSParametrizedQueryExecutionRequest LeftStrafe;
	
private:
	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	virtual void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void OnCeaseRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	void Strafe(uint8* NodeMemory);

	/**
	 * Ÿ���� ���ʿ� �ִٸ�, ���������� ȸ��(�̵�)
	 * Ÿ���� �����ʿ� �ִٸ�, �������� ȸ��(�̵�)
	 * AI�� Ÿ���� �ڷ� �ִ� �Ÿ��� �̵��ϵ��� �մϴ�.
	*/
	EStrafeDirection GetStrafeQuery(uint8* NodeMemory) const;
	virtual uint16 GetInstanceMemorySize() const override;

	/** Delegate Bind Function */
	void OnQueryFinished(TSharedPtr<FEnvQueryResult> Result);

public:
	FQueryFinishedSignature QueryFinishedDelegate;
};