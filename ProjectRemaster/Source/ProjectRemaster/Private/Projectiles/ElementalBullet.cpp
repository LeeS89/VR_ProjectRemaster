// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectiles/ElementalBullet.h"
#include "Particles/ParticleSystemComponent.h"

AElementalBullet::AElementalBullet()
{
	Trail = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particle System Component"));
	Trail->SetupAttachment(RootComponent);
}

void AElementalBullet::ToggleActiveState(bool bActive, const FVector& SpawnLocation, const FRotator& SpawnRotation, AActor* NewOwner, APawn* NewInstigator)
{
	Super::ToggleActiveState(bActive, SpawnLocation, SpawnRotation, NewOwner, NewInstigator);

	Trail->SetVisibility(bActive);
	if (bActive)
	{	
		Trail->Activate(true);
	}
	else
	{
		Trail->Deactivate();
	}
}
