// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "BulletMovementComponent.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTREMASTER_API UBulletMovementComponent : public UProjectileMovementComponent
{
	GENERATED_BODY()

private:

	UPROPERTY(EditDefaultsOnly, meta = (ToolTip = "When deflected, multiply the speed by this amount"))
	float SpeedMultiplier{ 3.0f };

public:

	UBulletMovementComponent();

	UFUNCTION()
	void DeflectBullet(const FVector& NewNormal);

	UFUNCTION()
	void InitializeMovement(const FRotator& SpawnRotation);
	
};
