// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "I_CHasRotatingComponent.generated.h"

UINTERFACE(MinimalAPI)
class UI_CHasRotatingComponent : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class DARKSOUL_API II_CHasRotatingComponent
{
	GENERATED_BODY()

public:
	virtual FRotator GetDesiredRotation() = 0;
};
