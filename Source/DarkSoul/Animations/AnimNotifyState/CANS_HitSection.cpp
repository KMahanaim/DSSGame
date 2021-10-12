// Copyright Epic Games, Inc. All Rights Reserved.
#include "CANS_HitSection.h"
#include "DarkSoul/_Utility/CLog.h"
#include "DarkSoul/Components/CCollisionHandlerComponent.h"

/// Unreal Engine
#include "Components/SkeletalMeshComponent.h"

FString UCANS_HitSection::GetNotifyName_Implementation() const
{
	return L"Hit Section";
}

void UCANS_HitSection::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);
	CLOG_CHECK_RETURN(MeshComp->GetOwner());

	UCCollisionHandlerComponent* CollisionHandler = Cast<UCCollisionHandlerComponent>(MeshComp->GetOwner()->GetComponentByClass(UCCollisionHandlerComponent::StaticClass()));
	CLOG_ERROR_CHECK_RETURN(CollisionHandler);

	CollisionHandler->ActivateCollision();
}

void UCANS_HitSection::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);
	CLOG_CHECK_RETURN(MeshComp->GetOwner());

	UCCollisionHandlerComponent* CollisionHandler = Cast<UCCollisionHandlerComponent>(MeshComp->GetOwner()->GetComponentByClass(UCCollisionHandlerComponent::StaticClass()));
	CLOG_ERROR_CHECK_RETURN(CollisionHandler);

	CollisionHandler->DeActivateCollision();
}