// Fill out your copyright notice in the Description page of Project Settings.


#include "TestClasses(DELETE_AFTER_USE)/BulletBase.h"
#include "TestClasses(DELETE_AFTER_USE)/BulletTraceComponent.h"
#include "UtilityClasses/TargetingUtility.h"
#include "PooledActors/PoolManager.h"
#include "PooledActors/PooledParticleEffect.h"
#include "Components/PointLightComponent.h"
#include <GameFramework/ProjectileMovementComponent.h>
#include "WeaponComponents/BulletCollisionComponent.h"
#include "WeaponComponents/VFXComponent.h"
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

	PointLightComp = CreateDefaultSubobject<UPointLightComponent>(TEXT("Point Light Component"));
	PointLightComp->SetupAttachment(StaticMeshComp);

	ProjectileMovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement Component"));
	
	TraceComp = CreateDefaultSubobject<UBulletTraceComponent>(TEXT("Trace Component"));

	CollisionComp = CreateDefaultSubobject<UBulletCollisionComponent>(TEXT("Collision Component"));

	VFXComp = CreateDefaultSubobject<UVFXComponent>(TEXT("VFX Component"));

	ProjectileMovementComp->bRotationFollowsVelocity = true;
	ProjectileMovementComp->SetUpdatedComponent(StaticMeshComp);

}



// Called when the game starts or when spawned
void ABulletBase::BeginPlay()
{
	Super::BeginPlay();
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APoolManager::StaticClass(), FoundActors);

	if (FoundActors.Num() <= 0) { return; }

	ParticlePoolManager = Cast<APoolManager>(FoundActors[0]); // Get the first found pool manager
	
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
		
	}
}




void ABulletBase::ToggleActiveState(bool bActive, const FVector& SpawnLocation, const FRotator& SpawnRotation)
{
	SetActorLocation(SpawnLocation);
	SetActorRotation(SpawnRotation);
	ProjectileMovementComp->SetActive(bActive);
	PointLightComp->SetVisibility(bActive);

	if (bActive)
	{
		TraceComp->ResetHitFlag();
		//SetDeflectionHasBeenProcessed(false);
		
		FVector NewVelocity{ SpawnRotation.Vector() * ProjectileMovementComp->InitialSpeed };
		ProjectileMovementComp->Velocity = NewVelocity;

		Timer = DestroyTime;

	}
	bShouldPerformTrace = bActive;
	
	SetActorHiddenInGame(!bActive);
	SetActorEnableCollision(bActive);
	SetActorTickEnabled(bActive);
	//StaticMeshComp->SetSimulatePhysics(bActive);

}



//void ABulletBase::OnDeflected_Implementation(const FVector& DeflectionLocation, const FRotator& DeflectionRotation)
//{
//	//FVector OwnerLocation{ GetOwner()->GetActorLocation() };
//	//FVector Location = GetActorLocation();
//
//	//FVector NewNormal{ (OwnerLocation - Location).GetSafeNormal() };
//
//	FVector NewNormal = UTargetingUtility::GetDirectionToTarget(GetOwner(), this);
//
//	float VelocityLength = ProjectileMovementComp->Velocity.Size();
//
//	float SpeedMultiplier = 3.0f; 
//	float NewVelocityLength = VelocityLength * SpeedMultiplier;
//
//	ProjectileMovementComp->Velocity = NewNormal * NewVelocityLength;
//
//	PlayHitParticle(true, GetActorLocation(), GetActorRotation());
//
//}

//void ABulletBase::PlayHitParticle(bool bActive, const FVector& Location, const FRotator& Rotation)
//{
//	APooledParticleEffect* HitParticle{ ParticlePoolManager->GetParticle() };
//
//	if (!HitParticle) { return; }
//
//	HitParticle->ToggleActiveState(bActive, Location, Rotation);
//}

//bool ABulletBase::GetDeflectionHasBeenProcessed() const
//{
//	return bDeflectionHasBeenProcessed;
//}
//
//float ABulletBase::GetDamage()
//{
//	return DamageAmount;
//}

/// <summary>
/// The bullet Pool Manager listens for this event which returns the bullet to the pool
/// </summary>
//void ABulletBase::OnExpired()
//{
//	OnBulletExpired.Broadcast(this);
//}



