// Fill out your copyright notice in the Description page of Project Settings.


#include "PooledActors/PooledParticleEffect.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
APooledParticleEffect::APooledParticleEffect()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	ParticleSystemComp = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particle System Component"));
	ParticleSystemComp->SetupAttachment(RootComponent);

	ParticleSystemComp->bAutoActivate = false;

	ParticleSystemComp->OnSystemFinished.AddDynamic(this, &APooledParticleEffect::OnExpired);
}

// Called when the game starts or when spawned
void APooledParticleEffect::BeginPlay()
{
	Super::BeginPlay();

	if (ParticleSystemComp)
	{
		ParticleSystemComp->Deactivate();
	}
	
}

void APooledParticleEffect::OnExpired(UParticleSystemComponent* FinishedComponent)
{
	if (FinishedComponent == ParticleSystemComp)
	{
		//UE_LOG(LogTemp, Log, TEXT("Particle system finished, returning to pool"));
		OnParticleExpired.Broadcast(this);
	}
}

// Called every frame
void APooledParticleEffect::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APooledParticleEffect::ToggleActiveState(bool bActive, const FVector& SpawnLocation, const FRotator& SpawnRotation)
{
	SetActorLocation(SpawnLocation);
	SetActorRotation(SpawnRotation);
	SetActorHiddenInGame(!bActive);

	if (ParticleSystemComp)
	{
		if (bActive)
		{
			ParticleSystemComp->Activate(true);
		}
		else
		{
			ParticleSystemComp->Deactivate();
			ParticleSystemComp->SetActive(false, true);
		}
	}
}

