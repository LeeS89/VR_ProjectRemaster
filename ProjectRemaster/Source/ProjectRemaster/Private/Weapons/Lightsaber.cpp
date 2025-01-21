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

	SlicingPlane = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SlicingPlane"));
	SlicingPlane->SetupAttachment(StaticMeshComp);
	SlicingPlane->SetVisibility(false);
	SlicingPlane->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	WeaponTraceComp = CreateDefaultSubobject<UWeaponTraceComponent>(TEXT("Weapon Trace Component"));

	VFXComp = CreateDefaultSubobject<UVFXComponent>(TEXT("VFX Component"));
}



void ALightsaber::BeginPlay()
{
	Super::BeginPlay();
	StartRotation = SlicingPlane->GetComponentRotation();
	PreviousLocation = GetActorLocation();
}

void ALightsaber::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector CurrentLocation = GetActorLocation();

	// Calculate movement direction
	MovementDirection = (CurrentLocation - PreviousLocation).GetSafeNormal();

	// Update the previous location for the next tick
	PreviousLocation = CurrentLocation;

	// Adjust the slicing plane's rotation
	UpdateSlicingPlaneRotation();
}

void ALightsaber::UpdateSlicingPlaneRotation()
{
	if (!MovementDirection.IsNearlyZero()) // Ensure the saber is moving
	{
		FRotator SaberForwardRotation = StaticMeshComp->GetForwardVector().ToOrientationRotator();
		FRotator NewRotation = MovementDirection.ToOrientationRotator();

		// Lock the plane's rotation to the Z-axis (yaw) only
		//FRotator FinalRotation = FRotator(SaberForwardRotation.Pitch, NewRotation.Yaw, SaberForwardRotation.Roll);
		FRotator FinalRotation = FRotator(StartRotation.Pitch, NewRotation.Yaw, StartRotation.Roll);

		// Apply the new rotation to the slicing plane
		SlicingPlane->SetWorldRotation(FinalRotation);
	}
}

void ALightsaber::OnGrabbed_Implementation(UCustomXRHandComponent* HandComponent, FName SocketName)
{
	Super::OnGrabbed_Implementation(HandComponent, SocketName);

	//CapsuleComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void ALightsaber::OnReleased_Implementation()
{
	Super::OnReleased_Implementation();

	//CapsuleComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

FVector ALightsaber::GetSlicingPlanePoint() const
{
	return FVector();
}

FVector ALightsaber::GetSlicingPlaneNormal() const
{
	return FVector();
}




