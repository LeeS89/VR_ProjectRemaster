// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectiles/BulletBase.h"
#include <GameFramework/ProjectileMovementComponent.h>



// Sets default values
ABulletBase::ABulletBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root component"));
	RootComponent = RootComp;

	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh Component"));
	StaticMeshComp->SetupAttachment(RootComponent);

	ProjectileMovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement Component"));
	

}



// Called when the game starts or when spawned
void ABulletBase::BeginPlay()
{
	Super::BeginPlay();
	
	
}

// Called every frame
void ABulletBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}




void ABulletBase::ToggleActiveState(bool bActive, const FVector& SpawnLocation, const FRotator& SpawnRotation)
{
	SetActorLocation(SpawnLocation);
	SetActorRotation(SpawnRotation);
	ProjectileMovementComp->SetActive(bActive);

	if (bActive)
	{
		FVector NewVelocity{ SpawnRotation.Vector() * ProjectileMovementComp->InitialSpeed };
		ProjectileMovementComp->Velocity = NewVelocity;
	}
	
	SetActorHiddenInGame(!bActive);
	SetActorEnableCollision(bActive);
	SetActorTickEnabled(bActive);

}

void ABulletBase::OnDeflected_Implementation()
{
	FVector OwnerLocation{ GetOwner()->GetActorLocation() };
	FVector Location = GetActorLocation();

	FVector NewNormal{ (OwnerLocation - Location).GetSafeNormal() };

	float VelocityLength = ProjectileMovementComp->Velocity.Length();

	ProjectileMovementComp->Velocity = NewNormal * VelocityLength;

	/*FVector CurrentVelocity{ ProjectileMovementComp->Velocity };
	FVector ReversedVelocity{ CurrentVelocity * -1.0f };
	FVector NormalizedReversedVelocity{ ReversedVelocity.GetSafeNormal() };

	float VectorLength = CurrentVelocity.Length();

	ProjectileMovementComp->Velocity = NormalizedReversedVelocity * VectorLength;*/
}


/// <summary>
/// The bullet Pool Manager listens for this event which returns the bullet to the pool
/// </summary>
void ABulletBase::OnExpired_Implementation()
{
	OnBulletExpired.Broadcast(this);
}

