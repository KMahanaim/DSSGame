// Copyright Epic Games, Inc. All Rights Reserved.
#include "CAnimInstance.h"
#include "DarkSoul/_Utility/CLog.h"
#include "DarkSoul/Items/Weapons/CItemWeapon.h"
#include "DarkSoul/Characters/CCombatCharacter.h"
#include "DarkSoul/Components/CEquipmentComponent.h"

void UCAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	Owner = Cast<ACCombatCharacter>(TryGetPawnOwner());
	CLOG_CHECK_RETURN(Owner);

	UActorComponent* ActorComponent = Owner->GetComponentByClass(UCInverseKinemeticsComponent::StaticClass());
	OwnerIK = Cast<UCInverseKinemeticsComponent>(ActorComponent);
	CLOG_ERROR_CHECK(OwnerIK);

	ActorComponent = Owner->GetEquipmentComponent();
	OwnerEquipmentManager = Cast<UCEquipmentComponent>(ActorComponent);
	CLOG_ERROR_CHECK_RETURN(OwnerEquipmentManager);

	OwnerEquipmentManager->OnInCombatChanged.AddUObject(this, &UCAnimInstance::OnIsInCombatChange);
	OwnerEquipmentManager->OnWeaponChanged.AddUObject(this, &UCAnimInstance::OnWeaponChanged);
}

void UCAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (Owner == nullptr || OwnerIK == nullptr)
	{
		return;
	}
	
	IK = OwnerIK->GetIK();
	//BlockAlpha = Owner->BlockAlpha;
	bIsFalling = Owner->IsFalling();
	Speed = Owner->GetVelocity().Size();
	Direction = CalculateDirection(Owner->GetVelocity(), Owner->GetActorRotation());
}

void UCAnimInstance::OnIsInCombatChange(const bool bNewIsInCombat)
{
	bIsInCombat = bNewIsInCombat;
}

void UCAnimInstance::OnWeaponChanged(ACItemWeapon* NewWeapon)
{
	WeaponType = NewWeapon->GetWeaponType();
}