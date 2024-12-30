// Fill out your copyright notice in the Description page of Project Settings.


#include "UtilityClasses/TargetingUtility.h"
#include <BoxComponent.h>


/// <summary>
/// 
/// </summary>
/// <param name="Target"></param>
/// <param name="From"></param>
/// <param name="bGetRandomOffset">Works only for box Components, returns a random point from within the box extents</param>
/// <returns></returns>
FVector UTargetingUtility::GetDirectionToTarget(const UObject* Target, const UObject* From, bool bGetRandomOffset/*False by default*/)
{
	FVector TargetLocation = GetObjectLocation(Target, bGetRandomOffset);
	FVector FromLocation = GetObjectLocation(From);

	FVector DirectionToTarget = TargetLocation - FromLocation;

	return DirectionToTarget.GetSafeNormal();
}

FVector UTargetingUtility::GetObjectLocation(const UObject* Object, bool bGetRandomOffset)
{
	if (!Object) { return FVector::ZeroVector; }

	if (const AActor* Actor = Cast<AActor>(Object))
	{
		return Actor->GetActorLocation();
	}
	else if (const USceneComponent* Component = Cast<USceneComponent>(Object))
	{
		FVector ComponentLocation = Component->GetComponentLocation();
		if (bGetRandomOffset)
		{
			ApplyRandomOffset(Component, ComponentLocation);
		}
		return ComponentLocation;
	
	}
	return FVector::ZeroVector;
}

void UTargetingUtility::ApplyRandomOffset(const UObject* Target, FVector& LocationToOffset)
{

	if (const UBoxComponent* BoxComp = Cast<UBoxComponent>(Target))
	{
		FVector BoxExtent = BoxComp->GetScaledBoxExtent();
		FVector RandomOffset = FVector(
			FMath::RandRange(-BoxExtent.X, BoxExtent.X),
			FMath::RandRange(-BoxExtent.Y, BoxExtent.Y),
			FMath::RandRange(-BoxExtent.Z, BoxExtent.Z)
		);
		LocationToOffset += RandomOffset;
	}
	//return LocationToOffset;
}
