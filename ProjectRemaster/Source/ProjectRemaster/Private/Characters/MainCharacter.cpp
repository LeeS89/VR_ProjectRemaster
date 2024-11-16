// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/MainCharacter.h"
#include "Camera/CameraComponent.h"
#include "MotionControllerComponent.h"
#include "OculusXRHandComponent.h"

// Sets default values
AMainCharacter::AMainCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	VROrigin = CreateDefaultSubobject<USceneComponent>(TEXT("Scene Component"));


	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera Component"));
	//CameraComp->SetupAttachment(VROrigin);

	LeftMotionControllerComp = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("Left Motion Controller Component"));
	LeftMotionControllerComp->SetupAttachment(VROrigin);

	RightMotionControllerComp = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("Right Motion Controller Component"));
	RightMotionControllerComp->SetupAttachment(VROrigin);

	LeftXRHandComp = CreateDefaultSubobject<UOculusXRHandComponent>(TEXT("Left Oculus XR Hand Component"));
	LeftXRHandComp->SetupAttachment(LeftMotionControllerComp);

	RightXRHandComp = CreateDefaultSubobject<UOculusXRHandComponent>(TEXT("Right Oculus XR Hand Component"));
	RightXRHandComp->SetupAttachment(RightMotionControllerComp);

}

// Called when the game starts or when spawned
void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();
	
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

