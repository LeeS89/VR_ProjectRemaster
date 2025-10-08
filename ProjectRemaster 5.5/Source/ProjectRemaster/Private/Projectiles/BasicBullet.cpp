// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectiles/BasicBullet.h"

ABasicBullet::ABasicBullet()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ABasicBullet::BeginPlay()
{
	Super::BeginPlay();
}

void ABasicBullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABasicBullet::ToggleActiveState(bool bActive, const FVector& SpawnLocation, const FRotator& SpawnRotation, AActor* NewOwner, APawn* NewInstigator)
{
	Super::ToggleActiveState(bActive, SpawnLocation, SpawnRotation, NewOwner, NewInstigator);
}


void ABasicBullet::OnDeflected_Implementation(const FVector& DeflectionLocation, const FRotator& DeflectionRotation, APawn* NewInstigator)
{
	Super::OnDeflected_Implementation(DeflectionLocation, DeflectionRotation, nullptr);

	OnDeflectedDelegate.Broadcast(true, DeflectionLocation, DeflectionRotation);
}
