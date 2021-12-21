// Copyright Epic Games, Inc. All Rights Reserved.
#include "CCinematicPlay.h"
#include "DarkSoul/_Utility/CLog.h"
#include "DarkSoul/Interfaces/I_CBoss.h"
#include "DarkSoul/Characters/CPlayerCharacter.h"

/// Unreal Engine
#include "LevelSequence.h"
#include "LevelSequenceActor.h"
#include "LevelSequencePlayer.h"
#include "Components/BoxComponent.h"

ACCinematicPlay::ACCinematicPlay()
{
	PrimaryActorTick.bCanEverTick = false;

	Trigger = CreateDefaultSubobject<UBoxComponent>(FName("Trigger"));
}

void ACCinematicPlay::BeginPlay()
{
	Super::BeginPlay();

	bIsAlreadyPlay = false;

	// Delegate Bind
	{
		Trigger->OnComponentBeginOverlap.AddDynamic(this, &ACCinematicPlay::OnBeginOverlap);
	}
	
	CLOG_ERROR_CHECK_RETURN(Boss);
	CLOG_ERROR_CHECK_RETURN(SequenceAsset);
	CLOG_ERROR_CHECK_RETURN(LevelSequenceActor);

	// Create Cinematic Player
	{
		LevelSequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(this, SequenceAsset, FMovieSceneSequencePlaybackSettings(), LevelSequenceActor);
		LevelSequencePlayer->OnFinished.AddDynamic(this, &ACCinematicPlay::EndCinematic);
	}

	// Already Overlapping Actors Check
	{
		TArray<AActor*> Actors;
		Trigger->GetOverlappingActors(Actors, AActor::StaticClass());
		for (int32 Index = 0; Index < Actors.Num();)
		{
			AActor* Actor = Actors[Index];
			if (Actor == this)
			{
				continue;
			}

			Player = Cast<ACPlayerCharacter>(Actor);
			if (Player.IsValid() && !bIsAlreadyPlay)
			{
				if (LevelSequencePlayer != nullptr)
				{
					bIsAlreadyPlay = true;
					Player->OnCinematic();
					LevelSequencePlayer->Play();
				}
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// UFunction
//////////////////////////////////////////////////////////////////////////////////////////////////
void ACCinematicPlay::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Player = Cast<ACPlayerCharacter>(OtherActor);
	if (Player.IsValid() && !bIsAlreadyPlay)
	{
		if (LevelSequencePlayer != nullptr)
		{
			bIsAlreadyPlay = true;
			Player->OnCinematic();
			LevelSequencePlayer->Play();
		}
	}
}

void ACCinematicPlay::EndCinematic()
{
	Player->EndCinematic();

	II_CBoss* BossInterface = Cast<II_CBoss>(Boss);
	CLOG_ERROR_CHECK_RETURN(BossInterface);
	BossInterface->BossStart();
}