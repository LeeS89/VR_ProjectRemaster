// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Structs/FTraceSockets.h"
#include "WeaponTraceComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE_ThreeParams(
	FOnOverlappingSignature,
	UWeaponTraceComponent, OnOverlappingDelegate,
	AActor*, HitActor,
	const FVector&, HitLocation,
	const FRotator&, HitRotation
);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTREMASTER_API UWeaponTraceComponent : public UActorComponent
{
	GENERATED_BODY()

private:

	UPROPERTY(EditAnywhere, Category = "Trace Sockets")
	FTraceSockets Sockets;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* StaticMeshComp;

public:	
	// Sets default values for this component's properties
	UWeaponTraceComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void PerformTrace();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
	class USoundBase* DeflectSound;

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trace Channel")
	TEnumAsByte<ECollisionChannel> TraceChannel;

	class IDeflectableInterface* DeflectInterface;
	class IOverlappableInterface* OverlapInterface;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float CapsuleHalfHeight{ 8.0f };

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float CapsuleRadius{ 4.0f };

	UPROPERTY(EditAnywhere)
	bool bIsDebugMode{ false };

	UPROPERTY(BlueprintAssignable)
	FOnOverlappingSignature OnOverlappingDelegate;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UParticleSystemComponent* ActiveParticleSystem;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UParticleSystem* SparkParticles;
		
};
