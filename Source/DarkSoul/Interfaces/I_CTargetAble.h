// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once
#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "I_CTargetAble.generated.h"

UINTERFACE(MinimalAPI)
class UI_CTargetAble : public UInterface
{
	GENERATED_BODY()
};

/**
 * Ÿ������ ������ ������Ʈ�� �� �������̽��� ��ӹ޴´�.
 * ������ Ÿ������ �Ұ����ϴ�.
 */
class DARKSOUL_API II_CTargetAble
{
	GENERATED_BODY()
	
public:
	virtual void OnSelected() = 0;
	virtual void OnDeSelected() = 0;
	virtual const bool IsTargetAble() = 0;
};
