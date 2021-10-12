// Copyright Epic Games, Inc. All Rights Reserved.
#include "CEQS_ContextTarget.h"
#include "DarkSoul/_Utility/CLog.h"

/// Unreal Engine
#include "AIController.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Actor.h"

void UCEQS_ContextTarget::ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const
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
				UEnvQueryItemType_Actor::SetContextHelper(ContextData, target);
			}
		}
	}
	else
	{
		CLOG_FUNC_TEXT(L"Critical Error Owner is nullptr");
	}
}