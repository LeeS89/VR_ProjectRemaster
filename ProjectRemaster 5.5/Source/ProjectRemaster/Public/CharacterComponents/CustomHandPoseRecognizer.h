// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HandPoseRecognizer.h"
#include "OculusXRInputFunctionLibrary.h"
#include "Interfaces/MainPlayer.h"
#include "Interfaces/ResettableComponent.h"
#include "CustomHandPoseRecognizer.generated.h"

#pragma region Event Declarations
DECLARE_DYNAMIC_MULTICAST_DELEGATE(
	FOnFreezePoseRecognized
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

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
	FOnReleaseGrabbedActor, UCustomHandPoseRecognizer*, Recognizer
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(
	FOnPerformGrabTrace,
	EOculusXRHandType, HandUsed,
	UCustomHandPoseRecognizer*, PoseClass
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(
	FOnFrozenBulletAbilityComplete
);
#pragma endregion



UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class PROJECTREMASTER_API UCustomHandPoseRecognizer : public UHandPoseRecognizer, public IResettableComponent
{
	GENERATED_BODY()

private:

	IMainPlayer* PlayerRef;
	//bool bCanTriggerReturnFire;
#pragma region Pose Recognition members and entry function
	int RecognizedIndex;
	FString RecognizedName;
	float PoseDuration;
	float PoseError;
	float PoseConfidence;

	UFUNCTION()
	void ProcessHandPoses();
#pragma endregion

#pragma region Custom Pose Recognized Functions
	void MovementPoseRecognized();
	void BulletFreezAbilityPoseRecognized();
#pragma endregion

#pragma region Recognized Pose Handler Functions
	bool CheckIfCanTriggerMovementPoseResponse();
	bool CanTriggerBulletFreezeAbility(EOculusXRHandType Hand);
#pragma endregion

#pragma region Pose Released Handler functions
	void GrabbingPoseReleased();

	void BulletFreezeAbilityPoseComplete(EOculusXRHandType Hand);

	void MovementPoseFinished();
#pragma endregion
	
#pragma region Static memebers 
	/// <summary>
	/// These are used to prevent simultaneous execution 
	/// of pose functions on both hands
	/// </summary>
	static bool bLeftHandActive;
	static bool bRightHandActive;
	static EOculusXRHandType CurrentHandInControlOfMovement;
	static EOculusXRHandType CurrentHandTriggeringBulletFreezeAbility;
	static void SetHandActive(EOculusXRHandType Hand, bool bIsActive);
	static void ResetHandInControlIfNeeded();
#pragma endregion

public:

	UCustomHandPoseRecognizer();
	

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bIsHandGrabbing{ false };

#pragma region Events
	UPROPERTY(BlueprintAssignable)
	FOnPoseRecognizedSignature OnPoseRecognizedDelegate;
	
	UPROPERTY(BlueprintAssignable)
	FOnPoseReleasedSignature OnPoseReleasedDelegate;

	UPROPERTY(BlueprintAssignable)
	FOnFreezePoseRecognized OnFreezePoseRecognized;

	UPROPERTY(BlueprintAssignable)
	FOnReleaseGrabbedActor OnReleaseGrabbedActor;

	UPROPERTY(BlueprintAssignable)
	FOnPerformGrabTrace OnPerformGrabTrace;

	UPROPERTY(BlueprintAssignable)
	FOnFrozenBulletAbilityComplete OnFrozenBulletAbilityComplete;
#pragma endregion
	
#pragma region Hand Pose confidence Handlers
	UPROPERTY(VisibleAnywhere)
	float HandPoseConfidence{ 0.0f };

	UPROPERTY(EditAnywhere)
	float ReducedHandPoseConfidenceFloor{ 0.25f };

	UFUNCTION(BlueprintCallable)
	void ResetHandPoseConfidenceFloor();

	UFUNCTION(BlueprintCallable)
	void ReduceHandPoseConfidenceFloor();

#pragma endregion

#pragma region Input Toggling
	virtual void ResetComponent() override;
#pragma endregion


protected:

	virtual void BeginPlay() override;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY()
	bool bHasPoseBeenProcessed{ false };

	
};
