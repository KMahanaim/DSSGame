// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CUW_BossStatBar.generated.h"

class UProgressBar;
class UCExtendedStatComponent;

/**
 * 
 */
UCLASS()
class DARKSOUL_API UCUW_BossStatBar : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

public:
	void Init(UCExtendedStatComponent* ExtendedStatComponent);

private:
	void OnValueChanged(float NewValue, float MaxValue);

private:
	UProgressBar* StatBar = nullptr;
};
