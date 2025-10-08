// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectiles/ElementalBullet.h"
#include "Managers/ProjectileManager.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Particles/ParticleSystemComponent.h"

AElementalBullet::AElementalBullet()
{
	Trail = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particle System Component"));
	Trail->SetupAttachment(RootComponent);

	BulletParticles = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Niagara Component"));
	BulletParticles->SetupAttachment(RootComponent);
}

void AElementalBullet::BeginPlay()
{
	Super::BeginPlay();

	if (BulletEffectSystem)
	{
		//BulletParticles->SetAsset(BulletEffectSystem);
		Trail->Deactivate();
	}
}


void AElementalBullet::ToggleActiveState(bool bActive, const FVector& SpawnLocation, const FRotator& SpawnRotation, AActor* NewOwner, APawn* NewInstigator)
{
	Super::ToggleActiveState(bActive, SpawnLocation, SpawnRotation, NewOwner, NewInstigator);

	//BulletParticles->SetVisibility(bActive);
	Trail->SetVisibility(bActive);
	if (bActive)
	{	
		//BulletParticles->Activate(true);
		Trail->Activate(true);
	}
	else
	{
		//BulletParticles->Deactivate();
		Trail->Deactivate();
	}
	bIsFrozen = false;
}

void AElementalBullet::FreezeBullet()
{
	Super::FreezeBullet();

	if (BulletManager)
	{
		Trail->Deactivate();
		//BulletParticles->Deactivate();
		BulletManager->AddFrozenBullet(DamageType, this, StaticMeshComp);
	}
}

void AElementalBullet::UnFreezeBullet()
{
	Super::UnFreezeBullet();
	if (BulletManager && !bIsFrozen)
	{
		Trail->Activate();
		BulletManager->RemoveFrozenBullet(DamageType, this);
		bIsFrozen = true;
	}
}



