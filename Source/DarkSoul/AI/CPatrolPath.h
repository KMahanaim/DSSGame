// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CPatrolPath.generated.h"

class USceneComponent;
class USplineComponent;
class UBillboardComponent;

UCLASS()
class DARKSOUL_API ACPatrolPath : public AActor
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
		bool bCloseLoop;

	UPROPERTY(VisibleAnywhere, Category = "Component", meta = (AllowPrivateAccess = "true"))
		USceneComponent* Scene;

	UPROPERTY(VisibleAnywhere, Category = "Component", meta = (AllowPrivateAccess = "true"))
		UBillboardComponent* Billboard;

	UPROPERTY(VisibleAnywhere, Category = "Component", meta = (AllowPrivateAccess = "true"))
		USplineComponent* PatrolSpline;
	
public:	
	ACPatrolPath();

private:
	virtual void OnConstruction(const FTransform& Transform) override;

public:
	USplineComponent* GetSplineComponent() const { return PatrolSpline; }
};