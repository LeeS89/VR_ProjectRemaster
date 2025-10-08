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

	float DamageOvertimeAmount{ 0.0f };
	float DoTDuration{ 0.0f };

public:

	UPROPERTY()
	int32 StatusEffectChancePercentage{ 0 };

	UFUNCTION()
	TEnumAsByte<EDamageType> GetElementType() const { return DamageType; }

	UFUNCTION()
	void SetDamageOverTimeAmount(float DoT) { DamageOvertimeAmount = DoT; }

	UFUNCTION()
	float GetDamageOverTimeAmount() const { return DamageOvertimeAmount; }

	UFUNCTION()
	void SetDoTDuration(float Duration) { DoTDuration = Duration; }

	UFUNCTION()
	float GetDoTDuration() const { return DoTDuration; }

};
