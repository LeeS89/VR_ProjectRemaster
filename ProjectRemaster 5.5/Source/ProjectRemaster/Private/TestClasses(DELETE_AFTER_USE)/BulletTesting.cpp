// Fill out your copyright notice in the Description page of Project Settings.


#include "TestClasses(DELETE_AFTER_USE)/BulletTesting.h"
#include "PooledActors/PoolManager.h"
#include "Characters/MainCharacter.h"
#include "GameModes/MainGameMode.h"
#include "Projectiles/BaseBullet.h"
#include "ProceduralMeshComponent.h"
#include "UtilityClasses/TargetingUtility.h"
#include "CharacterComponents/StatsComponent.h"
#include <Kismet/GameplayStatics.h>
#include "Enums/EDamageType.h"
#include <KismetProceduralMeshLibrary.h>

// Sets default values
ABulletTesting::ABulletTesting()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh Component"));
	StaticMeshComp->SetupAttachment(RootComponent);
	StaticMeshComp->SetVisibility(false);

	ProcMesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("Procedural Mesh Component"));
	ProcMesh->SetupAttachment(StaticMeshComp);

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


void ABulletTesting::SliceMesh_Implementation(const FVector& PlaneLocation, const FVector& PlaneNormal, const float ImpulseStrength, UMaterialInstance* SliceMaterial)
{
	UProceduralMeshComponent* OutOtherHalfProcMesh;

	UKismetProceduralMeshLibrary::SliceProceduralMesh(
		ProcMesh,
		PlaneLocation,
		PlaneNormal,
		true,
		OutOtherHalfProcMesh,
		EProcMeshSliceCapOption::CreateNewSectionForCap,
		SliceMaterial
	);

	//ProcMesh->SetSimulatePhysics(true);
	//ProcMesh->SetEnableGravity(true);
	if (OutOtherHalfProcMesh)
	{
		OutOtherHalfProcMesh->SetSimulatePhysics(true);
		OutOtherHalfProcMesh->SetEnableGravity(true);

		OutOtherHalfProcMesh->AddImpulse(-(PlaneNormal * ImpulseStrength), NAME_None, true);

		//OutOtherHalfProcMesh->DestroyComponent();
	}
}



void ABulletTesting::Fire()
{
	ABaseBullet* Bullet;

	if (BulletInt == 0)
	{
		Bullet = BulletPoolManager->GetBullet(EDamageType::Fire);
		++BulletInt;
	}
	else
	{
		Bullet = BulletPoolManager->GetBullet(EDamageType::Poison);
		BulletInt = 0;
	}
	
	if (!Bullet) { return; }
	
	if (AimTargetComp)
	{
		FRotator DirectionToTarget = UTargetingUtility::GetDirectionToTarget(AimTargetComp, SpawnPoint, true).Rotation();


		Bullet->ToggleActiveState(true, SpawnPoint->GetComponentLocation(), DirectionToTarget, this, this);
	}
	else
	{
		Bullet->ToggleActiveState(true, SpawnPoint->GetComponentLocation(), SpawnPoint->GetComponentRotation(), this, this);
	}

}

