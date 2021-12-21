// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DarkSoul/Structs/CFInteractionMassage.h"
#include "CBossTrigger.generated.h"

class ACharacter;
class UBoxComponent;

/**
 * 
 */
UCLASS()
class DARKSOUL_API ACBossTrigger : public AActor
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "Interaction", meta = (AllowPrivateAccess = "true"))
		FName LevelName;

	UPROPERTY(EditDefaultsOnly, Category = "Interaction", meta = (AllowPrivateAccess = "true"))
		FInteractionMassage Massage;

	UPROPERTY(VisibleDefaultsOnly, Category = "Interaction", meta = (AllowPrivateAccess = "true"))
		UBoxComponent* BossTrigger;

//////////////////////////////////////////////////////////////////////////////////////////////////
// Delegate Bind UFUNCTION
//////////////////////////////////////////////////////////////////////////////////////////////////
private:
	UFUNCTION()
		void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

//////////////////////////////////////////////////////////////////////////////////////////////////
// General Function
//////////////////////////////////////////////////////////////////////////////////////////////////
public:
	ACBossTrigger();

protected:
	virtual void BeginPlay() override;

public:
	void Interaction();

private:
	void OpenLevel();

private:
	FTimerHandle LevelOpenDelayTimerHandle;
};