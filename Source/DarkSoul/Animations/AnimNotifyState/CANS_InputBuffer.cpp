// Copyright Epic Games, Inc. All Rights Reserved.
#include "CANS_InputBuffer.h"
#include "DarkSoul/_Utility/CLog.h"
#include "DarkSoul/Components/CInputBufferComponent.h"

/// Unreal Engine
#include "Components/SkeletalMeshComponent.h"

FString UCANS_InputBuffer::GetNotifyName_Implementation() const
{
	return L"Input Buffer";
}

void UCANS_InputBuffer::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);
	CLOG_CHECK_RETURN(MeshComp->GetOwner());

	UCInputBufferComponent* InputBuffer = Cast<UCInputBufferComponent>(MeshComp->GetOwner()->GetComponentByClass(UCInputBufferComponent::StaticClass()));
	if (InputBuffer != nullptr)
	{
		InputBuffer->OpenInputBuffer();
	}
}

void UCANS_InputBuffer::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);
	CLOG_CHECK_RETURN(MeshComp->GetOwner());

	UCInputBufferComponent* InputBuffer = Cast<UCInputBufferComponent>(MeshComp->GetOwner()->GetComponentByClass(UCInputBufferComponent::StaticClass()));
	if (InputBuffer != nullptr)
	{
		InputBuffer->CloseInputBuffer();
	}
}