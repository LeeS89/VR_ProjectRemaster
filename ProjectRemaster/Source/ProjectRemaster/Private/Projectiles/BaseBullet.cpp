// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectiles/BaseBullet.h"
#include "Components/PointLightComponent.h"
#include "WeaponComponents/BulletCollisionComponent.h"
#include <GameFramework/ProjectileMovementComponent.h>
#include "WeaponComponents/VFXComponent.h"
#include <UtilityClasses/TargetingUtility.h>

// Sets default values
ABaseBullet::ABaseBullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));

	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh Component"));
	StaticMeshComp->SetupAttachment(RootComponent);

	PointLightComp = CreateDefaultSubobject<UPointLightComponent>(TEXT("Point Light Component"));
	PointLightComp->SetupAttachment(StaticMeshComp);

	ProjectileMovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement Component"));

	CollisionComp = CreateDefaultSubobject<UBulletCollisionComponent>(TEXT("Collision Component"));

	VFXComp = CreateDefaultSubobject<UVFXComponent>(TEXT("VFX Component"));

	ProjectileMovementComp->bRotationFollowsVelocity = true;
	ProjectileMovementComp->SetUpdatedComponent(StaticMeshComp);
}


// Called when the game starts or when spawned
void ABaseBullet::BeginPlay()
{
	Super::BeginPlay();
	Timer = DestroyTime;
}



// Called every frame
void ABaseBullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TimeOut(DeltaTime);
}

void ABaseBullet::TimeOut(float DeltaTime)
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


void ABaseBullet::ToggleActiveState(bool bActive, const FVector& SpawnLocation, const FRotator& SpawnRotation)
{
	SetActorLocation(SpawnLocation);
	SetActorRotation(SpawnRotation);
	ProjectileMovementComp->SetActive(bActive);
	PointLightComp->SetVisibility(bActive);

	if (bActive)
	{
		//TraceComp->ResetHitFlag();
		SetDeflectionHasBeenProcessed(false);

		FVector NewVelocity{ SpawnRotation.Vector() * ProjectileMovementComp->InitialSpeed };
		ProjectileMovementComp->Velocity = NewVelocity;

		Timer = DestroyTime;

	}
	else
	{
		StaticMeshComp->SetRelativeLocation(FVector::ZeroVector);
		//SetHitHasBeenProcessed(false);
	}
	
	SetActorHiddenInGame(!bActive);
	SetActorEnableCollision(bActive);
	SetActorTickEnabled(bActive);
}

void ABaseBullet::OnDeflected_Implementation()
{
	FVector NewNormal = UTargetingUtility::GetDirectionToTarget(GetOwner(), this);

	float VelocityLength = ProjectileMovementComp->Velocity.Size();

	float SpeedMultiplier = 3.0f;
	float NewVelocityLength = VelocityLength * SpeedMultiplier;

	ProjectileMovementComp->Velocity = NewNormal * NewVelocityLength;

}


bool ABaseBullet::GetDeflectionHasBeenProcessed() const
{
	return bDeflectionHasBeenProcessed;
}

void ABaseBullet::OnExpired()
{
	//UE_LOG(LogTemp, Error, TEXT("EXPIRED SHOULD BE CALLED HERE -  INTERFACE FOUND"));
	//UE_LOG(LogTemp, Error, TEXT("BULLET SHOULD BE INVISIBLE NOW FROM HERE"));
	OnBulletHasExpired.Broadcast(this);
}

float ABaseBullet::GetDamage()
{
	return DamageAmount;
}



