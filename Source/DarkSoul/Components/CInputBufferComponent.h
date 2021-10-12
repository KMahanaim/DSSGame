// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DarkSoul/Enumerations/CEInputBufferKey.h"
#include "CInputBufferComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnInputBufferOpen);
DECLARE_MULTICAST_DELEGATE(FOnInputBufferClose);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnInputBufferConsumed, EInputBufferKey);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DARKSOUL_API UCInputBufferComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCInputBufferComponent();

	void UpdateKey(EInputBufferKey Key);

	void OpenInputBuffer();
	void CloseInputBuffer();
	void ConsumeInputBuffer();

	const EInputBufferKey GetStoredKey() const { return StoredKey; }

public:
	FOnInputBufferOpen OnInputBufferOpen;
	FOnInputBufferClose OnInputBufferClose;
	FOnInputBufferConsumed OnInputBufferConsumed;

private:
	bool bIsOpen = false;
	EInputBufferKey StoredKey;
};
