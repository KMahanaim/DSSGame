// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CSpriteComponent.generated.h"

DECLARE_DELEGATE(FChangeFrame);

class UPaperSprite;
class UPaperFlipbook;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DARKSOUL_API UCSpriteComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "Sprite", meta = (AllowPrivateAccess = "true"))
		UPaperFlipbook* PaperFlipbook;

public:	
	UCSpriteComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
public:
	void SetPaperFlipbook(UPaperFlipbook* NewPaperFlipbook) { PaperFlipbook = NewPaperFlipbook; }

	UPaperSprite* GetCurrentSprite() const;
	UPaperSprite* GetSprite(int32 KeyFrame) const;

public:
	FChangeFrame ChangeFrame;

private:
	int32 CurrentKeyFrame = 0;
	float PlayTime = 0.0f;
};