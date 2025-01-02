// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponComponents/BaseTraceComponent.h"
#include "BulletTraceComponent.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTREMASTER_API UBulletTraceComponent : public UBaseTraceComponent
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere)
	class ABulletBase* OwnerRef;

protected:

	// Called when the game starts
	virtual void BeginPlay() override;

	virtual void HandleTraceResults(const TArray<FHitResult>& HitResults) override;
	

};
