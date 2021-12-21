// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CCinematicPlay.generated.h"

class UBoxComponent;
class ULevelSequence;
class ALevelSequenceActor;
class ULevelSequencePlayer;

class ACPlayerCharacter;

UCLASS()
class DARKSOUL_API ACCinematicPlay : public AActor
{
	GENERATED_BODY()

private:

	UPROPERTY(EditInstanceOnly, Category = "Cinematic", meta = (AllowPrivateAccess = "true"))
		ULevelSequence* SequenceAsset;

	UPROPERTY(EditInstanceOnly, Category = "Cinematic", meta = (AllowPrivateAccess = "true"))
		ALevelSequenceActor* LevelSequenceActor;

	UPROPERTY(VisibleAnywhere, Category = "Cinematic", meta = (AllowPrivateAccess = "true"))
		ULevelSequencePlayer* LevelSequencePlayer;

	UPROPERTY(EditInstanceOnly, Category = "Boss", meta = (AllowPrivateAccess = "true"))
		AActor* Boss;

	UPROPERTY(VisibleAnywhere, Category = "Component", meta = (AllowPrivateAccess = "true"))
		UBoxComponent* Trigger;

//////////////////////////////////////////////////////////////////////////////////////////////////
// Delegate Bind UFUNCTION
//////////////////////////////////////////////////////////////////////////////////////////////////
private:
	UFUNCTION()
		void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void EndCinematic();

public:	
	ACCinematicPlay();

protected:
	virtual void BeginPlay() override;

private:
	bool bIsAlreadyPlay;
	TWeakObjectPtr<ACPlayerCharacter> Player;
};
