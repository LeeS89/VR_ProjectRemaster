// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BulletPoolManager.generated.h"

class ABulletBase;

UCLASS()
class PROJECTREMASTER_API ABulletPoolManager : public AActor
{
	GENERATED_BODY()
	
private:

	UPROPERTY(VisibleAnywhere, Category = "Debugging")
	TArray<ABulletBase*> BulletPool;

	UPROPERTY(EditDefaultsOnly, Category = "Pool types")
	TSubclassOf<ABulletBase> LaserBullet;
	
	UPROPERTY(EditDefaultsOnly, Category = "Pool sizes")
	int32 PoolSize{ 10 };

	void InitializePool();

	

public:	
	// Sets default values for this actor's properties
	ABulletPoolManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

	UFUNCTION(BlueprintCallable)
	ABulletBase* GetBulletFromPool();

	UFUNCTION(BlueprintCallable)
	void ReturnBulletToPool(ABulletBase* Bullet);

};
