// Copyright Epic Games, Inc. All Rights Reserved.
#include "CBossPhaseComponent.h"

UCBossPhaseComponent::UCBossPhaseComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UCBossPhaseComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UCBossPhaseComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UCBossPhaseComponent::InitPhase()
{
	CurrentPhase = 0;
}