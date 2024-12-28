// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/GrabbableObject.h"
#include "BaseWeapon.generated.h"

DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE(
	FOnWeaponGrabbedSignature,
	ABaseWeapon, OnWeaponGrabbedDelegate
);

DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE(
	FOnWeaponReleasedSignature,
	ABaseWeapon, OnWeaponReleasedDelegate
);

UCLASS()
class PROJECTREMASTER_API ABaseWeapon : public AActor, public IGrabbableObject
{
	GENERATED_BODY()

	
	
public:	
	// Sets default values for this actor's properties
	ABaseWeapon();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* StaticMeshComp;

	UPROPERTY(EditAnywhere)
	USceneComponent* RootComp;



protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadWrite)
	bool bIsGrabbed{ false };

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
