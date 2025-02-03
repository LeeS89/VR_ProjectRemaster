// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProjectileManager.generated.h"

class ABaseBullet;

UCLASS()
class PROJECTREMASTER_API AProjectileManager : public AActor
{
	GENERATED_BODY()

private:

	FTimerHandle CullCheckTimerHandle;

	TArray<ABaseBullet*> BulletsToMerge; // Buffer new bullets before merging
	FTimerHandle MergeTimerHandle;

	TArray<ABaseBullet*> FrozenBullets;

	TMap<int32, FTransform> OriginalInstanceTransforms;
	//TMap<int32, ABaseBullet*> InstanceToBulletMap;  
	TMap<int32, UParticleSystemComponent*> InstanceToParticleMap;

	UPROPERTY(VisibleAnywhere, Category = "Bullet Management")
	class UHierarchicalInstancedStaticMeshComponent* InstancedBulletMesh;

	void UpdateInstanceCulling();

	void UpdateNiagaraParticles(int32 InstanceIndex, ABaseBullet* Bullet);

	TArray <FVector> FrozenBulletLocations;

	
	
public:	
	// Sets default values for this actor's properties
	AProjectileManager();

	UFUNCTION(BlueprintImplementableEvent, Category = "Niagara")
	void UpdateFrozenBulletParticles_BP(const FVector& BulletLocations, int32 NumBullets);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "FX")
	class UNiagaraComponent* MergedBulletParticles;

	UPROPERTY(EditAnywhere, Category = "FX")
	class UNiagaraSystem* BulletEffectSystem;

public:	
	

	UFUNCTION()
	void AddFrozenBullet(ABaseBullet* Bullet, UStaticMeshComponent* BulletMesh, UParticleSystemComponent* BulletParticles);
	void StartCullingTimer();
};
