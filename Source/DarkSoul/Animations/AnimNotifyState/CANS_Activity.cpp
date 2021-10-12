// Copyright Epic Games, Inc. All Rights Reserved.
#include "CANS_Activity.h"
#include "DarkSoul/_Utility/CLog.h"
#include "DarkSoul/Components/CStateManagerComponent.h"

/// Unreal Engine
#include "Components/SkeletalMeshComponent.h"

FString UCANS_Activity::GetNotifyName_Implementation() const
{
	switch (Activity)
	{
		case EActivity::NONE :
		{
			return FString(L"None");
		}
			break;
		case EActivity::IS_BLOCKING_PRESSED:
		{
			return FString(L"Is Blocking Pressed");
		}
			break;
		case EActivity::IS_AIMING_PRESSED:
		{
			return FString(L"Is Aiming Pressed");
		}
			break;
		case EActivity::IS_IMMORTAL:
		{
			return FString(L"Is Immortal");
		}
			break;
		case EActivity::CANT_BE_INTERRUPTED:
		{
			return FString(L"Cant Be Interrupted");
		}
			break;
		case EActivity::IS_LOOKING_FORWARD:
		{
			return FString(L"Is Looking Forward");
		}
			break;
		case EActivity::CAN_PARRY_HIT:
		{
			return FString(L"can Parry Hit");
		}
			break;
		case EActivity::IS_ZOOMING:
		{
			return FString(L"Is Zooming");
		}
			break;
		default:
		{
			return FString(L"None");
		}
			break;
	}
}

void UCANS_Activity::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);
	CLOG_CHECK_RETURN(MeshComp->GetOwner());

	UCStateManagerComponent* stateManager = Cast<UCStateManagerComponent>(MeshComp->GetOwner()->GetComponentByClass(UCStateManagerComponent::StaticClass()));
	if (stateManager != nullptr)
	{
		stateManager->SetActivity(Activity, true);
	}
}

void UCANS_Activity::NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation)
{
	Super::NotifyEnd(MeshComp, Animation);
	CLOG_CHECK_RETURN(MeshComp->GetOwner());

	UCStateManagerComponent* stateManager = Cast<UCStateManagerComponent>(MeshComp->GetOwner()->GetComponentByClass(UCStateManagerComponent::StaticClass()));
	if (stateManager != nullptr)
	{
		stateManager->SetActivity(Activity, false);
	}
}