// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DarkSoul/Interfaces/I_CInteraction.h"
#include "CUW_Interaction.generated.h"

class UImage;
class UTextBlock;

/**
 * 
 */
UCLASS()
class DARKSOUL_API UCUW_Interaction : public UUserWidget, public II_CInteraction
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

public:
	virtual void Interaction() override;
	virtual void OnInteraction(FInteractionMassage InteractionMassage) override;
	virtual void OffInteraction() override;

private:
	UImage* KeyIcon = nullptr;
	UTextBlock* Massage = nullptr;
};