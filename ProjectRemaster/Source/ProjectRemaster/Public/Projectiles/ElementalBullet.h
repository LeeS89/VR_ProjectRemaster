// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Projectiles/BaseBullet.h"
#include "Interfaces/ElementalDamageInterface.h"
#include "ElementalBullet.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTREMASTER_API AElementalBullet : public ABaseBullet, public IElementalDamageInterface
{
	GENERATED_BODY()

private:

	UPROPERTY(VisibleAnywhere)
	float DoTAmount{ 0.0f };
	UPROPERTY(VisibleAnywhere)
	float DoTDuration{ 0.0f };

public:

	AElementalBullet();

public:

	virtual void SetDoTAmount(float Amount) override { DoTAmount = Amount; };

	virtual float GetDoTAmount() override { return DoTAmount; }

	virtual void SetDoTDuration(float Duration) override { DoTDuration = Duration; }

	virtual float GetDoTDuration() override { return DoTDuration; };
	
};
