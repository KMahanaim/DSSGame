// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once
#include "CoreMinimal.h"
#include "Engine/UserDefinedStruct.h"
#include "CFWeaponSocket.generated.h"

USTRUCT(BlueprintType)
struct FWeaponSocket
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attack Sockets")
		FName RightHand = L"None";

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attack Sockets")
		FName LeftHand = L"None";

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Line Trace Sockets")
		FName LineTraceSocketName = FName(L"LineTraceSocket_");

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Line Trace Sockets")
		uint8 NumberOfLineTraceSocket = 0;
};

UCLASS()
class DARKSOUL_API UCFWeaponSocket : public UUserDefinedStruct
{
	GENERATED_BODY()
	
};
