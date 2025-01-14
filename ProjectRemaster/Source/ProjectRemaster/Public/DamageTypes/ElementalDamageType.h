// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/DamageType.h"
#include "Enums/EDamageType.h"
#include "ElementalDamageType.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTREMASTER_API UElementalDamageType : public UDamageType
{
	GENERATED_BODY()

private:

	UPROPERTY(EditDefaultsOnly)
	TEnumAsByte<EDamageType> DamageType;

public:

	UFUNCTION()
	TEnumAsByte<EDamageType> GetElementType() const { return DamageType; }

};
