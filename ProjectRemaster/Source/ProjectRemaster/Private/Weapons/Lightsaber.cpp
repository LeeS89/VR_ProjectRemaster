// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/Lightsaber.h"
#include "CharacterComponents/WeaponTraceComponent.h"
#include "Components/CapsuleComponent.h"
#include "WeaponComponents/VFXComponent.h"

ALightsaber::ALightsaber()
{
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule Component"));
	CapsuleComp->SetupAttachment(StaticMeshComp);

	WeaponTraceComp = CreateDefaultSubobject<UWeaponTraceComponent>(TEXT("Weapon Trace Component"));

	VFXComp = CreateDefaultSubobject<UVFXComponent>(TEXT("VFX Component"));
}



void ALightsaber::BeginPlay()
{
	Super::BeginPlay();
}

void ALightsaber::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ALightsaber::OnGrabbed_Implementation(UCustomXRHandComponent* HandComponent, FName SocketName)
{
	Super::OnGrabbed_Implementation(HandComponent, SocketName);

	CapsuleComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void ALightsaber::OnReleased_Implementation()
{
	Super::OnReleased_Implementation();

	CapsuleComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}


