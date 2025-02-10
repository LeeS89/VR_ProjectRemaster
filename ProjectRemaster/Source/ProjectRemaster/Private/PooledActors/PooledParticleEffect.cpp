// Fill out your copyright notice in the Description page of Project Settings.


#include "PooledActors/PooledParticleEffect.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"


// Sets default values
APooledParticleEffect::APooledParticleEffect()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	ParticleSystem = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Niagara System"));
	ParticleSystem->SetupAttachment(RootComponent);


	ParticleSystem->OnSystemFinished.AddDynamic(this, &APooledParticleEffect::OnExpired);

	
}

// Called when the game starts or when spawned
void APooledParticleEffect::BeginPlay()
{
	Super::BeginPlay();

	if (ImpactParticles)
	{
		ParticleSystem->SetAsset(ImpactParticles);
		ParticleSystem->Deactivate();
	}
	
}

void APooledParticleEffect::OnExpired(UNiagaraComponent* PSystem)
{
	if (PSystem == ParticleSystem)
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

void APooledParticleEffect::ToggleActiveState(bool bActive, const FVector& SpawnLocation, const FRotator& SpawnRotation, AActor* NewOwner, APawn* NewInstigator)
{
	SetActorLocation(SpawnLocation);
	SetActorRotation(SpawnRotation);
	SetActorHiddenInGame(!bActive);
	ParticleSystem->SetWorldLocation(SpawnLocation);
	

	if (ParticleSystem)
	{
		if (bActive)
		{
			ParticleSystem->Activate(true);
		}
		else
		{
			ParticleSystem->Deactivate();
			ParticleSystem->SetActive(false, true);
		}
	}
}



