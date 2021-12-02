// Copyright Epic Games, Inc. All Rights Reserved.
#include "CBossAI.h"
#include "DarkSoul/Components/CBossPhaseComponent.h"

ACBossAI::ACBossAI()
{
	// Create Components
	{
		Phase = CreateDefaultSubobject<UCBossPhaseComponent>(FName("Phase"));
	}
}

void ACBossAI::BeginPlay()
{
	Super::BeginPlay();
}