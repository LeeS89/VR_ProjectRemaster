// Fill out your copyright notice in the Description page of Project Settings.


#include "TestClasses(DELETE_AFTER_USE)/LaserBullet.h"
#include <GameFramework/ProjectileMovementComponent.h>

ALaserBullet::ALaserBullet()
{
	PrimaryActorTick.bCanEverTick = true;

	/*ProjectileMovementComp->bRotationFollowsVelocity = true;
	ProjectileMovementComp->SetUpdatedComponent(StaticMeshComp);*/
}

void ALaserBullet::BeginPlay()
{
	Super::BeginPlay();
}

void ALaserBullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALaserBullet::ToggleActiveState(bool bActive, const FVector& SpawnLocation, const FRotator& SpawnRotation)
{
	Super::ToggleActiveState(bActive, SpawnLocation, SpawnRotation);

}
