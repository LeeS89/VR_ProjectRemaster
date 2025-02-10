// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Projectiles/BaseBullet.h"
#include "ElementalBullet.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTREMASTER_API AElementalBullet : public ABaseBullet
{
	GENERATED_BODY()

private:

	UPROPERTY(VisibleAnywhere)
	float DoTAmount{ 0.0f };
	UPROPERTY(VisibleAnywhere)
	float DoTDuration{ 0.0f };

	bool bIsFrozen{ false };

public:

	AElementalBullet();

	UPROPERTY(EditDefaultsOnly, Category = "Particles")
	class UParticleSystemComponent* Trail;

	virtual void ToggleActiveState(bool bActive, const FVector& SpawnLocation = FVector::ZeroVector, const FRotator& SpawnRotation = FRotator::ZeroRotator, AActor* NewOwner = nullptr, APawn* NewInstigator = nullptr) final override;

	virtual void FreezeBullet() final override;

	virtual void UnFreezeBullet() final override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "FX")
	class UNiagaraComponent* BulletParticles;

	UPROPERTY(EditAnywhere, Category = "FX")
	class UNiagaraSystem* BulletEffectSystem;

protected:

	virtual void BeginPlay() override;
	
};
