// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectiles/BaseBullet.h"
#include "Components/PointLightComponent.h"
#include "WeaponComponents/BulletCollisionComponent.h"
#include "Projectiles/BulletMovementComponent.h"
#include "WeaponComponents/VFXComponent.h"
#include <UtilityClasses/TargetingUtility.h>
#include <Kismet/KismetSystemLibrary.h>



// Sets default values
ABaseBullet::ABaseBullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh Component"));
	RootComponent = StaticMeshComp;

	PointLightComp = CreateDefaultSubobject<UPointLightComponent>(TEXT("Point Light Component"));
	PointLightComp->SetupAttachment(StaticMeshComp);

	MovementComp = CreateDefaultSubobject<UBulletMovementComponent>(TEXT("Projectile Movement Component"));

	CollisionComp = CreateDefaultSubobject<UBulletCollisionComponent>(TEXT("Collision Component"));

	VFXComp = CreateDefaultSubobject<UVFXComponent>(TEXT("VFX Component"));

	MovementComp->SetUpdatedComponent(StaticMeshComp);
}


// Called when the game starts or when spawned
void ABaseBullet::BeginPlay()
{
	Super::BeginPlay();
	DestroyTimeCountdown = DestroyTime;
}

void ABaseBullet::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (OnDeflectedDelegate.IsBound())
	{
		OnDeflectedDelegate.Clear();
	}
	
}


// Called every frame
void ABaseBullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TimeOut(DeltaTime);
}

void ABaseBullet::TimeOut(float DeltaTime)
{
	if (DestroyTimeCountdown > 0.0f)
	{
		DestroyTimeCountdown -= DeltaTime;
	}
	else
	{
		OnExpired();
	}
}


void ABaseBullet::ToggleActiveState(bool bActive, const FVector& SpawnLocation, const FRotator& SpawnRotation)
{
	if (!MovementComp || !PointLightComp) { return; }
	SetActorLocation(SpawnLocation);
	SetActorRotation(SpawnRotation);

	MovementComp->SetActive(bActive);
	PointLightComp->SetVisibility(bActive);

	if (bActive)
	{
		MovementComp->InitializeMovement(SpawnRotation);
		DestroyTimeCountdown = DestroyTime;
	}
	else
	{
		StaticMeshComp->SetRelativeLocation(FVector::ZeroVector);
		SetDeflectionHasBeenProcessed(false);
		SetHitHasBeenProcessed(false);
	}
	
	SetActorHiddenInGame(!bActive);
	SetActorEnableCollision(bActive);
	SetActorTickEnabled(bActive);
}

void ABaseBullet::NotifyHit(UPrimitiveComponent* MyComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	if (!CollisionComp) { return; }

	CollisionComp->HandleHit(MyComp, OtherActor, OtherComp, NormalImpulse, Hit);
}

void ABaseBullet::OnDeflected_Implementation(const FVector& DeflectionLocation, const FRotator& DeflectionRotation)
{
	
	FVector NewNormal = UTargetingUtility::GetDirectionToTarget(GetOwner(), this);
	MovementComp->DeflectBullet(NewNormal);
	
	OnDeflectedDelegate.Broadcast(true, DeflectionLocation, DeflectionRotation);
	SetDeflectionHasBeenProcessed(true);

}


bool ABaseBullet::GetDeflectionHasBeenProcessed() const
{
	return bDeflectionHasBeenProcessed;
}


void ABaseBullet::OnExpired()
{
	OnBulletHasExpired.Broadcast(this);
}




