// Copyright Epic Games, Inc. All Rights Reserved.
#include "CMontageManagerComponent.h"
#include "DarkSoul/_Utility/CLog.h"
#include "DarkSoul/Characters/CCombatCharacter.h"

/// Unreal Engine
#include "Kismet/KismetSystemLibrary.h"

UCMontageManagerComponent::UCMontageManagerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UCMontageManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	AActor* Owner = GetOwner();

	if (Owner != nullptr)
	{
		if (UKismetSystemLibrary::DoesImplementInterface(Cast<UObject>(Owner), UI_CMontageManagerComponent::StaticClass()) == false)
		{
			CLOG_FUNC_TEXT(L"Has Not MontageManager Interface");
		}
	}
}

UAnimMontage* UCMontageManagerComponent::GetMontageforAction(EMontageAction Action, UINT Index/**= 0*/)
{
	UAnimMontage* Result = nullptr;
	LastRequestedAction = Action;

	FMontageAction* MontageAction = GetMontage(Action);
	if (MontageAction != nullptr)
	{
		if (MontageAction->Montages.Num() != 0)
		{
			if (MontageAction->Montages.Num() > static_cast<int32>(Index))
			{
				Result = MontageAction->Montages[Index];
			}
		}
		else
		{
			CLOG_FUNC_TEXT(L"Montages is NULL");
		}
	}
	else
	{
		CLOG_FUNC_TEXT(L"FMontageAction is NULL");
	}

	return Result;
}

int8 UCMontageManagerComponent::GetMontageActionLastIndex(EMontageAction Action) const
{
	FMontageAction* MontageAction = GetMontage(Action);
	if (MontageAction == nullptr)
	{
		return -1;
	}
	else
	{
		return MontageAction->Montages.Num() - 1;
	}
}

FMontageAction* UCMontageManagerComponent::GetMontage(EMontageAction Action) const
{
	FMontageAction* Result = nullptr;
	const UDataTable* MontageDataTable = (Cast<ACCombatCharacter>(GetOwner()))->GetMontages(Action);

	if (MontageDataTable != nullptr)
	{
		const UEnum* enumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EMontageAction"), true);
		if (enumPtr != nullptr)
		{
			FText ActionName = enumPtr->GetDisplayNameTextByIndex(static_cast<int32>(Action));
			Result = MontageDataTable->FindRow<FMontageAction>(FName(*(ActionName.ToString())), ActionName.ToString());
		}
		else
		{
			CLOG_FUNC_TEXT(L"enumPtr is not exist");
		}
	}
	else
	{
		CLOG_FUNC_TEXT(L"Montage DataTable is NULL");
	}

	return Result;
}