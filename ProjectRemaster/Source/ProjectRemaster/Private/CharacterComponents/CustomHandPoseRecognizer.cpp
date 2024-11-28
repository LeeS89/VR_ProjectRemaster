// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterComponents/CustomHandPoseRecognizer.h"


UCustomHandPoseRecognizer::UCustomHandPoseRecognizer()
{
}

void UCustomHandPoseRecognizer::BeginPlay()
{
	Super::BeginPlay();

	HandPoseConfidence = DefaultConfidenceFloor;
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


