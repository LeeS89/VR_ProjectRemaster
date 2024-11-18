// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterComponents/LocomotionComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"

// Sets default values for this component's properties
ULocomotionComponent::ULocomotionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	CurrentHandInControl = EHandInControl::Neither;
}



// Called when the game starts
void ULocomotionComponent::BeginPlay()
{
	Super::BeginPlay();

	CharacterRef = GetOwner<ACharacter>();
	PlayerController = Cast<APlayerController>(CharacterRef->GetController());

	MovementComp = CharacterRef->GetCharacterMovement();

	MovementComp->MaxWalkSpeed = MaxSpeed;
	
}


void ULocomotionComponent::MoveForward(float ScaleValue)
{
	if (!IsValid(CharacterRef)) { return; }

	FVector CharacterForwardVector{ CharacterRef->GetActorForwardVector() };

	CharacterRef->AddMovementInput(CharacterForwardVector * ScaleValue);
}

bool ULocomotionComponent::CanMoveForward(TEnumAsByte<EHandInControl> HandInControl) const
{
	return CurrentHandInControl == EHandInControl::Neither || CurrentHandInControl == HandInControl;
}

void ULocomotionComponent::ResetHandInControl()
{
	CurrentHandInControl = EHandInControl::Neither;
}

void ULocomotionComponent::HandleRotation(float Direction)
{
	float DeltaTime{ GetWorld()->GetDeltaSeconds() };

	CharacterRef->AddControllerYawInput(Direction * RotationSpeed * DeltaTime);
}


// Called every frame
void ULocomotionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

