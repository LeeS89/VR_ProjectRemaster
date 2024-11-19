// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HandPoseRecognizer.h"
#include "CustomHandPoseRecognizer.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class PROJECTREMASTER_API UCustomHandPoseRecognizer : public UHandPoseRecognizer
{
	GENERATED_BODY()

public:

	UCustomHandPoseRecognizer();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bIsHandGrabbing{ false };
};
