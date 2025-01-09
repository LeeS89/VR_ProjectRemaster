// Fill out your copyright notice in the Description page of Project Settings.


#include "TestClasses(DELETE_AFTER_USE)/BulletTesting.h"
#include "PooledActors/PoolManager.h"
#include "Characters/MainCharacter.h"
#include "GameModes/MainGameMode.h"
#include "Projectiles/BaseBullet.h"

#include "UtilityClasses/TargetingUtility.h"
#include "CharacterComponents/StatsComponent.h"
#include <Kismet/GameplayStatics.h>

// Sets default values
ABulletTesting::ABulletTesting()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh Component"));
	StaticMeshComp->SetupAttachment(RootComponent);

	SpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Spawn Point"));
	SpawnPoint->SetupAttachment(StaticMeshComp);

	StatsComp = CreateDefaultSubobject<UStatsComponent>(TEXT("Stats Component"));
}



// Called when the game starts or when spawned
void ABulletTesting::BeginPlay()
{
	Super::BeginPlay();

	GetWorldTimerManager().SetTimer(FireRateTimerHandle, this, &ABulletTesting::Fire, FireRate, true);
	AMainGameMode* GameMode = Cast<AMainGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

	if (!GameMode) { return; }

	ACharacter* PlayerCharacter = GameMode->GetPlayerCharacter();

	PlayerRef = Cast<AMainCharacter>(PlayerCharacter);

	if (!PlayerRef) { return; }

	if (!PlayerRef->Implements<UMainPlayer>()) { return; }

	AimTargetComp = PlayerRef->GetTargetComponent();

	if (AimTargetComp)
	{
		UE_LOG(LogTemp, Error, TEXT("DEFENCE COLLIDER FOUND!!!"));
	}
	/*if (!PlayerRef)
	{
		UE_LOG(LogTemp, Error, TEXT("WHY ME!!!!"));
	}*/

	GetWorldTimerManager().SetTimer(FireRateTimerHandle, this, &ABulletTesting::Fire, FireRate, true);
	
}

// Called every frame
void ABulletTesting::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}



void ABulletTesting::Fire()
{

	//ABulletBase* Bullet{ BulletPoolManager->GetBullet() };
	ABaseBullet* Bullet{ BulletPoolManager->GetBullet() };
	if (!Bullet) { return; }
	Bullet->SetOwner(this);
	Bullet->SetInstigator(this);
	

	if (AimTargetComp)
	{
		FRotator DirectionToTarget = UTargetingUtility::GetDirectionToTarget(AimTargetComp, SpawnPoint, true).Rotation();


		Bullet->ToggleActiveState(true, SpawnPoint->GetComponentLocation(), DirectionToTarget);
	}
	else
	{
		Bullet->ToggleActiveState(true, SpawnPoint->GetComponentLocation(), SpawnPoint->GetComponentRotation());
	}

}

