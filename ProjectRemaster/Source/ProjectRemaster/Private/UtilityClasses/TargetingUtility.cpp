// Fill out your copyright notice in the Description page of Project Settings.


#include "UtilityClasses/TargetingUtility.h"


FVector UTargetingUtility::GetDirectionToTarget(const UObject* Target, const UObject* From)
{
	FVector TargetLocation = GetObjectLocation(Target);
	FVector FromLocation = GetObjectLocation(From);

	FVector DirectionToTarget = TargetLocation - FromLocation;

	return DirectionToTarget.GetSafeNormal();
}

FVector UTargetingUtility::GetObjectLocation(const UObject* Object)
{
	if (!Object) { return FVector::ZeroVector; }

	if (const AActor* Actor = Cast<AActor>(Object))
	{
		return Actor->GetActorLocation();
	}
	else if (const USceneComponent* Component = Cast<USceneComponent>(Object))
	{
		return Component->GetComponentLocation();
	}

	return FVector::ZeroVector;
}
