// Copyright Epic Games, Inc. All Rights Reserved.
#include "CItemBase.h"

ACItemBase::ACItemBase()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ACItemBase::BeginPlay()
{
	Super::BeginPlay();
}

void ACItemBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}