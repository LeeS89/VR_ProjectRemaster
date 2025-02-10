// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Enums/EHandInControl.h"
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

	UFUNCTION(BlueprintCallable, Category = "Movement")
	bool CanMoveForward(TEnumAsByte<EHandInControl> HandInControl) const;

	UFUNCTION(BlueprintCallable, Category = "Movement")
	void ResetHandInControl();

	UFUNCTION(BlueprintCallable, Category = "Rotation")
	void HandleRotation(float Direction);

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = ( Tooltip = "'Hand In Control', refers to the hand currently being used to trigger movement "))
	TEnumAsByte<EHandInControl> CurrentHandInControl;

	UFUNCTION(BlueprintCallable)
	void SetCurrentHandInControl(TEnumAsByte<EHandInControl> HandInControl);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadWrite)
	bool bCanMove{ false };

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
