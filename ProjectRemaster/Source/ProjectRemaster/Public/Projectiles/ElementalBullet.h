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


public:

	AElementalBullet();

	UPROPERTY(EditDefaultsOnly, Category = "Particles")
	class UParticleSystemComponent* Trail;

	virtual void ToggleActiveState(bool bActive, const FVector& SpawnLocation = FVector::ZeroVector, const FRotator& SpawnRotation = FRotator::ZeroRotator) final override;

};
