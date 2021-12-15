// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CCharacterSpawner.generated.h"

class ACBaseAI;

UCLASS()
class DARKSOUL_API ACCharacterSpawner : public AActor
{
	GENERATED_BODY()

private:
	UPROPERTY(EditInstanceOnly, Category = "Character", meta = (AllowPrivateAccess = "true"))
		TSubclassOf<ACBaseAI> SpawnCharacter;

	UPROPERTY(EditInstanceOnly, Category = "Spawn", meta = (AllowPrivateAccess = "true"))
		int32 NumberOfSpawn = 0;

	UPROPERTY(EditInstanceOnly, Category = "Spawn", meta = (AllowPrivateAccess = "true"))
		float SpawnDistance = 100;
	
public:	
	ACCharacterSpawner();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

private:
	bool bAreadySpawn = false;
};