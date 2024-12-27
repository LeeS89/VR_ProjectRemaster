// Fill out your copyright notice in the Description page of Project Settings.


#include "TestClasses(DELETE_AFTER_USE)/BulletTest.h"
#include "PooledActors/BulletPoolManager.h"
#include "Characters/MainCharacter.h"
#include "GameModes/MainGameMode.h"
#include "Projectiles/BulletBase.h"
#include "UtilityClasses/TargetingUtility.h"
#include <Kismet/GameplayStatics.h>

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

	AMainGameMode* GameMode = Cast<AMainGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

	if (!GameMode) { return; }

	ACharacter* PlayerCharacter = GameMode->GetPlayerCharacter();

	PlayerRef = Cast<AMainCharacter>(PlayerCharacter);

	/*if (!PlayerRef)
	{
		UE_LOG(LogTemp, Error, TEXT("WHY ME!!!!"));
	}*/

	GetWorldTimerManager().SetTimer(FireRateTimerHandle, this, &ABulletTest::Fire, FireRate, true);
	
}

void ABulletTest::Fire()
{
	ABulletBase* Bullet{ BulletPoolManager->GetBulletFromPool() };
	if (!Bullet) { return; }
	Bullet->SetOwner(OwnerTest);
	//Bullet->SetActorLocation(SpawnPoint->GetComponentLocation());
	//Bullet->SetActorRotation(SpawnPoint->GetComponentRotation());
	/////////////////////////////////////////////////
	//FVector PlayerLocation = PlayerRef->GetActorLocation();
	/*FVector CurrentLocation = SpawnPoint->GetComponentLocation();
	FVector DirectionToPlayer = PlayerLocation - CurrentLocation;
	FVector DirectionToPlayerNormalized = DirectionToPlayer.GetSafeNormal();*/
	///////////////////////////////////////////////////////
	FRotator DirectionToPlayer = UTargetingUtility::GetDirectionToTarget(PlayerRef, SpawnPoint).Rotation();
	//FRotator BulletRotation = DirectionToPlayer.Rotation();

	Bullet->ToggleActiveState(true, SpawnPoint->GetComponentLocation(), DirectionToPlayer);

	

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

