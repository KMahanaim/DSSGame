// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DarkSoul/Enumerations/CEStatsType.h"
#include "CUW_StatBars.generated.h"

class UTexture2D;
class UCUW_StatBar;
class UCExtendedStatComponent;

/**
 * ÇÃ·¹ÀÌ¾î ½ºÅÈÀ» ¹­¾îµÎ´Â À§Á¬
 */
UCLASS()
class DARKSOUL_API UCUW_StatBars : public UUserWidget
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
		UTexture2D* MPTexture;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
		UTexture2D* SPTexture;

protected:
	virtual void NativeConstruct() override;

public:
	void SetStatBar(const EStatsType TargetType, UCExtendedStatComponent* ExtendedStat);

private:
	TMap<EStatsType, UCUW_StatBar*> StatBars;
};