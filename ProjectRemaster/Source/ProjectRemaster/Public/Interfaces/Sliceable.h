// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Sliceable.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class USliceable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PROJECTREMASTER_API ISliceable
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	virtual bool GetIsOverlapping() { return false; }

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SliceMesh(const FVector& PlaneLocation, const FVector& PlaneNormal, const float ImpulseStrength, class UMaterialInstance* SliceMaterial);
};
