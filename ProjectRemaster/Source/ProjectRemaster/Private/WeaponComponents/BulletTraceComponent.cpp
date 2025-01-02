// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponComponents/BulletTraceComponent.h"
#include "Projectiles/BulletBase.h"

void UBulletTraceComponent::BeginPlay()
{
	Super::BeginPlay();

	StaticMeshComp = GetOwner()->FindComponentByClass<UStaticMeshComponent>();

	OwnerRef = GetOwner<ABulletBase>();
}

void UBulletTraceComponent::HandleTraceResults(const TArray<FHitResult>& HitResults)
{
	if (HitResults.Num() <= 0) { return; }

	FHitResult HitResult{ HitResults[0] };

	if (!OwnerRef || !OwnerRef->Implements<UDeflectableInterface>()) { return; }

	IDeflectableInterface* BulletInterface = Cast<IDeflectableInterface>(OwnerRef);
	BulletInterface->PlayHitParticle(true, HitResult.ImpactPoint, HitResult.ImpactPoint.Rotation());

	BulletInterface->OnExpired();

}
