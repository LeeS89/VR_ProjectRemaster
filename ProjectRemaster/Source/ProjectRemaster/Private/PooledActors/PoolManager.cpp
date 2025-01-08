// Fill out your copyright notice in the Description page of Project Settings.


#include "PooledActors/PoolManager.h"
#include "Projectiles/BulletBase.h"
#include "Projectiles/BaseBullet.h"
#include "PooledActors/PooledParticleEffect.h"

// Sets default values
APoolManager::APoolManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

APoolManager::~APoolManager()
{
	UE_LOG(LogTemp, Warning, TEXT("APoolManager is being destroyed"));
}



// Called when the game starts or when spawned
void APoolManager::BeginPlay()
{
	Super::BeginPlay();

	if (BulletClass)
	{
		BulletPool = new TObjectPool<ABulletBase>(BulletPoolSize, BulletClass, GetWorld());
		BulletPool->InitializePool();
	}

	if (BulletTypeClass)
	{
		BulletToPool = new TObjectPool<ABaseBullet>(BulletPoolSize, BulletTypeClass, GetWorld());
		BulletToPool->InitializePool();
	}

	if (ParticleClass)
	{
		ParticlePool = new TObjectPool<APooledParticleEffect>(ParticlePoolsize, ParticleClass, GetWorld());
		ParticlePool->InitializePool();
	}
	
}

void APoolManager::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (BulletPool)
	{
		delete BulletPool;
		BulletPool = nullptr;
	}

	if (BulletToPool)
	{
		delete BulletToPool;
		BulletToPool = nullptr;
	}

	if (ParticlePool)
	{
		delete ParticlePool;
		ParticlePool = nullptr;
	}
}

// Called every frame
void APoolManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

#pragma region Bullet Region


ABulletBase* APoolManager::GetBullet()
{
	if (!BulletPool) { return nullptr; }

	ABulletBase* Bullet{ BulletPool->GetObjectFromPool() };
	if (Bullet)
	{
		Bullet->OnBulletExpired.AddDynamic(this, &APoolManager::ReturnBulletToPool);
	}
	return Bullet;
}

ABaseBullet* APoolManager::GetBaseBullet()
{
	if (!BulletPool) { return nullptr; }

	ABaseBullet* Bullet{ BulletToPool->GetObjectFromPool() };
	if (Bullet)
	{
		Bullet->OnBulletHasExpired.AddDynamic(this, &APoolManager::ReturnBaseBulletToPool);
	}
	return Bullet;
}



void APoolManager::ReturnBulletToPool(ABulletBase* Bullet)
{
	
	if (!BulletPool || !Bullet) { return; }

	BulletPool->ReturnObjectToPool(Bullet);
	Bullet->OnBulletExpired.RemoveDynamic(this, &APoolManager::ReturnBulletToPool);

}
void APoolManager::ReturnBaseBulletToPool(ABaseBullet* Bullet)
{
	UE_LOG(LogTemp, Error, TEXT("BULLET SHOULD BE INVISIBLE NOW FROM HERE IN POOL"));
	if (!BulletToPool || !Bullet) { return; }
	
	BulletToPool->ReturnObjectToPool(Bullet);
	Bullet->OnBulletHasExpired.RemoveDynamic(this, &APoolManager::ReturnBaseBulletToPool);
}
#pragma endregion

APooledParticleEffect* APoolManager::GetParticle()
{
	if (!ParticlePool) { return nullptr; }

	APooledParticleEffect* Particle{ ParticlePool->GetObjectFromPool() };
	if (Particle)
	{
		Particle->OnParticleExpired.AddDynamic(this, &APoolManager::ReturnParticleToPool);
	}
	return Particle;
}

void APoolManager::ReturnParticleToPool(APooledParticleEffect* Particle)
{
	if (!ParticlePool || !Particle) { return; }
	ParticlePool->ReturnObjectToPool(Particle);
	Particle->OnParticleExpired.RemoveDynamic(this, &APoolManager::ReturnParticleToPool);
}

