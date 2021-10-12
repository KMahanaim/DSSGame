// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once
#include "CoreMinimal.h"
#include "DarkSoul/Items/CItemBase.h"
#include "DarkSoul/Structs/CFStat.h"
#include "DarkSoul/Structs/CFWeaponSocket.h"
#include "DarkSoul/Enumerations/CEWeaponType.h"
#include "DarkSoul/Enumerations/CECombatType.h"
#include "CItemWeapon.generated.h"

class UDataTable;

UCLASS()
class DARKSOUL_API ACItemWeapon : public ACItemBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
		/** Root Component */
		USceneComponent* Scene;

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
		USkeletalMeshComponent* Mesh = nullptr;

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
		USkeletalMeshComponent* MeshSub = nullptr;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
		UDataTable* MontagesDataTable = nullptr;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
		FStat Damage;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
		ECombatType CombatType = ECombatType::NONE;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
		EWeaponType WeaponType = EWeaponType::NONE;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
		/** ���Ⱑ ���Ϳ� ������ ���� �̸��� ������ �ִ�. */
		FWeaponSocket WeaponSocket;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
		/** �ּ����� �ƴ��� �Ǵ��� �� �ֽ��ϴ�. */
		bool bIsTwinWeapon = false;

public:
	ACItemWeapon();

protected:
	virtual void BeginPlay() override;

public:	
	FStat GetDamageStat() const { return Damage; }
	ECombatType GetCombatType() const { return CombatType; }
	EWeaponType GetWeaponType() const { return WeaponType; }
	const bool GetIsTwinWeapon() const { return bIsTwinWeapon; }
	const FWeaponSocket* GetWeaponSocket() const { return &WeaponSocket; }
	UDataTable* GetMontagesDataTable() const { return MontagesDataTable; }
	TWeakObjectPtr<ACItemWeapon> GetLinkedWeapon() const { return LinkedWeapon; }
	/** @param bIsSub TRUE Get SubMesh, FALSE Get Mesh */
	const USkeletalMeshComponent* GetMesh(const bool bIsSub = false) const;

	void SetLinkedWeapon(ACItemWeapon* NewLinkedWeapon) { LinkedWeapon = NewLinkedWeapon; }

	/** 
	 * ���Ⱑ ���� �ּ��� ��� ���� ���͸� 2�� ������Ű��, �ݵ�� ���� �����ؾ� �Ѵ�.
	 * ������ ��������� �̷����� ������ 2�� �� �ϳ��� ����ڰ� �˰� �־ �Ҿ������ �ʴ´�.
	 * @param LinkWeapon �����ų ������ �����͸� �Ѱ��ָ� �ȴ�.
	*/
	void ToLinkWeapon(ACItemWeapon* LinkWeapon);
	/** 
	 * ���� ������ �� ��, ������ ������ �ʰ� �ϸ�, ������ ���� ���⸦ �ٽ� ���̰� ���ش�.
	 * @param bDrawWeapon true : ���⸦ ���, false : ���⸦ �����Ѵ�.
	*/
	void VisibleWeapon();
	void InVisibleWeapon();

	void SimulatePhysics();

private:
	TWeakObjectPtr<ACItemWeapon> LinkedWeapon = nullptr;
};