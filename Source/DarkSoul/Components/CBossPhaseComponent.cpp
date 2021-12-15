// Copyright Epic Games, Inc. All Rights Reserved.
#include "CBossPhaseComponent.h"

/// Unreal Engine
#include "Engine/TriggerBox.h"

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

void UCBossPhaseComponent::Init()
{
	CurrentPhase = 0;
}

void UCBossPhaseComponent::Start()
{
	CurrentPhase = 1;
}

void UCBossPhaseComponent::Retry()
{
}

void UCBossPhaseComponent::End()
{
}