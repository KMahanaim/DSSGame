// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CGameLoadingComponent.generated.h"

class UTexture2D;
class UPaperSprite;
class UPaperFlipbook;
class UMaterialInterface;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DARKSOUL_API UCGameLoadingComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly, Category = "Loading", meta = (AllowPrivateAccess = "true"))
		UMaterialInterface* LoadingBackground;

	UPROPERTY(EditDefaultsOnly, Category = "Loading", meta = (AllowPrivateAccess = "true"))
		UPaperFlipbook* PaperFlipbook = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Loading", meta = (AllowPrivateAccess = "true"))
		FVector2D DrawSize;

	UPROPERTY(EditDefaultsOnly, Category = "Loading", meta = (AllowPrivateAccess = "true"))
		int NumberOfWidth;

	UPROPERTY(EditDefaultsOnly, Category = "Loading", meta = (AllowPrivateAccess = "true"))
		int NumberOfHeight;

	UPROPERTY(EditDefaultsOnly, Category = "Loading", meta = (AllowPrivateAccess = "true"))
		FVector2D LoadingIconLocation;

public:	
	UCGameLoadingComponent();

protected:
	virtual void BeginPlay() override;

public:	
	void LoadingStart();
	void PaperTurn();

	void GetTextureUV(FVector2D& TopLeft, FVector2D& BottomRight);
	const FVector2D GetDrawSize() const { return DrawSize; };
	UPaperSprite* GetSprite() const;
	UTexture2D* GetResourceTexture2D() const;
	const FVector2D GetLoadingIconLocation() const { return LoadingIconLocation; };
	UMaterialInterface* GetLoadingBackground() const { return LoadingBackground; };

private:
	bool bIsInLoading;
	int32 CurrentKeyFrame;
	float Interval;

	FTimerHandle PaperTurnTimerHandle;
};
