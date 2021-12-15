// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DarkSoul/Enumerations/CEStateType.h"
#include "DarkSoul/Enumerations/CEActivity.h"
#include "CStateManagerComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnActivityChanged, EActivity, bool);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnStateChanged, EStateType, EStateType);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DARKSOUL_API UCStateManagerComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly, Category = "Debug", meta = (AllowPrivateAccess = "true"))
		bool bIsOutLogSetState = false;

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
		TMap<EActivity, bool> Activities;

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
		EStateType CurrentState = EStateType::IDLE;

public:	
	UCStateManagerComponent();

	void SetState(const EStateType NewState);
	void SetActivity(const EActivity Activity, const bool Value);

	const bool GetActivityValue(const EActivity Activity);
	FString GetEStateAsString(EStateType State);
	const EStateType GetState() const { return CurrentState; }

	void TimerResetIdleState();
	void ResetState(float InTime = 0.0f);

	const bool IsStateEqual(const EStateType StateToCompare);

private:
	void SetIdleState();

public:
	FOnStateChanged OnStateChanged;
	FOnActivityChanged OnActivityChanged;

private:
	FTimerHandle ResetStateTimerHandle;
};