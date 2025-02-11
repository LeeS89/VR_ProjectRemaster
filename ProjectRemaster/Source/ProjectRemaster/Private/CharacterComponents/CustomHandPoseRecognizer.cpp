// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterComponents/CustomHandPoseRecognizer.h"



EOculusXRHandType UCustomHandPoseRecognizer::CurrentHandInControl = EOculusXRHandType::None;
bool UCustomHandPoseRecognizer::bLeftHandActive = false;
bool UCustomHandPoseRecognizer::bRightHandActive = false;


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
		switch (RecognizedIndex)
		{
		case 0:
			if (!bHasPoseBeenProcessed)
			{
				OnPerformGrabTrace.Broadcast(Side, this);
				
				if (CheckIfCanTriggerPoseResponse())
				{
					OnPoseRecognizedDelegate.Broadcast(Side, this);
				}
				bHasPoseBeenProcessed = true;
				SetHandActive(Side, true);
			}
			break;
		case 1:
			OnFreezePoseRecognizedDelegate.Broadcast();
			break;
		}

	}
	else
	{
		if (bHasPoseBeenProcessed)
		{
			if (CheckIfCanTriggerPoseResponse())
			{
				OnPoseReleasedDelegate.Broadcast(this);
			}
			bHasPoseBeenProcessed = false;
			SetHandActive(Side, false);
		}

		if (bIsHandGrabbing)
		{
			bIsHandGrabbing = false;
			OnReleaseGrabbedActor.Broadcast(this);
		}
		
	}
}

/// <summary>
/// The movement pose can only trigger a response if
/// neither hand is currently in control
/// If neither hand is in control and the current hands
/// grab trace (Performed in blueprints) returns false, this function returns true and sets 
/// the relevant hand to be in control
/// </summary>
/// <returns></returns>
bool UCustomHandPoseRecognizer::CheckIfCanTriggerPoseResponse()
{
	if (bIsHandGrabbing) { return false; }

	if (CurrentHandInControl == EOculusXRHandType::None)
	{
		CurrentHandInControl = (Side == EOculusXRHandType::HandLeft) ? EOculusXRHandType::HandLeft : EOculusXRHandType::HandRight;
		return true;
	}

	return (CurrentHandInControl == EOculusXRHandType::HandLeft && Side == EOculusXRHandType::HandLeft) ||
			(CurrentHandInControl == EOculusXRHandType::HandRight && Side == EOculusXRHandType::HandRight);
}



void UCustomHandPoseRecognizer::SetHandActive(EOculusXRHandType Hand, bool bIsActive)
{
	if (Hand == EOculusXRHandType::HandLeft)
	{
		bLeftHandActive = bIsActive;
	}
	else if (Hand == EOculusXRHandType::HandRight)
	{
		bRightHandActive = bIsActive;
	}

	// Check if both hands are inactive and reset control if needed
	ResetHandInControlIfNeeded();
}

void UCustomHandPoseRecognizer::ResetHandInControlIfNeeded()
{
	if (!bLeftHandActive && !bRightHandActive) // If both hands are NOT active
	{
		CurrentHandInControl = EOculusXRHandType::None;
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


