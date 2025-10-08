// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "TargetingUtility.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTREMASTER_API UTargetingUtility : public UObject
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	static FVector GetDirectionToTarget(const UObject* Target, const UObject* From, bool bGetRandomOffset = false);

private:

	static FVector GetObjectLocation(const UObject* Object, bool bGetRandomOffset = false);

	static void ApplyRandomOffset(const UObject* Target, FVector& LocationToOffset);
};
