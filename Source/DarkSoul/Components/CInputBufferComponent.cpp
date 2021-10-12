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
 * Ű ���ε��� �Լ����� InputBuffer->UpdateKey() �Լ� ȣ���� �ϸ�,
 * Delegate Bind(OnInputBufferConsumed) �� OnInputBufferConsumed �Լ��� ȣ���Ѵ�.
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