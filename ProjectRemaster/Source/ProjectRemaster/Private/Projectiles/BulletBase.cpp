// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectiles/BulletBase.h"
#include "WeaponComponents/BulletTraceComponent.h"
#include "UtilityClasses/TargetingUtility.h"
#include "PooledActors/PoolManager.h"
#include "PooledActors/PooledParticleEffect.h"
#include <GameFramework/ProjectileMovementComponent.h>
#include <Kismet/GameplayStatics.h>


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
	
	TraceComp = CreateDefaultSubobject<UBulletTraceComponent>(TEXT("Trace Component"));
}



// Called when the game starts or when spawned
void ABulletBase::BeginPlay()
{
	Super::BeginPlay();
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APoolManager::StaticClass(), FoundActors);

	if (FoundActors.Num() > 0)
	{
		ParticlePoolManager = Cast<APoolManager>(FoundActors[0]); // Get the first found pool manager
		UE_LOG(LogTemp, Log, TEXT("Pool Manager found: %s"), *ParticlePoolManager->GetName());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No Pool Manager found in the scene!"));
	}

	//ParticlePoolManager = UGameplayStatics::

	Timer = DestroyTime;
}



// Called every frame
void ABulletBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TimeOut(DeltaTime);

}

void ABulletBase::TimeOut(float DeltaTime)
{
	if (Timer > 0.0f)
	{
		Timer -= DeltaTime;
	}
	else
	{
		OnExpired();
	}
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

		Timer = DestroyTime;

	}
	bShouldPerformTrace = bActive;
	
	SetActorHiddenInGame(!bActive);
	SetActorEnableCollision(bActive);
	SetActorTickEnabled(bActive);

}

void ABulletBase::OnDeflected_Implementation()
{
	//FVector OwnerLocation{ GetOwner()->GetActorLocation() };
	//FVector Location = GetActorLocation();

	//FVector NewNormal{ (OwnerLocation - Location).GetSafeNormal() };

	FVector NewNormal = UTargetingUtility::GetDirectionToTarget(GetOwner(), this);

	float VelocityLength = ProjectileMovementComp->Velocity.Length();

	ProjectileMovementComp->Velocity = NewNormal * VelocityLength;

	PlayHitParticle(true, GetActorLocation(), GetActorRotation());

}

void ABulletBase::PlayHitParticle(bool bActive, const FVector& Location, const FRotator& Rotation)
{
	APooledParticleEffect* HitParticle{ ParticlePoolManager->GetParticle() };

	if (!HitParticle) { return; }

	HitParticle->ToggleActiveState(bActive, Location, Rotation);
}



/// <summary>
/// The bullet Pool Manager listens for this event which returns the bullet to the pool
/// </summary>
void ABulletBase::OnExpired()
{
	OnBulletExpired.Broadcast(this);
}

