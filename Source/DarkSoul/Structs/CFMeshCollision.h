// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once
#include "CoreMinimal.h"
#include "Engine/UserDefinedStruct.h"
#include "CFMeshCollision.generated.h"

USTRUCT(BlueprintType)
struct FMeshCollision
{
	GENERATED_USTRUCT_BODY();

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		TWeakObjectPtr<const USceneComponent> Mesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		FName LineTraceSocketName = FName(L"LineTraceSocket_");

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		uint8 NumberOfLineTraceSocket = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		TArray<FVector> SocketLastLocations;
};

UCLASS()
class DARKSOUL_API UCFMeshCollision : public UUserDefinedStruct
{
	GENERATED_BODY()
	
};
