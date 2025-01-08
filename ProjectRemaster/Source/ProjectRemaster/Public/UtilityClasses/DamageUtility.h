// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "DamageUtility.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTREMASTER_API UDamageUtility : public UObject
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	static bool ShouldDamageActor(const APawn* Instigator, AActor* Target);
	
};
