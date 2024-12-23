// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/BulletInterface.h"
#include "BulletBase.generated.h"

UCLASS()
class PROJECTREMASTER_API ABulletBase : public AActor, public IBulletInterface
{
	GENERATED_BODY()

private:

	bool bIsInUse{ false };

	
public:	
	// Sets default values for this actor's properties
	ABulletBase();

	UPROPERTY(EditAnywhere)
	USceneComponent* RootComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UStaticMeshComponent* StaticMeshComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	class UProjectileMovementComponent* ProjectileMovementComp;

	bool IsInUse() const { return bIsInUse; }

	void SetIsInUse(bool InUse) { bIsInUse = InUse; }

	void ToggleActiveState(bool bActive, const FVector& SpawnLocation, const FRotator& SpawnRotation);

	virtual void OnDeflected_Implementation() override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
