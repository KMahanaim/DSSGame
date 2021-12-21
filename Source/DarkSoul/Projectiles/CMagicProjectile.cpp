// Copyright Epic Games, Inc. All Rights Reserved.
#include "CMagicProjectile.h"
#include "DarkSoul/_Utility/CLog.h"
#include "DarkSoul/Interfaces/I_CCanBeAttacked.h"

/// Unreal Engine
#include "TimerManager.h"
#include "Engine/World.h"
#include "Components/SceneComponent.h"
#include "Components/CapsuleComponent.h"
#include "Particles/ParticleSystemComponent.h"

ACMagicProjectile::ACMagicProjectile()
{
	PrimaryActorTick.bCanEverTick = true;

	// 컴포넌트 생성
	Scene = CreateDefaultSubobject<USceneComponent>(FName("Scene"));
	Particle = CreateDefaultSubobject<UParticleSystemComponent>(FName("Particle"));
	CapsuleCollision = CreateDefaultSubobject<UCapsuleComponent>(FName("Capsule"));

	// 컴포넌트 연결
	SetRootComponent(Scene);
	CapsuleCollision->AttachToComponent(Scene, FAttachmentTransformRules::KeepRelativeTransform);
	Particle->AttachToComponent(Scene, FAttachmentTransformRules::KeepRelativeTransform);

	CapsuleCollision->SetCapsuleHalfHeight(450.0f);
	CapsuleCollision->SetCapsuleRadius(200.0f);
}

void ACMagicProjectile::BeginPlay()
{
	Super::BeginPlay();

	// 델리게이트 바인딩
	{
		CapsuleCollision->OnComponentBeginOverlap.AddDynamic(this, &ACMagicProjectile::OnBeginOverlap);
		CapsuleCollision->OnComponentEndOverlap.AddDynamic(this, &ACMagicProjectile::OnEndOverlap);
	}
}

void ACMagicProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector Movement = GetActorLocation();
	Movement += GetActorForwardVector() * Speed * DeltaTime;
	SetActorRelativeLocation(Movement);
}

void ACMagicProjectile::Fire(FRotator SetDirection)
{
	SetLifeSpan(LifeTime);
	SetActorRelativeRotation(SetDirection.Quaternion());
	GetWorld()->GetTimerManager().SetTimer(DamageTimerHandle, this, &ACMagicProjectile::TickDamage, Interval, true);
}

void ACMagicProjectile::TickDamage()
{
	for (int32 Index = 0; Index < OverlapActors.Num(); Index++)
	{
		II_CCanBeAttacked* DamageInterface = Cast<II_CCanBeAttacked>(OverlapActors[Index]);

		FHitData HitData;
		HitData.bCanBeBlocked = false;
		HitData.bCanBeParried = false;
		HitData.bCanReceiveImpact = false;
		HitData.Damage = Damage;
		HitData.DamageCauser = GetOwner();

		EAttackResult Result;
		DamageInterface->TakeDamage(HitData, Result);
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// UFunction
//////////////////////////////////////////////////////////////////////////////////////////////////
void ACMagicProjectile::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Cast<II_CCanBeAttacked>(OtherActor) != nullptr)
	{
		OverlapActors.Emplace(OtherActor);
	}
}

void ACMagicProjectile::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (Cast<II_CCanBeAttacked>(OtherActor) != nullptr)
	{
		OverlapActors.Remove(OtherActor);
	}
}