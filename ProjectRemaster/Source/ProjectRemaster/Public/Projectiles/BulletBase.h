// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/DeflectableInterface.h"
#include "BulletBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBulletExpired, ABulletBase*, Bullet);

UCLASS()
class PROJECTREMASTER_API ABulletBase : public AActor, public IDeflectableInterface
{
	GENERATED_BODY()

private:

	bool bIsInUse{ false };


	
public:	
	// Sets default values for this actor's properties
	ABulletBase();

	UPROPERTY(BlueprintAssignable)
	FOnBulletExpired OnBulletExpired;

	UPROPERTY(EditAnywhere)
	USceneComponent* RootComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UStaticMeshComponent* StaticMeshComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	class UProjectileMovementComponent* ProjectileMovementComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	class UBulletTraceComponent* TraceComp;

	bool IsInUse() const { return bIsInUse; }

	void SetIsInUse(bool InUse) { bIsInUse = InUse; }

	virtual void ToggleActiveState(bool bActive, const FVector& SpawnLocation = FVector::ZeroVector, const FRotator& SpawnRotation = FRotator::ZeroRotator);

	virtual void OnDeflected_Implementation() override;

	/*virtual void OnExpired_Implementation() override;*/

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void OnExpired();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bShouldPerformTrace{ false };


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
