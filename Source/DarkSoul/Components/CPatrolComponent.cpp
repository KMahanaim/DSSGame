// Copyright Epic Games, Inc. All Rights Reserved.
#include "CPatrolComponent.h"
#include "DarkSoul/_Utility/CLog.h"
#include "DarkSoul/AI/CPatrolPath.h"

/// Unreal Engine
#include "Components/SplineComponent.h"

UCPatrolComponent::UCPatrolComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UCPatrolComponent::UpdatePatrolIndex()
{
	if (PatrolPath == nullptr) return;
	USplineComponent* splinePath = PatrolPath->GetSplineComponent();
	if (splinePath == nullptr) return;

	if (bReverseDirection)
	{
		if (PointIndex <= 0)
		{
			if (splinePath->IsClosedLoop())
			{
				PointIndex = splinePath->GetNumberOfSplinePoints() - 1;
				return;
			}
			else
			{
				PointIndex = 1;
				bReverseDirection = false;
				return;
			}
		}
		else
		{
			PointIndex--;
			return;
		}
	}
	else
	{
		// PointIndex가 Spline의 마지막 인덱스와 동일한 경우, Spline이 루프 상태인지 아닌지 확인한다.
		//CLOG_FUNC_TEXT(GetOwner()->GetName() + L" : " + FString::FromInt(splinePath->GetNumberOfSplinePoints()));
		if (PointIndex >= splinePath->GetNumberOfSplinePoints() - 1)
		{
			if (splinePath->IsClosedLoop())
			{
				PointIndex = 0;
			}
			else
			{
				bReverseDirection = true;
				PointIndex = splinePath->GetNumberOfSplinePoints() - 2;
				return;
			}
		}
		else
		{
			PointIndex++;
			return;
		}
	}
}

FVector UCPatrolComponent::GetSplinePointLocation(const int32 Index) const
{
	if (PatrolPath == nullptr) return FVector(0, 0, 0);
	return PatrolPath->GetSplineComponent()->GetLocationAtSplinePoint(Index, ESplineCoordinateSpace::World);
}

const bool UCPatrolComponent::IsPatrolPathValid() const
{
	if (PatrolPath == nullptr) return false;
	return true;
}