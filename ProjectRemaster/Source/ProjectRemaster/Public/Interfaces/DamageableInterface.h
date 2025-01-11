// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "DamageableInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UDamageableInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PROJECTREMASTER_API IDamageableInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

   /* UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Damage")
    void ApplyDamageOverTime();*/

    // Set DoT Amount
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Damage")
    void SetDoT(float InDamageOverTime);

    // Set DoT Duration
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Damage")
    void SetDoTDuration(float InDoTDuration);

    // Apply Instant Damage
    //UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Damage")
    //void ApplyInstantDamage(float DamageAmount);

    //UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Damage")
    //void ApplyDoTEffect();
};
