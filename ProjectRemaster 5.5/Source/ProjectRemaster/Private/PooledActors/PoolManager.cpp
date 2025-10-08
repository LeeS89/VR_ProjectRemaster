// Fill out your copyright notice in the Description page of Project Settings.


#include "PooledActors/PoolManager.h"
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
	//UE_LOG(LogTemp, Warning, TEXT("APoolManager is being destroyed"));
}



// Called when the game starts or when spawned
void APoolManager::BeginPlay()
{
	Super::BeginPlay();

	if (FireBulletClass)
	{
		FireBulletPool = new TObjectPool<ABaseBullet>(BulletPoolSize, FireBulletClass, GetWorld());
		FireBulletPool->InitializePool();
	}

	if (PoisonBulletClass)
	{
		PoisonBulletPool = new TObjectPool<ABaseBullet>(BulletPoolSize, PoisonBulletClass, GetWorld());
		PoisonBulletPool->InitializePool();
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

	if (FireBulletPool)
	{
		delete FireBulletPool;
		FireBulletPool = nullptr;
	}

	if (PoisonBulletPool)
	{
		delete PoisonBulletPool;
		PoisonBulletPool = nullptr;
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




ABaseBullet* APoolManager::GetBullet(TEnumAsByte<EDamageType> BulletType)
{
	if (BulletPoolSize <= 0) { return nullptr; }

	ABaseBullet* Bullet = nullptr;
	switch (BulletType)
	{
	case EDamageType::Fire:
		Bullet = FireBulletPool->GetObjectFromPool();
		break;
	case EDamageType::Poison:
		Bullet = PoisonBulletPool->GetObjectFromPool();
		break;
	default:
		UE_LOG(LogTemp, Warning, TEXT("Invalid Bullet Type"));
		break;
	}

	//ABaseBullet* Bullet{ FireBulletPool->GetObjectFromPool() };
	if (Bullet)
	{
		Bullet->OnBulletHasExpired.AddDynamic(this, &APoolManager::ReturnBulletToPool);
	}
	return Bullet;
}



void APoolManager::ReturnBulletToPool(ABaseBullet* Bullet)
{
	
	if (!FireBulletPool || !Bullet) { return; }

	FireBulletPool->ReturnObjectToPool(Bullet);
	Bullet->OnBulletHasExpired.RemoveDynamic(this, &APoolManager::ReturnBulletToPool);

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

