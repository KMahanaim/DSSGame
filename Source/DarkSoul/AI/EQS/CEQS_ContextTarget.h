// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once
#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryContext.h"
#include "CEQS_ContextTarget.generated.h"

/**
 * 
 */
UCLASS()
class DARKSOUL_API UCEQS_ContextTarget : public UEnvQueryContext
{
	GENERATED_BODY()

private:
	virtual void ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const override;
};