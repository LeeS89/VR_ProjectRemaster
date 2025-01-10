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


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTREMASTER_API UBulletCollisionComponent : public UActorComponent
{
	GENERATED_BODY()

private:

	AActor* OwnerRef;

	bool ShouldRespondToHit(AActor* OtherActor, UPrimitiveComponent* OtherComp);

	UStaticMeshComponent* MeshComp;

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

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage")
	UDataTable* DamageDataTable;

	UPROPERTY()
	float Damage;

	UPROPERTY()
	float DamageOverTime;

	UPROPERTY()
	float DoTDuration;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UDamageType> DamageTypeClass;

public:

	void InitializeDamageType(const FString& DamageTypeName);

};
