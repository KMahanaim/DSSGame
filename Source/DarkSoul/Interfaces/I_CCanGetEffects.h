// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once
#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "DarkSoul/Enumerations/CEEffectType.h"
#include "I_CCanGetEffects.generated.h"

UINTERFACE(MinimalAPI)
class UI_CCanGetEffects : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class DARKSOUL_API II_CCanGetEffects
{
	GENERATED_BODY()

public:
	virtual const bool CanEffectBeApplied(EEffectType Type, AActor* Applier) = 0;
};
