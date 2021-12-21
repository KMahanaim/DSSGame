// Copyright Epic Games, Inc. All Rights Reserved.
#include "CBossTrigger.h"
#include "DarkSoul/_Utility/CLog.h"
#include "DarkSoul/Characters/CPlayerCharacter.h"

/// Unreal Engine
#include "TimerManager.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/BoxComponent.h"

ACBossTrigger::ACBossTrigger()
{
	PrimaryActorTick.bCanEverTick = false;

	BossTrigger = CreateDefaultSubobject<UBoxComponent>(FName("BossTrigger"));
}

void ACBossTrigger::BeginPlay()
{
	Super::BeginPlay();

	// Delegate Bind
	{
		BossTrigger->OnComponentBeginOverlap.AddDynamic(this, &ACBossTrigger::OnBeginOverlap);
		BossTrigger->OnComponentEndOverlap.AddDynamic(this, &ACBossTrigger::OnEndOverlap);
	}

	// Already Overlapping Actors Check
	{
		TArray<AActor*> Actors;
		BossTrigger->GetOverlappingActors(Actors, AActor::StaticClass());
		for (int32 Index = 0; Index < Actors.Num();)
		{
			AActor* Actor = Actors[Index];
			if (Actor == this)
			{
				continue;
			}

			const bool bCanBeInteraction = UKismetSystemLibrary::DoesImplementInterface(Actor, UI_CInteraction::StaticClass());
			if (bCanBeInteraction)
			{
				II_CInteraction* ActorInteraction = Cast<II_CInteraction>(Actor);
				ActorInteraction->OnInteraction(Massage);
				break;
			}
		}
	}
}

void ACBossTrigger::Interaction()
{
	// Check the overlapping
	TArray<AActor*> Actors;
	BossTrigger->GetOverlappingActors(Actors, AActor::StaticClass());
	for (int32 Index = 0; Index < Actors.Num();)
	{
		AActor* Actor = Actors[Index];
		if (Actor == this)
		{
			continue;
		}

		const bool bCanBeInteraction = UKismetSystemLibrary::DoesImplementInterface(Actor, UI_CInteraction::StaticClass());
		if (bCanBeInteraction)
		{
			ACPlayerCharacter* Player = Cast<ACPlayerCharacter>(Actor);
			if (Player == nullptr)
			{
				break;
			}

			// Level Move
			Player->OnBeginLoading();
			if (LevelName.IsNone())
			{
				CLOG_ERROR_RETURN_TEXT(L"Level Name is NONE, Check the Trigger Blueprint");
			}

			GetWorld()->GetTimerManager().SetTimer(LevelOpenDelayTimerHandle, this, &ACBossTrigger::OpenLevel, 3, false);
			break;
		}
	}
}

void ACBossTrigger::OpenLevel()
{
	UGameplayStatics::OpenLevel(this, LevelName);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// UFunction
//////////////////////////////////////////////////////////////////////////////////////////////////
void ACBossTrigger::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ACPlayerCharacter* Player = Cast<ACPlayerCharacter>(OtherActor);
	CLOG_ERROR_CHECK_RETURN(Player);

	// Bind
	Player->PlayInteraction.Unbind();
	Player->PlayInteraction.BindUObject(this, &ACBossTrigger::Interaction);

	// On Interaction Massage Box
	Player->OnInteraction(Massage);
}

void ACBossTrigger::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ACPlayerCharacter* Player = Cast<ACPlayerCharacter>(OtherActor);
	CLOG_ERROR_CHECK_RETURN(Player);

	// UnBind
	Player->PlayInteraction.Unbind();

	// Off Interaction Massage Box
	Player->OffInteraction();
}