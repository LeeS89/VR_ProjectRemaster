// Fill out your copyright notice in the Description page of Project Settings.


#include "TestClasses(DELETE_AFTER_USE)/AnotherTest.h"

// Sets default values
AAnotherTest::AAnotherTest()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
}

// Called when the game starts or when spawned
void AAnotherTest::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAnotherTest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

