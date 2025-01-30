// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BulletManager.generated.h"

class ABaseBullet;

UCLASS()
class PROJECTREMASTER_API ABulletManager : public AActor
{
	GENERATED_BODY()

private:

	TArray<ABaseBullet*> FrozenBullets;
	TArray<FTransform> FrozenTransforms;

	void MergeBulletIntoFrozenMesh(ABaseBullet* Bullet);

	UPROPERTY(EditAnywhere)
	class UProceduralMeshComponent* ProceduralMesh;

	TArray<FVector> MergedVertices;   // Stores all accumulated vertices
	TArray<int32> MergedTriangles;


	////////////////////
	TArray<ABaseBullet*> BulletsToMerge; // Buffer new bullets before merging
	FTimerHandle MergeTimerHandle;

	void ProcessBulletMerging();


/// <summary>
/// //////////////////
/// </summary>
/// 	 TArray<ABaseBullet*> FrozenBullets;
	UPROPERTY(EditAnywhere)
	class UInstancedStaticMeshComponent* InstancedBulletMesh;

	
	
public:	
	// Sets default values for this actor's properties
	ABulletManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	

	UFUNCTION()
	void HandleFrozenBulletMerge(ABaseBullet* Bullet);

	UFUNCTION()
	void AddFrozenBullet(ABaseBullet* Bullet, UStaticMeshComponent* BulletMesh, UParticleSystemComponent* BulletParticles);

};
