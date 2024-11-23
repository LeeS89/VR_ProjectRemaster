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
	
public:	
	// Sets default values for this actor's properties
	APlayerWeapon();

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
