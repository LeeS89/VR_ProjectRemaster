// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/PooledObjectInterface.h"
#include "PooledParticleEffect.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnParticleExpired, APooledParticleEffect*, Particle);

UCLASS()
class PROJECTREMASTER_API APooledParticleEffect : public AActor, public IPooledObjectInterface
{
	GENERATED_BODY()

private:

	bool bIsInUse{ false };
	
public:	
	// Sets default values for this actor's properties
	APooledParticleEffect();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly)
	class UParticleSystemComponent* ParticleSystemComp;

	UFUNCTION()
	void OnExpired(UParticleSystemComponent* FinishedComponent);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(BlueprintAssignable)
	FOnParticleExpired OnParticleExpired;

	// Interface Functions
	virtual bool IsInUse() const override { return bIsInUse; }

	virtual void SetIsInUse(bool InUse) override { bIsInUse = InUse; }

	virtual void ToggleActiveState(bool bActive, const FVector& SpawnLocation = FVector::ZeroVector, const FRotator& SpawnRotation = FRotator::ZeroRotator) override;

};
