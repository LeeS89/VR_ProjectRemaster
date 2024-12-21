// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectiles/LaserBullet.h"
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
}
