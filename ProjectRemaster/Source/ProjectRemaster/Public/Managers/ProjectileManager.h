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
	

	UPROPERTY(VisibleAnywhere)
	TArray<ABaseBullet*> AllFrozenBullets;
	UPROPERTY(VisibleAnywhere)
	TArray<ABaseBullet*> FrozenFireBullets;
	TArray<ABaseBullet*> FrozenPoisonBullets;


	UFUNCTION()
	void UpdateInstanceMesh(TEnumAsByte<EDamageType> DamageType, ABaseBullet* Bullet, UStaticMeshComponent* BulletMesh, UHierarchicalInstancedStaticMeshComponent* InstanceMesh);

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
	void UpdateFrozenBulletParticles_BP(EDamageType ParticleType, const FVector& BulletLocation);

	UFUNCTION(BlueprintImplementableEvent, Category = "Niagara")
	void RemoveFrozenBulletParticles_BP(EDamageType ParticleType, const FVector& BulletPosition);

	
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
	void RemoveFrozenBullet(TEnumAsByte<EDamageType> DamageType, ABaseBullet* Bullet);
	
	//Redundant
	void StartCullingTimer();
};
