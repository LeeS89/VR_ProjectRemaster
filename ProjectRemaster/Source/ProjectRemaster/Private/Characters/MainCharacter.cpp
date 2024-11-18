// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/MainCharacter.h"
#include "Camera/CameraComponent.h"
#include "MotionControllerComponent.h"
#include "HandPoseRecognizer.h"
#include "CharacterComponents/CustomXRHandComponent.h"
#include "CharacterComponents/TraceComponent.h"
#include "CharacterComponents/VRCameraComponent.h"
#include "CharacterComponents/LocomotionComponent.h"

// Sets default values
AMainCharacter::AMainCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	VROrigin = CreateDefaultSubobject<USceneComponent>(TEXT("Scene Component"));
	VROrigin->SetupAttachment(RootComponent);

	VRCameraComp = CreateDefaultSubobject<UVRCameraComponent>(TEXT("Camera Component"));
	VRCameraComp->SetupAttachment(RootComponent);

	// Hand Components - Visuals, Gesture Recognizers and tracking
	LeftMotionControllerComp = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("Left Motion Controller Component"));
	LeftMotionControllerComp->SetupAttachment(VROrigin);

	RightMotionControllerComp = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("Right Motion Controller Component"));
	RightMotionControllerComp->SetupAttachment(VROrigin);

	LeftXRHandComp = CreateDefaultSubobject<UCustomXRHandComponent>(TEXT("Left Oculus XR Hand Component"));
	LeftXRHandComp->SetupAttachment(LeftMotionControllerComp);

	RightXRHandComp = CreateDefaultSubobject<UCustomXRHandComponent>(TEXT("Right Oculus XR Hand Component"));
	RightXRHandComp->SetupAttachment(RightMotionControllerComp);

	// Left Hand Pose Recognizers
	LeftPoseRecognizer = CreateDefaultSubobject<UHandPoseRecognizer>(TEXT("Left Hand Pose Recognizer"));
	LeftPoseRecognizer->SetupAttachment(LeftMotionControllerComp);

	FistPoseRecognizerLeft = CreateDefaultSubobject<UHandPoseRecognizer>(TEXT("Left Fist Pose Recognizer"));
	FistPoseRecognizerLeft->SetupAttachment(LeftMotionControllerComp);

	// Right Hand Pose Recognizers
	RightPoseRecognizer = CreateDefaultSubobject<UHandPoseRecognizer>(TEXT("Right Hand Pose Recognizer"));
	RightPoseRecognizer->SetupAttachment(RightMotionControllerComp);

	FistPoseRecognizerRight = CreateDefaultSubobject<UHandPoseRecognizer>(TEXT("Right Fist Pose Recognizer"));
	FistPoseRecognizerRight->SetupAttachment(RightMotionControllerComp);

	// Actor Components
	LocomotionComp = CreateDefaultSubobject<ULocomotionComponent>(TEXT("Locomotion Component"));

	TraceComp = CreateDefaultSubobject<UTraceComponent>(TEXT("Trace Component"));
}

// Called when the game starts or when spawned
void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

TArray<class UCustomXRHandComponent*> AMainCharacter::GetHandComponents()
{
	TArray<UCustomXRHandComponent*> HandComponents;

	HandComponents.Add(LeftXRHandComp);
	HandComponents.Add(RightXRHandComp);

	return HandComponents;
}

// Called every frame
void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

