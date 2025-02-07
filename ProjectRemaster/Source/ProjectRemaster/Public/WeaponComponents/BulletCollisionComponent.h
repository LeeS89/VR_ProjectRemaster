// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interfaces/DeflectableInterface.h"
#include "Engine/DamageEvents.h"
#include "Enums/EDamageType.h"
#include "BulletCollisionComponent.generated.h"

class UElementalDamageType;

DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE_ThreeParams(
	FOnHitSignature,
	UBulletCollisionComponent, OnHitDelegate,
	bool, bSetActive,
	const FVector&, Location,
	const FRotator&, Rotation
);

DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE_TwoParams(
	FOnSetDoTParamsSignature,
	UBulletCollisionComponent, OnSetDoTParamsDelegate,
	AActor*, OtherActor, const UElementalDamageType*, ElementDamageType/*, const float, OutDoTAmount, const float, OutDoTDuration*/
);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTREMASTER_API UBulletCollisionComponent : public UActorComponent
{
	GENERATED_BODY()

private:

	AActor* OwnerRef;

	UPROPERTY(EditAnywhere)
	float DamageMultiplier{ 1.0f };

	UPROPERTY(EditDefaultsOnly)
	int32 StatusEffectChancePercentage{ 0 };

	bool ShouldRespondToHit(AActor* OtherActor, UPrimitiveComponent* OtherComp);

	UStaticMeshComponent* MeshComp;

	UPROPERTY(VisibleAnywhere)
	UElementalDamageType* DamageTypeInstance;

	UPROPERTY()
	FDamageEvent TargetDamageEvent;

	UPROPERTY(VisibleAnywhere)
	TSubclassOf<UElementalDamageType> DamageTypeClass;

	UPROPERTY(VisibleAnywhere, Category = "Damage")
	TEnumAsByte<EDamageType> DamageType;

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
	FOnSetDoTParamsSignature OnSetDoTParamsDelegate;


protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage")
	class UDataTable* DamageDataTable;

	UPROPERTY()
	float Damage;

	/*UPROPERTY()
	float DamageOverTime;

	UPROPERTY()
	float DoTDuration;*/

	

public:

	void InitializeDamageType(EDamageType DamageTypeToInitialize);

	UPROPERTY()
	float DamageOverTime;

	UPROPERTY()
	float DoTDuration;
};
