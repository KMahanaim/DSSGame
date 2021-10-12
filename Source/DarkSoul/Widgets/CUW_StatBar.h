// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CUW_StatBar.generated.h"

class UProgressBar;
class UCExtendedStatComponent;

/**
 * �÷��̾� HUD�� ��Ÿ�� ĳ���� ���¹�
 */
UCLASS()
class DARKSOUL_API UCUW_StatBar : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

public:
	void Init(UCExtendedStatComponent* ExtendedStatComponent);
	void SetStatBarFillImage(UObject* Texture);
	void SetStatBarFillImage(UTexture2D* Texture);

private:
	void OnValueChanged(float NewValue, float MaxValue);

private:
	UProgressBar* StatBar = nullptr;
};