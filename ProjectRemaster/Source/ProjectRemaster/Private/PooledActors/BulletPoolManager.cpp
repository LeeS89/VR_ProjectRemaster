// Fill out your copyright notice in the Description page of Project Settings.


#include "PooledActors/BulletPoolManager.h"
#include "Projectiles/BulletBase.h"



// Sets default values
ABulletPoolManager::ABulletPoolManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void ABulletPoolManager::BeginPlay()
{
	Super::BeginPlay();

	if (!IsValid(LaserBullet)) { return; }

	InitializePool();
	
}

void ABulletPoolManager::InitializePool()
{
	for (int32 i = 0; i < PoolSize; ++i)
	{
		ABulletBase* Bullet = GetWorld()->SpawnActor<ABulletBase>(LaserBullet, FVector::ZeroVector, FRotator::ZeroRotator);
		if (Bullet)
		{
			Bullet->SetIsInUse(false);
			Bullet->ToggleActiveState(false, GetActorLocation(), GetActorRotation());
			BulletPool.Add(Bullet);
		}
	}
}

ABulletBase* ABulletPoolManager::GetBulletFromPool()
{
	for (ABulletBase* Bullet : BulletPool)
	{
		if (!Bullet->IsInUse())
		{
			Bullet->OnBulletExpired.AddDynamic(this, &ABulletPoolManager::ReturnBulletToPool);
			Bullet->SetIsInUse(true);
		
			return Bullet;
		}
	}
	return nullptr;
}

void ABulletPoolManager::ReturnBulletToPool(ABulletBase* Bullet)
{
	if (!Bullet) { return; }

	Bullet->OnBulletExpired.RemoveDynamic(this, &ABulletPoolManager::ReturnBulletToPool);
	Bullet->ToggleActiveState(false);
	Bullet->SetIsInUse(false);
}



