// Fill out your copyright notice in the Description page of Project Settings.

#include "GameFramework/Character.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "CharacterComponents/VRCameraComponent.h"

UVRCameraComponent::UVRCameraComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UVRCameraComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	CheckRotation();
	//ACharacter* PlayerCharacter = GetOwner<ACharacter>();

	//FCollisionShape Box{
	//	FCollisionShape::MakeBox(BoxSize)
	//};

	//FCollisionQueryParams IgnoreParams{
	//	FName{TEXT("Ignore Params")},
	//	false,
	//	GetOwner()
	//};
	//
	//FHitResult OutResult;

	//bool bHasFoundTargets{ GetWorld()->SweepSingleByChannel(
	//	OutResult,
	//	TraceLocation->GetComponentLocation(),
	//	TraceLocation->GetComponentLocation(),
	//	TraceLocation->GetComponentQuat(),
	//	ECollisionChannel::ECC_GameTraceChannel2,
	//	Box,
	//	IgnoreParams
	//) };

	//if (bHasFoundTargets)
	//{

	//	//// Get the impact normal from the sweep result
	//	//FVector WallNormal = OutResult.ImpactNormal;

	//	//// Calculate the nudge offset
	//	//float NudgeDistance = 10.0f;
	//	//FVector NudgeOffset = WallNormal * NudgeDistance;

	//	//// Move the actor smoothly
	//	//FVector StartLocation = GetOwner()->GetActorLocation();
	//	//FVector TargetLocation = StartLocation + NudgeOffset;

	//	//FVector NewLocation = FMath::Lerp(StartLocation, TargetLocation, 0.1f); // Smooth interpolation
	//	//GetOwner()->SetActorLocation(NewLocation);



	//	FVector WallNormal = OutResult.ImpactNormal; // This is the normal of the surface hit
	//	WallNormal.Normalize();
	//	

	//	FVector PushDirection = WallNormal * Pushbackforce; // Replace WallNormal with the normal of the wall you're pushing away from
	//	PlayerCharacter->LaunchCharacter(PushDirection, true, true);
	//}

	//FVector CenterPoint{
	//			UKismetMathLibrary::VLerp(
	//				TraceLocation->GetComponentLocation(), TraceLocation->GetComponentLocation(), 0.5f
	//			)
	//};

	//UKismetSystemLibrary::DrawDebugBox(
	//	GetWorld(),
	//	CenterPoint,
	//	Box.GetExtent(),
	//	bHasFoundTargets ? FLinearColor::Green : FLinearColor::Red,
	//	TraceLocation->GetComponentQuat().Rotator(),
	//	0.1f,
	//	1.0f

	//);


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
