// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OculusXRHandComponent.h"
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


	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	AActor* CurrentGrabbedActor;

	UFUNCTION(BlueprintCallable)
	void SetGrabbedActor(AActor* GrabbedActor) { CurrentGrabbedActor = GrabbedActor; }

	UFUNCTION(BlueprintCallable)
	AActor* GetGrabbedActor() { return CurrentGrabbedActor; }
	
};
