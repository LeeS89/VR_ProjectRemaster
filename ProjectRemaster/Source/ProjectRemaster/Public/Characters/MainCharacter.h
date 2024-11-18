// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interfaces/MainPlayer.h"
#include "GameFramework/Character.h"
#include "MainCharacter.generated.h"

UCLASS()
class PROJECTREMASTER_API AMainCharacter : public ACharacter, public IMainPlayer
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMainCharacter();

	// Camera and Hand Visuals
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class UVRCameraComponent* VRCameraComp;

	// Hand Components - Visuals, Gesture Recognizers and tracking
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class UMotionControllerComponent* LeftMotionControllerComp;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class UMotionControllerComponent* RightMotionControllerComp;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class UCustomXRHandComponent* LeftXRHandComp;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class UCustomXRHandComponent* RightXRHandComp;


	// Hand Pose Recognizers
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class UHandPoseRecognizer* LeftPoseRecognizer;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class UHandPoseRecognizer* RightPoseRecognizer;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class UHandPoseRecognizer* FistPoseRecognizerLeft;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class UHandPoseRecognizer* FistPoseRecognizerRight;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	USceneComponent* VROrigin;

	// Actor Components
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class ULocomotionComponent* LocomotionComp;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class UTraceComponent* TraceComp;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

	virtual TArray<class UCustomXRHandComponent*> GetHandComponents() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
