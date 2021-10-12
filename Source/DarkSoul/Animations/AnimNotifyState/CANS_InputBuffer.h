// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once
#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "CANS_InputBuffer.generated.h"

/**
 * 입력값을 받아 다음 행동을 실행한다.
 * 콤보 공격에 사용된다.
 */
UCLASS()
class DARKSOUL_API UCANS_InputBuffer : public UAnimNotifyState
{
	GENERATED_BODY()
	
public:
	virtual FString GetNotifyName_Implementation() const override;

	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
};