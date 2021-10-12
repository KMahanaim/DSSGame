// Copyright Epic Games, Inc. All Rights Reserved.
#include "CPatrolPath.h"
#include "DarkSoul/_Utility/CLog.h"

/// Unreal Engine
#include "Components/SplineComponent.h"
#include "Components/BillboardComponent.h"

ACPatrolPath::ACPatrolPath()
{
	PrimaryActorTick.bCanEverTick = false;

	// 1. Create Component
	{
		Scene = CreateDefaultSubobject<USceneComponent>(FName("Scene"));
		Billboard = CreateDefaultSubobject<UBillboardComponent>(FName("Billboard"));
		PatrolSpline = CreateDefaultSubobject<USplineComponent>(FName("PatrolSpline"));
	}

	// 2. Set Component Attach
	{
		RootComponent = Scene;
		Billboard->AttachToComponent(Scene, FAttachmentTransformRules::KeepRelativeTransform);
		PatrolSpline->AttachToComponent(Scene, FAttachmentTransformRules::KeepRelativeTransform);
	}
}

void ACPatrolPath::OnConstruction(const FTransform& Transform)
{
	PatrolSpline->SetClosedLoop(bCloseLoop, true);
}