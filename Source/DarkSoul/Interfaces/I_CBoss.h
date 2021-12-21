// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once
#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "I_CBoss.generated.h"

UINTERFACE(MinimalAPI)
class UI_CBoss : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class DARKSOUL_API II_CBoss
{
	GENERATED_BODY()

public:
	virtual void BossStart() = 0;
};
