// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/DeflectableInterface.h"
#include "Interfaces/PooledObjectInterface.h"
#include "BulletBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBulletExpired, ABulletBase*, Bullet);

UCLASS()
class PROJECTREMASTER_API ABulletBase : public AActor,  public IPooledObjectInterface
{
	GENERATED_BODY()

private:

	bool bIsInUse{ false };

	bool bDeflectionHasBeenProcessed{ false };

	bool bHasHitBeenProcesed{ false };

protected:

	UPROPERTY(EditAnywhere)
	class APoolManager* ParticlePoolManager;

	UPROPERTY(EditAnywhere)
	USceneComponent* RootComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UStaticMeshComponent* StaticMeshComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	class UPointLightComponent* PointLightComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	class UProjectileMovementComponent* ProjectileMovementComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	class UBulletTraceComponent* TraceComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	class UBulletCollisionComponent* CollisionComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	class UVFXComponent* VFXComp;


	
public:	
	// Sets default values for this actor's properties
	ABulletBase();

	UPROPERTY(BlueprintAssignable)
	FOnBulletExpired OnBulletExpired;

	// Interface functions
	virtual bool IsInUse() const override { return bIsInUse; }

	virtual void SetIsInUse(bool InUse) override { bIsInUse = InUse; }

	virtual void ToggleActiveState(bool bActive, const FVector& SpawnLocation = FVector::ZeroVector, const FRotator& SpawnRotation = FRotator::ZeroRotator, AActor* NewOwner = nullptr, APawn* NewInstigator = nullptr) override;

	/*virtual void OnDeflected_Implementation(const FVector& DeflectionLocation, const FRotator& DeflectionRotation) override;

	virtual void PlayHitParticle(bool bActive, const FVector& Location, const FRotator& Rotation) override;

	virtual bool GetDeflectionHasBeenProcessed() const override;

	virtual void SetDeflectionHasBeenProcessed(bool HasBeenProcessed) override { bDeflectionHasBeenProcessed = HasBeenProcessed; }

	virtual void OnExpired() override;*/

	

	//virtual bool GetHitHasBeenProcessed() const override { return bHasHitBeenProcesed; }

	//virtual void SetHitHasBeenProcessed(bool HasBeenProcessed) override { bHasHitBeenProcesed = HasBeenProcessed; }
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

	UPROPERTY(EditDefaultsOnly)
	float DamageAmount{ 5.0f };

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
