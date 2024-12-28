// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/BaseWeapon.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Interfaces/BulletInterface.h"
#include "Particles/ParticleSystemComponent.h"
#include "CharacterComponents/CustomXRHandComponent.h"

// Sets default values
ABaseWeapon::ABaseWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));
	RootComponent = RootComp;

	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh Component"));
	StaticMeshComp->SetupAttachment(RootComponent);

	/*CapsuleComp = CreateDefaultSubobject<ULightsaberCapsule>(TEXT("Capsule Component"));
	CapsuleComp->SetupAttachment(StaticMeshComp);*/

}

// Called when the game starts or when spawned
void ABaseWeapon::BeginPlay()
{
	Super::BeginPlay();
	
	
}

// Called every frame
void ABaseWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	

}

void ABaseWeapon::OnGrabbed_Implementation(UCustomXRHandComponent* HandComponent, FName SocketName)
{
	bIsGrabbed = true;
	OnWeaponGrabbedDelegate.Broadcast();
	AttachToComponent(HandComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale, SocketName);
}

void ABaseWeapon::OnReleased_Implementation()
{
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	OnWeaponReleasedDelegate.Broadcast();
	bIsGrabbed = false;
}

bool ABaseWeapon::IsGrabbed()
{
	return bIsGrabbed;
}

