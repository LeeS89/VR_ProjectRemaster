// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponComponents/BaseTraceComponent.h"
#include <Kismet/KismetSystemLibrary.h>
#include <Kismet/KismetMathLibrary.h>
#include "Components/CapsuleComponent.h"
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

	
}


// Called every frame
void UBaseTraceComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UBaseTraceComponent::PerformTrace(const FVector& StartLocation, const FVector& EndLocation, const FQuat& Rotation)
{
	
	FCollisionShape Capsule{
		FCollisionShape::MakeCapsule(CapsuleRadius, HalfHeight)
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

	if (bIsDebugMode)
	{

		FVector CenterPoint{
			UKismetMathLibrary::VLerp(
				StartLocation, EndLocation, 0.5f
			)
		};


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
	if (!MeshComp) { return; }
	
	FVector StartSocketLocation{ MeshComp->GetSocketLocation(Sockets.Start) };
	FVector EndSocketLocation{ MeshComp->GetSocketLocation(Sockets.End) };
	FQuat SocketRotation{ MeshComp->GetSocketQuaternion(Sockets.Rotation) };
	UE_LOG(LogTemp, Error, TEXT("Should Be tracing"));
	PerformTrace(StartSocketLocation, EndSocketLocation, SocketRotation);
}

void UBaseTraceComponent::HandleTraceResults(const TArray<FHitResult>& OutResults)
{
	// Implementation provided in Derived classes
}

