// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HandPoseRecognizer.h"
#include "OculusXRInputFunctionLibrary.h"
#include "CustomHandPoseRecognizer.generated.h"


DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE_TwoParams(
	FOnPoseRecognizedSignature,
	UCustomHandPoseRecognizer, OnPoseRecognizedDelegate,
	EOculusXRHandType, HandUsed,
	UCustomHandPoseRecognizer*, PoseClass
);

DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE_OneParam(
	FOnPoseReleasedSignature,
	UCustomHandPoseRecognizer, OnPoseReleasedDelegate,
	UCustomHandPoseRecognizer*, PoseClass
);

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

	UFUNCTION(BlueprintCallable)
	
	bool GetRecognizedPose(int& Index, FString& Name, float& Duration, float& Error, float& Confidence);

	UPROPERTY(BlueprintAssignable)
	FOnPoseRecognizedSignature OnPoseRecognizedDelegate;

	UPROPERTY(BlueprintAssignable)
	FOnPoseReleasedSignature OnPoseReleasedDelegate;
};
