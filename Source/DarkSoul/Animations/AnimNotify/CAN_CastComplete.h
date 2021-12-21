// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once
#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "CAN_CastComplete.generated.h"

/**
 * 
 */
UCLASS()
class DARKSOUL_API UCAN_CastComplete : public UAnimNotify
{
	GENERATED_BODY()	

public:
	virtual FString GetNotifyName_Implementation() const override;
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
};