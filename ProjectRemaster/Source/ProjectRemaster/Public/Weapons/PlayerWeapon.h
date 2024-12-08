// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/GrabbableObject.h"
#include "PlayerWeapon.generated.h"

DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE(
	FOnWeaponGrabbedSignature,
	APlayerWeapon, OnWeaponGrabbedDelegate
);

DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE(
	FOnWeaponReleasedSignature,
	APlayerWeapon, OnWeaponReleasedDelegate
);

UCLASS()
class PROJECTREMASTER_API APlayerWeapon : public AActor, public IGrabbableObject
{
	GENERATED_BODY()

	bool bIsGrabbed{ false };

	UPROPERTY(EditDefaultsOnly)
	FName StartSocket;

	UPROPERTY(EditDefaultsOnly)
	FName EndSocket;

	UPROPERTY(EditDefaultsOnly)
	FName UseSocketRotation;

	UPROPERTY(VisibleAnywhere)
	class UParticleSystemComponent* ActiveParticleSystem;
	
public:	
	// Sets default values for this actor's properties
	APlayerWeapon();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* StaticMeshComp;

	UPROPERTY(EditAnywhere)
	USceneComponent* RootComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float CapsuleHalfHeight{ 8.0f };

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float CapsuleRadius{ 4.0f };

	UPROPERTY(EditAnywhere)
	class UParticleSystem* SparkParticles;

	/*UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Decals")
	class UMaterialInterface* DecalMaterial;*/

	UPROPERTY(EditAnywhere)
	bool bIsDebugMode{ false };


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void OnGrabbed_Implementation(class UCustomXRHandComponent* HandComponent, FName SocketName) override;

	virtual void OnReleased_Implementation() override;

	virtual bool IsGrabbed() override;

	UPROPERTY(BlueprintAssignable)
	FOnWeaponGrabbedSignature OnWeaponGrabbedDelegate;

	UPROPERTY(BlueprintAssignable)
	FOnWeaponReleasedSignature OnWeaponReleasedDelegate;
};
