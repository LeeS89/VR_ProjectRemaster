// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Projectiles/BulletBase.h"
#include "LaserBullet.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTREMASTER_API ALaserBullet : public ABulletBase
{
	GENERATED_BODY()

public:

	ALaserBullet();

protected:

	virtual void BeginPlay() override;

public:

	virtual void Tick(float DeltaTime) override;

	virtual void ToggleActiveState(bool bActive, const FVector& SpawnLocation = FVector::ZeroVector, const FRotator& SpawnRotation = FRotator::ZeroRotator) final override;

	UPROPERTY(EditAnywhere)
	float DestroyTime{ 1.5f };
	
};
