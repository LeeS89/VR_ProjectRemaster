// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Structs/FTraceSockets.h"
#include "OculusXRInputFunctionLibrary.h"
#include "PlayerAbilitiesComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTREMASTER_API UPlayerAbilitiesComponent : public UActorComponent
{
	GENERATED_BODY()

private:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float HalfHeight{ 8.0f };

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float CapsuleRadius{ 4.0f };

public:	
	// Sets default values for this component's properties
	UPlayerAbilitiesComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	void PerformBulletTrace();

	UFUNCTION(BlueprintCallable)
	void HandleBulletTraceResults(const TArray<FHitResult>& HitResults);

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere)
	TMap<EOculusXRHandType, FTraceSockets > HandSockets;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trace Channel")
	TEnumAsByte<ECollisionChannel> TraceChannel;
		
};
