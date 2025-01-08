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

private:

	UPROPERTY(VisibleAnywhere)
	class ABulletBase* OwnerRef;

	// flag to check if the trace has already responded to a hit
	bool bHasHit{ false };

	void DamageTarget(AActor* ActorToDamage);

	FVector PreviousLocation{ FVector::ZeroVector };

protected:

	// Called when the game starts
	virtual void BeginPlay() override;

	virtual void HandleTraceResults(const TArray<FHitResult>& HitResults) override;

	virtual void SetTraceLocationAndRotation() final override;
	
public:

	void ResetHitFlag() { bHasHit = false; }

};
