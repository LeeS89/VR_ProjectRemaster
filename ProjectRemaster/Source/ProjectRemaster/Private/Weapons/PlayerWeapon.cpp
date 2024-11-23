// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/PlayerWeapon.h"
#include "CharacterComponents/CustomXRHandComponent.h"

// Sets default values
APlayerWeapon::APlayerWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APlayerWeapon::BeginPlay()
{
	Super::BeginPlay();
	
	
}

// Called every frame
void APlayerWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APlayerWeapon::OnGrabbed_Implementation(UCustomXRHandComponent* HandComponent, FName SocketName)
{
	bIsGrabbed = true;
	OnWeaponGrabbedDelegate.Broadcast();
	AttachToComponent(HandComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale, SocketName);
}

void APlayerWeapon::OnReleased_Implementation()
{
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	OnWeaponReleasedDelegate.Broadcast();
	bIsGrabbed = false;
}

bool APlayerWeapon::IsGrabbed()
{
	return bIsGrabbed;
}

