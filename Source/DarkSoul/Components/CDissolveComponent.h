// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CDissolveComponent.generated.h"

class UMaterial;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DARKSOUL_API UCDissolveComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly, Category = "Dissolve", meta = (AllowPrivateAccess = "true"))
		UMaterial* DissolveMaterial;

public:	
	UCDissolveComponent();

protected:
	virtual void BeginPlay() override;
};