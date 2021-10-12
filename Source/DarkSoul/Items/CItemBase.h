// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DarkSoul/Structs/CFItem.h"
#include "CItemBase.generated.h"

UCLASS()
class DARKSOUL_API ACItemBase : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
		FItem Item;
	
public:
	ACItemBase();

	const FItem* GetItem() const { return &Item; }

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
};