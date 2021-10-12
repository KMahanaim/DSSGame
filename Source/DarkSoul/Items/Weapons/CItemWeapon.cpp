// Copyright Epic Games, Inc. All Rights Reserved.
#include "CItemWeapon.h"
#include "DarkSoul/_Utility/CLog.h"

/// Unreal Engine
#include "Engine/CollisionProfile.h"
#include "Components/SkeletalMeshComponent.h"

ACItemWeapon::ACItemWeapon()
{
	PrimaryActorTick.bCanEverTick = false;

	// Create Components
	{
		Scene = CreateDefaultSubobject<USceneComponent>(FName(L"Scene"));
		Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(FName(L"Mesh"));
		MeshSub = CreateDefaultSubobject<USkeletalMeshComponent>(FName(L"MeshSub"));

		// 설정
		RootComponent = Scene;
		Mesh->AttachToComponent(Scene, FAttachmentTransformRules::KeepRelativeTransform);
		MeshSub->AttachToComponent(Mesh, FAttachmentTransformRules::KeepRelativeTransform);
	}

	// Set Defaults
	{
		// 무기 충돌을 모두 무시시킨다.
		Mesh->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
		MeshSub->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);

		Damage.Type = EStatsType::DAMAGE;
	}
}

void ACItemWeapon::BeginPlay()
{
	Super::BeginPlay();
}

const USkeletalMeshComponent* ACItemWeapon::GetMesh(const bool bIsSub) const
{
	if (bIsSub)
	{
		if (MeshSub->SkeletalMesh != nullptr)
		{
			return MeshSub;
		}
	}
	else
	{
		return Mesh;
	}

	return nullptr;
}

void ACItemWeapon::ToLinkWeapon(ACItemWeapon* LinkWeapon)
{
	if (LinkWeapon == nullptr) return;
	
	// 양방향으로 연결시킨다.
	LinkWeapon->SetLinkedWeapon(this);
	this->SetLinkedWeapon(LinkWeapon);
}

void ACItemWeapon::VisibleWeapon()
{
	SetHidden(false);
	if (Mesh != nullptr)
	{
		Mesh->SetVisibility(true);
	}
	if (bIsTwinWeapon)
	{
		if (LinkedWeapon != nullptr)
		{
			if (LinkedWeapon->Mesh != nullptr)
			{
				LinkedWeapon->Mesh->SetVisibility(true);
			}
			LinkedWeapon->SetHidden(false);
		}
		else
		{
			CLOG_FUNC_TEXT(L"Linked Weapon is Lost, Check the bIsTwinWeapon or Find the Linked Weapon");
		}
	}
}

void ACItemWeapon::InVisibleWeapon()
{
	SetHidden(true);
	if (Mesh != nullptr)
	{
		Mesh->SetVisibility(false);
	}

	if (bIsTwinWeapon)
	{
		if (LinkedWeapon != nullptr)
		{
			if (LinkedWeapon->Mesh != nullptr)
			{
				LinkedWeapon->Mesh->SetVisibility(false);
			}
			LinkedWeapon->SetHidden(true);
		}
		else
		{
			CLOG_FUNC_TEXT(L"Linked Weapon is Lost, Check the bIsTwinWeapon or Find the Linked Weapon");
		}
	}
}

void ACItemWeapon::SimulatePhysics()
{
	Mesh->SetCollisionProfileName(FName("Ragdoll"));
	Mesh->SetSimulatePhysics(true);
	if (MeshSub != nullptr)
	{
		MeshSub->SetCollisionProfileName(FName("Ragdoll"));
		MeshSub->SetSimulatePhysics(true);
	}
}