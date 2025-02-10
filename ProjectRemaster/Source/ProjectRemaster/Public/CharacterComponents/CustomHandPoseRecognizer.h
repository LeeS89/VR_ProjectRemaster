// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HandPoseRecognizer.h"
#include "OculusXRInputFunctionLibrary.h"
#include "CustomHandPoseRecognizer.generated.h"

DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE(
	FOnFreezePoseRecognizedSignature,
	UCustomHandPoseRecognizer, OnFreezePoseRecognizeddelegate
);

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

private:

	int RecognizedIndex;
	FString RecognizedName;
	float PoseDuration;
	float PoseError;
	float PoseConfidence;

	UFUNCTION()
	void ProcessHandPoses();

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

	UPROPERTY(BlueprintAssignable)
	FOnFreezePoseRecognizedSignature OnFreezePoseRecognizeddelegate;

	UPROPERTY(VisibleAnywhere)
	float HandPoseConfidence{ 0.0f };

	UPROPERTY(EditAnywhere)
	float ReducedHandPoseConfidenceFloor{ 0.25f };

	UFUNCTION(BlueprintCallable)
	void ResetHandPoseConfidenceFloor();

	UFUNCTION(BlueprintCallable)
	void ReduceHandPoseConfidenceFloor();

protected:

	virtual void BeginPlay() override;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY()
	bool bHasPoseBeenProcessed{ false };
};
