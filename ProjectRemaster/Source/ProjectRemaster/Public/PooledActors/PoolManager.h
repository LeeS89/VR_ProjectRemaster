// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TObjectPool.h"
#include "PoolManager.generated.h"

class ABulletBase;
class APooledParticleEffect;

UCLASS()
class PROJECTREMASTER_API APoolManager : public AActor
{
	GENERATED_BODY()

private:
	// Bullet Pool
	TObjectPool<ABulletBase>* BulletPool;

	UPROPERTY(EditAnywhere, Category = "Object to Pool")
	TSubclassOf<ABulletBase> BulletClass;

	UPROPERTY(EditAnywhere, Category = "Pool Settings")
	int32 BulletPoolsize{ 50 };

	// Impact Particle Pool
	TObjectPool<APooledParticleEffect>* ParticlePool;

	UPROPERTY(EditAnywhere, Category = "Object to Pool")
	TSubclassOf<APooledParticleEffect> ParticleClass;

	UPROPERTY(EditAnywhere, Category = "Pool Settings")
	int32 ParticlePoolsize{ 20 };
	
public:	

	APoolManager();
	~APoolManager();

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	ABulletBase* GetBullet();

	APooledParticleEffect* GetParticle();

	UFUNCTION()
	void ReturnParticleToPool(APooledParticleEffect* Particle);

	UFUNCTION()
	void ReturnBulletToPool(ABulletBase* Bullet);
};
