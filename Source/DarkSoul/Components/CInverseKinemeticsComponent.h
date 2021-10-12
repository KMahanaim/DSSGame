// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DarkSoul/Structs/CFInverseKinemetics.h"
#include "CInverseKinemeticsComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DARKSOUL_API UCInverseKinemeticsComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Inverse Kinemetics")
		FName LeftFootSocket = "foot_l";

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Inverse Kinemetics")
		FName RightFootSocket = "foot_r";

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Inverse Kinemetics")
		float TraceDistance = 20.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Inverse Kinemetics")
		// 발과 지면 사이의 간격
		float AdjustOffset = 3.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Inverse Kinemetics")
		float HipsInterpSpeed = 17.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Inverse Kinemetics")
		float FeetInterpSpeed = 22.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Inverse Kinemetics")
		bool bDebugDraw = false;

public:
	UCInverseKinemeticsComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	float Trace(FName Socket, FVector& OutImpactNormal);
	FRotator NormalToRotator(FVector Normal);

public:
	FInverseKinemetics& GetIK() { return IK; }

private:
	TWeakObjectPtr<class ACharacter> Character;
	float CapsuleHalfHeight;
	FInverseKinemetics IK;
};
