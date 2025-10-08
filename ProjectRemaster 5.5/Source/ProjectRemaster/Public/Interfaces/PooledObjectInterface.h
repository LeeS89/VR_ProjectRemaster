// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PooledObjectInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UPooledObjectInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PROJECTREMASTER_API IPooledObjectInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	virtual bool IsInUse() const = 0;

	virtual void SetIsInUse(bool bInUse) = 0;

	virtual void ToggleActiveState(bool bActive, const FVector& SpawnLocation = FVector::ZeroVector, const FRotator& SpawnRotation = FRotator::ZeroRotator, AActor* NewOwner = nullptr, APawn* NewInstigator = nullptr) = 0;
};
