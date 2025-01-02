// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BulletTest.generated.h"

UCLASS()
class PROJECTREMASTER_API ABulletTest : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABulletTest();

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* StaticMeshComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	USceneComponent* SpawnPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AActor* OwnerTest;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class ABulletBase> BulletClass;

	UPROPERTY(EditAnywhere)
	float FireRate = 3.f;

	UPROPERTY(EditAnywhere)
	class APoolManager* BulletPoolManager;

private:
	FTimerHandle FireRateTimerHandle;

	void Fire();

	UPROPERTY(VisibleAnywhere)
	class AMainCharacter* PlayerRef;

	UPROPERTY(VisibleAnywhere)
	class USceneComponent* AimTargetComp;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
