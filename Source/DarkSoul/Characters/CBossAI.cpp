// Copyright Epic Games, Inc. All Rights Reserved.
#include "CBossAI.h"
#include "DarkSoul/Controllers/CBossAIcontroller.h"
#include "DarkSoul/Components/CBossPhaseComponent.h"

ACBossAI::ACBossAI()
{
	// Create Components
	{
		Phase = CreateDefaultSubobject<UCBossPhaseComponent>(FName("Phase"));
	}

	// Set Controller
	{
		AIControllerClass = ACBossAIController::StaticClass();
	}

	// Set Boss Name
	{
		BossName = FName(GetName());
	}
}

void ACBossAI::BeginPlay()
{
	Super::BeginPlay();
}

void ACBossAI::BossStart()
{
	Phase->Start();
}