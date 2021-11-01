// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DarkSoul/Structs/CFDissolve.h"
#include "CDissolveComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnDissolveFinished, UPrimitiveComponent*, bool);

class UMaterialInstance;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DARKSOUL_API UCDissolveComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly, Category = "Dissolve", meta = (AllowPrivateAccess = "true"))
		UMaterialInstance* DissolveMaterial;

	UPROPERTY(EditDefaultsOnly, Category = "Dissolve", meta = (AllowPrivateAccess = "true"))
		FName DissolveValueName = "Amount";

	UPROPERTY(EditDefaultsOnly, Category = "Dissolve", meta = (AllowPrivateAccess = "true"))
		FName DissolveColorName = "Color";

	UPROPERTY(EditDefaultsOnly, Category = "Dissolve", meta = (AllowPrivateAccess = "true"))
		float DissolveInterpSpeed = 1.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Dissolve", meta = (AllowPrivateAccess = "true"))
		float DissolveTimerDelay = 0.016f;

	UPROPERTY(EditDefaultsOnly, Category = "Dissolve", meta = (AllowPrivateAccess = "true"))
		FLinearColor DissolveColor;

	UPROPERTY(EditDefaultsOnly, Category = "Dissolve", meta = (AllowPrivateAccess = "true"))
		TArray<FDissolve> Dissolves;

public:
	UFUNCTION(BlueprintCallable)
		void StartDissolve(UPrimitiveComponent* Component, bool bReverse = false);

public:	
	UCDissolveComponent();

protected:
	virtual void BeginPlay() override;

public:
	void StopDissolve(UPrimitiveComponent* Component);
	void RestoreComponentMaterials(int32 Index);
	int32 FindComponent(UPrimitiveComponent* Component);
	void RemoveComponent(int32 Index);

private:
	void Dissolve();

public:
	FOnDissolveFinished OnDissolveFinished;

private:
	FTimerHandle DissolveTimerHandle;
};