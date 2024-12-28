// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponComponents/VFXComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include <Kismet/GameplayStatics.h>

// Sets default values for this component's properties
UVFXComponent::UVFXComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UVFXComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UVFXComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UVFXComponent::HandleOverlap(AActor* HitActor, const FVector& HitLocation, const FRotator& HitRotation)
{
	if (!HitActor) { return; }

	if (ActiveParticles.Contains(HitActor))
	{
		UParticleSystemComponent* ParticleComp{ ActiveParticles[HitActor] };
		if (ParticleComp)
		{
			ParticleComp->SetWorldLocationAndRotation(HitLocation, HitRotation);
		}
	}
	else
	{
		UParticleSystemComponent* ParticleComp{ UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(),
			ParticleTemplate,
			HitLocation,
			HitRotation
		) };

		if (ParticleComp)
		{
			ActiveParticles.Add(HitActor, ParticleComp);
		}
	}
}

void UVFXComponent::HandleEndOverlap(AActor* EndOverlapActor)
{
	if (!EndOverlapActor) { return; }

	if (ActiveParticles.Contains(EndOverlapActor))
	{
		UParticleSystemComponent* ParticleComp{ ActiveParticles[EndOverlapActor] };

		if (ParticleComp)
		{
			ParticleComp->Deactivate();
			ParticleComp->DestroyComponent();
		}

		ActiveParticles.Remove(EndOverlapActor);
	}
}

bool UVFXComponent::HasActiveParticles() const
{
	return ActiveParticles.Num() > 0;
}

