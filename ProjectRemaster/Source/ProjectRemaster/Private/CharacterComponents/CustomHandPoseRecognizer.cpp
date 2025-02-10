// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterComponents/CustomHandPoseRecognizer.h"


UCustomHandPoseRecognizer::UCustomHandPoseRecognizer()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UCustomHandPoseRecognizer::BeginPlay()
{
	Super::BeginPlay();

	HandPoseConfidence = DefaultConfidenceFloor;
}

void UCustomHandPoseRecognizer::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	ProcessHandPoses();
}

void UCustomHandPoseRecognizer::ProcessHandPoses()
{
	bool bSuccess = UHandPoseRecognizer::GetRecognizedHandPose(RecognizedIndex, RecognizedName, PoseDuration, PoseError, PoseConfidence);
	if (bSuccess)
	{

		if (RecognizedIndex == 0 && !bHasPoseBeenProcessed)
		{
			OnPoseRecognizedDelegate.Broadcast(Side, this);
			bHasPoseBeenProcessed = true;
		}
		else if (RecognizedIndex == 1)
		{
			OnFreezePoseRecognizeddelegate.Broadcast();
		}

	}
	else
	{
		if (bHasPoseBeenProcessed)
		{
			OnPoseReleasedDelegate.Broadcast(this);
			bHasPoseBeenProcessed = false;
		}
		
	}
}

void UCustomHandPoseRecognizer::ResetHandPoseConfidenceFloor()
{
	DefaultConfidenceFloor = HandPoseConfidence;
}

void UCustomHandPoseRecognizer::ReduceHandPoseConfidenceFloor()
{
	DefaultConfidenceFloor = ReducedHandPoseConfidenceFloor;
}



bool UCustomHandPoseRecognizer::GetRecognizedPose(int& Index, FString& Name, float& Duration, float& Error, float& Confidence)
{
	bool bResult = UHandPoseRecognizer::GetRecognizedHandPose(Index, Name, Duration, Error, Confidence);
	
	if (bResult)
	{
		OnPoseRecognizedDelegate.Broadcast(Side, this);
		
	}
	else
	{
		OnPoseReleasedDelegate.Broadcast(this);
	}

	return bResult;
}


