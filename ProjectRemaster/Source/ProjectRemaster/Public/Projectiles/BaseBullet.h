// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/DeflectableInterface.h"
#include "Interfaces/PooledObjectInterface.h"
#include "BaseBullet.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBulletHasExpired, ABaseBullet*, Bullet);

UCLASS()
class PROJECTREMASTER_API ABaseBullet : public AActor, public IDeflectableInterface, public IPooledObjectInterface
{
	GENERATED_BODY()

private:

	bool bIsInUse{ false };

	bool bDeflectionHasBeenProcessed{ false };

	UPROPERTY(VisibleAnywhere)
	bool bHasHitBeenProcesed{ false };

protected:

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(
		FOnDeflectedSignature,
		bool, SetActive,
		const FVector&, Location,
		const FRotator&, Rotation
	);

	virtual void NotifyHit(
		UPrimitiveComponent* MyComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		bool bSelfMoved,
		FVector HitLocation,
		FVector HitNormal,
		FVector NormalImpulse,
		const FHitResult& Hit
	) override;

	// Actor Components
	UPROPERTY(BlueprintAssignable)
	FOnDeflectedSignature OnDeflectedDelegate;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UStaticMeshComponent* StaticMeshComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	class UPointLightComponent* PointLightComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	class UBulletMovementComponent* MovementComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	class UBulletCollisionComponent* CollisionComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	class UVFXComponent* VFXComp;

	
public:	
	// Sets default values for this actor's properties
	ABaseBullet();

	UPROPERTY(BlueprintAssignable)
	FOnBulletHasExpired OnBulletHasExpired;

	// Pooled object Interface functions
	virtual bool IsInUse() const override { return bIsInUse; }

	virtual void SetIsInUse(bool InUse) override { bIsInUse = InUse; }

	virtual void ToggleActiveState(bool bActive, const FVector& SpawnLocation = FVector::ZeroVector, const FRotator& SpawnRotation = FRotator::ZeroRotator) override;

	// Deflectable Interface functions
	virtual void OnDeflected_Implementation(const FVector& DeflectionLocation, const FRotator& DeflectionRotation) override;

	virtual bool GetDeflectionHasBeenProcessed() const override;

	virtual void SetDeflectionHasBeenProcessed(bool HasBeenProcessed) override { bDeflectionHasBeenProcessed = HasBeenProcessed; }

	virtual bool GetHitHasBeenProcessed() const override { return bHasHitBeenProcesed; }

	virtual void SetHitHasBeenProcessed(bool HasBeenProcessed) override { bHasHitBeenProcesed = HasBeenProcessed; }

	virtual void FreezeBullet() override;

	UFUNCTION(BlueprintCallable)
	virtual void OnExpired() override;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	void TimeOut(float DeltaTime);

	UPROPERTY(EditAnywhere)
	float DestroyTime{ 1.5f };

	UPROPERTY(VisibleAnywhere)
	float DestroyTimeCountdown{ 0.0f };

	UPROPERTY(EditDefaultsOnly)
	float DamageAmount{ 5.0f };

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
