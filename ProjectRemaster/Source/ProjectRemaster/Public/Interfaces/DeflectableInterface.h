// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "DeflectableInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UDeflectableInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PROJECTREMASTER_API IDeflectableInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintNativeEvent)
	void OnDeflected();

	virtual void OnExpired() = 0;

	virtual void PlayHitParticle(bool bActive, const FVector& Location, const FRotator& Rotation);

	virtual bool GetDeflectionHasBeenProcessed() const = 0;

	virtual void SetDeflectionHasBeenProcessed(bool HasBeenProcessed) = 0;

	/*virtual bool GetHitHasBeenProcessed() const = 0;

	virtual void SetHitHasBeenProcessed(bool HasBeenProcessed) = 0;*/

	virtual float GetDamage() { return 0.0f; }
};
