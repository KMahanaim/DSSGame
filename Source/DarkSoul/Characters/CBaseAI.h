// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once
#include "CoreMinimal.h"
#include "DarkSoul/Characters/CCombatCharacter.h"
#include "DarkSoul/Interfaces/I_CTargetAble.h"
#include "CBaseAI.generated.h"

class UWidgetComponent;
class UCPatrolComponent;
class UTextRenderComponent;

UCLASS()
class DARKSOUL_API ACBaseAI : public ACCombatCharacter, public II_CTargetAble
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly, Category = "Life", meta = (AllowPrivateAccess = "true"))
		float DestroyTime = 8.0f;

	UPROPERTY(VisibleAnywhere, Category = "Widget", meta = (AllowPrivateAccess = "true"))
		UWidgetComponent* StatBar;

	UPROPERTY(VisibleAnywhere, Category = "Widget", meta = (AllowPrivateAccess = "true"))
		UWidgetComponent* TargetMark;

	UPROPERTY(VisibleAnywhere, Category = "Component", meta = (AllowPrivateAccess = "true"))
		UCPatrolComponent* PatrolPath;

	UPROPERTY(VisibleAnywhere, Category = "Component", meta = (AllowPrivateAccess = "true"))
		UTextRenderComponent* BehaviorText;

//////////////////////////////////////////////////////////////////////////////////////////////////
// General Functions
//////////////////////////////////////////////////////////////////////////////////////////////////
public:
	ACBaseAI();

protected:
	virtual void BeginPlay() override;

	virtual void Death() final;

public:
	virtual bool TakeDamage(const FHitData& HitData, EAttackResult& OutResultType) final;

	/** Action */
	virtual float MeleeAttackAction(EMeleeAttackType AttackType) final;

private:
	void HideWidget();

protected:
	/* Effect Play */
	virtual void Stunned() final;
	virtual void BackAttackDown() final;
	virtual void Impact() final;
	virtual void Parried() final;

//////////////////////////////////////////////////////////////////////////////////////////////////
// Get, Set Functions
//////////////////////////////////////////////////////////////////////////////////////////////////
public:
	virtual FRotator GetDesiredRotation() override;

	UCPatrolComponent* GetPatrolComponent() const { return PatrolPath; }
	virtual FGenericTeamId GetGenericTeamId() const { return FGenericTeamId(2); }

//////////////////////////////////////////////////////////////////////////////////////////////////
// Delegate Bind Functions
//////////////////////////////////////////////////////////////////////////////////////////////////
private:
	void OnInCombatChanged(bool bIsInCombat);

	virtual void OnSelected() override;
	virtual void OnDeSelected() override;
	virtual const bool IsTargetAble() override;

public:
	void OnBehaviorChanged(FString NewBehavior);
};