// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <Enums/EDamageType.h>
#include "FDamageTypeInfo.generated.h"


/**
 * 
 */
USTRUCT(BlueprintType)
struct PROJECTREMASTER_API FDamageTypeInfo : public FTableRowBase
{
	GENERATED_BODY();

	/*UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Damage")
	FString Name;*/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	TEnumAsByte<EDamageType> DamageType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	float BaseDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	float DamageOverTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	float Duration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	TSubclassOf<UDamageType> DamageTypeClass;
};
