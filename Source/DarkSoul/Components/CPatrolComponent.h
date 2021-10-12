// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CPatrolComponent.generated.h"

class ACPatrolPath;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DARKSOUL_API UCPatrolComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleAnywhere, Category = "Patrol", meta = (AllowPrivateAccess = "true"))
		int32 PointIndex = 0;

	UPROPERTY(EditAnywhere, Category = "Patrol", meta = (AllowPrivateAccess = "true"))
		bool bReverseDirection = false;

	UPROPERTY(EditInstanceOnly, Category = "Patrol", meta = (AllowPrivateAccess = "true"))
		ACPatrolPath* PatrolPath;

public:	
	UCPatrolComponent();

public:
	void UpdatePatrolIndex();
	FVector GetSplinePointLocation(const int32 Index) const;

	const int32 GetPointIndex() const { return PointIndex; }
	ACPatrolPath* GetPatrolPath() const { return PatrolPath; }

	const bool IsPatrolPathValid() const;
};