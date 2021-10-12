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
 * 타겟팅이 가능한 오브젝트는 이 인터페이스를 상속받는다.
 * 없으면 타겟팅이 불가능하다.
 */
class DARKSOUL_API II_CTargetAble
{
	GENERATED_BODY()
	
public:
	virtual void OnSelected() = 0;
	virtual void OnDeSelected() = 0;
	virtual const bool IsTargetAble() = 0;
};
