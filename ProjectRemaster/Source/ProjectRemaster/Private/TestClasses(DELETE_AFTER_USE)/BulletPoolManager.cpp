// Fill out your copyright notice in the Description page of Project Settings.


#include "TestClasses(DELETE_AFTER_USE)/BulletPoolManager.h"
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

	if (!IsValid(BulletType)) { return; }

	InitializePool();
	
}

void ABulletPoolManager::InitializePool()
{
	for (int32 i = 0; i < PoolSize; ++i)
	{
		ABulletBase* BulletToSpawn = GetWorld()->SpawnActor<ABulletBase>(BulletType, FVector::ZeroVector, FRotator::ZeroRotator);
		if (BulletType)
		{
			BulletToSpawn->SetIsInUse(false);
			BulletToSpawn->ToggleActiveState(false, GetActorLocation(), GetActorRotation());
			BulletPool.Add(BulletToSpawn);
		}
	}
}

ABulletBase* ABulletPoolManager::GetBulletFromPool()
{
	for (ABulletBase* Bullet : BulletPool)
	{
		if (!Bullet->IsInUse())
		{
			//Bullet->OnBulletExpired.AddDynamic(this, &ABulletPoolManager::ReturnBulletToPool);
			Bullet->SetIsInUse(true);
		
			return Bullet;
		}
	}
	return nullptr;
}

void ABulletPoolManager::ReturnBulletToPool(ABulletBase* Bullet)
{
	if (!Bullet) { return; }

	//Bullet->OnBulletExpired.RemoveDynamic(this, &ABulletPoolManager::ReturnBulletToPool);
	Bullet->ToggleActiveState(false);
	Bullet->SetIsInUse(false);
}



