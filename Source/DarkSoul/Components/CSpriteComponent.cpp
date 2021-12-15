// Copyright Epic Games, Inc. All Rights Reserved.
#include "CSpriteComponent.h"
#include "DarkSoul/_Utility/CLog.h"

/// Unreal Engine
#include "PaperFlipbook.h"
#include "PaperSprite.h"

UCSpriteComponent::UCSpriteComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UCSpriteComponent::BeginPlay()
{
	Super::BeginPlay();

	PlayTime = 0.0f;
}

void UCSpriteComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);


}

UPaperSprite* UCSpriteComponent::GetCurrentSprite() const
{
	return PaperFlipbook->GetSpriteAtFrame(CurrentKeyFrame);
}

UPaperSprite* UCSpriteComponent::GetSprite(int32 KeyFrame) const
{
	return nullptr;
}