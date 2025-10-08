// Fill out your copyright notice in the Description page of Project Settings.


#include "Throwaway.h"

// Sets default values
AThrowaway::AThrowaway()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AThrowaway::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AThrowaway::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

