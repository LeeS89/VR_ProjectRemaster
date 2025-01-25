// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "MainPlayer.generated.h"
//#include <CharacterComponents/CustomXRHandComponent.h>

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UMainPlayer : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PROJECTREMASTER_API IMainPlayer
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	virtual TArray<class UCustomXRHandComponent*> GetHandComponents(){
		return TArray<UCustomXRHandComponent*>();
	}

	virtual USceneComponent* GetTargetComponent() = 0;

	virtual void GetTraceLocation(FVector& OutLocation, FQuat& OutRotation) { return; }
};
