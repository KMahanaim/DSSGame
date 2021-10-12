// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DarkSoul/Structs/DataTableRowBase/CFMontageAction.h"
#include "CMontageManagerComponent.generated.h"

class UAnimMontage;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DARKSOUL_API UCMontageManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCMontageManagerComponent();

protected:
	virtual void BeginPlay() override;

public:	
	UAnimMontage* GetMontageforAction(EMontageAction Action, UINT Index = 0);
	int8 GetMontageActionLastIndex(EMontageAction Action) const;
	EMontageAction GetLastRequestedAction() const { return LastRequestedAction; }

private:
	FMontageAction* GetMontage(EMontageAction Action) const;

private:
	EMontageAction LastRequestedAction;
};
