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
		/** 무기가 액터에 부착될 소켓 이름을 가지고 있다. */
		FWeaponSocket WeaponSocket;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
		/** 쌍수인지 아닌지 판단할 수 있습니다. */
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
	 * 무기가 만약 쌍수일 경우 무기 액터를 2개 스폰시키고, 반드시 서로 연결해야 한다.
	 * 연결은 양방향으로 이뤄지기 때문에 2개 중 하나만 사용자가 알고 있어도 잃어버리지 않는다.
	 * @param LinkWeapon 연결시킬 무기의 포인터를 넘겨주면 된다.
	*/
	void ToLinkWeapon(ACItemWeapon* LinkWeapon);
	/** 
	 * 무기 변경을 할 때, 수납은 보이지 않게 하며, 장착할 때는 무기를 다시 보이게 해준다.
	 * @param bDrawWeapon true : 무기를 든다, false : 무기를 수납한다.
	*/
	void VisibleWeapon();
	void InVisibleWeapon();

	void SimulatePhysics();

private:
	TWeakObjectPtr<ACItemWeapon> LinkedWeapon = nullptr;
};