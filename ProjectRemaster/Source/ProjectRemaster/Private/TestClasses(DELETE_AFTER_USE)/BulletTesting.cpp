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
#include <KismetProceduralMeshLibrary.h>

// Sets default values
ABulletTesting::ABulletTesting()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

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

void ABulletTesting::RegisterPointOfEntry(const FVector& ImpactPoint, const FVector& ImpactNormal)
{
	PointOfEntry = ImpactPoint;
	POINormal = ImpactNormal;
}

void ABulletTesting::RegisterPointOfExit(const FVector& ExitPoint, const FVector& ExitNormal)
{
	PointOfExit = ExitPoint;
	POENormal = ExitNormal;
	//float DirectionCheck = FVector::DotProduct(POINormal, POENormal);
	//UE_LOG(LogTemp, Error, TEXT("Direction Check Value: %f"), DirectionCheck);
	
	FVector MeshCenter = ProcMesh->Bounds.Origin;  // Center of the mesh
	FVector MeshExtent = ProcMesh->Bounds.BoxExtent;  // Half-size of the mesh along each axis

	// Calculate the slicing plane position
	FVector PlanePosition = MeshCenter;  // Start at the center of the mesh

	// Use the X and Y coordinates of the impact point, keeping Z at the center
	PlanePosition.X = PointOfEntry.X;  // Align X with the impact point
	PlanePosition.Y = PointOfEntry.Y;  // Align Y with the impact point

	// Use the provided ImpactNormal for slicing direction
	FVector PlaneNormal = PointOfEntry.GetSafeNormal();


		FVector PlanePoint = PointOfEntry;// + PointOfExit) / 2;
		//FVector PlaneNormal = ExitNormal.GetSafeNormal();//(PointOfExit - PointOfEntry).GetSafeNormal();
		UProceduralMeshComponent* OutOtherHalfProcMesh;

		UKismetProceduralMeshLibrary::SliceProceduralMesh(
			ProcMesh,
			PlanePosition,
			PlaneNormal,
			true,
			OutOtherHalfProcMesh,
			EProcMeshSliceCapOption::CreateNewSectionForCap,
			nullptr
		);

		ProcMesh->SetSimulatePhysics(true);
		ProcMesh->SetEnableGravity(true);
		if (OutOtherHalfProcMesh)
		{
			OutOtherHalfProcMesh->SetSimulatePhysics(true);
			OutOtherHalfProcMesh->SetEnableGravity(true);
		}

	//Destroy();
	//UKismetSystemLibrary::Sl
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

