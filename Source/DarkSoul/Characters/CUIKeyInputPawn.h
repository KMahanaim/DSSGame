// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "CUIKeyInputPawn.generated.h"

DECLARE_DELEGATE(FArrowUp);
DECLARE_DELEGATE(FArrowDown);
DECLARE_DELEGATE(FArrowRight);
DECLARE_DELEGATE(FArrowLeft);
DECLARE_DELEGATE(FEnter);

UCLASS()
class DARKSOUL_API ACUIKeyInputPawn : public APawn
{
	GENERATED_BODY()

public:
	ACUIKeyInputPawn();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	void ArrowUp();
	void ArrowDown();
	void ArrowRight();
	void ArrowLeft();
	void StickUpDown(float AxisValue);
	void StickRightLeft(float AixsValue);
	void Enter();

public:
	FArrowUp OnArrowUp;
	FArrowDown OnArrowDown;
	FArrowRight OnArrowRight;
	FArrowLeft OnArrowLeft;
	FEnter OnEnter;

private:
	bool bIsAlreadyStickMove;
};
