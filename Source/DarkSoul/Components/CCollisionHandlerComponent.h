// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DarkSoul/Structs/CFWeaponSocket.h"
#include "DarkSoul/Structs/CFMeshCollision.h"
#include "CCollisionHandlerComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnCollisionActivated);
DECLARE_MULTICAST_DELEGATE(FOnCollisionDeActivated);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnHit, FHitResult);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DARKSOUL_API UCCollisionHandlerComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly, Category = "Trace Line Debug", meta = (AllowPrivateAccess = "true"))
		bool bDrawDebugLine = false;

	UPROPERTY(EditDefaultsOnly, Category = "Trace Line Debug", meta = (AllowPrivateAccess = "true"))
		FColor DebugColor = FColor::Red;

	UPROPERTY(EditDefaultsOnly, Category = "Trace Line Debug", meta = (ClampMax = 5, ClampMin = 0, UIMax = 5, UIMin = 0), meta = (AllowPrivateAccess = "true"))
		float DebugLineThickness = 1.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Trace Line", meta = (AllowPrivateAccess = "true"))
		float TimeInterval = 0.5f;

public:	
	UCCollisionHandlerComponent();

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/** 무기마다 소켓의 위치가 다르기 때문에, 무기 메쉬를 최신화 합니다. */
	void AddMeshCollision(const USceneComponent* NewWeaponMesh, const FWeaponSocket* WeaponSocket);
	void ResetMeshCollision() { MeshCollisions.Empty(); }

	/** 충돌을(공격 가능) 활성화 합니다. CANS_HitSection에서 호출합니다. */
	void ActivateCollision();
	/** 충돌을(공격 불가능) 비활성화 합니다. CANS_HitSection에서 호출합니다. */
	void DeActivateCollision();

	/** 소켓의 마지막 위치를 최신화 합니다. */
	void UpdateLastSocketLocation();
	/** 충돌이 활성화 되면 언리얼 Trace로 충돌 여부를 확인합니다. */
	void UpdateTrace();

private:
	/** 충돌 트레이스 라인을 그리기 위한 함수입니다. */
	bool PerformCollisionTraceLine(const FVector& StartLocation, const FVector& EndLocation);
	/** 피해를 받은 액터 리스트 초기화 */
	void ClearHitActorArray();

public:
	FOnHit OnHit;
	FOnCollisionActivated OnCollisionActivated;
	FOnCollisionDeActivated OnCollisionDeActivated;

private:
	/** 트레이스 간격 타이머 입니다. */
	float Timer = 0.0f;
	/** 공격 중 실제로 데미지가 들어가는 구간에서 true가 됩니다. */
	bool bCollisionActivity = false;
	/** 충돌 추적을 할 준비가 되었는지 확인해야 합니다. */
	bool bTraceActivity = false;
	/** Mesh Collision Sockets and Last Locations */
	TArray<FMeshCollision> MeshCollisions;
	/** Hit Actors */
	TArray<AActor*> HitActors;
};