// Fill out your copyright notice in the Description page of Project Settings.


#include "TestClasses(DELETE_AFTER_USE)/BulletTest.h"
#include "PooledActors/BulletPoolManager.h"
#include "Projectiles/BulletBase.h"

// Sets default values
ABulletTest::ABulletTest()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh Component"));
	StaticMeshComp->SetupAttachment(RootComponent);

	SpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Spawn Point"));
	SpawnPoint->SetupAttachment(StaticMeshComp);

	

}



// Called when the game starts or when spawned
void ABulletTest::BeginPlay()
{
	Super::BeginPlay();

	GetWorldTimerManager().SetTimer(FireRateTimerHandle, this, &ABulletTest::Fire, FireRate, true);
	
}

void ABulletTest::Fire()
{
	ABulletBase* Bullet{ BulletPoolManager->GetBulletFromPool() };
	if (!Bullet) { return; }

	//Bullet->SetActorLocation(SpawnPoint->GetComponentLocation());
	//Bullet->SetActorRotation(SpawnPoint->GetComponentRotation());
	Bullet->SetOwner(OwnerTest);
	Bullet->ToggleActiveState(true, SpawnPoint->GetComponentLocation(), SpawnPoint->GetComponentRotation());


	/*FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = OwnerTest;
	SpawnParams.Instigator = GetInstigator();*/

	//ABulletBase* Bullet = GetWorld()->SpawnActor<ABulletBase>(BulletClass, SpawnPoint->GetComponentLocation(), SpawnPoint->GetComponentRotation(), SpawnParams);
	/*if (!SpawnParams.Instigator)
	{
		UE_LOG(LogTemp, Warning, TEXT("NO INSTIGATOR!!!"));
	}*/

}

// Called every frame
void ABulletTest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

