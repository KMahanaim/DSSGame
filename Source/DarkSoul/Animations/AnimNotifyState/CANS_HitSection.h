// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once
#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "CANS_HitSection.generated.h"

/**
 * ���� ��� ��� �� ���� �������� �� �� �ִ� ������ �����Ѵ�.
 */
UCLASS()
class DARKSOUL_API UCANS_HitSection : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	virtual FString GetNotifyName_Implementation() const override;

	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
};
