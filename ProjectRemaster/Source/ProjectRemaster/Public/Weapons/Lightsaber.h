// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/BaseWeapon.h"
#include "Lightsaber.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTREMASTER_API ALightsaber : public ABaseWeapon
{
	GENERATED_BODY()

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	ALightsaber();

	virtual void Tick(float DeltaTime) override;
	
};
