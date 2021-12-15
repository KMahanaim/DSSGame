// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once
#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "DarkSoul/Structs/CFInteractionMassage.h"
#include "I_CInteraction.generated.h"

UINTERFACE(MinimalAPI)
class UI_CInteraction : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class DARKSOUL_API II_CInteraction
{
	GENERATED_BODY()

public:
	/** Interaction Execution */
	virtual void Interaction() = 0;
	/** Interaction On Switch */
	virtual void OnInteraction(FInteractionMassage InteractionMassage) = 0;
	/** Interaction Off Switch */
	virtual void OffInteraction() = 0;
};
