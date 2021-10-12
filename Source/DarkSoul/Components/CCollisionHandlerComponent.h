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

	/** ���⸶�� ������ ��ġ�� �ٸ��� ������, ���� �޽��� �ֽ�ȭ �մϴ�. */
	void AddMeshCollision(const USceneComponent* NewWeaponMesh, const FWeaponSocket* WeaponSocket);
	void ResetMeshCollision() { MeshCollisions.Empty(); }

	/** �浹��(���� ����) Ȱ��ȭ �մϴ�. CANS_HitSection���� ȣ���մϴ�. */
	void ActivateCollision();
	/** �浹��(���� �Ұ���) ��Ȱ��ȭ �մϴ�. CANS_HitSection���� ȣ���մϴ�. */
	void DeActivateCollision();

	/** ������ ������ ��ġ�� �ֽ�ȭ �մϴ�. */
	void UpdateLastSocketLocation();
	/** �浹�� Ȱ��ȭ �Ǹ� �𸮾� Trace�� �浹 ���θ� Ȯ���մϴ�. */
	void UpdateTrace();

private:
	/** �浹 Ʈ���̽� ������ �׸��� ���� �Լ��Դϴ�. */
	bool PerformCollisionTraceLine(const FVector& StartLocation, const FVector& EndLocation);
	/** ���ظ� ���� ���� ����Ʈ �ʱ�ȭ */
	void ClearHitActorArray();

public:
	FOnHit OnHit;
	FOnCollisionActivated OnCollisionActivated;
	FOnCollisionDeActivated OnCollisionDeActivated;

private:
	/** Ʈ���̽� ���� Ÿ�̸� �Դϴ�. */
	float Timer = 0.0f;
	/** ���� �� ������ �������� ���� �������� true�� �˴ϴ�. */
	bool bCollisionActivity = false;
	/** �浹 ������ �� �غ� �Ǿ����� Ȯ���ؾ� �մϴ�. */
	bool bTraceActivity = false;
	/** Mesh Collision Sockets and Last Locations */
	TArray<FMeshCollision> MeshCollisions;
	/** Hit Actors */
	TArray<AActor*> HitActors;
};