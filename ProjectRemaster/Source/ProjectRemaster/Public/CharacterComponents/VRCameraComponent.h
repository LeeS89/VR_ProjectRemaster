// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "Interfaces/ResettableComponent.h"
#include "VRCameraComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE_OneParam(
	FOnRotateSignature,
	UVRCameraComponent, OnRotateDelegate,
	float, MoveDirection
);

/**
 * Handles the conditions required to start and stop rotating
 * When the condition is true, this is broadcast using a custom event
 * the event also broadcasts the direction in which to rotate
 */
UCLASS()
class PROJECTREMASTER_API UVRCameraComponent : public UCameraComponent, public IResettableComponent
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Rotate/Stop rotate thresholds", meta = (ClampMin = "0.0", Tooltip = "To rotate, the angle difference between the camera forward direction and the actor forward direction must be greater than this."))
	float RotateThreshold{ 20.0f };

	UPROPERTY(EditAnywhere, Category = "Rotate/Stop rotate thresholds", meta = (ClampMin = "0.0", Tooltip = "To stop rotating, the angle difference between the camera forward direction and the actor forward direction must be less than this."))
	float StopRotateThrehold{ 18.0f };

	bool bCanRotate{ false };

	FVector MoveDirection;
	FQuat StartRotation;
	FQuat TargetRotation;
	float AngleDifference;

	
public:

	UVRCameraComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(BlueprintAssignable)
	FOnRotateSignature OnRotateDelegate;

	UFUNCTION()
	virtual void ResetComponent() override;
	
private:

	void CheckRotation();
};
