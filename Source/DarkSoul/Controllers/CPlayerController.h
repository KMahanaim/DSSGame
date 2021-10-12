// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once
#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CPlayerController.generated.h"

/**
 * �÷��̾ ���� �����ϱ� ���� �ʿ��� ��Ʈ�ѷ� Ŭ������.
 */
UCLASS()
class DARKSOUL_API ACPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	virtual void PostInitializeComponents() override;
	virtual void OnPossess(APawn* aPawn) override;
};
