// Fill out your copyright notice in the Description page of Project Settings.


#include "TestClasses(DELETE_AFTER_USE)/TestGrabActor.h"

// Sets default values
ATestGrabActor::ATestGrabActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void ATestGrabActor::OnGrabbed(FName SocketName)
{

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

