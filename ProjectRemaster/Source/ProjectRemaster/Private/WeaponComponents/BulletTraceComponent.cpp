// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponComponents/BulletTraceComponent.h"
#include "Projectiles/BulletBase.h"

void UBulletTraceComponent::BeginPlay()
{
	Super::BeginPlay();

	StaticMeshComp = GetOwner()->FindComponentByClass<UStaticMeshComponent>();
}

void UBulletTraceComponent::HandleTraceResults(const TArray<FHitResult>& hitResults)
{
	
}
