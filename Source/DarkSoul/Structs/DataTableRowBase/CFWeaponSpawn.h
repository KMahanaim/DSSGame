// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once
#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Engine/UserDefinedStruct.h"
#include "DarkSoul/Enumerations/CEWeaponType.h"
#include "CFWeaponSpawn.generated.h"

class ACItemWeapon;
class UAnimMontage;

/**
 * 
 */
USTRUCT(BlueprintType)
struct FWeaponSpawn : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FName WeaponPath;
};

UCLASS()
class DARKSOUL_API UCFWeaponSpawn : public UUserDefinedStruct
{
	GENERATED_BODY()
	
};
