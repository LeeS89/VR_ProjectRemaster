// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interfaces/DeflectableInterface.h"
#include "BulletCollisionComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE_ThreeParams(
	FOnHitSignature,
	UBulletCollisionComponent, OnHitDelegate,
	bool, bSetActive,
	const FVector&, Location,
	const FRotator&, Rotation
);

DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE(
	FOnHitTestSignature,
	UBulletCollisionComponent, OnHitTestDelegate
	
);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTREMASTER_API UBulletCollisionComponent : public UActorComponent
{
	GENERATED_BODY()

private:

	AActor* OwnerRef;

public:	
	// Sets default values for this component's properties
	UBulletCollisionComponent();

	

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	IDeflectableInterface* BulletInterface;

	UFUNCTION()
	virtual void HandleHit(UPrimitiveComponent* HitComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		FVector NormalImpulse,
		const FHitResult& Hit);

	UFUNCTION()
	void DamageTarget(AActor* ActorToDamage);

	UPROPERTY(BlueprintAssignable)
	FOnHitSignature OnHitDelegate;

	UPROPERTY(BlueprintAssignable)
	FOnHitTestSignature OnHitTestDelegate;
};
