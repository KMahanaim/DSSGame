// Copyright Epic Games, Inc. All Rights Reserved.
#include "CCollisionHandlerComponent.h"
#include "_Utility/CLog.h"

/// Unreal Engine
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "Components/StaticMeshComponent.h"

UCCollisionHandlerComponent::UCCollisionHandlerComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UCCollisionHandlerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFuncion)
{
	// 충돌 트레이스를 실행하기 전에 데이터들을 점검하고 값들을 정상화 시킨다.
	// 충돌 검사 최초에만 작동하면 된다.
	if (bCollisionActivity)
	{
		if (bTraceActivity == false)
		{
			UpdateLastSocketLocation();
		}
		else
		{
			UpdateTrace();
			UpdateLastSocketLocation();
		}
	}
}

void UCCollisionHandlerComponent::AddMeshCollision(const USceneComponent* NewWeaponMesh, const FWeaponSocket* WeaponSocket)
{
	CLOG_ERROR_CHECK_RETURN(NewWeaponMesh);

	FMeshCollision NewMeshCollision;
	NewMeshCollision.Mesh = NewWeaponMesh;
	NewMeshCollision.LineTraceSocketName = WeaponSocket->LineTraceSocketName;
	NewMeshCollision.NumberOfLineTraceSocket = WeaponSocket->NumberOfLineTraceSocket;
	NewMeshCollision.SocketLastLocations.Init(FVector::ZeroVector, WeaponSocket->NumberOfLineTraceSocket);

	MeshCollisions.Emplace(NewMeshCollision);
}

void UCCollisionHandlerComponent::ActivateCollision()
{
	ClearHitActorArray();
	bCollisionActivity = true;
	OnCollisionActivated.Broadcast();
}

void UCCollisionHandlerComponent::DeActivateCollision()
{
	bTraceActivity = false;
	bCollisionActivity = false;
	OnCollisionDeActivated.Broadcast();
}

/** 
 * 최초 충돌 검사 전에 한 번 호출 된다
 * 충돌 검사 중에는 매번 호출된다.
 */
void UCCollisionHandlerComponent::UpdateLastSocketLocation()
{
	for (uint8 MeshIndex = 0; MeshIndex < MeshCollisions.Num(); MeshIndex++)
	{
		const USceneComponent* Mesh = MeshCollisions[MeshIndex].Mesh.Get();
		for (uint8 SocketIndex = 0; SocketIndex < MeshCollisions[MeshIndex].NumberOfLineTraceSocket; SocketIndex++)
		{
			FName SocketName = *(MeshCollisions[MeshIndex].LineTraceSocketName.ToString() + FString::FromInt(SocketIndex));
			MeshCollisions[MeshIndex].SocketLastLocations[SocketIndex] = Mesh->GetSocketLocation(SocketName);
		}
	}

	bTraceActivity = true;
}

void UCCollisionHandlerComponent::UpdateTrace()
{
	for (uint8 MeshIndex = 0; MeshIndex < MeshCollisions.Num(); MeshIndex++)
	{
		const USceneComponent* Mesh = MeshCollisions[MeshIndex].Mesh.Get();
		for (uint8 SocketIndex = 0; SocketIndex < MeshCollisions[MeshIndex].NumberOfLineTraceSocket; SocketIndex++)
		{
			FName SocketName = *(MeshCollisions[MeshIndex].LineTraceSocketName.ToString() + FString::FromInt(SocketIndex));
			PerformCollisionTraceLine(MeshCollisions[MeshIndex].SocketLastLocations[SocketIndex], Mesh->GetSocketLocation(SocketName));
		}
	}
}

bool UCCollisionHandlerComponent::PerformCollisionTraceLine(const FVector& StartLocation, const FVector& EndLocation)
{
	TArray<FHitResult> HitResults;
	FCollisionQueryParams CollisionQueryParams;
	CollisionQueryParams.AddIgnoredActor(GetOwner());

	// Draw Debug
	if (bDrawDebugLine)
	{
		DrawDebugLine(GetWorld(), StartLocation, EndLocation, DebugColor, false, 2.0f, 0, DebugLineThickness);
	}

	// Line Trace
	GetWorld()->LineTraceMultiByChannel
	(
		HitResults,
		StartLocation,
		EndLocation,
		ECollisionChannel::ECC_GameTraceChannel1,
		CollisionQueryParams
	);

	// Check the result
	if (HitResults.Num() > 0)
	{
		for (auto& HitResult : HitResults)
		{
			// Already Contains the actor
			if (HitActors.Contains(HitResult.GetActor())) continue;

			HitActors.Emplace(HitResult.GetActor());
			OnHit.Broadcast(HitResult);
		}
		return true;
	}

	return false;
}

void UCCollisionHandlerComponent::ClearHitActorArray()
{
	for (int32 Index = HitActors.Num() - 1; Index >= 0; Index--)
	{
		HitActors.RemoveAt(Index);
	}

	if (HitActors.Num() > 0)
	{
		CLOG_FUNC_TEXT(L"Array was not removed completely");
	}
}