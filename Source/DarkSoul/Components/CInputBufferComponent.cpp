// Copyright Epic Games, Inc. All Rights Reserved.
#include "CInputBufferComponent.h"
#include "DarkSoul/_Utility/CLog.h"

UCInputBufferComponent::UCInputBufferComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UCInputBufferComponent::ConsumeInputBuffer()
{
	OnInputBufferConsumed.Broadcast(StoredKey);
	StoredKey = EInputBufferKey::NONE;
}

/**
 * 키 바인딩된 함수에서 InputBuffer->UpdateKey() 함수 호출을 하면,
 * Delegate Bind(OnInputBufferConsumed) 된 OnInputBufferConsumed 함수를 호출한다.
 */
void UCInputBufferComponent::UpdateKey(EInputBufferKey Key)
{
	StoredKey = Key;

	if (bIsOpen == false)
	{
		ConsumeInputBuffer();
	}
}

void UCInputBufferComponent::OpenInputBuffer()
{
	bIsOpen = true;
	OnInputBufferOpen.Broadcast();
}

void UCInputBufferComponent::CloseInputBuffer()
{
	bIsOpen = false;

	OnInputBufferClose.Broadcast();
	ConsumeInputBuffer();
}