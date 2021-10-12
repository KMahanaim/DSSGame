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
 * ���� ���� �� �ִ� ������Ʈ�� ��ӹ޴� �������̽���.
 */
class DARKSOUL_API II_CCanBeAttacked
{
	GENERATED_BODY()

public:
	virtual bool IsAlive() const = 0;
	/**
	 * ������ ������ �Լ��� ȣ��ȴ�.
	 * @param HiData, ���� ���¿� ���� ������ ������ �ִ�.
	 * @param OutResultType, ���� ���� ���� ���� ����� ������.
	 * @return ������ ������ ������ false�� ��ȯ�Ѵ�.
	 */
	virtual bool TakeDamage(const FHitData& HitData, EAttackResult& OutResultType) = 0;
};
