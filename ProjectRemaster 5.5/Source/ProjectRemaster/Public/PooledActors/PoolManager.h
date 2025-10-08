// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TObjectPool.h"
#include "Enums/EDamageType.h"
#include "PoolManager.generated.h"

class ABulletBase;
class APooledParticleEffect;
class ABaseBullet;

UCLASS()
class PROJECTREMASTER_API APoolManager : public AActor
{
	GENERATED_BODY()

private:
	// Bullet Pool
	TObjectPool<ABaseBullet>* FireBulletPool;
	TObjectPool<ABaseBullet>* PoisonBulletPool;

	UPROPERTY(EditAnywhere, Category = "Object to Pool")
	TSubclassOf<ABaseBullet> PoisonBulletClass;

	UPROPERTY(EditAnywhere, Category = "Object to Pool")
	TSubclassOf<ABaseBullet> FireBulletClass;

	UPROPERTY(EditAnywhere, Category = "Pool Settings")
	int32 BulletPoolSize{ 50 };

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

	ABaseBullet* GetBullet(TEnumAsByte<EDamageType> BulletType);


	APooledParticleEffect* GetParticle();

	UFUNCTION()
	void ReturnParticleToPool(APooledParticleEffect* Particle);

	UFUNCTION()
	void ReturnBulletToPool(ABaseBullet* Bullet);

};
