// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once
#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "DarkSoul/Enumerations/CEMontageAction.h"
#include "I_CMontageManagerComponent.generated.h"

class UDataTable;

UINTERFACE(MinimalAPI)
class UI_CMontageManagerComponent : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class DARKSOUL_API II_CMontageManagerComponent
{
	GENERATED_BODY()

public:
	virtual const UDataTable* GetMontages(EMontageAction SelectMontage) const = 0;
};
