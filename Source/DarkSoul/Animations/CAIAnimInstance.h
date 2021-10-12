// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once
#include "CoreMinimal.h"
#include "Animations/CAnimInstance.h"
#include "DarkSoul/Enumerations/CEActivity.h"
#include "CAIAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class DARKSOUL_API UCAIAnimInstance : public UCAnimInstance
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly, Category = "State", meta = (AllowPrivateAccess = "true"))
		float BlockInterpSpeed = 5.0f;

public:
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

private:
	void OnActivityChanged(EActivity Activity, bool Value);

private:
	bool bIsBlocking = false;
};
