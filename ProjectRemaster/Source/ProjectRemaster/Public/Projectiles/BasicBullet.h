// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Projectiles/BaseBullet.h"
#include "BasicBullet.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTREMASTER_API ABasicBullet : public ABaseBullet
{
	GENERATED_BODY()

public:

	ABasicBullet();

protected:

	virtual void BeginPlay() override;

public:

	virtual void Tick(float DeltaTime) override;

	virtual void ToggleActiveState(bool bActive, const FVector& SpawnLocation = FVector::ZeroVector, const FRotator& SpawnRotation = FRotator::ZeroRotator) final override;

	virtual void OnDeflected_Implementation(const FVector& DeflectionLocation, const FRotator& DeflectionRotation) final override;
	
};
