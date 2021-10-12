// Copyright Epic Games, Inc. All Rights Reserved.
#include "CDissolveComponent.h"

/// Unreal Engine
#include "Materials/Material.h"

UCDissolveComponent::UCDissolveComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	DissolveMaterial = CreateDefaultSubobject<UMaterial>(FName("DissolveMaterial"));
}

void UCDissolveComponent::BeginPlay()
{
	Super::BeginPlay();
}