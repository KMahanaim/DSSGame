// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once
#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "DarkSoul/Structs/CFHitData.h"
#include "DarkSoul/Enumerations/CEAttackResult.h"
#include "I_CCanBeAttacked.generated.h"

UINTERFACE(MinimalAPI)
class UI_CCanBeAttacked : public UInterface
{
	GENERATED_BODY()
};

/**
 * 공격 받을 수 있는 오브젝트는 상속받는 인터페이스다.
 */
class DARKSOUL_API II_CCanBeAttacked
{
	GENERATED_BODY()

public:
	virtual bool IsAlive() const = 0;
	/**
	 * 공격을 받으면 함수가 호출된다.
	 * @param HiData, 공격 형태에 대한 정보를 가지고 있다.
	 * @param OutResultType, 공격 성공 실패 등의 결과를 가진다.
	 * @return 공격이 통하지 않으면 false를 반환한다.
	 */
	virtual bool TakeDamage(const FHitData& HitData, EAttackResult& OutResultType) = 0;
};
