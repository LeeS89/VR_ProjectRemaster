// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Enums/EDamageType.h"
#include "ProjectileManager.generated.h"

class ABaseBullet;
class UHierarchicalInstancedStaticMeshComponent;

UCLASS()
class PROJECTREMASTER_API AProjectileManager : public AActor
{
	GENERATED_BODY()

private:

	
	TArray<ABaseBullet*> FrozenBullets;

	//TMap<int32, FTransform> OriginalInstanceTransforms;

	UPROPERTY(VisibleAnywhere, Category = "Bullet Management")
	UHierarchicalInstancedStaticMeshComponent* FireInstancedBulletMesh;

	UPROPERTY(VisibleAnywhere, Category = "Bullet Management")
	UHierarchicalInstancedStaticMeshComponent* PoisonInstancedBulletMesh;

	// Temporary Pointer to Current InstanceMesh
	UHierarchicalInstancedStaticMeshComponent* TargetMesh;

	//Redundant
	void UpdateInstanceCulling();

	void UpdateNiagaraParticles(TEnumAsByte<EDamageType> DamageType, UHierarchicalInstancedStaticMeshComponent* TMPMesh ,int32 InstanceIndex);

	void CreateBulletInstance(TEnumAsByte<EDamageType> DamageType, ABaseBullet* Bullet, UStaticMeshComponent* BulletMesh);
	
public:	
	// Sets default values for this actor's properties
	AProjectileManager();

	UFUNCTION(BlueprintImplementableEvent, Category = "Niagara")
	void UpdateFrozenBulletParticles_BP(EDamageType ParticleType, const FVector& BulletLocations, int32 NumBullets);

	UFUNCTION(BlueprintImplementableEvent, Category = "Niagara")
	void RemoveFrozenBulletParticles_BP(const FVector& BulletPosition);

	UPROPERTY(BlueprintReadWrite)
	TArray<FVector> UpdatedPositions;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "FX")
	class UNiagaraComponent* MergedBulletParticles;

	UPROPERTY(EditAnywhere, Category = "FX")
	class UNiagaraSystem* BulletEffectSystem;

public:	
	

	UFUNCTION()
	void AddFrozenBullet(TEnumAsByte<EDamageType> DamageType, ABaseBullet* Bullet, UStaticMeshComponent* BulletMesh);

	UFUNCTION()
	void RemoveFrozenBullet(ABaseBullet* Bullet);
	
	//Redundant
	void StartCullingTimer();
};
