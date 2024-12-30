// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Structs/FTraceSockets.h"
#include "WeaponComponents/BaseTraceComponent.h"
#include "WeaponTraceComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE_ThreeParams(
	FOnOverlappingSignature,
	UWeaponTraceComponent, OnOverlappingDelegate,
	AActor*, HitActor,
	const FVector&, HitLocation,
	const FRotator&, HitRotation
);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTREMASTER_API UWeaponTraceComponent : public UBaseTraceComponent
{
	GENERATED_BODY()


public:	
	// Sets default values for this component's properties
	UWeaponTraceComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
	class USoundBase* DeflectSound;

public:


	class IDeflectableInterface* DeflectInterface;
	class IOverlappableInterface* OverlapInterface;


	UPROPERTY(BlueprintAssignable)
	FOnOverlappingSignature OnOverlappingDelegate;


protected:

	virtual void HandleTraceResults(const TArray<FHitResult>& HitResults) override;
		
};
