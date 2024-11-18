// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OculusXRHandComponent.h"
#include "Enums/EHand.h"
#include "CustomXRHandComponent.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTREMASTER_API UCustomXRHandComponent : public UOculusXRHandComponent
{
	GENERATED_BODY()

public:
	UCustomXRHandComponent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hand")
	TEnumAsByte<EHand> GrabHandSide;


	
};
