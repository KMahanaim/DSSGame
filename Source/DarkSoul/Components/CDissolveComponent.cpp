// Copyright Epic Games, Inc. All Rights Reserved.
#include "CDissolveComponent.h"
#include "DarkSoul/_Utility/CLog.h"

/// Unreal Engine
#include "TimerManager.h"
#include "Engine/World.h"
#include "Materials/Material.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/PrimitiveComponent.h"

UCDissolveComponent::UCDissolveComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	DissolveMaterial = CreateDefaultSubobject<UMaterial>(FName("DissolveMaterial"));
}

void UCDissolveComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UCDissolveComponent::StartDissolve(UPrimitiveComponent* Component, bool bReverse)
{
	if (Component == nullptr)
	{
		return;
	}

	int32 ComponentIndex = FindComponent(Component);
	if (ComponentIndex != INDEX_NONE)
	{
		Dissolves[ComponentIndex].bReverse = bReverse;
		Dissolves[ComponentIndex].bIsRunning = true;

		GetWorld()->GetTimerManager().SetTimer(DissolveTimerHandle, this, &UCDissolveComponent::Dissolve, 0.016f, true);
	}
	else
	{
		TArray<UMaterialInterface*> Materials;
		TArray<UMaterialInstanceDynamic*> MaterialInstanceDynamics;

		for (int32 MaterialIndex = 0; MaterialIndex < Component->GetNumMaterials() - 1; MaterialIndex++)
		{
			Materials.Emplace(Component->GetMaterial(MaterialIndex));
			UMaterialInstanceDynamic* MaterialInstanceDynamic = Component->CreateDynamicMaterialInstance(MaterialIndex, DissolveMaterial, FName("NONE"));
			MaterialInstanceDynamic->SetVectorParameterValue(DissolveColorName, DissolveColor);
			MaterialInstanceDynamics.Emplace(MaterialInstanceDynamic);
		}

		FDissolve NewDissolve;
		NewDissolve.Component = Component;
		NewDissolve.Value = static_cast<float>(!bReverse);
		NewDissolve.Materials = Materials;
		NewDissolve.DissolveMaterials = MaterialInstanceDynamics;
		NewDissolve.bReverse = bReverse;
		NewDissolve.bIsRunning = true;
		Dissolves.Emplace(NewDissolve);

		GetWorld()->GetTimerManager().SetTimer(DissolveTimerHandle, this, &UCDissolveComponent::Dissolve, 0.016f, true);
	}
}

void UCDissolveComponent::StopDissolve(UPrimitiveComponent* Component)
{
	int32 Index = FindComponent(Component);
	if (Index != INDEX_NONE)
	{
		Dissolves[Index].bIsRunning = false;
	}
}

void UCDissolveComponent::RestoreComponentMaterials(int32 Index)
{
	FDissolve* Dissolve = &Dissolves[Index];
	CLOG_ERROR_CHECK_RETURN(Dissolve);

	for (int32 MaterialIndex = 0; MaterialIndex < Dissolve->Component->GetNumMaterials() - 1; MaterialIndex++)
	{
		Dissolve->Component->SetMaterial(MaterialIndex, Dissolve->Materials[MaterialIndex]);
	}
}

int32 UCDissolveComponent::FindComponent(UPrimitiveComponent* Component)
{
	for (int32 Index = 0; Index < Dissolves.Num(); Index++)
	{
		if (Dissolves[Index].Component == Component)
		{
			return Index;
		}
	}

	return INDEX_NONE;
}

void UCDissolveComponent::RemoveComponent(int32 Index)
{
	Dissolves.RemoveAt(Index);
}

void UCDissolveComponent::Dissolve()
{
	bool bKeepDissolving = false;

	for (int32 Index = 0; Index < Dissolves.Num(); Index++)
	{
		FDissolve* Dissolve = &Dissolves[Index];
		if (Dissolve->Component != nullptr)
		{
			if (Dissolve->bIsRunning)
			{
				float NewValue = UKismetMathLibrary::FInterpTo_Constant(Dissolve->Value, Dissolve->bReverse, GetWorld()->GetDeltaSeconds(), DissolveInterpSpeed);
				Dissolve->Value = NewValue;

				// Check the value reached destination
				if (FMath::IsNearlyEqual(NewValue, static_cast<float>(!Dissolve->bReverse), 0.0001f))
				{
					if (Dissolve->bReverse)
					{
						RestoreComponentMaterials(Index);
						OnDissolveFinished.Broadcast(Dissolve->Component, Dissolve->bReverse);
						RemoveComponent(Index);
						bKeepDissolving = true;
					}
					else
					{
						Dissolve->bIsRunning = false;
					}
				}
				else
				{
					bKeepDissolving = true;
					for (int32 DissolveMaterialIndex = 0; DissolveMaterialIndex < Dissolve->DissolveMaterials.Num(); DissolveMaterialIndex++)
					{
						Dissolve->DissolveMaterials[DissolveMaterialIndex]->SetScalarParameterValue(DissolveValueName, NewValue);
					}
				}
			}
		}
		else
		{
			RemoveComponent(Index);
		}
	}

	if (bKeepDissolving == false)
	{
		GetWorld()->GetTimerManager().ClearTimer(DissolveTimerHandle);
	}
}