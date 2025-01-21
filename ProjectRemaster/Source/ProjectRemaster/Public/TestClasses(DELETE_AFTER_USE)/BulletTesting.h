// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Interfaces/EnemyInterface.h"
#include "Interfaces/Sliceable.h"
#include "BulletTesting.generated.h"

UCLASS()
class PROJECTREMASTER_API ABulletTesting : public APawn, public IEnemyInterface, public ISliceable
{
	GENERATED_BODY()

private:

	FVector PointOfEntry;
	FVector POINormal;
	FVector PointOfExit;
	FVector POENormal;

	

public:
	// Sets default values for this pawn's properties
	ABulletTesting();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bTestSlice{ false };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* StaticMeshComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	USceneComponent* SpawnPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AActor* OwnerTest;

	/*UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class ABulletBase> BulletClass;*/

	UPROPERTY(EditAnywhere)
	float FireRate = 3.f;

	UPROPERTY(EditAnywhere)
	class APoolManager* BulletPoolManager;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UStatsComponent* StatsComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UProceduralMeshComponent* ProcMesh;

	

private:
	FTimerHandle FireRateTimerHandle;

	void Fire();

	UPROPERTY(VisibleAnywhere)
	class AMainCharacter* PlayerRef;

	UPROPERTY(VisibleAnywhere)
	class USceneComponent* AimTargetComp;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	bool bIsOverlapping{ false };

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	virtual void RegisterPointOfEntry(const FVector& ImpactPoint, const FVector& ImpactNormal) override;

	virtual void RegisterPointOfExit(const FVector& ExitPoint, const FVector& ExitNormal) override;

	virtual bool GetIsOverlapping() override { return bIsOverlapping; }
};
