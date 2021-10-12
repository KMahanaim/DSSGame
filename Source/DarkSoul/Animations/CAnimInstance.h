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
 * 언리얼 에디터에서 특정 값을 전달하기 위한 클래스다.
 * 캐릭터의 속도, 방향 등을 전달한다. 즉, 특정 상태 값들이다.
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