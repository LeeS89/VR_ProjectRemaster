// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MainCharacter.generated.h"

UCLASS()
class PROJECTREMASTER_API AMainCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMainCharacter();

	// Camera and Hand Visuals
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class UCameraComponent* CameraComp;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class UMotionControllerComponent* LeftMotionControllerComp;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class UMotionControllerComponent* RightMotionControllerComp;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class UOculusXRHandComponent* LeftXRHandComp;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class UOculusXRHandComponent* RightXRHandComp;

	// Hand Pose Recognizers
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class UHandPoseRecognizer* LeftPoseRecognizer;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class UHandPoseRecognizer* RightPoseRecognizer;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	USceneComponent* VROrigin;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
