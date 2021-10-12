// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once
#include "CoreMinimal.h"
#include "Animations/CAnimInstance.h"
#include "CPlayerAnimInstance.generated.h"

UCLASS()
class DARKSOUL_API UCPlayerAnimInstance : public UCAnimInstance
{
	GENERATED_BODY()
	
public:
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
};