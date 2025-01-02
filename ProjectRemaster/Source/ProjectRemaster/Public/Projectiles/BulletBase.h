// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/DeflectableInterface.h"
#include "Interfaces/PooledObjectInterface.h"
#include "BulletBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBulletExpired, ABulletBase*, Bullet);

UCLASS()
class PROJECTREMASTER_API ABulletBase : public AActor, public IDeflectableInterface, public IPooledObjectInterface
{
	GENERATED_BODY()

private:

	bool bIsInUse{ false };


	
public:	
	// Sets default values for this actor's properties
	ABulletBase();

	UPROPERTY(EditAnywhere)
	class APoolManager* ParticlePoolManager;

	UPROPERTY(BlueprintAssignable)
	FOnBulletExpired OnBulletExpired;

	UPROPERTY(EditAnywhere)
	USceneComponent* RootComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UStaticMeshComponent* StaticMeshComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	class UProjectileMovementComponent* ProjectileMovementComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	class UBulletTraceComponent* TraceComp;

	// Interface functions
	virtual bool IsInUse() const override { return bIsInUse; }

	virtual void SetIsInUse(bool InUse) override { bIsInUse = InUse; }

	virtual void ToggleActiveState(bool bActive, const FVector& SpawnLocation = FVector::ZeroVector, const FRotator& SpawnRotation = FRotator::ZeroRotator) override;

	virtual void OnDeflected_Implementation() override;

	virtual void PlayHitParticle(bool bActive, const FVector& Location, const FRotator& Rotation) override;

	virtual void OnExpired() override;
	/*virtual void OnExpired_Implementation() override;*/

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void TimeOut(float DeltaTime);


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bShouldPerformTrace{ false };

	UPROPERTY(EditAnywhere)
	float DestroyTime{ 1.5f };

	UPROPERTY(VisibleAnywhere)
	float Timer{ 0.0f };


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
