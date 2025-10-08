// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "VFXComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTREMASTER_API UVFXComponent : public UActorComponent
{
	GENERATED_BODY()

private:

	UPROPERTY(VisibleAnywhere)
	TMap<AActor*, UParticleSystemComponent*> ActiveParticles;

public:	
	// Sets default values for this component's properties
	UVFXComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(VisibleAnywhere)
	class APoolManager* ImpactParticlePoolManager;

	UFUNCTION(BlueprintCallable)
	void HandleOverlap(AActor* HitActor, const FVector& HitLocation, const FRotator& HitRotation);

	UFUNCTION(BlueprintCallable)
	void HandleEndOverlap(AActor* EndOverlapActor);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UParticleSystem* ParticleTemplate;

	UFUNCTION(BlueprintCallable)
	bool HasActiveParticles() const;

	UFUNCTION(BlueprintCallable)
	void PlayHitParticle(bool bActive, const FVector& Location, const FRotator& Rotation);
};
