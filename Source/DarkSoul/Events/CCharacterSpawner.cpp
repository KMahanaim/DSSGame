// Copyright Epic Games, Inc. All Rights Reserved.
#include "CCharacterSpawner.h"
#include "DarkSoul/_Utility/CLog.h"
#include "DarkSoul/Characters/CBaseAI.h"

/// Unreal Engine
#include "Engine/World.h"

ACCharacterSpawner::ACCharacterSpawner()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ACCharacterSpawner::BeginPlay()
{
	Super::BeginPlay();
}

void ACCharacterSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bAreadySpawn == false)
	{
		bAreadySpawn = true;

		FTransform Transform;
		FActorSpawnParameters SpawnParameters;

		FVector SpawnCenterLocation = GetActorLocation();
		float SQRTNumber = FMath::Sqrt(NumberOfSpawn);
		int32 Width = FMath::FloorToInt(SQRTNumber);
		float Length = Width * SpawnDistance;
		float HalfLength = Length / 2;

		for (int32 XIndex = 0; XIndex < NumberOfSpawn;)
		{
			for (int32 YIndex = 0; YIndex < Width; YIndex++)
			{
				Transform.SetLocation(((HalfLength - (YIndex * SpawnDistance)) * GetActorRightVector()) + SpawnCenterLocation);
				Transform.SetRotation(GetActorRotation().Quaternion());

				SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

				ACBaseAI* Spawn = GetWorld()->SpawnActor<ACBaseAI>(SpawnCharacter.Get(), Transform, SpawnParameters);
				XIndex++;

				if (NumberOfSpawn <= XIndex)
				{
					break;
				}
			}

			SpawnCenterLocation -= SpawnDistance * GetActorForwardVector();
		}
	}
}