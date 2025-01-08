// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponComponents/BaseTraceComponent.h"
#include <Kismet/KismetSystemLibrary.h>
#include <Kismet/KismetMathLibrary.h>
#include <Kismet/GameplayStatics.h>

// Sets default values for this component's properties
UBaseTraceComponent::UBaseTraceComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UBaseTraceComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UBaseTraceComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UBaseTraceComponent::PerformTrace(const FVector& StartLocation, const FVector& EndLocation, const FQuat& Rotation)
{
	/*FVector StartSocketLocation{ StaticMeshComp->GetSocketLocation(Sockets.Start) };
	FVector EndSocketLocation{ StaticMeshComp->GetSocketLocation(Sockets.End) };
	FQuat SocketRotation{ StaticMeshComp->GetSocketQuaternion(Sockets.Rotation) };*/



	FCollisionShape Capsule{
		FCollisionShape::MakeCapsule(CapsuleRadius, CapsuleHalfHeight)
	};

	FCollisionQueryParams IgnoreParams{
		FName{ TEXT("Ignore Params") },
		true,
		GetOwner()
	};


	TArray<FHitResult> OutResults;

	bool bHasFoundTargets{ GetWorld()->SweepMultiByChannel(
		OutResults,
		StartLocation,
		EndLocation,
		FQuat::Identity,
		TraceChannel,
		Capsule,
		IgnoreParams
	) };


	if (bHasFoundTargets)
	{
		HandleTraceResults(OutResults);
	}

	///////////////////////////
	// Trace Hit Logic Here
	///////////////////////////
	if (bIsDebugMode)
	{

		FVector CenterPoint{
			UKismetMathLibrary::VLerp(
				StartLocation, EndLocation, 0.5f
			)
		};

		//FVector CenterPoint{ (StartSocketLocation + EndSocketLocation) / 2.0f };
		//float CapsuleHeight = CenterPoint.Size() / 2.0f;

		UKismetSystemLibrary::DrawDebugCapsule(
			GetWorld(),
			CenterPoint,
			Capsule.GetCapsuleAxisHalfLength(),
			Capsule.GetCapsuleRadius(),
			Rotation.Rotator(),
			bHasFoundTargets ? FLinearColor::Yellow : FLinearColor::Black,
			0.0f,
			1.0f
		);

	}
}

void UBaseTraceComponent::SetTraceLocationAndRotation()
{
	if (!StaticMeshComp) { return; }

	FVector StartSocketLocation{ StaticMeshComp->GetSocketLocation(Sockets.Start) };
	FVector EndSocketLocation{ StaticMeshComp->GetSocketLocation(Sockets.End) };
	FQuat SocketRotation{ StaticMeshComp->GetSocketQuaternion(Sockets.Rotation) };

	PerformTrace(StartSocketLocation, EndSocketLocation, SocketRotation);
}

void UBaseTraceComponent::HandleTraceResults(const TArray<FHitResult>& OutResults)
{
	// Implementation provided in Derived classes
}

