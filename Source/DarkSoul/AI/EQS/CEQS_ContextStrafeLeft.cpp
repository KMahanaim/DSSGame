// Copyright Epic Games, Inc. All Rights Reserved.
#include "CEQS_ContextStrafeLeft.h"
#include "DarkSoul/_Utility/CLog.h"

/// Unreal Engine
#include "AIController.h"
#include "Kismet/KismetMathLibrary.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Point.h"

void UCEQS_ContextStrafeLeft::ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const
{
	Super::ProvideContext(QueryInstance, ContextData);

	if (QueryInstance.Owner.IsValid())
	{
		AAIController* ownerController = Cast<AAIController>(Cast<APawn>(QueryInstance.Owner)->GetController());
		if (ownerController == nullptr)
		{
			CLOG_FUNC_TEXT(L"Owner Controller is nullptr");
		}
		else
		{
			AActor* target = Cast<AActor>(ownerController->GetBlackboardComponent()->GetValueAsObject(L"Target"));
			if (target == nullptr)
			{
				CLOG_FUNC_TEXT(L"Target is nullptr");
			}
			else
			{
				FVector ownerLocation = Cast<APawn>(QueryInstance.Owner)->GetActorLocation();
				FVector targetLocation = target->GetActorLocation();
				FRotator lookAtRotation = FRotator::ZeroRotator;
				// 1. Ÿ�� �ٶ󺸴� ���⿡�� �̵� ���� ��� (���� - 90)
				lookAtRotation.Yaw = UKismetMathLibrary::FindLookAtRotation(ownerLocation, targetLocation).Yaw - 90.0f;
				// 2. ���������� �̵� ��Ű�� ���� �Ÿ� ����
				FVector moveVector = UKismetMathLibrary::Conv_RotatorToVector(lookAtRotation) * 300.0f;
				//CLog::Print(moveVector);
				// 3. ���� ��ġ ���� �̵��� ���ϱ�
				moveVector = ownerLocation + moveVector;

				UEnvQueryItemType_Point::SetContextHelper(ContextData, moveVector);
			}
		}
	}
	else
	{
		CLOG_FUNC_TEXT(L"Critical Error Owner is nullptr");
	}
}