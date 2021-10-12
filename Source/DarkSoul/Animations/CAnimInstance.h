// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once
#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "DarkSoul/Enumerations/CEWeaponType.h"
#include "DarkSoul/Components/CInverseKinemeticsComponent.h"
#include "CAnimInstance.generated.h"

class ACItemWeapon;
class ACCombatCharacter;
class UCEquipmentComponent;

/**
 * �𸮾� �����Ϳ��� Ư�� ���� �����ϱ� ���� Ŭ������.
 * ĳ������ �ӵ�, ���� ���� �����Ѵ�. ��, Ư�� ���� �����̴�.
 */
UCLASS()
class DARKSOUL_API UCAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
private:
	UPROPERTY(BlueprintReadOnly, Category = "State", meta = (AllowPrivateAccess = "true"))
		bool bIsFalling = false;

	UPROPERTY(BlueprintReadOnly, Category = "State", meta = (AllowPrivateAccess = "true"))
		bool bIsInCombat = false;

	UPROPERTY(BlueprintReadOnly, Category = "State", meta = (AllowPrivateAccess = "true"))
		EWeaponType WeaponType = EWeaponType::NONE;

	UPROPERTY(BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
		float Speed = 0.0f;

	UPROPERTY(BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
		float Direction = 0.0f;

	UPROPERTY(BlueprintReadOnly, Category = "IK", meta = (AllowPrivateAccess = "true"))
		FInverseKinemetics IK;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "State", meta = (AllowPrivateAccess = "true"))
		float BlockAlpha = 0.0f;

public:
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

private:
	void OnIsInCombatChange(const bool bNewIsInCombat);
	void OnWeaponChanged(ACItemWeapon* NewWeapon);

protected:
	TWeakObjectPtr<ACCombatCharacter> Owner;
	TWeakObjectPtr<UCEquipmentComponent> OwnerEquipmentManager;
	TWeakObjectPtr<UCInverseKinemeticsComponent> OwnerIK;
};