// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterComponents/VRCameraComponent.h"

UVRCameraComponent::UVRCameraComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UVRCameraComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	CheckRotation();
}

void UVRCameraComponent::CheckRotation()
{
	MoveDirection = GetForwardVector();
	MoveDirection.Z = 0.0f; // Limits rotation to Y axis
	MoveDirection.Normalize();

	
	FVector OwnerForwardVector{ GetOwner()->GetActorForwardVector() };
	OwnerForwardVector.Z = 0.0f;
	OwnerForwardVector.Normalize();


	AngleDifference = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(OwnerForwardVector, MoveDirection)));

	float Direction{ FVector::CrossProduct(OwnerForwardVector, MoveDirection).Z > 0 ? 1.0f : -1.0f };

	if (!bCanRotate && AngleDifference > RotateThreshold)
	{
		bCanRotate = true;
	}
	else if (bCanRotate && AngleDifference < StopRotateThrehold)
	{
		bCanRotate = false;
	}

	if (bCanRotate)
	{
		OnRotateDelegate.Broadcast(Direction);
	}
}
