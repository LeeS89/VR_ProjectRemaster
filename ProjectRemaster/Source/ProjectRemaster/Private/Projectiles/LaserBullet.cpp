// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectiles/LaserBullet.h"
#include "PooledActors/BulletPoolManager.h"
#include <GameFramework/ProjectileMovementComponent.h>

ALaserBullet::ALaserBullet()
{
	PrimaryActorTick.bCanEverTick = true;

	ProjectileMovementComp->bRotationFollowsVelocity = true;
}

void ALaserBullet::BeginPlay()
{
	Super::BeginPlay();
}

void ALaserBullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (DestroyTime >= 0.0f)
	{
		DestroyTime -= DeltaTime;
	}
	else
	{
		OnExpired();
	}
}

void ALaserBullet::ToggleActiveState(bool bActive, const FVector& SpawnLocation, const FRotator& SpawnRotation)
{
	Super::ToggleActiveState(bActive, SpawnLocation, SpawnRotation);

	if (bActive)
	{
		DestroyTime = 5.0f;
	}
}
