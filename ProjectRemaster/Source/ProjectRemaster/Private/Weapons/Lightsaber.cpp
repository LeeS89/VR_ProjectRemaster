// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/Lightsaber.h"
#include "CharacterComponents/WeaponTraceComponent.h"
#include "WeaponComponents/VFXComponent.h"

ALightsaber::ALightsaber()
{
	PrimaryActorTick.bCanEverTick = true;

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


