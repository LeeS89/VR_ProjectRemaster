// Fill out your copyright notice in the Description page of Project Settings.

#include "CharacterComponents/CustomXRHandComponent.h"
//#include "Components/SceneComponent.h"
#include "TestClasses(DELETE_AFTER_USE)/TestGrabActor.h"

// Sets default values
ATestGrabActor::ATestGrabActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void ATestGrabActor::OnGrabbed_Implementation(UCustomXRHandComponent* HandComponent, FName SocketName)
{
	AttachToComponent(HandComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale, SocketName);
}

void ATestGrabActor::OnReleased_Implementation()
{
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
}

// Called when the game starts or when spawned
void ATestGrabActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATestGrabActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

