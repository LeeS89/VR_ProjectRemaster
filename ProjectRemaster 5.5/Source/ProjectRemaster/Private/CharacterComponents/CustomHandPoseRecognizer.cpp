// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterComponents/CustomHandPoseRecognizer.h"



EOculusXRHandType UCustomHandPoseRecognizer::CurrentHandInControlOfMovement = EOculusXRHandType::None;
EOculusXRHandType UCustomHandPoseRecognizer::CurrentHandTriggeringBulletFreezeAbility = EOculusXRHandType::None;
bool UCustomHandPoseRecognizer::bLeftHandActive = false;
bool UCustomHandPoseRecognizer::bRightHandActive = false;



UCustomHandPoseRecognizer::UCustomHandPoseRecognizer()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UCustomHandPoseRecognizer::BeginPlay()
{
	Super::BeginPlay();
	FString CValue = (CurrentHandTriggeringBulletFreezeAbility == EOculusXRHandType::None) ? TEXT("None") : TEXT("L or R");
	UE_LOG(LogTemp, Error, TEXT("CurrentHandTriggeringBulletFreezeAbility is: %s"), *CValue);
	HandPoseConfidence = DefaultConfidenceFloor;

	if (GetOwner()->Implements<UMainPlayer>())
	{
		PlayerRef = Cast<IMainPlayer>(GetOwner());
	}
	
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
			MovementPoseRecognized();
			break;
		case 1:
			BulletFreezAbilityPoseRecognized();
			break;
		}
	}
	else
	{
		MovementPoseFinished();


		BulletFreezeAbilityPoseComplete(Side);


		GrabbingPoseReleased();

	}
}

#pragma region Movement Pose Handlers
void UCustomHandPoseRecognizer::MovementPoseRecognized()
{
	if (!bHasPoseBeenProcessed)
	{
		OnPerformGrabTrace.Broadcast(Side, this);

		if (CheckIfCanTriggerMovementPoseResponse())
		{
			OnPoseRecognizedDelegate.Broadcast(Side, this);
		}
		bHasPoseBeenProcessed = true;
		SetHandActive(Side, true);
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
bool UCustomHandPoseRecognizer::CheckIfCanTriggerMovementPoseResponse()
{
	if (bIsHandGrabbing) { return false; }

	if (CurrentHandInControlOfMovement == EOculusXRHandType::None)
	{
		CurrentHandInControlOfMovement = (Side == EOculusXRHandType::HandLeft) ? EOculusXRHandType::HandLeft : EOculusXRHandType::HandRight;
		return true;
	}

	return (CurrentHandInControlOfMovement == EOculusXRHandType::HandLeft && Side == EOculusXRHandType::HandLeft) ||
		(CurrentHandInControlOfMovement == EOculusXRHandType::HandRight && Side == EOculusXRHandType::HandRight);
}

void UCustomHandPoseRecognizer::MovementPoseFinished()
{
	if (!bHasPoseBeenProcessed) { return; }

	if (CheckIfCanTriggerMovementPoseResponse())
	{
		OnPoseReleasedDelegate.Broadcast(this);
	}
	bHasPoseBeenProcessed = false;
	SetHandActive(Side, false);

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
		CurrentHandInControlOfMovement = EOculusXRHandType::None;
	}
}
#pragma endregion


#pragma region Bullet Freeze Ability Pose Handlers
void UCustomHandPoseRecognizer::BulletFreezAbilityPoseRecognized()
{
	if (!PlayerRef) { return; }
	
	if (CanTriggerBulletFreezeAbility(Side))
	{
		OnFreezePoseRecognized.Broadcast();
	}
}

bool UCustomHandPoseRecognizer::CanTriggerBulletFreezeAbility(EOculusXRHandType Hand)
{
	if (CurrentHandTriggeringBulletFreezeAbility == EOculusXRHandType::None)
	{
		CurrentHandTriggeringBulletFreezeAbility = Hand;
		
		return true;
	}

	return CurrentHandTriggeringBulletFreezeAbility == Hand;
}

void UCustomHandPoseRecognizer::BulletFreezeAbilityPoseComplete(EOculusXRHandType Hand)
{
	if (Hand != CurrentHandTriggeringBulletFreezeAbility) { return; }
	CurrentHandTriggeringBulletFreezeAbility = EOculusXRHandType::None;

	if (!PlayerRef || !PlayerRef->CheckFrozenBulletcountGreaterThanZero()) { return; }
	
	OnFrozenBulletAbilityComplete.Broadcast();
	
}

#pragma endregion


#pragma region Grab Pose Handlers
void UCustomHandPoseRecognizer::GrabbingPoseReleased()
{
	if (!bIsHandGrabbing) { return; }

	bIsHandGrabbing = false;
	OnReleaseGrabbedActor.Broadcast(this);
}


void UCustomHandPoseRecognizer::ResetHandPoseConfidenceFloor()
{
	DefaultConfidenceFloor = HandPoseConfidence;
}

void UCustomHandPoseRecognizer::ReduceHandPoseConfidenceFloor()
{
	DefaultConfidenceFloor = ReducedHandPoseConfidenceFloor;
}
void UCustomHandPoseRecognizer::ResetComponent()
{
	SetComponentTickEnabled(false);
	GrabbingPoseReleased();
	if (CurrentHandTriggeringBulletFreezeAbility != EOculusXRHandType::None)
	{
		CurrentHandTriggeringBulletFreezeAbility = EOculusXRHandType::None;
	}
	if (bLeftHandActive || bRightHandActive)
	{
		SetHandActive(Side, false);
	}
}
#pragma endregion






