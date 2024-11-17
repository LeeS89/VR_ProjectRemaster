// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LocomotionComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTREMASTER_API ULocomotionComponent : public UActorComponent
{
	GENERATED_BODY()

	ACharacter* CharacterRef;

	class UCharacterMovementComponent* MovementComp;

	UPROPERTY(EditAnywhere)
	float MaxSpeed{ 300.0f };

	UPROPERTY(EditAnywhere)
	float RotationSpeed{ 5.0f };

	class APlayerController* PlayerController;

public:	
	// Sets default values for this component's properties
	ULocomotionComponent();

	UFUNCTION(BlueprintCallable, Category = "Movement")
	void MoveForward(float ScaleValue);

	UFUNCTION(BlueprintCallable, Category = "Rotation")
	void HandleRotation(float Direction);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
